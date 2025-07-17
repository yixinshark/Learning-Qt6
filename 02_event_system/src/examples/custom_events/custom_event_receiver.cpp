#include "custom_event_receiver.h"
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QStringConverter>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

CustomEventReceiver::CustomEventReceiver(QWidget *parent)
    : QWidget(parent)
    , m_mainLayout(nullptr)
    , m_processingEnabled(true)
    , m_dataEventFilterEnabled(true)
    , m_commandEventFilterEnabled(true)
    , m_maxLogEntries(1000)
    , m_statisticsTimer(new QTimer(this))
{
    setupUI();
    
    // 设置统计更新定时器
    m_statisticsTimer->setInterval(1000); // 每秒更新一次
    connect(m_statisticsTimer, &QTimer::timeout, this, &CustomEventReceiver::onUpdateStatistics);
    m_statisticsTimer->start();
}

void CustomEventReceiver::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    
    setupControlPanel();
    setupEventLog();
    setupStatisticsPanel();
    
    // 详细信息显示
    m_detailGroup = new QGroupBox("最后接收事件详情", this);
    QVBoxLayout* detailLayout = new QVBoxLayout(m_detailGroup);
    
    m_lastEventDetail = new QTextEdit(this);
    m_lastEventDetail->setMaximumHeight(120);
    m_lastEventDetail->setReadOnly(true);
    m_lastEventDetail->setPlainText("等待接收事件...");
    detailLayout->addWidget(m_lastEventDetail);
    
    m_mainLayout->addWidget(m_detailGroup);
}

void CustomEventReceiver::setupControlPanel()
{
    m_controlGroup = new QGroupBox("接收控制", this);
    QVBoxLayout* layout = new QVBoxLayout(m_controlGroup);
    
    // 第一行：处理开关和过滤器
    QHBoxLayout* row1 = new QHBoxLayout();
    m_enableProcessingCheck = new QCheckBox("启用事件处理", this);
    m_enableProcessingCheck->setChecked(m_processingEnabled);
    
    m_dataEventFilterCheck = new QCheckBox("接收数据事件", this);
    m_dataEventFilterCheck->setChecked(m_dataEventFilterEnabled);
    
    m_commandEventFilterCheck = new QCheckBox("接收命令事件", this);
    m_commandEventFilterCheck->setChecked(m_commandEventFilterEnabled);
    
    row1->addWidget(m_enableProcessingCheck);
    row1->addWidget(m_dataEventFilterCheck);
    row1->addWidget(m_commandEventFilterCheck);
    row1->addStretch();
    layout->addLayout(row1);
    
    // 第二行：日志设置和操作按钮
    QHBoxLayout* row2 = new QHBoxLayout();
    row2->addWidget(new QLabel("最大日志条数:"));
    
    m_maxLogEntriesSpin = new QSpinBox(this);
    m_maxLogEntriesSpin->setRange(10, 10000);
    m_maxLogEntriesSpin->setValue(m_maxLogEntries);
    row2->addWidget(m_maxLogEntriesSpin);
    
    m_clearLogBtn = new QPushButton("清空日志", this);
    m_saveLogBtn = new QPushButton("保存日志", this);
    m_resetStatsBtn = new QPushButton("重置统计", this);
    
    row2->addWidget(m_clearLogBtn);
    row2->addWidget(m_saveLogBtn);
    row2->addWidget(m_resetStatsBtn);
    row2->addStretch();
    layout->addLayout(row2);
    
    m_mainLayout->addWidget(m_controlGroup);
    
    // 连接信号
    connect(m_enableProcessingCheck, &QCheckBox::toggled, this, &CustomEventReceiver::setEventProcessingEnabled);
    connect(m_dataEventFilterCheck, &QCheckBox::toggled, this, &CustomEventReceiver::setDataEventFilter);
    connect(m_commandEventFilterCheck, &QCheckBox::toggled, this, &CustomEventReceiver::setCommandEventFilter);
    connect(m_maxLogEntriesSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &CustomEventReceiver::setMaxLogEntries);
    connect(m_clearLogBtn, &QPushButton::clicked, this, &CustomEventReceiver::clearEventLog);
    connect(m_saveLogBtn, &QPushButton::clicked, this, &CustomEventReceiver::saveEventLog);
    connect(m_resetStatsBtn, &QPushButton::clicked, this, &CustomEventReceiver::resetStatistics);
}

void CustomEventReceiver::setupEventLog()
{
    m_logGroup = new QGroupBox("事件接收日志", this);
    QVBoxLayout* layout = new QVBoxLayout(m_logGroup);
    
    m_eventLogTable = new QTableWidget(this);
    m_eventLogTable->setColumnCount(4);
    m_eventLogTable->setHorizontalHeaderLabels({"时间", "事件类型", "处理状态", "描述"});
    
    // 设置列宽
    QHeaderView* header = m_eventLogTable->horizontalHeader();
    header->setStretchLastSection(true);
    header->resizeSection(0, 120); // 时间列
    header->resizeSection(1, 100); // 事件类型列
    header->resizeSection(2, 80);  // 处理状态列
    
    m_eventLogTable->setAlternatingRowColors(true);
    m_eventLogTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_eventLogTable->setSortingEnabled(true);
    
    layout->addWidget(m_eventLogTable);
    m_mainLayout->addWidget(m_logGroup);
}

void CustomEventReceiver::setupStatisticsPanel()
{
    m_statsGroup = new QGroupBox("接收统计", this);
    QVBoxLayout* layout = new QVBoxLayout(m_statsGroup);
    
    // 第一行：基本统计
    QHBoxLayout* row1 = new QHBoxLayout();
    m_totalEventsLabel = new QLabel("总事件: 0", this);
    m_dataEventsLabel = new QLabel("数据事件: 0", this);
    m_commandEventsLabel = new QLabel("命令事件: 0", this);
    
    row1->addWidget(m_totalEventsLabel);
    row1->addWidget(m_dataEventsLabel);
    row1->addWidget(m_commandEventsLabel);
    row1->addStretch();
    layout->addLayout(row1);
    
    // 第二行：处理统计
    QHBoxLayout* row2 = new QHBoxLayout();
    m_processedEventsLabel = new QLabel("已处理: 0", this);
    m_ignoredEventsLabel = new QLabel("已忽略: 0", this);
    m_eventRateLabel = new QLabel("事件率: 0/秒", this);
    
    row2->addWidget(m_processedEventsLabel);
    row2->addWidget(m_ignoredEventsLabel);
    row2->addWidget(m_eventRateLabel);
    row2->addStretch();
    layout->addLayout(row2);
    
    // 第三行：时间信息
    QHBoxLayout* row3 = new QHBoxLayout();
    m_firstEventLabel = new QLabel("首次事件: 无", this);
    m_lastEventLabel = new QLabel("最后事件: 无", this);
    
    row3->addWidget(m_firstEventLabel);
    row3->addWidget(m_lastEventLabel);
    row3->addStretch();
    layout->addLayout(row3);
    
    m_mainLayout->addWidget(m_statsGroup);
}

bool CustomEventReceiver::event(QEvent* event)
{
    // 检查是否是自定义事件
    if (event->type() == static_cast<QEvent::Type>(DataEventType)) {
        DataEvent* dataEvent = static_cast<DataEvent*>(event);
        return handleDataEvent(dataEvent);
    } else if (event->type() == static_cast<QEvent::Type>(CommandEventType)) {
        CommandEvent* commandEvent = static_cast<CommandEvent*>(event);
        return handleCommandEvent(commandEvent);
    }
    
    // 调用基类处理其他事件
    return QWidget::event(event);
}

bool CustomEventReceiver::handleDataEvent(DataEvent* event)
{
    if (!m_processingEnabled || !m_dataEventFilterEnabled) {
        m_statistics.ignoredEvents++;
        addLogEntry("DataEvent", QDateTime::currentDateTime().toString("hh:mm:ss.zzz"), 
                   "事件被过滤", false);
        return false;
    }
    
    // 更新统计
    m_statistics.totalEvents++;
    m_statistics.dataEvents++;
    m_statistics.processedEvents++;
    
    if (m_statistics.firstEventTime.isNull()) {
        m_statistics.firstEventTime = QDateTime::currentDateTime();
    }
    m_statistics.lastEventTime = QDateTime::currentDateTime();
    
    // 格式化事件信息
    QString description = formatDataEventInfo(event);
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    
    // 添加到日志
    addLogEntry("DataEvent", timestamp, description, true);
    
    // 更新详细信息显示
    QString detail = QString("数据事件详情:\n时间戳: %1\n数据类型: %2\n数据内容: %3\n序列化大小: %4 字节")
                    .arg(QDateTime::fromMSecsSinceEpoch(event->timestamp()).toString())
                    .arg(event->dataTypeName())
                    .arg(formatEventData(event->data()))
                    .arg(event->serialize().size());
    m_lastEventDetail->setPlainText(detail);
    
    // 发送信号
    emit eventReceived("DataEvent", description);
    emit dataEventReceived(event->data());
    
    event->accept();
    return true;
}

bool CustomEventReceiver::handleCommandEvent(CommandEvent* event)
{
    if (!m_processingEnabled || !m_commandEventFilterEnabled) {
        m_statistics.ignoredEvents++;
        addLogEntry("CommandEvent", QDateTime::currentDateTime().toString("hh:mm:ss.zzz"), 
                   "事件被过滤", false);
        return false;
    }
    
    // 更新统计
    m_statistics.totalEvents++;
    m_statistics.commandEvents++;
    m_statistics.processedEvents++;
    
    if (m_statistics.firstEventTime.isNull()) {
        m_statistics.firstEventTime = QDateTime::currentDateTime();
    }
    m_statistics.lastEventTime = QDateTime::currentDateTime();
    
    // 格式化事件信息
    QString description = formatCommandEventInfo(event);
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    
    // 添加到日志
    addLogEntry("CommandEvent", timestamp, description, true);
    
    // 更新详细信息显示
    QString detail = QString("命令事件详情:\n时间戳: %1\n命令: %2\n参数数量: %3\n参数内容: %4\n序列化大小: %5 字节")
                    .arg(QDateTime::fromMSecsSinceEpoch(event->timestamp()).toString())
                    .arg(event->command())
                    .arg(event->parameters().size())
                    .arg(formatEventData(QVariant(event->parameters())))
                    .arg(event->serialize().size());
    m_lastEventDetail->setPlainText(detail);
    
    // 发送信号
    emit eventReceived("CommandEvent", description);
    emit commandEventReceived(event->command(), event->parameters());
    
    event->accept();
    return true;
}

QString CustomEventReceiver::formatDataEventInfo(DataEvent* event)
{
    QString dataStr = formatEventData(event->data());
    if (dataStr.length() > 50) {
        dataStr = dataStr.left(47) + "...";
    }
    return QString("%1: %2").arg(event->dataTypeName()).arg(dataStr);
}

QString CustomEventReceiver::formatCommandEventInfo(CommandEvent* event)
{
    return QString("命令: %1 (%2个参数)").arg(event->command()).arg(event->parameters().size());
}

QString CustomEventReceiver::formatEventData(const QVariant& data)
{
    switch (data.typeId()) {
    case QMetaType::QString:
        return QString("\"%1\"").arg(data.toString());
    case QMetaType::Int:
    case QMetaType::Double:
        return data.toString();
    case QMetaType::Bool:
        return data.toBool() ? "true" : "false";
    case QMetaType::QVariantMap: {
        QVariantMap map = data.toMap();
        QStringList items;
        for (auto it = map.begin(); it != map.end(); ++it) {
            items << QString("%1: %2").arg(it.key()).arg(formatEventData(it.value()));
        }
        return QString("{%1}").arg(items.join(", "));
    }
    case QMetaType::QVariantList: {
        QVariantList list = data.toList();
        QStringList items;
        for (const QVariant& item : list) {
            items << formatEventData(item);
        }
        return QString("[%1]").arg(items.join(", "));
    }
    default:
        return data.toString();
    }
}

void CustomEventReceiver::addLogEntry(const QString& eventType, const QString& timestamp, 
                                     const QString& description, bool processed)
{
    // 检查是否需要删除旧条目
    if (m_eventLogTable->rowCount() >= m_maxLogEntries) {
        m_eventLogTable->removeRow(0);
    }
    
    // 添加新行
    int row = m_eventLogTable->rowCount();
    m_eventLogTable->insertRow(row);
    
    m_eventLogTable->setItem(row, 0, new QTableWidgetItem(timestamp));
    m_eventLogTable->setItem(row, 1, new QTableWidgetItem(eventType));
    m_eventLogTable->setItem(row, 2, new QTableWidgetItem(processed ? "已处理" : "已忽略"));
    m_eventLogTable->setItem(row, 3, new QTableWidgetItem(description));
    
    // 设置行颜色
    if (!processed) {
        for (int col = 0; col < 4; ++col) {
            m_eventLogTable->item(row, col)->setBackground(QColor(255, 240, 240)); // 淡红色
        }
    }
    
    // 滚动到最新条目
    m_eventLogTable->scrollToBottom();
}

void CustomEventReceiver::updateStatisticsDisplay()
{
    m_totalEventsLabel->setText(QString("总事件: %1").arg(m_statistics.totalEvents));
    m_dataEventsLabel->setText(QString("数据事件: %1").arg(m_statistics.dataEvents));
    m_commandEventsLabel->setText(QString("命令事件: %1").arg(m_statistics.commandEvents));
    m_processedEventsLabel->setText(QString("已处理: %1").arg(m_statistics.processedEvents));
    m_ignoredEventsLabel->setText(QString("已忽略: %1").arg(m_statistics.ignoredEvents));
    
    if (!m_statistics.firstEventTime.isNull()) {
        m_firstEventLabel->setText(QString("首次事件: %1").arg(m_statistics.firstEventTime.toString("hh:mm:ss")));
    }
    if (!m_statistics.lastEventTime.isNull()) {
        m_lastEventLabel->setText(QString("最后事件: %1").arg(m_statistics.lastEventTime.toString("hh:mm:ss")));
    }
    
    // 计算事件率
    if (!m_statistics.firstEventTime.isNull() && !m_statistics.lastEventTime.isNull()) {
        qint64 duration = m_statistics.firstEventTime.msecsTo(m_statistics.lastEventTime);
        if (duration > 0) {
            double rate = (double)m_statistics.totalEvents * 1000.0 / duration;
            m_eventRateLabel->setText(QString("事件率: %1/秒").arg(rate, 0, 'f', 1));
        }
    }
}

const CustomEventReceiver::EventStatistics& CustomEventReceiver::statistics() const
{
    return m_statistics;
}

void CustomEventReceiver::resetStatistics()
{
    m_statistics = EventStatistics();
    updateStatisticsDisplay();
    emit statisticsUpdated(m_statistics);
}

void CustomEventReceiver::setEventProcessingEnabled(bool enabled)
{
    m_processingEnabled = enabled;
}

void CustomEventReceiver::setDataEventFilter(bool enabled)
{
    m_dataEventFilterEnabled = enabled;
}

void CustomEventReceiver::setCommandEventFilter(bool enabled)
{
    m_commandEventFilterEnabled = enabled;
}

void CustomEventReceiver::setMaxLogEntries(int maxEntries)
{
    m_maxLogEntries = maxEntries;
    
    // 如果当前日志条数超过新限制，删除多余的条目
    while (m_eventLogTable->rowCount() > m_maxLogEntries) {
        m_eventLogTable->removeRow(0);
    }
}

void CustomEventReceiver::clearEventLog()
{
    m_eventLogTable->setRowCount(0);
    m_lastEventDetail->setPlainText("等待接收事件...");
}

void CustomEventReceiver::saveEventLog()
{
    QString fileName = QFileDialog::getSaveFileName(this, "保存事件日志", 
                                                   QString("event_log_%1.txt")
                                                   .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")),
                                                   "文本文件 (*.txt);;所有文件 (*.*)");
    
    if (fileName.isEmpty()) return;
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "保存失败", QString("无法创建文件: %1").arg(fileName));
        return;
    }
    
    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    
    // 写入头部信息
    out << "Qt自定义事件系统 - 事件接收日志\n";
    out << "生成时间: " << QDateTime::currentDateTime().toString() << "\n";
    out << "总事件数: " << m_statistics.totalEvents << "\n";
    out << "数据事件: " << m_statistics.dataEvents << "\n";
    out << "命令事件: " << m_statistics.commandEvents << "\n";
    out << "已处理: " << m_statistics.processedEvents << "\n";
    out << "已忽略: " << m_statistics.ignoredEvents << "\n";
    out << "\n" << QString("-").repeated(80) << "\n\n";
    
    // 写入日志条目
    for (int row = 0; row < m_eventLogTable->rowCount(); ++row) {
        QString timestamp = m_eventLogTable->item(row, 0)->text();
        QString eventType = m_eventLogTable->item(row, 1)->text();
        QString status = m_eventLogTable->item(row, 2)->text();
        QString description = m_eventLogTable->item(row, 3)->text();
        
        out << QString("[%1] %2 - %3: %4\n")
               .arg(timestamp)
               .arg(eventType)
               .arg(status)
               .arg(description);
    }
    
    file.close();
    QMessageBox::information(this, "保存成功", QString("事件日志已保存到: %1").arg(fileName));
}

void CustomEventReceiver::onUpdateStatistics()
{
    updateStatisticsDisplay();
    emit statisticsUpdated(m_statistics);
}