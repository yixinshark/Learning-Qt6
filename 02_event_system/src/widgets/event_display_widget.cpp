#include "event_display_widget.h"
#include <QHeaderView>
#include <QScrollBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>

EventDisplayWidget::EventDisplayWidget(QWidget* parent)
    : QWidget(parent)
    , m_mainLayout(nullptr)
    , m_filterLayout(nullptr)
    , m_controlLayout(nullptr)
    , m_eventTable(nullptr)
    , m_eventModel(nullptr)
    , m_filterEdit(nullptr)
    , m_eventTypeFilter(nullptr)
    , m_autoScrollCheck(nullptr)
    , m_clearButton(nullptr)
    , m_exportButton(nullptr)
    , m_eventCountLabel(nullptr)
    , m_filteredCountLabel(nullptr)
    , m_updateTimer(nullptr)
    , m_autoScroll(true)
{
    setupUI();
    
    // 连接事件日志器
    EventLogger* logger = EventLogger::instance();
    connect(logger, &EventLogger::eventLogged, this, &EventDisplayWidget::addEventRecord);
    
    // 设置更新定时器
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &EventDisplayWidget::updateEventDisplay);
    m_updateTimer->start(100); // 每100ms更新一次显示
    
    qDebug() << "EventDisplayWidget initialized";
}

void EventDisplayWidget::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(5, 5, 5, 5);
    m_mainLayout->setSpacing(5);
    
    // 标题
    QLabel* titleLabel = new QLabel("事件日志显示");
    titleLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #2c3e50;");
    m_mainLayout->addWidget(titleLabel);
    
    setupFilterControls();
    setupEventTable();
    
    // 信息显示
    QHBoxLayout* infoLayout = new QHBoxLayout();
    m_eventCountLabel = new QLabel("总事件: 0");
    m_filteredCountLabel = new QLabel("显示: 0");
    
    infoLayout->addWidget(m_eventCountLabel);
    infoLayout->addWidget(m_filteredCountLabel);
    infoLayout->addStretch();
    
    m_mainLayout->addLayout(infoLayout);
}

void EventDisplayWidget::setupFilterControls()
{
    // 过滤控件布局
    m_filterLayout = new QHBoxLayout();
    
    // 搜索框
    QLabel* filterLabel = new QLabel("过滤:");
    m_filterEdit = new QLineEdit();
    m_filterEdit->setPlaceholderText("输入关键词过滤事件...");
    connect(m_filterEdit, &QLineEdit::textChanged, this, &EventDisplayWidget::onFilterTextChanged);
    
    // 事件类型过滤
    QLabel* typeLabel = new QLabel("类型:");
    m_eventTypeFilter = new QComboBox();
    m_eventTypeFilter->addItem("全部", -1);
    m_eventTypeFilter->addItem("鼠标事件", QEvent::MouseButtonPress);
    m_eventTypeFilter->addItem("键盘事件", QEvent::KeyPress);
    m_eventTypeFilter->addItem("绘制事件", QEvent::Paint);
    m_eventTypeFilter->addItem("自定义事件", QEvent::User);
    connect(m_eventTypeFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &EventDisplayWidget::onEventTypeFilterChanged);
    
    m_filterLayout->addWidget(filterLabel);
    m_filterLayout->addWidget(m_filterEdit, 1);
    m_filterLayout->addWidget(typeLabel);
    m_filterLayout->addWidget(m_eventTypeFilter);
    
    m_mainLayout->addLayout(m_filterLayout);
    
    // 控制按钮布局
    m_controlLayout = new QHBoxLayout();
    
    // 自动滚动复选框
    m_autoScrollCheck = new QCheckBox("自动滚动");
    m_autoScrollCheck->setChecked(true);
    connect(m_autoScrollCheck, &QCheckBox::toggled, this, &EventDisplayWidget::onAutoScrollToggled);
    
    // 清除按钮
    m_clearButton = new QPushButton("清除");
    connect(m_clearButton, &QPushButton::clicked, this, &EventDisplayWidget::clearEventHistory);
    
    // 导出按钮
    m_exportButton = new QPushButton("导出");
    connect(m_exportButton, &QPushButton::clicked, [this]() {
        QString fileName = QFileDialog::getSaveFileName(this, "导出事件日志", 
                                                       "event_log.csv", "CSV Files (*.csv)");
        if (!fileName.isEmpty()) {
            // 导出逻辑
            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << "时间,事件类型,源对象,目标对象,已接受,详情\n";
                for (const auto& record : m_filteredEvents) {
                    out << record.timestamp.toString("hh:mm:ss.zzz") << ","
                        << record.eventName << ","
                        << (record.sender ? record.sender->objectName() : "N/A") << ","
                        << (record.receiver ? record.receiver->objectName() : "N/A") << ","
                        << (record.accepted ? "是" : "否") << ","
                        << record.details << "\n";
                }
                QMessageBox::information(this, "导出完成", "事件日志已导出到: " + fileName);
            }
        }
    });
    
    m_controlLayout->addWidget(m_autoScrollCheck);
    m_controlLayout->addStretch();
    m_controlLayout->addWidget(m_clearButton);
    m_controlLayout->addWidget(m_exportButton);
    
    m_mainLayout->addLayout(m_controlLayout);
}

void EventDisplayWidget::setupEventTable()
{
    // 创建表格模型
    m_eventModel = new QStandardItemModel(this);
    m_eventModel->setHorizontalHeaderLabels({
        "时间", "事件类型", "源对象", "目标对象", "已接受", "详情"
    });
    
    // 创建表格视图
    m_eventTable = new QTableView();
    m_eventTable->setModel(m_eventModel);
    m_eventTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_eventTable->setAlternatingRowColors(true);
    m_eventTable->setSortingEnabled(true);
    
    // 设置列宽
    QHeaderView* header = m_eventTable->horizontalHeader();
    header->setStretchLastSection(true);
    header->resizeSection(TimeColumn, 100);
    header->resizeSection(TypeColumn, 120);
    header->resizeSection(SourceColumn, 100);
    header->resizeSection(TargetColumn, 100);
    header->resizeSection(AcceptedColumn, 60);
    
    // 连接选择变化信号
    connect(m_eventTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &EventDisplayWidget::onEventSelectionChanged);
    
    m_mainLayout->addWidget(m_eventTable, 1);
}

void EventDisplayWidget::addEventRecord(const EventLogger::EventRecord& record)
{
    m_allEvents.append(record);
    
    if (passesFilter(record)) {
        m_filteredEvents.append(record);
        
        // 添加到表格模型
        int row = m_eventModel->rowCount();
        m_eventModel->insertRow(row);
        
        m_eventModel->setItem(row, TimeColumn, 
            new QStandardItem(record.timestamp.toString("hh:mm:ss.zzz")));
        m_eventModel->setItem(row, TypeColumn, 
            new QStandardItem(record.eventName));
        m_eventModel->setItem(row, SourceColumn, 
            new QStandardItem(record.sender ? record.sender->objectName() : "N/A"));
        m_eventModel->setItem(row, TargetColumn, 
            new QStandardItem(record.receiver ? record.receiver->objectName() : "N/A"));
        m_eventModel->setItem(row, AcceptedColumn, 
            new QStandardItem(record.accepted ? "是" : "否"));
        m_eventModel->setItem(row, DetailsColumn, 
            new QStandardItem(record.details));
        
        // 自动滚动到底部
        if (m_autoScroll) {
            m_eventTable->scrollToBottom();
        }
    }
    
    updateEventCount();
}

void EventDisplayWidget::clearEventHistory()
{
    m_allEvents.clear();
    m_filteredEvents.clear();
    m_eventModel->clear();
    m_eventModel->setHorizontalHeaderLabels({
        "时间", "事件类型", "源对象", "目标对象", "已接受", "详情"
    });
    updateEventCount();
    
    // 同时清除EventLogger的历史
    EventLogger::instance()->clearHistory();
}

void EventDisplayWidget::applyFilter()
{
    m_filteredEvents.clear();
    m_eventModel->clear();
    m_eventModel->setHorizontalHeaderLabels({
        "时间", "事件类型", "源对象", "目标对象", "已接受", "详情"
    });
    
    for (const auto& record : m_allEvents) {
        if (passesFilter(record)) {
            addEventRecord(record);
        }
    }
}

bool EventDisplayWidget::passesFilter(const EventLogger::EventRecord& record) const
{
    // 文本过滤
    QString filterText = m_filterEdit->text().toLower();
    if (!filterText.isEmpty()) {
        bool textMatch = record.eventName.toLower().contains(filterText) ||
                        record.details.toLower().contains(filterText) ||
                        (record.sender && record.sender->objectName().toLower().contains(filterText)) ||
                        (record.receiver && record.receiver->objectName().toLower().contains(filterText));
        if (!textMatch) {
            return false;
        }
    }
    
    // 事件类型过滤
    int selectedType = m_eventTypeFilter->currentData().toInt();
    if (selectedType != -1) {
        if (static_cast<int>(record.eventType) != selectedType) {
            return false;
        }
    }
    
    return true;
}

void EventDisplayWidget::onEventSelectionChanged()
{
    // 可以在这里添加选中事件的详细信息显示
    QModelIndexList selected = m_eventTable->selectionModel()->selectedRows();
    if (!selected.isEmpty()) {
        int row = selected.first().row();
        if (row < m_filteredEvents.size()) {
            const EventLogger::EventRecord& record = m_filteredEvents[row];
            qDebug() << "Selected event:" << record.eventName << "at" << record.timestamp;
        }
    }
}

void EventDisplayWidget::onFilterTextChanged()
{
    applyFilter();
}

void EventDisplayWidget::onEventTypeFilterChanged()
{
    applyFilter();
}

void EventDisplayWidget::onAutoScrollToggled(bool enabled)
{
    m_autoScroll = enabled;
}

void EventDisplayWidget::updateEventDisplay()
{
    updateEventCount();
}

void EventDisplayWidget::updateEventCount()
{
    m_eventCountLabel->setText(QString("总事件: %1").arg(m_allEvents.size()));
    m_filteredCountLabel->setText(QString("显示: %1").arg(m_filteredEvents.size()));
}