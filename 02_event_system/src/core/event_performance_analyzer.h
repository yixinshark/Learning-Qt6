#ifndef EVENT_PERFORMANCE_ANALYZER_H
#define EVENT_PERFORMANCE_ANALYZER_H

#include <QObject>
#include <QEvent>
#include <QElapsedTimer>
#include <QHash>
#include <QMutex>
#include <QTimer>
#include <QDateTime>

/**
 * @brief EventPerformanceAnalyzer 事件性能分析器
 * 
 * 专门用于分析和优化事件处理性能的工具类
 * 提供详细的性能指标和优化建议
 */
class EventPerformanceAnalyzer : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 性能指标结构体
     */
    struct PerformanceMetrics {
        qint64 totalProcessingTime;     // 总处理时间（纳秒）
        qint64 minProcessingTime;       // 最小处理时间（纳秒）
        qint64 maxProcessingTime;       // 最大处理时间（纳秒）
        qint64 avgProcessingTime;       // 平均处理时间（纳秒）
        int eventCount;                 // 事件数量
        double eventsPerSecond;         // 每秒事件数
        QDateTime firstEventTime;       // 第一个事件时间
        QDateTime lastEventTime;        // 最后一个事件时间
        
        PerformanceMetrics() 
            : totalProcessingTime(0), minProcessingTime(LLONG_MAX), 
              maxProcessingTime(0), avgProcessingTime(0), eventCount(0), 
              eventsPerSecond(0.0) {}
    };

    /**
     * @brief 性能问题类型枚举
     */
    enum PerformanceIssue {
        NoIssue = 0,
        SlowProcessing = 1,     // 处理速度慢
        HighFrequency = 2,      // 事件频率过高
        MemoryLeak = 4,         // 可能的内存泄漏
        DeadLock = 8,           // 可能的死锁
        Bottleneck = 16         // 性能瓶颈
    };
    Q_DECLARE_FLAGS(PerformanceIssues, PerformanceIssue)

    /**
     * @brief 优化建议结构体
     */
    struct OptimizationSuggestion {
        PerformanceIssue issue;
        QString description;
        QString suggestion;
        int priority;           // 优先级 (1-10, 10最高)
        
        OptimizationSuggestion(PerformanceIssue i = NoIssue, 
                             const QString& desc = QString(), 
                             const QString& sugg = QString(), 
                             int prio = 1)
            : issue(i), description(desc), suggestion(sugg), priority(prio) {}
    };

    /**
     * @brief 获取单例实例
     * @return 性能分析器实例
     */
    static EventPerformanceAnalyzer* instance();

    /**
     * @brief 开始事件处理计时
     * @param eventType 事件类型
     * @param object 处理对象
     * @return 计时器ID
     */
    int startEventTiming(QEvent::Type eventType, QObject* object);

    /**
     * @brief 结束事件处理计时
     * @param timerId 计时器ID
     */
    void endEventTiming(int timerId);

    /**
     * @brief 获取事件类型的性能指标
     * @param eventType 事件类型
     * @return 性能指标
     */
    PerformanceMetrics getEventTypeMetrics(QEvent::Type eventType) const;

    /**
     * @brief 获取对象的性能指标
     * @param object 对象指针
     * @return 性能指标
     */
    PerformanceMetrics getObjectMetrics(QObject* object) const;

    /**
     * @brief 获取总体性能指标
     * @return 总体性能指标
     */
    PerformanceMetrics getOverallMetrics() const;

    /**
     * @brief 分析性能问题
     * @return 发现的性能问题列表
     */
    QList<OptimizationSuggestion> analyzePerformance() const;

    /**
     * @brief 获取性能热点（处理时间最长的事件类型）
     * @param topN 返回前N个热点
     * @return 热点事件类型列表
     */
    QList<QPair<QEvent::Type, PerformanceMetrics>> getPerformanceHotspots(int topN = 10) const;

    /**
     * @brief 获取性能趋势数据
     * @param minutes 获取最近N分钟的数据
     * @return 趋势数据点列表
     */
    QList<QPair<QDateTime, double>> getPerformanceTrend(int minutes = 10) const;

    /**
     * @brief 重置所有性能数据
     */
    void resetAnalysis();

    /**
     * @brief 启用或禁用性能分析
     * @param enabled 是否启用
     */
    void setEnabled(bool enabled);

    /**
     * @brief 检查性能分析是否启用
     * @return 是否启用
     */
    bool isEnabled() const;

    /**
     * @brief 设置性能阈值
     * @param slowThresholdMs 慢处理阈值（毫秒）
     * @param highFrequencyThreshold 高频率阈值（每秒事件数）
     */
    void setPerformanceThresholds(double slowThresholdMs, int highFrequencyThreshold);

signals:
    /**
     * @brief 检测到性能问题时发出的信号
     * @param suggestions 优化建议列表
     */
    void performanceIssueDetected(const QList<OptimizationSuggestion>& suggestions);

    /**
     * @brief 性能指标更新信号
     * @param metrics 更新的性能指标
     */
    void metricsUpdated(const PerformanceMetrics& metrics);

private slots:
    /**
     * @brief 定期分析性能
     */
    void performPeriodicAnalysis();

private:
    /**
     * @brief 私有构造函数
     */
    explicit EventPerformanceAnalyzer(QObject* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~EventPerformanceAnalyzer() override = default;

    // 禁用拷贝
    EventPerformanceAnalyzer(const EventPerformanceAnalyzer&) = delete;
    EventPerformanceAnalyzer& operator=(const EventPerformanceAnalyzer&) = delete;

    /**
     * @brief 计算性能指标
     * @param timings 时间数据列表
     * @return 计算出的性能指标
     */
    PerformanceMetrics calculateMetrics(const QList<qint64>& timings) const;

    /**
     * @brief 检测性能问题
     * @param metrics 性能指标
     * @return 检测到的问题列表
     */
    QList<OptimizationSuggestion> detectIssues(const PerformanceMetrics& metrics) const;

    /**
     * @brief 更新趋势数据
     */
    void updateTrendData();

    // 计时器数据结构
    struct TimingData {
        QElapsedTimer timer;
        QEvent::Type eventType;
        QObject* object;
        QDateTime startTime;
        
        TimingData() : eventType(QEvent::None), object(nullptr) {}
    };

    // 静态实例
    static EventPerformanceAnalyzer* s_instance;
    static QMutex s_mutex;

    // 数据存储
    QHash<int, TimingData> m_activeTimers;              // 活动计时器
    QHash<QEvent::Type, QList<qint64>> m_eventTimings;  // 事件类型计时数据
    QHash<QObject*, QList<qint64>> m_objectTimings;     // 对象计时数据
    QList<QPair<QDateTime, double>> m_trendData;        // 趋势数据
    
    // 配置
    bool m_enabled;
    double m_slowThresholdMs;           // 慢处理阈值（毫秒）
    int m_highFrequencyThreshold;      // 高频率阈值（每秒事件数）
    
    // 计时器和互斥锁
    QTimer* m_analysisTimer;
    int m_nextTimerId;
    mutable QMutex m_dataMutex;
    mutable QMutex m_configMutex;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(EventPerformanceAnalyzer::PerformanceIssues)

#endif // EVENT_PERFORMANCE_ANALYZER_H