#ifndef PERFORMANCE_MONITOR_WIDGET_H
#define PERFORMANCE_MONITOR_WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QProgressBar>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QTextEdit>
#include <QPlainTextEdit>
#include "../core/event_logger.h"

/**
 * @brief PerformanceMonitorWidget 性能监控面板
 * 
 * 提供实时的事件系统性能监控和可视化
 * 显示事件处理速度、平均处理时间、事件类型分布等信息
 */
class PerformanceMonitorWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口
     */
    explicit PerformanceMonitorWidget(QWidget *parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~PerformanceMonitorWidget() override = default;

public slots:
    /**
     * @brief 更新性能数据显示
     */
    void updatePerformanceData();

    /**
     * @brief 处理性能更新信号
     * @param avgTime 平均处理时间
     * @param eventsPerSecond 每秒事件数
     */
    void onPerformanceUpdate(double avgTime, int eventsPerSecond);

    /**
     * @brief 重置性能统计
     */
    void resetStatistics();

    /**
     * @brief 切换性能监控开关
     * @param enabled 是否启用
     */
    void togglePerformanceMonitoring(bool enabled);

private slots:
    /**
     * @brief 定时更新显示
     */
    void updateDisplay();

private:
    /**
     * @brief 初始化UI组件
     */
    void setupUI();

    /**
     * @brief 更新实时统计显示
     */
    void updateRealTimeStats();

    /**
     * @brief 更新事件类型统计表格
     */
    void updateEventTypeTable();

    /**
     * @brief 更新对象统计表格
     */
    void updateObjectTable();

    /**
     * @brief 更新趋势显示
     * @param avgTime 平均处理时间
     * @param eventsPerSecond 每秒事件数
     */
    void updateTrendDisplay(double avgTime, int eventsPerSecond);

    // UI组件
    QVBoxLayout* m_mainLayout;
    
    // 实时统计区域
    QGroupBox* m_realTimeGroup;
    QLabel* m_eventsPerSecondLabel;
    QLabel* m_avgProcessingTimeLabel;
    QLabel* m_totalEventsLabel;
    QProgressBar* m_cpuUsageBar;
    
    // 控制按钮
    QHBoxLayout* m_controlLayout;
    QPushButton* m_resetButton;
    QPushButton* m_toggleButton;
    
    // 事件类型统计表格
    QGroupBox* m_eventTypeGroup;
    QTableWidget* m_eventTypeTable;
    
    // 对象统计表格
    QGroupBox* m_objectGroup;
    QTableWidget* m_objectTable;
    
    // 性能趋势显示
    QGroupBox* m_chartGroup;
    QPlainTextEdit* m_performanceTrendDisplay;
    
    // 定时器
    QTimer* m_updateTimer;
    
    // 数据存储
    QList<QPair<QDateTime, int>> m_eventsPerSecondHistory;
    QList<QPair<QDateTime, double>> m_avgTimeHistory;
    
    // EventLogger引用
    EventLogger* m_eventLogger;
    
    // 配置
    static const int MAX_CHART_POINTS = 60; // 图表最大显示点数（1分钟，每秒一个点）
};

#endif // PERFORMANCE_MONITOR_WIDGET_H