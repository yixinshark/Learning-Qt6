#include "debug_panel_widget.h"
#include <QApplication>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QProcess>
#include <QDebug>
#include <QRandomGenerator>

DebugPanelWidget::DebugPanelWidget(QWidget* parent)
    : QWidget(parent)
    , m_tabWidget(nullptr)
    , m_totalEventCount(0)
    , m_lastUpdateTime(QDateTime::currentDateTime())
    , m_debugMode(false)
    , m_verboseLogging(false)
    , m_performanceMonitoring(true)
{
    setupUI();
    
    // 连接事件日志器
    EventLogger* logger = EventLogger::instance();
    connect(logger, &EventLogger::eventLogged, this, &DebugPanelWidget::onEventLogged);
    
    // 设置性能监控定时器
    m_performanceTimer = new QTimer(this);
    connect(m_performanceTimer, &QTimer::timeout, this, &DebugPanelWidget::onPerformanceTimerTimeout);
    m_performanceTimer->start(1000); // 每秒更新一次
    
    // 初始化对象层次结构
    // 延迟初始化，避免构造函数中的潜在问题
    QTimer::singleShot(1000, this, &DebugPanelWidget::refreshObjectHierarchy);
    
    qDebug() << "DebugPanelWidget initialized";
}

void DebugPanelWidget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 5);
    
    // 标题
    QLabel* titleLabel = new QLabel("调试面板");
    titleLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #2c3e50;");
    mainLayout->addWidget(titleLabel);
    
    // 创建标签页控件
    m_tabWidget = new QTabWidget();
    
    setupObjectHierarchyTab();
    setupPerformanceTab();
    setupDebugControlTab();
    
    mainLayout->addWidget(m_tabWidget);
}

void DebugPanelWidget::setupObjectHierarchyTab()
{
    m_hierarchyTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(m_hierarchyTab);
    
    // 控制按钮
    QHBoxLayout* controlLayout = new QHBoxLayout();
    m_refreshButton = new QPushButton("刷新");
    connect(m_refreshButton, &QPushButton::clicked, this, &DebugPanelWidget::refreshObjectHierarchy);
    controlLayout->addWidget(m_refreshButton);
    
    m_highlightPathButton = new QPushButton("高亮路径");
    connect(m_highlightPathButton, &QPushButton::clicked, [this]() {
        // 演示事件传播路径高亮功能
        QModelIndexList selected = m_objectTreeView->selectionModel()->selectedRows();
        if (!selected.isEmpty()) {
            QObject* selectedObj = m_objectHierarchyModel->getObject(selected.first());
            if (selectedObj && selectedObj->parent()) {
                // 高亮从选中对象到其父对象的路径
                // TODO: 实现高亮功能
                qDebug() << "Highlighting path from" << selectedObj->objectName() << "to parent";
            }
        }
    });
    controlLayout->addWidget(m_highlightPathButton);
    
    m_clearHighlightButton = new QPushButton("清除高亮");
    connect(m_clearHighlightButton, &QPushButton::clicked, [this]() {
        if (m_objectHierarchyModel) {
            // TODO: 实现清除高亮功能
            qDebug() << "Clearing highlight";
        }
    });
    controlLayout->addWidget(m_clearHighlightButton);
    
    controlLayout->addStretch();
    layout->addLayout(controlLayout);
    
    // 对象树视图
    m_objectHierarchyModel = new ObjectHierarchyModel(this);
    
    m_objectTreeView = new QTreeView();
    m_objectTreeView->setModel(m_objectHierarchyModel);
    m_objectTreeView->setAlternatingRowColors(true);
    m_objectTreeView->setRootIsDecorated(true);
    m_objectTreeView->setExpandsOnDoubleClick(true);
    connect(m_objectTreeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &DebugPanelWidget::onObjectSelected);
    layout->addWidget(m_objectTreeView, 1);
    
    // 选中对象信息
    m_selectedObjectLabel = new QLabel("选中对象: 无");
    m_selectedObjectLabel->setStyleSheet("font-weight: bold;");
    layout->addWidget(m_selectedObjectLabel);
    
    // 对象详情
    QLabel* detailsLabel = new QLabel("对象详情:");
    layout->addWidget(detailsLabel);
    
    m_objectDetailsText = new QTextEdit();
    m_objectDetailsText->setMaximumHeight(100);
    m_objectDetailsText->setReadOnly(true);
    layout->addWidget(m_objectDetailsText);
    
    m_tabWidget->addTab(m_hierarchyTab, "对象层次");
}

void DebugPanelWidget::setupPerformanceTab()
{
    m_performanceTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(m_performanceTab);
    
    // 性能指标显示
    QGroupBox* metricsGroup = new QGroupBox("性能指标");
    QGridLayout* metricsLayout = new QGridLayout(metricsGroup);
    
    metricsLayout->addWidget(new QLabel("总事件数:"), 0, 0);
    m_totalEventsLabel = new QLabel("0");
    metricsLayout->addWidget(m_totalEventsLabel, 0, 1);
    
    metricsLayout->addWidget(new QLabel("事件/秒:"), 1, 0);
    m_eventsPerSecondLabel = new QLabel("0");
    metricsLayout->addWidget(m_eventsPerSecondLabel, 1, 1);
    
    metricsLayout->addWidget(new QLabel("平均处理时间:"), 2, 0);
    m_avgProcessingTimeLabel = new QLabel("0 ms");
    metricsLayout->addWidget(m_avgProcessingTimeLabel, 2, 1);
    
    metricsLayout->addWidget(new QLabel("内存使用:"), 3, 0);
    m_memoryUsageLabel = new QLabel("0 MB");
    metricsLayout->addWidget(m_memoryUsageLabel, 3, 1);
    
    // CPU使用率进度条
    metricsLayout->addWidget(new QLabel("CPU使用率:"), 4, 0);
    m_cpuUsageBar = new QProgressBar();
    m_cpuUsageBar->setRange(0, 100);
    metricsLayout->addWidget(m_cpuUsageBar, 4, 1);
    
    // 内存使用率进度条
    metricsLayout->addWidget(new QLabel("内存使用率:"), 5, 0);
    m_memoryUsageBar = new QProgressBar();
    m_memoryUsageBar->setRange(0, 100);
    metricsLayout->addWidget(m_memoryUsageBar, 5, 1);
    
    layout->addWidget(metricsGroup);
    
    // 事件统计表格
    QLabel* statsLabel = new QLabel("事件类型统计:");
    layout->addWidget(statsLabel);
    
    m_eventStatsModel = new QStandardItemModel(this);
    m_eventStatsModel->setHorizontalHeaderLabels({"事件类型", "数量", "百分比"});
    
    m_eventStatsTable = new QTableView();
    m_eventStatsTable->setModel(m_eventStatsModel);
    m_eventStatsTable->setAlternatingRowColors(true);
    m_eventStatsTable->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(m_eventStatsTable, 1);
    
    m_tabWidget->addTab(m_performanceTab, "性能监控");
}

void DebugPanelWidget::setupDebugControlTab()
{
    m_debugControlTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(m_debugControlTab);
    
    // 调试选项
    QGroupBox* optionsGroup = new QGroupBox("调试选项");
    QVBoxLayout* optionsLayout = new QVBoxLayout(optionsGroup);
    
    m_debugModeCheck = new QCheckBox("启用调试模式");
    connect(m_debugModeCheck, &QCheckBox::toggled, this, &DebugPanelWidget::onDebugModeToggled);
    optionsLayout->addWidget(m_debugModeCheck);
    
    m_verboseLoggingCheck = new QCheckBox("详细日志记录");
    optionsLayout->addWidget(m_verboseLoggingCheck);
    
    m_performanceMonitoringCheck = new QCheckBox("性能监控");
    m_performanceMonitoringCheck->setChecked(true);
    optionsLayout->addWidget(m_performanceMonitoringCheck);
    
    layout->addWidget(optionsGroup);
    
    // 配置选项
    QGroupBox* configGroup = new QGroupBox("配置");
    QGridLayout* configLayout = new QGridLayout(configGroup);
    
    configLayout->addWidget(new QLabel("最大事件数:"), 0, 0);
    m_maxEventsSpinBox = new QSpinBox();
    m_maxEventsSpinBox->setRange(100, 10000);
    m_maxEventsSpinBox->setValue(1000);
    configLayout->addWidget(m_maxEventsSpinBox, 0, 1);
    
    configLayout->addWidget(new QLabel("更新间隔(ms):"), 1, 0);
    m_updateIntervalSpinBox = new QSpinBox();
    m_updateIntervalSpinBox->setRange(100, 5000);
    m_updateIntervalSpinBox->setValue(1000);
    connect(m_updateIntervalSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int value) {
                m_performanceTimer->setInterval(value);
            });
    configLayout->addWidget(m_updateIntervalSpinBox, 1, 1);
    
    layout->addWidget(configGroup);
    
    // 控制按钮
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    m_exportButton = new QPushButton("导出调试信息");
    connect(m_exportButton, &QPushButton::clicked, this, &DebugPanelWidget::exportDebugInfo);
    buttonLayout->addWidget(m_exportButton);
    
    m_clearButton = new QPushButton("清除数据");
    connect(m_clearButton, &QPushButton::clicked, this, &DebugPanelWidget::clearDebugData);
    buttonLayout->addWidget(m_clearButton);
    
    buttonLayout->addStretch();
    layout->addLayout(buttonLayout);
    
    // 调试输出
    QLabel* outputLabel = new QLabel("调试输出:");
    layout->addWidget(outputLabel);
    
    m_debugOutputText = new QTextEdit();
    m_debugOutputText->setReadOnly(true);
    m_debugOutputText->setMaximumHeight(150);
    layout->addWidget(m_debugOutputText);
    
    m_tabWidget->addTab(m_debugControlTab, "调试控制");
}

void DebugPanelWidget::refreshObjectHierarchy()
{
    if (m_objectHierarchyModel) {
        m_objectHierarchyModel->refresh();
        m_objectTreeView->expandAll();
    }
}



void DebugPanelWidget::onObjectSelected()
{
    QModelIndexList selected = m_objectTreeView->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        m_selectedObjectLabel->setText("选中对象: 无");
        m_objectDetailsText->clear();
        return;
    }
    
    QModelIndex index = selected.first();
    QObject* obj = m_objectHierarchyModel->getObject(index);
    
    if (obj) {
        QString objectName = obj->objectName().isEmpty() ? 
                           QString("<%1>").arg(obj->metaObject()->className()) :
                           obj->objectName();
        
        m_selectedObjectLabel->setText(QString("选中对象: %1").arg(objectName));
        
        // 显示对象详情
        QString details;
        details += QString("类名: %1\n").arg(obj->metaObject()->className());
        details += QString("对象名: %1\n").arg(obj->objectName().isEmpty() ? "未设置" : obj->objectName());
        details += QString("父对象: %1\n").arg(obj->parent() ? obj->parent()->objectName() : "无");
        details += QString("子对象数: %1\n").arg(obj->children().size());
        details += QString("事件数: %1\n").arg(0); // TODO: 实现事件计数功能
        
        // 显示属性
        const QMetaObject* metaObj = obj->metaObject();
        details += QString("属性数: %1\n").arg(metaObj->propertyCount());
        
        // 如果是QWidget，显示额外信息
        if (QWidget* widget = qobject_cast<QWidget*>(obj)) {
            details += QString("可见: %1\n").arg(widget->isVisible() ? "是" : "否");
            details += QString("启用: %1\n").arg(widget->isEnabled() ? "是" : "否");
            details += QString("大小: %1x%2").arg(widget->width()).arg(widget->height());
        }
        
        m_objectDetailsText->setText(details);
        
        // 发出对象选择信号
        // TODO: 实现对象选择信号
        qDebug() << "Object selected:" << objectName;
    }
}

void DebugPanelWidget::onEventLogged(const EventLogger::EventRecord& record)
{
    m_totalEventCount++;
    m_eventTypeStats[record.eventType]++;
    
    // 更新对象层次模型中的事件计数
    if (m_objectHierarchyModel && record.receiver) {
        // TODO: 实现事件计数功能
        // m_objectHierarchyModel->incrementEventCount(record.receiver);
    }
    
    // 记录处理时间（模拟）
    double processingTime = 0.1 + (QRandomGenerator::global()->bounded(50)) / 100.0; // 0.1-0.6ms
    m_processingTimes.append(processingTime);
    if (m_processingTimes.size() > 1000) {
        m_processingTimes.removeFirst();
    }
    
    if (m_debugMode && m_verboseLoggingCheck->isChecked()) {
        QString debugMsg = QString("[%1] %2: %3 -> %4")
                          .arg(record.timestamp.toString("hh:mm:ss.zzz"))
                          .arg(record.eventName)
                          .arg(record.sender ? record.sender->objectName() : "N/A")
                          .arg(record.receiver ? record.receiver->objectName() : "N/A");
        
        m_debugOutputText->append(debugMsg);
        
        // 限制调试输出行数
        QTextDocument* doc = m_debugOutputText->document();
        if (doc->blockCount() > 100) {
            QTextCursor cursor = m_debugOutputText->textCursor();
            cursor.movePosition(QTextCursor::Start);
            cursor.select(QTextCursor::BlockUnderCursor);
            cursor.removeSelectedText();
            cursor.deleteChar();
        }
        
        m_debugOutputText->moveCursor(QTextCursor::End);
    }
}

void DebugPanelWidget::onDebugModeToggled(bool enabled)
{
    m_debugMode = enabled;
    if (enabled) {
        m_debugOutputText->append("=== 调试模式已启用 ===");
    } else {
        m_debugOutputText->append("=== 调试模式已禁用 ===");
    }
}

void DebugPanelWidget::onEventFilterChanged()
{
    // 实现事件过滤逻辑
}

void DebugPanelWidget::onPerformanceTimerTimeout()
{
    if (m_performanceMonitoringCheck->isChecked()) {
        updatePerformanceMetrics();
        updateEventStatistics();
    }
}

void DebugPanelWidget::updatePerformanceMetrics()
{
    // 更新总事件数
    m_totalEventsLabel->setText(QString::number(m_totalEventCount));
    
    // 计算事件/秒
    QDateTime now = QDateTime::currentDateTime();
    qint64 elapsed = m_lastUpdateTime.msecsTo(now);
    if (elapsed > 0) {
        static int lastEventCount = 0;
        int eventDiff = m_totalEventCount - lastEventCount;
        double eventsPerSecond = (eventDiff * 1000.0) / elapsed;
        m_eventsPerSecondLabel->setText(QString::number(eventsPerSecond, 'f', 1));
        lastEventCount = m_totalEventCount;
    }
    m_lastUpdateTime = now;
    
    // 计算平均处理时间
    if (!m_processingTimes.isEmpty()) {
        double sum = 0;
        for (double time : m_processingTimes) {
            sum += time;
        }
        double avgTime = sum / m_processingTimes.size();
        m_avgProcessingTimeLabel->setText(QString("%1 ms").arg(avgTime, 0, 'f', 2));
    }
    
    // 模拟CPU和内存使用率
    int cpuUsage = 10 + (QRandomGenerator::global()->bounded(20)); // 10-30%
    int memoryUsage = 30 + (QRandomGenerator::global()->bounded(40)); // 30-70%
    
    m_cpuUsageBar->setValue(cpuUsage);
    m_memoryUsageBar->setValue(memoryUsage);
    
    m_memoryUsageLabel->setText(QString("%1 MB").arg(50 + memoryUsage));
}

void DebugPanelWidget::updateEventStatistics()
{
    m_eventStatsModel->clear();
    m_eventStatsModel->setHorizontalHeaderLabels({"事件类型", "数量", "百分比"});
    
    for (auto it = m_eventTypeStats.begin(); it != m_eventTypeStats.end(); ++it) {
        QEvent::Type type = it.key();
        int count = it.value();
        double percentage = (double)count / m_totalEventCount * 100.0;
        
        QString typeName = QString("Event_%1").arg(static_cast<int>(type));
        // 可以添加更多已知事件类型的名称映射
        
        QList<QStandardItem*> row;
        row << new QStandardItem(typeName);
        row << new QStandardItem(QString::number(count));
        row << new QStandardItem(QString("%1%").arg(percentage, 0, 'f', 1));
        
        m_eventStatsModel->appendRow(row);
    }
}

void DebugPanelWidget::exportDebugInfo()
{
    QString fileName = QFileDialog::getSaveFileName(this, "导出调试信息", 
                                                   "debug_info.json", "JSON Files (*.json)");
    if (fileName.isEmpty()) return;
    
    QJsonObject debugInfo;
    debugInfo["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    debugInfo["total_events"] = m_totalEventCount;
    debugInfo["debug_mode"] = m_debugMode;
    debugInfo["verbose_logging"] = m_verboseLoggingCheck->isChecked();
    debugInfo["performance_monitoring"] = m_performanceMonitoringCheck->isChecked();
    
    // 事件统计
    QJsonObject eventStats;
    for (auto it = m_eventTypeStats.begin(); it != m_eventTypeStats.end(); ++it) {
        eventStats[QString::number(static_cast<int>(it.key()))] = it.value();
    }
    debugInfo["event_statistics"] = eventStats;
    
    // 性能数据
    QJsonObject performance;
    if (!m_processingTimes.isEmpty()) {
        double sum = 0;
        for (double time : m_processingTimes) {
            sum += time;
        }
        performance["avg_processing_time"] = sum / m_processingTimes.size();
    }
    debugInfo["performance"] = performance;
    
    QJsonDocument doc(debugInfo);
    
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        QMessageBox::information(this, "导出完成", "调试信息已导出到: " + fileName);
    } else {
        QMessageBox::warning(this, "导出失败", "无法写入文件: " + fileName);
    }
}

void DebugPanelWidget::updatePerformanceStats()
{
    // 这个方法被MOC生成的代码调用，但实际功能由updatePerformanceMetrics实现
    updatePerformanceMetrics();
}

void DebugPanelWidget::clearDebugData()
{
    m_totalEventCount = 0;
    m_eventTypeStats.clear();
    m_processingTimes.clear();
    m_debugOutputText->clear();
    
    updatePerformanceMetrics();
    updateEventStatistics();
    
    m_debugOutputText->append("=== 调试数据已清除 ===");
}