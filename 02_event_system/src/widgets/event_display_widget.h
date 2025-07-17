#ifndef EVENT_DISPLAY_WIDGET_H
#define EVENT_DISPLAY_WIDGET_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QTimer>

#include "../core/event_logger.h"

/**
 * @brief 事件显示控件 - 提供事件信息的实时显示
 * 
 * 这个控件负责：
 * - 实时显示事件日志
 * - 提供事件过滤功能
 * - 支持事件搜索
 * - 显示事件详细信息
 */
class EventDisplayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EventDisplayWidget(QWidget* parent = nullptr);
    ~EventDisplayWidget() override = default;

public slots:
    void addEventRecord(const EventLogger::EventRecord& record);
    void clearEventHistory();
    void applyFilter();

private slots:
    void onEventSelectionChanged();
    void onFilterTextChanged();
    void onEventTypeFilterChanged();
    void onAutoScrollToggled(bool enabled);
    void updateEventDisplay();

private:
    void setupUI();
    void setupEventTable();
    void setupFilterControls();
    void updateEventCount();
    bool passesFilter(const EventLogger::EventRecord& record) const;
    
    // UI组件
    QVBoxLayout* m_mainLayout;
    QHBoxLayout* m_filterLayout;
    QHBoxLayout* m_controlLayout;
    
    // 事件表格
    QTableView* m_eventTable;
    QStandardItemModel* m_eventModel;
    
    // 过滤控件
    QLineEdit* m_filterEdit;
    QComboBox* m_eventTypeFilter;
    QCheckBox* m_autoScrollCheck;
    QPushButton* m_clearButton;
    QPushButton* m_exportButton;
    
    // 信息显示
    QLabel* m_eventCountLabel;
    QLabel* m_filteredCountLabel;
    
    // 数据
    QList<EventLogger::EventRecord> m_allEvents;
    QList<EventLogger::EventRecord> m_filteredEvents;
    
    // 更新定时器
    QTimer* m_updateTimer;
    bool m_autoScroll;
    
    // 表格列枚举
    enum Column {
        TimeColumn = 0,
        TypeColumn,
        SourceColumn,
        TargetColumn,
        AcceptedColumn,
        DetailsColumn,
        ColumnCount
    };
};

#endif // EVENT_DISPLAY_WIDGET_H