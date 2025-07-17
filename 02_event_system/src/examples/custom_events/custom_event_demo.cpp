#include "custom_event_demo.h"
#include <QApplication>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

CustomEventDemo::CustomEventDemo(QWidget *parent)
    : QWidget(parent)
    , m_mainLayout(nullptr)
    , m_demoTimer(new QTimer(this))
    , m_demoStep(0)
    , m_demoRunning(false)
    , m_totalEventsSent(0)
    , m_totalEventsReceived(0)
{
    setupUI();
    
    // 连接演示定时器
    connect(m_demoTimer, &QTimer::timeout, this, &CustomEventDemo::onDemoTimer);
    
    // 设置事件目标
    m_sender->setEventTarget(m_receiver);
    
    // 连接信号
    connect(m_sender, &CustomEventSender::eventSent, this, &CustomEventDemo::onEventSent);
    connect(m_receiver, &CustomEventReceiver::eventReceived, this, &CustomEventDemo::onEventReceived);
    connect(m_receiver, &CustomEventReceiver::dataEventReceived, this, &CustomEventDemo::onDataEventReceived);
    connect(m_receiver, &CustomEventReceiver::commandEventReceived, this, &CustomEventDemo::onCommandEventReceived);
    connect(m_receiver, &CustomEventReceiver::statisticsUpdated, this, &CustomEventDemo::onStatisticsUpdated);
}

void CustomEventDemo::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    
    // 设置演示控制面板
    setupDemoControls();
    
    // 创建主分割器
    m_mainSplitter = new QSplitter(Qt::Horizontal, this);
    
    // 创建发送器和接收器
    m_sender = new CustomEventSender(this);
    m_receiver = new CustomEventReceiver(this);
    
    // 添加到分割器
    m_mainSplitter->addWidget(m_sender);
    m_mainSplitter->addWidget(m_receiver);
    
    // 设置分割器比例
    m_mainSplitter->setStretchFactor(0, 1);
    m_mainSplitter->setStretchFactor(1, 2);
    
    m_mainLayout->addWidget(m_mainSplitter);
    
    // 设置事件流显示
    setupEventFlow();
}

void CustomEventDemo::setupDemoControls()
{
    m_demoControlGroup = new QGroupBox("演示控制", this);
    QVBoxLayout* layout = new QVBoxLayout(m_demoControlGroup);
    
    // 第一行：基本控制
    QHBoxLayout* row1 = new QHBoxLayout();
    m_startDemoBtn = new QPushButton("开始自动演示", this);
    m_stopDemoBtn = new QPushButton("停止演示", this);
    m_resetDemoBtn = new QPushButton("重置演示", this);
    m_stopDemoBtn->setEnabled(false);
    
    row1->addWidget(m_startDemoBtn);
    row1->addWidget(m_stopDemoBtn);
    row1->addWidget(m_resetDemoBtn);
    row1->addStretch();
    layout->addLayout(row1);
    
    // 第二行：预设演示场景
    QHBoxLayout* row2 = new QHBoxLayout();
    m_basicDataDemoBtn = new QPushButton("基础数据演示", this);
    m_commandDemoBtn = new QPushButton("命令演示", this);
    m_serializationDemoBtn = new QPushButton("序列化演示", this);
    m_performanceDemoBtn = new QPushButton("性能演示", this);
    
    row2->addWidget(m_basicDataDemoBtn);
    row2->addWidget(m_commandDemoBtn);
    row2->addWidget(m_serializationDemoBtn);
    row2->addWidget(m_performanceDemoBtn);
    row2->addStretch();
    layout->addLayout(row2);
    
    m_mainLayout->addWidget(m_demoControlGroup);
    
    // 连接信号
    connect(m_startDemoBtn, &QPushButton::clicked, this, &CustomEventDemo::startDemo);
    connect(m_stopDemoBtn, &QPushButton::clicked, this, &CustomEventDemo::stopDemo);
    connect(m_resetDemoBtn, &QPushButton::clicked, this, &CustomEventDemo::resetDemo);
    connect(m_basicDataDemoBtn, &QPushButton::clicked, this, &CustomEventDemo::runBasicDataDemo);
    connect(m_commandDemoBtn, &QPushButton::clicked, this, &CustomEventDemo::runCommandDemo);
    connect(m_serializationDemoBtn, &QPushButton::clicked, this, &CustomEventDemo::runSerializationDemo);
    connect(m_performanceDemoBtn, &QPushButton::clicked, this, &CustomEventDemo::runPerformanceDemo);
}

void CustomEventDemo::setupEventFlow()
{
    m_eventFlowGroup = new QGroupBox("事件流监控", this);
    QVBoxLayout* layout = new QVBoxLayout(m_eventFlowGroup);
    
    // 统计标签
    m_flowStatsLabel = new QLabel("发送: 0 | 接收: 0 | 演示状态: 就绪", this);
    m_flowStatsLabel->setStyleSheet("QLabel { font-weight: bold; color: #333; }");
    layout->addWidget(m_flowStatsLabel);
    
    // 事件流显示
    m_eventFlowDisplay = new QTextEdit(this);
    m_eventFlowDisplay->setMaximumHeight(150);
    m_eventFlowDisplay->setReadOnly(true);
    m_eventFlowDisplay->setPlainText("等待事件流...\n");
    layout->addWidget(m_eventFlowDisplay);
    
    m_mainLayout->addWidget(m_eventFlowGroup);
}

void CustomEventDemo::startDemo()
{
    if (m_demoRunning) return;
    
    m_demoRunning = true;
    m_demoStep = 0;
    m_demoStartTime = QDateTime::currentDateTime();
    
    m_startDemoBtn->setEnabled(false);
    m_stopDemoBtn->setEnabled(true);
    
    // 开始演示定时器，每2秒执行一步
    m_demoTimer->start(2000);
    
    updateEventFlowDisplay();
    m_eventFlowDisplay->append("=== 开始自动演示 ===");
}

void CustomEventDemo::stopDemo()
{
    if (!m_demoRunning) return;
    
    m_demoRunning = false;
    m_demoTimer->stop();
    
    m_startDemoBtn->setEnabled(true);
    m_stopDemoBtn->setEnabled(false);
    
    updateEventFlowDisplay();
    m_eventFlowDisplay->append("=== 演示已停止 ===");
}

void CustomEventDemo::resetDemo()
{
    stopDemo();
    
    m_demoStep = 0;
    m_totalEventsSent = 0;
    m_totalEventsReceived = 0;
    
    // 重置接收器统计
    m_receiver->resetStatistics();
    m_receiver->clearEventLog();
    
    // 清空事件流显示
    m_eventFlowDisplay->clear();
    m_eventFlowDisplay->setPlainText("演示已重置，等待开始...\n");
    
    updateEventFlowDisplay();
}

void CustomEventDemo::runBasicDataDemo()
{
    m_eventFlowDisplay->append("=== 基础数据演示开始 ===");
    
    // 发送不同类型的数据事件
    QTimer::singleShot(500, [this]() {
        // 发送字符串数据
        DataEvent* stringEvent = new DataEvent("Hello, Custom Events!");
        QApplication::postEvent(m_receiver, stringEvent);
        m_eventFlowDisplay->append("发送字符串数据事件");
    });
    
    QTimer::singleShot(1000, [this]() {
        // 发送数字数据
        DataEvent* numberEvent = new DataEvent(42);
        QApplication::postEvent(m_receiver, numberEvent);
        m_eventFlowDisplay->append("发送数字数据事件");
    });
    
    QTimer::singleShot(1500, [this]() {
        // 发送Map数据
        QVariantMap map;
        map["name"] = "张三";
        map["age"] = 25;
        map["city"] = "北京";
        DataEvent* mapEvent = new DataEvent(map);
        QApplication::postEvent(m_receiver, mapEvent);
        m_eventFlowDisplay->append("发送Map数据事件");
    });
    
    QTimer::singleShot(2000, [this]() {
        // 发送List数据
        QVariantList list = {"苹果", "香蕉", "橙子", "葡萄"};
        DataEvent* listEvent = new DataEvent(list);
        QApplication::postEvent(m_receiver, listEvent);
        m_eventFlowDisplay->append("发送List数据事件");
        m_eventFlowDisplay->append("=== 基础数据演示完成 ===");
    });
}

void CustomEventDemo::runCommandDemo()
{
    m_eventFlowDisplay->append("=== 命令演示开始 ===");
    
    QTimer::singleShot(500, [this]() {
        // 发送简单命令
        CommandEvent* simpleCmd = new CommandEvent("start_process");
        QApplication::postEvent(m_receiver, simpleCmd);
        m_eventFlowDisplay->append("发送简单命令: start_process");
    });
    
    QTimer::singleShot(1000, [this]() {
        // 发送带参数的命令
        QVariantMap params;
        params["task_id"] = "001";
        params["priority"] = "high";
        params["timeout"] = 30;
        CommandEvent* paramCmd = new CommandEvent("execute_task", params);
        QApplication::postEvent(m_receiver, paramCmd);
        m_eventFlowDisplay->append("发送带参数命令: execute_task");
    });
    
    QTimer::singleShot(1500, [this]() {
        // 发送复杂命令
        QVariantMap complexParams;
        complexParams["operation"] = "batch_process";
        complexParams["files"] = QVariantList({"file1.txt", "file2.txt", "file3.txt"});
        complexParams["options"] = QVariantMap({{"compress", true}, {"backup", false}});
        CommandEvent* complexCmd = new CommandEvent("file_operation", complexParams);
        QApplication::postEvent(m_receiver, complexCmd);
        m_eventFlowDisplay->append("发送复杂命令: file_operation");
        m_eventFlowDisplay->append("=== 命令演示完成 ===");
    });
}

void CustomEventDemo::runSerializationDemo()
{
    m_eventFlowDisplay->append("=== 序列化演示开始 ===");
    
    demonstrateDataSerialization();
}

void CustomEventDemo::runPerformanceDemo()
{
    m_eventFlowDisplay->append("=== 性能演示开始 ===");
    
    // 批量发送事件测试性能
    const int eventCount = 100;
    QDateTime startTime = QDateTime::currentDateTime();
    
    for (int i = 0; i < eventCount; ++i) {
        if (i % 2 == 0) {
            DataEvent* event = new DataEvent(QString("性能测试数据 #%1").arg(i + 1));
            QApplication::postEvent(m_receiver, event);
        } else {
            QVariantMap params;
            params["test_index"] = i + 1;
            params["timestamp"] = QDateTime::currentMSecsSinceEpoch();
            CommandEvent* event = new CommandEvent("performance_test", params);
            QApplication::postEvent(m_receiver, event);
        }
    }
    
    QTimer::singleShot(1000, [this, eventCount, startTime]() {
        QDateTime endTime = QDateTime::currentDateTime();
        qint64 duration = startTime.msecsTo(endTime);
        double rate = (double)eventCount * 1000.0 / duration;
        
        m_eventFlowDisplay->append(QString("性能测试完成: %1个事件，用时%2ms，速率%3事件/秒")
                                  .arg(eventCount)
                                  .arg(duration)
                                  .arg(rate, 0, 'f', 1));
        m_eventFlowDisplay->append("=== 性能演示完成 ===");
    });
}

void CustomEventDemo::onEventSent(const QString& eventType, const QString& description)
{
    m_totalEventsSent++;
    updateEventFlowDisplay();
    
    QString logEntry = QString("[发送] %1: %2").arg(eventType).arg(description);
    m_eventFlowDisplay->append(logEntry);
}

void CustomEventDemo::onEventReceived(const QString& eventType, const QString& description)
{
    m_totalEventsReceived++;
    updateEventFlowDisplay();
    
    QString logEntry = QString("[接收] %1: %2").arg(eventType).arg(description);
    m_eventFlowDisplay->append(logEntry);
}

void CustomEventDemo::onDataEventReceived(const QVariant& data)
{
    // 可以在这里处理特定的数据事件逻辑
    Q_UNUSED(data)
}

void CustomEventDemo::onCommandEventReceived(const QString& command, const QVariantMap& parameters)
{
    // 可以在这里处理特定的命令事件逻辑
    QString logEntry = QString("[命令处理] %1 (参数: %2个)").arg(command).arg(parameters.size());
    m_eventFlowDisplay->append(logEntry);
}

void CustomEventDemo::onStatisticsUpdated(const CustomEventReceiver::EventStatistics& stats)
{
    // 更新统计显示
    Q_UNUSED(stats)
    updateEventFlowDisplay();
}

void CustomEventDemo::onDemoTimer()
{
    if (!m_demoRunning) return;
    
    switch (m_demoStep) {
    case 0:
        m_eventFlowDisplay->append("步骤1: 发送基础数据事件");
        runBasicDataDemo();
        break;
    case 1:
        m_eventFlowDisplay->append("步骤2: 发送命令事件");
        runCommandDemo();
        break;
    case 2:
        m_eventFlowDisplay->append("步骤3: 演示序列化功能");
        runSerializationDemo();
        break;
    case 3:
        m_eventFlowDisplay->append("步骤4: 性能测试");
        runPerformanceDemo();
        break;
    default:
        // 演示完成
        stopDemo();
        m_eventFlowDisplay->append("=== 自动演示完成 ===");
        return;
    }
    
    m_demoStep++;
}

void CustomEventDemo::updateEventFlowDisplay()
{
    QString status = m_demoRunning ? "运行中" : "就绪";
    m_flowStatsLabel->setText(QString("发送: %1 | 接收: %2 | 演示状态: %3")
                             .arg(m_totalEventsSent)
                             .arg(m_totalEventsReceived)
                             .arg(status));
}

void CustomEventDemo::demonstrateDataSerialization()
{
    // 创建一个复杂的数据事件
    QVariantMap complexData;
    complexData["user_info"] = QVariantMap({
        {"name", "李四"},
        {"age", 30},
        {"email", "lisi@example.com"}
    });
    complexData["preferences"] = QVariantList({"theme_dark", "lang_zh", "notifications_on"});
    complexData["metadata"] = QVariantMap({
        {"created", QDateTime::currentDateTime().toString()},
        {"version", "1.0"},
        {"checksum", "abc123"}
    });
    
    DataEvent* event = new DataEvent(complexData);
    
    // 演示序列化
    QByteArray serialized = event->serialize();
    m_eventFlowDisplay->append(QString("序列化数据大小: %1 字节").arg(serialized.size()));
    
    // 演示反序列化
    DataEvent* deserializedEvent = new DataEvent();
    bool success = deserializedEvent->deserialize(serialized);
    
    if (success) {
        m_eventFlowDisplay->append("反序列化成功");
        QApplication::postEvent(m_receiver, deserializedEvent);
        m_eventFlowDisplay->append("发送反序列化后的事件");
    } else {
        m_eventFlowDisplay->append("反序列化失败");
        delete deserializedEvent;
    }
    
    delete event;
    m_eventFlowDisplay->append("=== 序列化演示完成 ===");
}

void CustomEventDemo::demonstrateEventBatching()
{
    // 批量事件处理演示
    m_eventFlowDisplay->append("开始批量事件演示...");
    
    const int batchSize = 50;
    for (int i = 0; i < batchSize; ++i) {
        if (i % 3 == 0) {
            DataEvent* event = new DataEvent(QString("批量数据 #%1").arg(i + 1));
            QApplication::postEvent(m_receiver, event);
        } else if (i % 3 == 1) {
            QVariantMap params;
            params["batch_id"] = i + 1;
            params["total"] = batchSize;
            CommandEvent* event = new CommandEvent("batch_process", params);
            QApplication::postEvent(m_receiver, event);
        } else {
            DataEvent* event = new DataEvent(QVariantList({i + 1, "batch_item", true}));
            QApplication::postEvent(m_receiver, event);
        }
    }
    
    m_eventFlowDisplay->append(QString("批量发送 %1 个事件完成").arg(batchSize));
}

void CustomEventDemo::demonstrateEventFiltering()
{
    // 事件过滤演示
    m_eventFlowDisplay->append("开始事件过滤演示...");
    
    // 先禁用数据事件接收
    m_receiver->setDataEventFilter(false);
    m_eventFlowDisplay->append("禁用数据事件接收");
    
    // 发送一些事件
    DataEvent* dataEvent = new DataEvent("这个数据事件应该被过滤");
    CommandEvent* cmdEvent = new CommandEvent("test_command");
    
    QApplication::postEvent(m_receiver, dataEvent);
    QApplication::postEvent(m_receiver, cmdEvent);
    
    m_eventFlowDisplay->append("发送数据事件和命令事件");
    
    // 1秒后重新启用数据事件接收
    QTimer::singleShot(1000, [this]() {
        m_receiver->setDataEventFilter(true);
        m_eventFlowDisplay->append("重新启用数据事件接收");
        
        DataEvent* event = new DataEvent("这个数据事件应该被接收");
        QApplication::postEvent(m_receiver, event);
        m_eventFlowDisplay->append("发送新的数据事件");
        m_eventFlowDisplay->append("=== 事件过滤演示完成 ===");
    });
}