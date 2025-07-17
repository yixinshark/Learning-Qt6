#include "performance_monitor_widget.h"
#include <QHeaderView>
#include <QDateTime>
#include <QDebug>
#include <QSplitter>
#include <QScrollBar>

PerformanceMonitorWidget::PerformanceMonitorWidget(QWidget *parent)
    : QWidget(parent)
    , m_mainLayout(nullptr)
    , m_realTimeGroup(nullptr)
    , m_eventsPerSecondLabel(nullptr)
    , m_avgProcessingTimeLabel(nullptr)
    , m_totalEventsLabel(nullptr)
    , m_cpuUsageBar(nullptr)
    , m_controlLayout(nullptr)
    , m_resetButton(nullptr)
    , m_toggleButton(nullptr)
    , m_eventTypeGroup(nullptr)
    , m_eventTypeTable(nullptr)
    , m_objectGroup(nullptr)
    , m_objectTable(nullptr)
    , m_chartGroup(nullptr)
    , m_performanceTrendDisplay(nullptr)
    , m_updateTimer(nullptr)
    , m_eventLogger(EventLogger::instance())
{
    qDebug() << "PerformanceMonitorWidget: Starting construction";
    
    setupUI();
    qDebug() << "PerformanceMonitorWidget: UI setup completed";
    
    // 连接EventLogger的信号
    connect(m_eventLogger, &EventLogger::performanceUpdate,
            this, &PerformanceMonitorWidget::onPerformanceUpdate);
    qDebug() << "PerformanceMonitorWidget: Signal connected";
    
    // 重新启用定时器，但使用更长的间隔
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &PerformanceMonitorWidget::updateDisplay);
    m_updateTimer->start(2000); // 使用2秒间隔，减少更新频率
    qDebug() << "PerformanceMonitorWidget: Timer started with 2s interval";
    
    // 延迟初始更新
    QTimer::singleShot(500, this, &PerformanceMonitorWidget::updatePerformanceData);
    qDebug() << "PerformanceMonitorWidget: Construction completed";
}

void PerformanceMonitorWidget::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    
    // 创建分割器来组织布局
    QSplitter* mainSplitter = new QSplitter(Qt::Vertical, this);
    m_mainLayout->addWidget(mainSplitter);
    
    // 实时统计区域
    m_realTimeGroup = new QGroupBox("实时性能统计", this);
    QGridLayout* realTimeLayout = new QGridLayout(m_realTimeGroup);
    
    m_eventsPerSecondLabel = new QLabel("每秒事件数: 0", this);
    m_avgProcessingTimeLabel = new QLabel("平均处理时间: 0.0 ms", this);
    m_totalEventsLabel = new QLabel("总事件数: 0", this);
    
    m_cpuUsageBar = new QProgressBar(this);
    m_cpuUsageBar->setRange(0, 100);
    m_cpuUsageBar->setValue(0);
    m_cpuUsageBar->setFormat("CPU使用率: %p%");
    
    realTimeLayout->addWidget(m_eventsPerSecondLabel, 0, 0);
    realTimeLayout->addWidget(m_avgProcessingTimeLabel, 0, 1);
    realTimeLayout->addWidget(m_totalEventsLabel, 1, 0);
    realTimeLayout->addWidget(m_cpuUsageBar, 1, 1);
    
    mainSplitter->addWidget(m_realTimeGroup);
    
    // 控制按钮区域
    QWidget* controlWidget = new QWidget(this);
    m_controlLayout = new QHBoxLayout(controlWidget);
    
    m_resetButton = new QPushButton("重置统计", this);
    m_toggleButton = new QPushButton("禁用监控", this);
    m_toggleButton->setCheckable(true);
    
    connect(m_resetButton, &QPushButton::clicked, this, &PerformanceMonitorWidget::resetStatistics);
    connect(m_toggleButton, &QPushButton::toggled, this, &PerformanceMonitorWidget::togglePerformanceMonitoring);
    
    m_controlLayout->addWidget(m_resetButton);
    m_controlLayout->addWidget(m_toggleButton);
    m_controlLayout->addStretch();
    
    mainSplitter->addWidget(controlWidget);
    
    // 创建水平分割器用于表格和趋势显示
    QSplitter* contentSplitter = new QSplitter(Qt::Horizontal, this);
    
    // 左侧：统计表格
    QWidget* tablesWidget = new QWidget(this);
    QVBoxLayout* tablesLayout = new QVBoxLayout(tablesWidget);
    
    // 事件类型统计表格
    m_eventTypeGroup = new QGroupBox("事件类型统计", this);
    QVBoxLayout* eventTypeLayout = new QVBoxLayout(m_eventTypeGroup);
    
    m_eventTypeTable = new QTableWidget(this);
    m_eventTypeTable->setColumnCount(3);
    QStringList eventTypeHeaders;
    eventTypeHeaders << "事件类型" << "数量" << "平均时间(ms)";
    m_eventTypeTable->setHorizontalHeaderLabels(eventTypeHeaders);
    m_eventTypeTable->horizontalHeader()->setStretchLastSection(true);
    
    eventTypeLayout->addWidget(m_eventTypeTable);
    tablesLayout->addWidget(m_eventTypeGroup);
    
    // 对象统计表格
    m_objectGroup = new QGroupBox("对象统计", this);
    QVBoxLayout* objectLayout = new QVBoxLayout(m_objectGroup);
    
    m_objectTable = new QTableWidget(this);
    m_objectTable->setColumnCount(3);
    QStringList objectHeaders;
    objectHeaders << "对象" << "数量" << "平均时间(ms)";
    m_objectTable->setHorizontalHeaderLabels(objectHeaders);
    m_objectTable->horizontalHeader()->setStretchLastSection(true);
    
    objectLayout->addWidget(m_objectTable);
    tablesLayout->addWidget(m_objectGroup);
    
    contentSplitter->addWidget(tablesWidget);
    
    // 右侧：性能趋势显示
    m_chartGroup = new QGroupBox("性能趋势", this);
    QVBoxLayout* chartLayout = new QVBoxLayout(m_chartGroup);
    
    m_performanceTrendDisplay = new QPlainTextEdit(this);
    m_performanceTrendDisplay->setReadOnly(true);
    m_performanceTrendDisplay->setMaximumBlockCount(100); // 限制显示行数
    m_performanceTrendDisplay->setPlainText("性能趋势数据将在这里显示...\n");
    
    chartLayout->addWidget(m_performanceTrendDisplay);
    contentSplitter->addWidget(m_chartGroup);
    
    mainSplitter->addWidget(contentSplitter);
    
    // 设置分割器比例
    mainSplitter->setStretchFactor(0, 0); // 实时统计区域固定高度
    mainSplitter->setStretchFactor(1, 0); // 控制按钮区域固定高度
    mainSplitter->setStretchFactor(2, 1); // 内容区域可伸缩
    
    contentSplitter->setStretchFactor(0, 1); // 表格区域
    contentSplitter->setStretchFactor(1, 1); // 趋势显示区域
}

void PerformanceMonitorWidget::updatePerformanceData()
{
    updateRealTimeStats();
    updateEventTypeTable();
    updateObjectTable();
}

void PerformanceMonitorWidget::onPerformanceUpdate(double avgTime, int eventsPerSecond)
{
    updateTrendDisplay(avgTime, eventsPerSecond);
}

void PerformanceMonitorWidget::resetStatistics()
{
    m_eventLogger->resetPerformanceStats();
    
    // 清空趋势显示
    m_eventsPerSecondHistory.clear();
    m_avgTimeHistory.clear();
    m_performanceTrendDisplay->clear();
    m_performanceTrendDisplay->appendPlainText("性能统计已重置...\n");
    
    // 更新显示
    updatePerformanceData();
    
    qDebug() << "Performance statistics reset";
}

void PerformanceMonitorWidget::togglePerformanceMonitoring(bool enabled)
{
    m_eventLogger->setPerformanceMonitoringEnabled(!enabled);
    m_toggleButton->setText(enabled ? "启用监控" : "禁用监控");
    
    qDebug() << "Performance monitoring" << (enabled ? "disabled" : "enabled");
}

void PerformanceMonitorWidget::updateDisplay()
{
    updatePerformanceData();
}

void PerformanceMonitorWidget::updateRealTimeStats()
{
    QHash<QString, QVariant> stats = m_eventLogger->getPerformanceStats();
    
    int totalEvents = stats.value("totalEvents", 0).toInt();
    int eventsPerSecond = stats.value("eventsPerSecond", 0).toInt();
    
    m_totalEventsLabel->setText(QString("总事件数: %1").arg(totalEvents));
    m_eventsPerSecondLabel->setText(QString("每秒事件数: %1").arg(eventsPerSecond));
    
    // 计算总体平均处理时间
    double totalAvgTime = 0.0;
    int typeCount = 0;
    
    QVariantMap eventTypes = stats.value("eventTypes").toMap();
    for (auto it = eventTypes.begin(); it != eventTypes.end(); ++it) {
        QVariantMap typeStats = it.value().toMap();
        double avgTime = typeStats.value("avgTime", 0.0).toDouble();
        if (avgTime > 0) {
            totalAvgTime += avgTime;
            typeCount++;
        }
    }
    
    if (typeCount > 0) {
        totalAvgTime /= typeCount;
    }
    
    m_avgProcessingTimeLabel->setText(QString("平均处理时间: %1 ms").arg(totalAvgTime, 0, 'f', 2));
    
    // 模拟CPU使用率（基于事件频率）
    int cpuUsage = qMin(eventsPerSecond * 2, 100); // 简单的启发式计算
    m_cpuUsageBar->setValue(cpuUsage);
}

void PerformanceMonitorWidget::updateEventTypeTable()
{
    QHash<QString, QVariant> stats = m_eventLogger->getPerformanceStats();
    QVariantMap eventTypes = stats.value("eventTypes").toMap();
    
    m_eventTypeTable->setRowCount(eventTypes.size());
    
    int row = 0;
    for (auto it = eventTypes.begin(); it != eventTypes.end(); ++it, ++row) {
        QString typeName = it.key();
        QVariantMap typeStats = it.value().toMap();
        
        int count = typeStats.value("count", 0).toInt();
        double avgTime = typeStats.value("avgTime", 0.0).toDouble();
        
        m_eventTypeTable->setItem(row, 0, new QTableWidgetItem(typeName));
        m_eventTypeTable->setItem(row, 1, new QTableWidgetItem(QString::number(count)));
        m_eventTypeTable->setItem(row, 2, new QTableWidgetItem(QString::number(avgTime, 'f', 2)));
    }
}

void PerformanceMonitorWidget::updateObjectTable()
{
    QHash<QString, QVariant> stats = m_eventLogger->getPerformanceStats();
    QVariantMap objects = stats.value("objects").toMap();
    
    m_objectTable->setRowCount(objects.size());
    
    int row = 0;
    for (auto it = objects.begin(); it != objects.end(); ++it, ++row) {
        QString objectName = it.key();
        QVariantMap objectStats = it.value().toMap();
        
        int count = objectStats.value("count", 0).toInt();
        double avgTime = objectStats.value("avgTime", 0.0).toDouble();
        
        m_objectTable->setItem(row, 0, new QTableWidgetItem(objectName));
        m_objectTable->setItem(row, 1, new QTableWidgetItem(QString::number(count)));
        m_objectTable->setItem(row, 2, new QTableWidgetItem(QString::number(avgTime, 'f', 2)));
    }
}

void PerformanceMonitorWidget::updateTrendDisplay(double avgTime, int eventsPerSecond)
{
    QDateTime now = QDateTime::currentDateTime();
    
    // 添加新数据点
    m_eventsPerSecondHistory.append(qMakePair(now, eventsPerSecond));
    m_avgTimeHistory.append(qMakePair(now, avgTime));
    
    // 限制历史数据点数量
    while (m_eventsPerSecondHistory.size() > MAX_CHART_POINTS) {
        m_eventsPerSecondHistory.removeFirst();
    }
    while (m_avgTimeHistory.size() > MAX_CHART_POINTS) {
        m_avgTimeHistory.removeFirst();
    }
    
    // 更新趋势显示
    QString trendText = QString("[%1] 事件/秒: %2, 平均时间: %3ms\n")
                        .arg(now.toString("hh:mm:ss"))
                        .arg(eventsPerSecond)
                        .arg(avgTime, 0, 'f', 2);
    
    m_performanceTrendDisplay->appendPlainText(trendText);
    
    // 自动滚动到底部
    QScrollBar* scrollBar = m_performanceTrendDisplay->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}