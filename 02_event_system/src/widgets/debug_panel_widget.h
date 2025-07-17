#ifndef DEBUG_PANEL_WIDGET_H
#define DEBUG_PANEL_WIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeView>
#include <QStandardItemModel>
#include <QTableView>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QProgressBar>
#include <QTextEdit>
#include <QTimer>
#include <QGroupBox>

#include "../core/event_logger.h"
#include "../core/object_hierarchy_model.h"

/**
 * @brief 调试面板控件 - 集成事件日志和调试功能
 * 
 * 这个控件提供：
 * - 对象层次结构显示
 * - 事件统计和性能监控
 * - 事件过滤和搜索
 * - 调试控制功能
 */
class DebugPanelWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DebugPanelWidget(QWidget* parent = nullptr);
    ~DebugPanelWidget() override = default;

public slots:
    void refreshObjectHierarchy();
    void updatePerformanceStats();
    void onEventLogged(const EventLogger::EventRecord& record);

private slots:
    void onObjectSelected();
    void onDebugModeToggled(bool enabled);
    void onEventFilterChanged();
    void onPerformanceTimerTimeout();
    void exportDebugInfo();
    void clearDebugData();

private:
    void setupUI();
    void setupObjectHierarchyTab();
    void setupPerformanceTab();
    void setupDebugControlTab();
    void updateEventStatistics();
    void updatePerformanceMetrics();
    
    // UI组件
    QTabWidget* m_tabWidget;
    
    // 对象层次标签页
    QWidget* m_hierarchyTab;
    QTreeView* m_objectTreeView;
    ObjectHierarchyModel* m_objectHierarchyModel;
    QPushButton* m_refreshButton;
    QPushButton* m_highlightPathButton;
    QPushButton* m_clearHighlightButton;
    QLabel* m_selectedObjectLabel;
    QTextEdit* m_objectDetailsText;
    
    // 性能监控标签页
    QWidget* m_performanceTab;
    QLabel* m_totalEventsLabel;
    QLabel* m_eventsPerSecondLabel;
    QLabel* m_avgProcessingTimeLabel;
    QLabel* m_memoryUsageLabel;
    QProgressBar* m_cpuUsageBar;
    QProgressBar* m_memoryUsageBar;
    QTableView* m_eventStatsTable;
    QStandardItemModel* m_eventStatsModel;
    
    // 调试控制标签页
    QWidget* m_debugControlTab;
    QCheckBox* m_debugModeCheck;
    QCheckBox* m_verboseLoggingCheck;
    QCheckBox* m_performanceMonitoringCheck;
    QSpinBox* m_maxEventsSpinBox;
    QSpinBox* m_updateIntervalSpinBox;
    QPushButton* m_exportButton;
    QPushButton* m_clearButton;
    QTextEdit* m_debugOutputText;
    
    // 数据和状态
    QTimer* m_performanceTimer;
    QMap<QEvent::Type, int> m_eventTypeStats;
    QList<double> m_processingTimes;
    int m_totalEventCount;
    QDateTime m_lastUpdateTime;
    bool m_debugMode;
    bool m_verboseLogging;
    bool m_performanceMonitoring;
};

#endif // DEBUG_PANEL_WIDGET_H