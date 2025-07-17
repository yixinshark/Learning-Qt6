#include "custom_event_sender.h"
#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariantList>
#include <QDebug>

CustomEventSender::CustomEventSender(QWidget *parent)
    : QWidget(parent)
    , m_mainLayout(nullptr)
    , m_eventTarget(nullptr)
    , m_eventsSent(0)
    , m_periodicTimer(new QTimer(this))
{
    setupUI();
    
    // 连接定时器
    connect(m_periodicTimer, &QTimer::timeout, this, &CustomEventSender::onPeriodicTimer);
}

void CustomEventSender::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    
    // 目标信息显示
    m_targetLabel = new QLabel("事件目标: 未设置", this);
    m_targetLabel->setStyleSheet("QLabel { font-weight: bold; color: #666; }");
    m_mainLayout->addWidget(m_targetLabel);
    
    // 状态显示
    m_statusLabel = new QLabel("已发送事件: 0", this);
    m_statusLabel->setStyleSheet("QLabel { color: #333; }");
    m_mainLayout->addWidget(m_statusLabel);
    
    // 设置各个控件组
    setupDataEventControls();
    setupCommandEventControls();
    setupBatchControls();
    
    // 添加弹性空间
    m_mainLayout->addStretch();
}

void CustomEventSender::setupDataEventControls()
{
    m_dataEventGroup = new QGroupBox("数据事件发送", this);
    QVBoxLayout* layout = new QVBoxLayout(m_dataEventGroup);
    
    // 字符串数据
    QHBoxLayout* stringLayout = new QHBoxLayout();
    stringLayout->addWidget(new QLabel("字符串数据:"));
    m_stringDataEdit = new QLineEdit("Hello, Custom Event!", this);
    m_sendStringBtn = new QPushButton("发送字符串", this);
    stringLayout->addWidget(m_stringDataEdit);
    stringLayout->addWidget(m_sendStringBtn);
    layout->addLayout(stringLayout);
    
    // 数字数据
    QHBoxLayout* numberLayout = new QHBoxLayout();
    numberLayout->addWidget(new QLabel("数字数据:"));
    m_numberDataSpin = new QSpinBox(this);
    m_numberDataSpin->setRange(-999999, 999999);
    m_numberDataSpin->setValue(42);
    m_sendNumberBtn = new QPushButton("发送数字", this);
    numberLayout->addWidget(m_numberDataSpin);
    numberLayout->addWidget(m_sendNumberBtn);
    layout->addLayout(numberLayout);
    
    // Map数据
    layout->addWidget(new QLabel("Map数据 (JSON格式):"));
    m_mapDataEdit = new QTextEdit(this);
    m_mapDataEdit->setMaximumHeight(80);
    m_mapDataEdit->setPlainText("{\"name\": \"张三\", \"age\": 25, \"city\": \"北京\"}");
    m_sendMapBtn = new QPushButton("发送Map数据", this);
    layout->addWidget(m_mapDataEdit);
    layout->addWidget(m_sendMapBtn);
    
    // List数据
    layout->addWidget(new QLabel("List数据 (JSON数组格式):"));
    m_listDataEdit = new QTextEdit(this);
    m_listDataEdit->setMaximumHeight(60);
    m_listDataEdit->setPlainText("[\"苹果\", \"香蕉\", \"橙子\", \"葡萄\"]");
    m_sendListBtn = new QPushButton("发送List数据", this);
    layout->addWidget(m_listDataEdit);
    layout->addWidget(m_sendListBtn);
    
    m_mainLayout->addWidget(m_dataEventGroup);
    
    // 连接信号
    connect(m_sendStringBtn, &QPushButton::clicked, this, &CustomEventSender::sendStringData);
    connect(m_sendNumberBtn, &QPushButton::clicked, this, &CustomEventSender::sendNumberData);
    connect(m_sendMapBtn, &QPushButton::clicked, this, &CustomEventSender::sendMapData);
    connect(m_sendListBtn, &QPushButton::clicked, this, &CustomEventSender::sendListData);
}

void CustomEventSender::setupCommandEventControls()
{
    m_commandEventGroup = new QGroupBox("命令事件发送", this);
    QVBoxLayout* layout = new QVBoxLayout(m_commandEventGroup);
    
    // 命令输入
    QHBoxLayout* commandLayout = new QHBoxLayout();
    commandLayout->addWidget(new QLabel("命令:"));
    m_commandEdit = new QLineEdit("execute_task", this);
    m_sendSimpleCommandBtn = new QPushButton("发送简单命令", this);
    commandLayout->addWidget(m_commandEdit);
    commandLayout->addWidget(m_sendSimpleCommandBtn);
    layout->addLayout(commandLayout);
    
    // 参数输入
    layout->addWidget(new QLabel("参数 (JSON格式):"));
    m_parametersEdit = new QTextEdit(this);
    m_parametersEdit->setMaximumHeight(80);
    m_parametersEdit->setPlainText("{\"task_id\": \"001\", \"priority\": \"high\", \"timeout\": 30}");
    m_sendCommandBtn = new QPushButton("发送带参数命令", this);
    layout->addWidget(m_parametersEdit);
    layout->addWidget(m_sendCommandBtn);
    
    m_mainLayout->addWidget(m_commandEventGroup);
    
    // 连接信号
    connect(m_sendSimpleCommandBtn, &QPushButton::clicked, this, &CustomEventSender::sendSimpleCommand);
    connect(m_sendCommandBtn, &QPushButton::clicked, this, &CustomEventSender::sendParameterizedCommand);
}

void CustomEventSender::setupBatchControls()
{
    m_batchGroup = new QGroupBox("批量和定时发送", this);
    QVBoxLayout* layout = new QVBoxLayout(m_batchGroup);
    
    // 批量发送
    QHBoxLayout* batchLayout = new QHBoxLayout();
    batchLayout->addWidget(new QLabel("批量数量:"));
    m_batchCountSpin = new QSpinBox(this);
    m_batchCountSpin->setRange(1, 1000);
    m_batchCountSpin->setValue(10);
    
    m_batchTypeCombo = new QComboBox(this);
    m_batchTypeCombo->addItems({"数据事件", "命令事件", "混合事件"});
    
    m_sendBatchBtn = new QPushButton("批量发送", this);
    
    batchLayout->addWidget(m_batchCountSpin);
    batchLayout->addWidget(m_batchTypeCombo);
    batchLayout->addWidget(m_sendBatchBtn);
    layout->addLayout(batchLayout);
    
    // 定时发送
    QHBoxLayout* periodicLayout = new QHBoxLayout();
    periodicLayout->addWidget(new QLabel("发送间隔(ms):"));
    m_intervalSpin = new QSpinBox(this);
    m_intervalSpin->setRange(100, 10000);
    m_intervalSpin->setValue(1000);
    
    m_startPeriodicBtn = new QPushButton("开始定时发送", this);
    m_stopPeriodicBtn = new QPushButton("停止定时发送", this);
    m_stopPeriodicBtn->setEnabled(false);
    
    periodicLayout->addWidget(m_intervalSpin);
    periodicLayout->addWidget(m_startPeriodicBtn);
    periodicLayout->addWidget(m_stopPeriodicBtn);
    layout->addLayout(periodicLayout);
    
    m_mainLayout->addWidget(m_batchGroup);
    
    // 连接信号
    connect(m_sendBatchBtn, &QPushButton::clicked, this, &CustomEventSender::sendBatchEvents);
    connect(m_startPeriodicBtn, &QPushButton::clicked, this, &CustomEventSender::startPeriodicSending);
    connect(m_stopPeriodicBtn, &QPushButton::clicked, this, &CustomEventSender::stopPeriodicSending);
}

void CustomEventSender::setEventTarget(QObject* target)
{
    m_eventTarget = target;
    if (target) {
        m_targetLabel->setText(QString("事件目标: %1 (%2)")
                              .arg(target->objectName().isEmpty() ? "未命名对象" : target->objectName())
                              .arg(target->metaObject()->className()));
    } else {
        m_targetLabel->setText("事件目标: 未设置");
    }
}

QObject* CustomEventSender::eventTarget() const
{
    return m_eventTarget;
}

void CustomEventSender::sendDataEvent()
{
    // 发送一个通用的数据事件
    sendStringData();
}

void CustomEventSender::sendStringData()
{
    if (!m_eventTarget) return;
    
    QString text = m_stringDataEdit->text();
    DataEvent* event = new DataEvent(QVariant(text));
    postEvent(event);
    
    emit eventSent("DataEvent", QString("字符串: \"%1\"").arg(text));
}

void CustomEventSender::sendNumberData()
{
    if (!m_eventTarget) return;
    
    int number = m_numberDataSpin->value();
    DataEvent* event = new DataEvent(QVariant(number));
    postEvent(event);
    
    emit eventSent("DataEvent", QString("数字: %1").arg(number));
}

void CustomEventSender::sendMapData()
{
    if (!m_eventTarget) return;
    
    QString jsonText = m_mapDataEdit->toPlainText();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonText.toUtf8(), &error);
    
    if (error.error != QJsonParseError::NoError) {
        emit eventSent("DataEvent", QString("JSON解析错误: %1").arg(error.errorString()));
        return;
    }
    
    QVariantMap map = doc.object().toVariantMap();
    DataEvent* event = new DataEvent(QVariant(map));
    postEvent(event);
    
    emit eventSent("DataEvent", QString("Map数据: %1个键值对").arg(map.size()));
}

void CustomEventSender::sendListData()
{
    if (!m_eventTarget) return;
    
    QString jsonText = m_listDataEdit->toPlainText();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonText.toUtf8(), &error);
    
    if (error.error != QJsonParseError::NoError) {
        emit eventSent("DataEvent", QString("JSON解析错误: %1").arg(error.errorString()));
        return;
    }
    
    QVariantList list = doc.array().toVariantList();
    DataEvent* event = new DataEvent(QVariant(list));
    postEvent(event);
    
    emit eventSent("DataEvent", QString("List数据: %1个元素").arg(list.size()));
}

void CustomEventSender::sendCommandEvent()
{
    // 发送一个通用的命令事件
    sendSimpleCommand();
}

void CustomEventSender::sendSimpleCommand()
{
    if (!m_eventTarget) return;
    
    QString command = m_commandEdit->text();
    CommandEvent* event = new CommandEvent(command);
    postEvent(event);
    
    emit eventSent("CommandEvent", QString("简单命令: \"%1\"").arg(command));
}

void CustomEventSender::sendParameterizedCommand()
{
    if (!m_eventTarget) return;
    
    QString command = m_commandEdit->text();
    QString jsonText = m_parametersEdit->toPlainText();
    
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonText.toUtf8(), &error);
    
    if (error.error != QJsonParseError::NoError) {
        emit eventSent("CommandEvent", QString("参数JSON解析错误: %1").arg(error.errorString()));
        return;
    }
    
    QVariantMap params = doc.object().toVariantMap();
    CommandEvent* event = new CommandEvent(command, params);
    postEvent(event);
    
    emit eventSent("CommandEvent", QString("带参数命令: \"%1\" (%2个参数)").arg(command).arg(params.size()));
}

void CustomEventSender::sendBatchEvents()
{
    if (!m_eventTarget) return;
    
    int count = m_batchCountSpin->value();
    QString type = m_batchTypeCombo->currentText();
    
    for (int i = 0; i < count; ++i) {
        if (type == "数据事件") {
            DataEvent* event = new DataEvent(QString("批量数据 #%1").arg(i + 1));
            postEvent(event);
        } else if (type == "命令事件") {
            QVariantMap params;
            params["batch_index"] = i + 1;
            params["total_count"] = count;
            CommandEvent* event = new CommandEvent("batch_command", params);
            postEvent(event);
        } else { // 混合事件
            if (i % 2 == 0) {
                DataEvent* event = new DataEvent(QString("混合数据 #%1").arg(i + 1));
                postEvent(event);
            } else {
                QVariantMap params;
                params["index"] = i + 1;
                CommandEvent* event = new CommandEvent("mixed_command", params);
                postEvent(event);
            }
        }
    }
    
    emit batchEventsSent(count);
    emit eventSent("BatchEvents", QString("批量发送 %1 个 %2").arg(count).arg(type));
}

void CustomEventSender::startPeriodicSending()
{
    int interval = m_intervalSpin->value();
    m_periodicTimer->start(interval);
    
    m_startPeriodicBtn->setEnabled(false);
    m_stopPeriodicBtn->setEnabled(true);
    m_intervalSpin->setEnabled(false);
    
    emit eventSent("PeriodicSending", QString("开始定时发送，间隔: %1ms").arg(interval));
}

void CustomEventSender::stopPeriodicSending()
{
    m_periodicTimer->stop();
    
    m_startPeriodicBtn->setEnabled(true);
    m_stopPeriodicBtn->setEnabled(false);
    m_intervalSpin->setEnabled(true);
    
    emit eventSent("PeriodicSending", "停止定时发送");
}

void CustomEventSender::onPeriodicTimer()
{
    if (!m_eventTarget) return;
    
    // 交替发送数据事件和命令事件
    static int counter = 0;
    counter++;
    
    if (counter % 2 == 1) {
        DataEvent* event = new DataEvent(QString("定时数据 #%1").arg(counter));
        postEvent(event);
    } else {
        QVariantMap params;
        params["timer_count"] = counter;
        params["timestamp"] = QDateTime::currentDateTime().toString();
        CommandEvent* event = new CommandEvent("periodic_command", params);
        postEvent(event);
    }
}

void CustomEventSender::postEvent(BaseCustomEvent* event)
{
    if (m_eventTarget && event) {
        QApplication::postEvent(m_eventTarget, event);
        m_eventsSent++;
        m_statusLabel->setText(QString("已发送事件: %1").arg(m_eventsSent));
    }
}

void CustomEventSender::sendEvent(BaseCustomEvent* event)
{
    if (m_eventTarget && event) {
        QApplication::sendEvent(m_eventTarget, event);
        m_eventsSent++;
        m_statusLabel->setText(QString("已发送事件: %1").arg(m_eventsSent));
        delete event; // sendEvent不会自动删除事件对象
    }
}