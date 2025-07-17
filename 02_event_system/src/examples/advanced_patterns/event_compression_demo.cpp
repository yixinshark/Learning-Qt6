#include "event_compression_demo.h"
#include <QApplication>
#include <QPainter>
#include <QRandomGenerator>
#include <QDebug>

EventCompressionDemo::EventCompressionDemo(QWidget *parent)
    : QWidget(parent)
    , m_mainLayout(nullptr)
    , m_controlLayout(nullptr)
    , m_controlGroup(nullptr)
    , m_displayGroup(nullptr)
    , m_logGroup(nullptr)
    , m_compressionEnabled(nullptr)
    , m_compressionInterval(nullptr)
    , m_generateEventsBtn(nullptr)
    , m_clearLogBtn(nullptr)
    , m_statusLabel(nullptr)
    , m_eventLoadBar(nullptr)
    , m_mousePositionLabel(nullptr)
    , m_paintCountLabel(nullptr)
    , m_dataUpdateLabel(nullptr)
    , m_compressionRatioLabel(nullptr)
    , m_logTextEdit(nullptr)
    , m_mouseCompressionTimer(nullptr)
    , m_paintCompressionTimer(nullptr)
    , m_dataCompressionTimer(nullptr)
    , m_totalMouseEvents(0)
    , m_compressedMouseEvents(0)
    , m_totalPaintEvents(0)
    , m_compressedPaintEvents(0)
    , m_totalDataUpdates(0)
    , m_compressedDataUpdates(0)
    , m_compressionEnabledFlag(true)
    , m_compressionIntervalMs(50)
{
    setupUI();
    
    // 初始化定时器
    m_mouseCompressionTimer = new QTimer(this);
    m_paintCompressionTimer = new QTimer(this);
    m_dataCompressionTimer = new QTimer(this);
    
    m_mouseCompressionTimer->setSingleShot(true);
    m_paintCompressionTimer->setSingleShot(true);
    m_dataCompressionTimer->setSingleShot(true);
    
    // 连接定时器信号
    connect(m_mouseCompressionTimer, &QTimer::timeout,
            this, &EventCompressionDemo::processCompressedMouseEvents);
    connect(m_paintCompressionTimer, &QTimer::timeout,
            this, &EventCompressionDemo::processCompressedPaintEvents);
    connect(m_dataCompressionTimer, &QTimer::timeout,
            this, &EventCompressionDemo::processBatchedDataUpdates);
    
    // 启用鼠标跟踪
    setMouseTracking(true);
    
    // 初始化性能计时器
    m_performanceTimer.start();
    
    logEvent("事件压缩演示初始化完成");
}

EventCompressionDemo::~EventCompressionDemo()
{
    // 析构函数，Qt会自动清理子对象
}

void EventCompressionDemo::setupUI()
{
    setWindowTitle("高级事件处理 - 事件压缩演示");
    setMinimumSize(800, 600);
    
    m_mainLayout = new QVBoxLayout(this);
    
    // 控制面板
    m_controlGroup = new QGroupBox("控制面板", this);
    m_controlLayout = new QHBoxLayout(m_controlGroup);
    
    m_compressionEnabled = new QCheckBox("启用事件压缩", this);
    m_compressionEnabled->setChecked(true);
    
    m_compressionInterval = new QSpinBox(this);
    m_compressionInterval->setRange(10, 1000);
    m_compressionInterval->setValue(50);
    m_compressionInterval->setSuffix(" ms");
    
    m_generateEventsBtn = new QPushButton("生成测试事件", this);
    m_clearLogBtn = new QPushButton("清空日志", this);
    
    m_statusLabel = new QLabel("状态: 就绪", this);
    m_eventLoadBar = new QProgressBar(this);
    m_eventLoadBar->setRange(0, 100);
    
    m_controlLayout->addWidget(m_compressionEnabled);
    m_controlLayout->addWidget(new QLabel("压缩间隔:"));
    m_controlLayout->addWidget(m_compressionInterval);
    m_controlLayout->addWidget(m_generateEventsBtn);
    m_controlLayout->addWidget(m_clearLogBtn);
    m_controlLayout->addStretch();
    m_controlLayout->addWidget(m_statusLabel);
    m_controlLayout->addWidget(m_eventLoadBar);
    
    // 显示区域
    m_displayGroup = new QGroupBox("事件统计", this);
    QVBoxLayout *displayLayout = new QVBoxLayout(m_displayGroup);
    
    m_mousePositionLabel = new QLabel("鼠标位置: (0, 0)", this);
    m_paintCountLabel = new QLabel("重绘次数: 0", this);
    m_dataUpdateLabel = new QLabel("数据更新: 0", this);
    m_compressionRatioLabel = new QLabel("压缩比率: 0%", this);
    
    displayLayout->addWidget(m_mousePositionLabel);
    displayLayout->addWidget(m_paintCountLabel);
    displayLayout->addWidget(m_dataUpdateLabel);
    displayLayout->addWidget(m_compressionRatioLabel);
    
    // 日志区域
    m_logGroup = new QGroupBox("事件日志", this);
    QVBoxLayout *logLayout = new QVBoxLayout(m_logGroup);
    
    m_logTextEdit = new QTextEdit(this);
    m_logTextEdit->setMaximumHeight(200);
    m_logTextEdit->setReadOnly(true);
    
    logLayout->addWidget(m_logTextEdit);
    
    // 添加到主布局
    m_mainLayout->addWidget(m_controlGroup);
    m_mainLayout->addWidget(m_displayGroup);
    m_mainLayout->addWidget(m_logGroup);
    
    // 连接信号
    connect(m_compressionEnabled, &QCheckBox::toggled,
            this, &EventCompressionDemo::onCompressionEnabledChanged);
    connect(m_compressionInterval, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &EventCompressionDemo::onCompressionIntervalChanged);
    connect(m_generateEventsBtn, &QPushButton::clicked,
            this, &EventCompressionDemo::onGenerateEventsClicked);
    connect(m_clearLogBtn, &QPushButton::clicked,
            this, &EventCompressionDemo::onClearLogClicked);
}

bool EventCompressionDemo::event(QEvent *event)
{
    // 拦截并处理特定类型的事件
    switch (event->type()) {
    case QEvent::UpdateRequest:
        // 处理更新请求事件
        if (m_compressionEnabledFlag) {
            PaintEventData paintData;
            paintData.region = rect();
            paintData.timestamp = QDateTime::currentDateTime();
            
            m_paintEventQueue.enqueue(paintData);
            m_totalPaintEvents++;
            
            if (!m_paintCompressionTimer->isActive()) {
                m_paintCompressionTimer->start(m_compressionIntervalMs);
            }
            
            return true; // 事件已处理
        }
        break;
    
    default:
        break;
    }
    
    return QWidget::event(event);
}

void EventCompressionDemo::mouseMoveEvent(QMouseEvent *event)
{
    if (m_compressionEnabledFlag) {
        // 将鼠标事件添加到队列中进行压缩处理
        MouseEventData mouseData;
        mouseData.position = event->pos();
        mouseData.buttons = event->buttons();
        mouseData.timestamp = QDateTime::currentDateTime();
        
        m_mouseEventQueue.enqueue(mouseData);
        m_totalMouseEvents++;
        
        // 启动或重启压缩定时器
        if (!m_mouseCompressionTimer->isActive()) {
            m_mouseCompressionTimer->start(m_compressionIntervalMs);
        }
    } else {
        // 直接处理鼠标事件
        m_mousePositionLabel->setText(QString("鼠标位置: (%1, %2)")
                                     .arg(event->pos().x())
                                     .arg(event->pos().y()));
        m_totalMouseEvents++;
        updateStatistics();
    }
    
    QWidget::mouseMoveEvent(event);
}

void EventCompressionDemo::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(rect(), QColor(240, 240, 240));
    
    // 绘制一些示例内容
    painter.setPen(QPen(Qt::blue, 2));
    painter.drawText(rect().center() - QPoint(50, 0), "事件压缩演示区域");
    
    QWidget::paintEvent(event);
}

void EventCompressionDemo::onCompressionEnabledChanged(bool enabled)
{
    m_compressionEnabledFlag = enabled;
    m_statusLabel->setText(enabled ? "状态: 压缩已启用" : "状态: 压缩已禁用");
    logEvent(QString("事件压缩%1").arg(enabled ? "已启用" : "已禁用"));
}

void EventCompressionDemo::onCompressionIntervalChanged(int interval)
{
    m_compressionIntervalMs = interval;
    logEvent(QString("压缩间隔设置为 %1 ms").arg(interval));
}

void EventCompressionDemo::onGenerateEventsClicked()
{
    generateTestEvents();
}

void EventCompressionDemo::onClearLogClicked()
{
    m_logTextEdit->clear();
    
    // 重置统计信息
    m_totalMouseEvents = 0;
    m_compressedMouseEvents = 0;
    m_totalPaintEvents = 0;
    m_compressedPaintEvents = 0;
    m_totalDataUpdates = 0;
    m_compressedDataUpdates = 0;
    
    updateStatistics();
    logEvent("日志和统计信息已清空");
}

void EventCompressionDemo::processCompressedMouseEvents()
{
    if (m_mouseEventQueue.isEmpty()) {
        return;
    }
    
    // 压缩算法：只保留最新的鼠标位置
    MouseEventData latestEvent = m_mouseEventQueue.last();
    int queueSize = m_mouseEventQueue.size();
    m_mouseEventQueue.clear();
    
    // 更新UI
    m_mousePositionLabel->setText(QString("鼠标位置: (%1, %2)")
                                 .arg(latestEvent.position.x())
                                 .arg(latestEvent.position.y()));
    
    m_compressedMouseEvents++;
    
    logEvent(QString("压缩处理 %1 个鼠标事件 -> 1 个事件")
             .arg(queueSize));
    
    updateStatistics();
}

void EventCompressionDemo::processCompressedPaintEvents()
{
    if (m_paintEventQueue.isEmpty()) {
        return;
    }
    
    // 压缩算法：合并所有重绘区域
    QRect combinedRegion;
    int queueSize = m_paintEventQueue.size();
    
    while (!m_paintEventQueue.isEmpty()) {
        PaintEventData paintData = m_paintEventQueue.dequeue();
        combinedRegion = combinedRegion.united(paintData.region);
    }
    
    m_compressedPaintEvents++;
    
    // 触发实际的重绘
    update(combinedRegion);
    
    logEvent(QString("压缩处理 %1 个重绘事件 -> 1 个重绘区域")
             .arg(queueSize));
    
    updateStatistics();
}

void EventCompressionDemo::processBatchedDataUpdates()
{
    if (m_dataUpdateQueue.isEmpty()) {
        return;
    }
    
    // 批处理数据更新：按键分组，只保留最新值
    QMap<QString, QVariant> latestUpdates;
    int queueSize = m_dataUpdateQueue.size();
    
    while (!m_dataUpdateQueue.isEmpty()) {
        DataUpdateEvent updateData = m_dataUpdateQueue.dequeue();
        latestUpdates[updateData.key] = updateData.value;
    }
    
    m_compressedDataUpdates++;
    
    // 应用批量更新
    for (auto it = latestUpdates.begin(); it != latestUpdates.end(); ++it) {
        // 这里可以执行实际的数据更新操作
    }
    
    m_dataUpdateLabel->setText(QString("数据更新: %1 批次")
                              .arg(m_compressedDataUpdates));
    
    logEvent(QString("批处理 %1 个数据更新 -> %2 个唯一更新")
             .arg(queueSize)
             .arg(latestUpdates.size()));
    
    updateStatistics();
}

void EventCompressionDemo::logEvent(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    QString logEntry = QString("[%1] %2").arg(timestamp, message);
    
    m_logTextEdit->append(logEntry);
    
    // 自动滚动到底部
    QTextCursor cursor = m_logTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    m_logTextEdit->setTextCursor(cursor);
}

void EventCompressionDemo::updateStatistics()
{
    m_paintCountLabel->setText(QString("重绘次数: %1").arg(m_compressedPaintEvents));
    updateCompressionRatio();
}

void EventCompressionDemo::updateCompressionRatio()
{
    int totalEvents = m_totalMouseEvents + m_totalPaintEvents + m_totalDataUpdates;
    int compressedEvents = m_compressedMouseEvents + m_compressedPaintEvents + m_compressedDataUpdates;
    
    if (totalEvents > 0) {
        double ratio = (1.0 - (double)compressedEvents / totalEvents) * 100.0;
        m_compressionRatioLabel->setText(QString("压缩比率: %1%")
                                        .arg(QString::number(ratio, 'f', 1)));
        
        // 更新进度条
        m_eventLoadBar->setValue(qMin(100, totalEvents / 10));
    }
}

QString EventCompressionDemo::formatTimestamp(const QDateTime &timestamp)
{
    return timestamp.toString("hh:mm:ss.zzz");
}

void EventCompressionDemo::generateTestEvents()
{
    logEvent("开始生成测试事件...");
    
    // 生成模拟的鼠标移动事件
    for (int i = 0; i < 50; ++i) {
        MouseEventData mouseData;
        mouseData.position = QPoint(
            QRandomGenerator::global()->bounded(width()),
            QRandomGenerator::global()->bounded(height())
        );
        mouseData.buttons = Qt::NoButton;
        mouseData.timestamp = QDateTime::currentDateTime();
        
        m_mouseEventQueue.enqueue(mouseData);
        m_totalMouseEvents++;
    }
    
    // 生成模拟的数据更新事件
    QStringList keys = {"temperature", "humidity", "pressure", "wind_speed"};
    for (int i = 0; i < 30; ++i) {
        DataUpdateEvent updateData;
        updateData.key = keys[QRandomGenerator::global()->bounded(keys.size())];
        updateData.value = QRandomGenerator::global()->bounded(100);
        updateData.timestamp = QDateTime::currentDateTime();
        
        m_dataUpdateQueue.enqueue(updateData);
        m_totalDataUpdates++;
    }
    
    // 启动压缩定时器
    if (m_compressionEnabledFlag) {
        if (!m_mouseCompressionTimer->isActive()) {
            m_mouseCompressionTimer->start(m_compressionIntervalMs);
        }
        if (!m_dataCompressionTimer->isActive()) {
            m_dataCompressionTimer->start(m_compressionIntervalMs);
        }
    }
    
    logEvent(QString("生成了 %1 个鼠标事件和 %2 个数据更新事件")
             .arg(50).arg(30));
    
    measureCompressionPerformance();
}

void EventCompressionDemo::measureCompressionPerformance()
{
    qint64 elapsed = m_performanceTimer.elapsed();
    
    logEvent(QString("性能测试 - 运行时间: %1 ms, 总事件数: %2")
             .arg(elapsed)
             .arg(m_totalMouseEvents + m_totalPaintEvents + m_totalDataUpdates));
}