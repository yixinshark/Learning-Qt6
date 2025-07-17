#include "event_pooling_demo.h"
#include <QApplication>
#include <QRandomGenerator>
#include <QDebug>
#include <QCheckBox>

// EventPool 实现
EventPool::EventPool(int initialSize)
    : m_totalEvents(0)
    , m_initialSize(initialSize)
{
    createEvents(initialSize);
}

EventPool::~EventPool()
{
    clearPool();
}

PooledEvent* EventPool::acquireEvent()
{
    QMutexLocker locker(&m_mutex);
    
    if (m_availableEvents.isEmpty()) {
        // 池为空，创建新的事件对象
        createEvents(m_initialSize / 2); // 扩展一半的初始大小
    }
    
    PooledEvent* event = m_availableEvents.pop();
    event->setInUse(true);
    return event;
}

void EventPool::releaseEvent(PooledEvent* event)
{
    if (!event) return;
    
    QMutexLocker locker(&m_mutex);
    
    event->reset();
    event->setInUse(false);
    m_availableEvents.push(event);
}

void EventPool::expandPool(int additionalSize)
{
    QMutexLocker locker(&m_mutex);
    createEvents(additionalSize);
}

void EventPool::shrinkPool()
{
    QMutexLocker locker(&m_mutex);
    
    // 只保留一半的可用事件
    int targetSize = m_availableEvents.size() / 2;
    while (m_availableEvents.size() > targetSize) {
        PooledEvent* event = m_availableEvents.pop();
        delete event;
        m_totalEvents--;
    }
}

void EventPool::clearPool()
{
    QMutexLocker locker(&m_mutex);
    
    while (!m_availableEvents.isEmpty()) {
        delete m_availableEvents.pop();
    }
    m_totalEvents = 0;
}

void EventPool::createEvents(int count)
{
    for (int i = 0; i < count; ++i) {
        PooledEvent* event = new PooledEvent();
        m_availableEvents.push(event);
        m_totalEvents++;
    }
}

// EventPoolingDemo 实现
EventPoolingDemo::EventPoolingDemo(QWidget *parent)
    : QWidget(parent)
    , m_mainLayout(nullptr)
    , m_controlGroup(nullptr)
    , m_statisticsGroup(nullptr)
    , m_logGroup(nullptr)
    , m_generateEventsBtn(nullptr)
    , m_clearPoolBtn(nullptr)
    , m_poolSizeSpinBox(nullptr)
    , m_autoExpandCheckBox(nullptr)
    , m_statusLabel(nullptr)
    , m_totalEventsLabel(nullptr)
    , m_availableEventsLabel(nullptr)
    , m_usedEventsLabel(nullptr)
    , m_poolEfficiencyLabel(nullptr)
    , m_poolUsageBar(nullptr)
    , m_memoryUsageLabel(nullptr)
    , m_performanceLabel(nullptr)
    , m_logTextEdit(nullptr)
    , m_eventPool(nullptr)
    , m_statisticsTimer(nullptr)
    , m_eventProcessingTimer(nullptr)
    , m_eventsProcessed(0)
    , m_totalEventsGenerated(0)
    , m_totalProcessingTime(0)
    , m_autoExpandEnabled(true)
    , m_maxPoolSize(1000)
{
    setupUI();
    
    // 初始化事件池
    m_eventPool = std::make_unique<EventPool>(100);
    
    // 初始化定时器
    m_statisticsTimer = new QTimer(this);
    m_eventProcessingTimer = new QTimer(this);
    
    connect(m_statisticsTimer, &QTimer::timeout,
            this, &EventPoolingDemo::updateStatistics);
    connect(m_eventProcessingTimer, &QTimer::timeout,
            this, &EventPoolingDemo::processPooledEvents);
    
    // 启动统计定时器
    m_statisticsTimer->start(1000); // 每秒更新一次统计信息
    
    // 启动性能计时器
    m_performanceTimer.start();
    
    logMessage("事件池化演示初始化完成");
    updateStatistics();
}

EventPoolingDemo::~EventPoolingDemo()
{
    // 清理待处理的事件
    while (!m_pendingEvents.isEmpty()) {
        PooledEvent* event = m_pendingEvents.dequeue();
        m_eventPool->releaseEvent(event);
    }
}

void EventPoolingDemo::setupUI()
{
    setWindowTitle("高级事件处理 - 事件池化演示");
    setMinimumSize(800, 700);
    
    m_mainLayout = new QVBoxLayout(this);
    
    // 控制面板
    m_controlGroup = new QGroupBox("控制面板", this);
    QHBoxLayout *controlLayout = new QHBoxLayout(m_controlGroup);
    
    m_generateEventsBtn = new QPushButton("生成测试事件", this);
    m_clearPoolBtn = new QPushButton("清空事件池", this);
    
    m_poolSizeSpinBox = new QSpinBox(this);
    m_poolSizeSpinBox->setRange(10, 1000);
    m_poolSizeSpinBox->setValue(100);
    m_poolSizeSpinBox->setSuffix(" 个");
    
    m_autoExpandCheckBox = new QCheckBox("自动扩展池", this);
    m_autoExpandCheckBox->setChecked(true);
    
    m_statusLabel = new QLabel("状态: 就绪", this);
    
    controlLayout->addWidget(m_generateEventsBtn);
    controlLayout->addWidget(m_clearPoolBtn);
    controlLayout->addWidget(new QLabel("池大小:"));
    controlLayout->addWidget(m_poolSizeSpinBox);
    controlLayout->addWidget(m_autoExpandCheckBox);
    controlLayout->addStretch();
    controlLayout->addWidget(m_statusLabel);
    
    // 统计面板
    m_statisticsGroup = new QGroupBox("池统计信息", this);
    QVBoxLayout *statsLayout = new QVBoxLayout(m_statisticsGroup);
    
    QHBoxLayout *statsRow1 = new QHBoxLayout();
    m_totalEventsLabel = new QLabel("总事件数: 0", this);
    m_availableEventsLabel = new QLabel("可用事件: 0", this);
    m_usedEventsLabel = new QLabel("使用中: 0", this);
    
    statsRow1->addWidget(m_totalEventsLabel);
    statsRow1->addWidget(m_availableEventsLabel);
    statsRow1->addWidget(m_usedEventsLabel);
    
    QHBoxLayout *statsRow2 = new QHBoxLayout();
    m_poolEfficiencyLabel = new QLabel("池效率: 0%", this);
    m_memoryUsageLabel = new QLabel("内存使用: 0 KB", this);
    m_performanceLabel = new QLabel("处理性能: 0 事件/秒", this);
    
    statsRow2->addWidget(m_poolEfficiencyLabel);
    statsRow2->addWidget(m_memoryUsageLabel);
    statsRow2->addWidget(m_performanceLabel);
    
    m_poolUsageBar = new QProgressBar(this);
    m_poolUsageBar->setRange(0, 100);
    
    statsLayout->addLayout(statsRow1);
    statsLayout->addLayout(statsRow2);
    statsLayout->addWidget(new QLabel("池使用率:", this));
    statsLayout->addWidget(m_poolUsageBar);
    
    // 日志区域
    m_logGroup = new QGroupBox("事件处理日志", this);
    QVBoxLayout *logLayout = new QVBoxLayout(m_logGroup);
    
    m_logTextEdit = new QTextEdit(this);
    m_logTextEdit->setMaximumHeight(200);
    m_logTextEdit->setReadOnly(true);
    
    logLayout->addWidget(m_logTextEdit);
    
    // 添加到主布局
    m_mainLayout->addWidget(m_controlGroup);
    m_mainLayout->addWidget(m_statisticsGroup);
    m_mainLayout->addWidget(m_logGroup);
    
    // 连接信号
    connect(m_generateEventsBtn, &QPushButton::clicked,
            this, &EventPoolingDemo::onGenerateEventsClicked);
    connect(m_clearPoolBtn, &QPushButton::clicked,
            this, &EventPoolingDemo::onClearPoolClicked);
    connect(m_poolSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &EventPoolingDemo::onPoolSizeChanged);
    connect(m_autoExpandCheckBox, &QCheckBox::toggled,
            this, &EventPoolingDemo::onAutoExpandToggled);
}

bool EventPoolingDemo::event(QEvent *event)
{
    if (event->type() == PooledEvent::PooledEventType) {
        PooledEvent *pooledEvent = static_cast<PooledEvent*>(event);
        handlePooledEvent(pooledEvent);
        return true;
    }
    
    return QWidget::event(event);
}

void EventPoolingDemo::onGenerateEventsClicked()
{
    generateTestEvents(50);
}

void EventPoolingDemo::onClearPoolClicked()
{
    // 清理待处理事件
    while (!m_pendingEvents.isEmpty()) {
        PooledEvent* event = m_pendingEvents.dequeue();
        m_eventPool->releaseEvent(event);
    }
    
    // 清空事件池
    m_eventPool->clearPool();
    m_eventPool = std::make_unique<EventPool>(m_poolSizeSpinBox->value());
    
    // 重置统计
    m_eventsProcessed = 0;
    m_totalEventsGenerated = 0;
    m_totalProcessingTime = 0;
    
    logMessage("事件池已清空并重新初始化");
    updateStatistics();
}

void EventPoolingDemo::onPoolSizeChanged(int size)
{
    if (m_eventPool->totalEvents() < size) {
        int additional = size - m_eventPool->totalEvents();
        m_eventPool->expandPool(additional);
        logMessage(QString("事件池扩展了 %1 个事件对象").arg(additional));
    }
    updateStatistics();
}

void EventPoolingDemo::onAutoExpandToggled(bool enabled)
{
    m_autoExpandEnabled = enabled;
    logMessage(QString("自动扩展池%1").arg(enabled ? "已启用" : "已禁用"));
}

void EventPoolingDemo::updateStatistics()
{
    if (!m_eventPool) return;
    
    int total = m_eventPool->totalEvents();
    int available = m_eventPool->availableEvents();
    int used = m_eventPool->usedEvents();
    
    m_totalEventsLabel->setText(QString("总事件数: %1").arg(total));
    m_availableEventsLabel->setText(QString("可用事件: %1").arg(available));
    m_usedEventsLabel->setText(QString("使用中: %1").arg(used));
    
    // 计算池效率
    double efficiency = total > 0 ? (double)used / total * 100.0 : 0.0;
    m_poolEfficiencyLabel->setText(QString("池效率: %1%")
                                  .arg(QString::number(efficiency, 'f', 1)));
    
    // 更新使用率进度条
    int usagePercent = total > 0 ? (used * 100) / total : 0;
    m_poolUsageBar->setValue(usagePercent);
    
    // 估算内存使用
    qint64 memoryUsage = total * sizeof(PooledEvent);
    m_memoryUsageLabel->setText(QString("内存使用: %1").arg(formatMemorySize(memoryUsage)));
    
    // 计算处理性能
    qint64 elapsed = m_performanceTimer.elapsed();
    if (elapsed > 0) {
        double eventsPerSecond = (double)m_eventsProcessed * 1000.0 / elapsed;
        m_performanceLabel->setText(QString("处理性能: %1 事件/秒")
                                   .arg(QString::number(eventsPerSecond, 'f', 1)));
    }
}

void EventPoolingDemo::processPooledEvents()
{
    // 处理待处理队列中的事件
    int processed = 0;
    const int maxBatchSize = 10;
    
    while (!m_pendingEvents.isEmpty() && processed < maxBatchSize) {
        PooledEvent* event = m_pendingEvents.dequeue();
        simulateEventProcessing(event);
        m_eventPool->releaseEvent(event);
        processed++;
        m_eventsProcessed++;
    }
    
    if (processed > 0) {
        logMessage(QString("批处理了 %1 个事件").arg(processed));
    }
    
    // 如果还有待处理事件，继续处理
    if (!m_pendingEvents.isEmpty()) {
        QTimer::singleShot(10, this, &EventPoolingDemo::processPooledEvents);
    }
}

void EventPoolingDemo::logMessage(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    QString logEntry = QString("[%1] %2").arg(timestamp, message);
    
    m_logTextEdit->append(logEntry);
    
    // 自动滚动到底部
    QTextCursor cursor = m_logTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    m_logTextEdit->setTextCursor(cursor);
}

void EventPoolingDemo::generateTestEvents(int count)
{
    logMessage(QString("开始生成 %1 个测试事件...").arg(count));
    
    QElapsedTimer timer;
    timer.start();
    
    for (int i = 0; i < count; ++i) {
        PooledEvent* event = m_eventPool->acquireEvent();
        
        // 设置事件数据
        event->setData("id", i);
        event->setData("type", QString("test_event_%1").arg(i % 5));
        event->setData("value", QRandomGenerator::global()->bounded(1000));
        event->setTimestamp(QDateTime::currentDateTime());
        event->setPriority(QRandomGenerator::global()->bounded(10));
        
        // 添加到待处理队列
        m_pendingEvents.enqueue(event);
        m_totalEventsGenerated++;
    }
    
    qint64 elapsed = timer.elapsed();
    logMessage(QString("事件生成完成，耗时 %1 ms").arg(elapsed));
    
    // 开始处理事件
    if (!m_eventProcessingTimer->isActive()) {
        m_eventProcessingTimer->start(50); // 每50ms处理一批
    }
    
    updateStatistics();
}

void EventPoolingDemo::updatePoolStatistics()
{
    updateStatistics();
}

void EventPoolingDemo::measurePerformance()
{
    // 性能测量在updateStatistics中进行
}

QString EventPoolingDemo::formatMemorySize(qint64 bytes)
{
    if (bytes < 1024) {
        return QString("%1 B").arg(bytes);
    } else if (bytes < 1024 * 1024) {
        return QString("%1 KB").arg(QString::number(bytes / 1024.0, 'f', 1));
    } else {
        return QString("%1 MB").arg(QString::number(bytes / (1024.0 * 1024.0), 'f', 1));
    }
}

void EventPoolingDemo::handlePooledEvent(PooledEvent *event)
{
    // 这个方法处理通过Qt事件系统发送的池化事件
    simulateEventProcessing(event);
    m_eventPool->releaseEvent(event);
    m_eventsProcessed++;
}

void EventPoolingDemo::simulateEventProcessing(PooledEvent *event)
{
    // 模拟事件处理工作
    QElapsedTimer timer;
    timer.start();
    
    // 获取事件数据
    int id = event->getData("id").toInt();
    QString type = event->getData("type").toString();
    int value = event->getData("value").toInt();
    int priority = event->priority();
    
    // 模拟一些处理时间
    QThread::usleep(QRandomGenerator::global()->bounded(100, 500));
    
    qint64 processingTime = timer.elapsed();
    m_totalProcessingTime += processingTime;
    
    // 偶尔记录详细信息
    if (id % 20 == 0) {
        logMessage(QString("处理事件 ID:%1, 类型:%2, 值:%3, 优先级:%4, 耗时:%5μs")
                  .arg(id).arg(type).arg(value).arg(priority).arg(processingTime));
    }
}