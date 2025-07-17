#include "event_performance_analyzer.h"
#include "event_manager.h"
#include <QDebug>
#include <QMutexLocker>
#include <QApplication>
#include <algorithm>

// 静态成员初始化
EventPerformanceAnalyzer* EventPerformanceAnalyzer::s_instance = nullptr;
QMutex EventPerformanceAnalyzer::s_mutex;

EventPerformanceAnalyzer::EventPerformanceAnalyzer(QObject* parent)
    : QObject(parent)
    , m_enabled(true)
    , m_slowThresholdMs(10.0)           // 10ms阈值
    , m_highFrequencyThreshold(100)     // 每秒100个事件
    , m_analysisTimer(nullptr)
    , m_nextTimerId(1)
{
    // 设置定期分析定时器
    m_analysisTimer = new QTimer(this);
    connect(m_analysisTimer, &QTimer::timeout, this, &EventPerformanceAnalyzer::performPeriodicAnalysis);
    m_analysisTimer->start(5000); // 每5秒分析一次
    
    qDebug() << "EventPerformanceAnalyzer initialized";
}

EventPerformanceAnalyzer* EventPerformanceAnalyzer::instance()
{
    if (s_instance == nullptr) {
        QMutexLocker locker(&s_mutex);
        if (s_instance == nullptr) {
            s_instance = new EventPerformanceAnalyzer();
        }
    }
    return s_instance;
}

int EventPerformanceAnalyzer::startEventTiming(QEvent::Type eventType, QObject* object)
{
    QMutexLocker configLocker(&m_configMutex);
    if (!m_enabled) {
        return -1;
    }
    configLocker.unlock();

    QMutexLocker dataLocker(&m_dataMutex);
    
    int timerId = m_nextTimerId++;
    TimingData& data = m_activeTimers[timerId];
    data.eventType = eventType;
    data.object = object;
    data.startTime = QDateTime::currentDateTime();
    data.timer.start();
    
    return timerId;
}

void EventPerformanceAnalyzer::endEventTiming(int timerId)
{
    if (timerId < 0) {
        return; // 无效的计时器ID
    }

    QMutexLocker dataLocker(&m_dataMutex);
    
    if (!m_activeTimers.contains(timerId)) {
        return; // 计时器不存在
    }
    
    TimingData& data = m_activeTimers[timerId];
    qint64 elapsedNs = data.timer.nsecsElapsed();
    
    // 记录事件类型的处理时间
    if (!m_eventTimings.contains(data.eventType)) {
        m_eventTimings[data.eventType] = QList<qint64>();
    }
    m_eventTimings[data.eventType].append(elapsedNs);
    
    // 限制每种事件类型的记录数量
    if (m_eventTimings[data.eventType].size() > 1000) {
        m_eventTimings[data.eventType].removeFirst();
    }
    
    // 记录对象的处理时间
    if (data.object) {
        if (!m_objectTimings.contains(data.object)) {
            m_objectTimings[data.object] = QList<qint64>();
        }
        m_objectTimings[data.object].append(elapsedNs);
        
        // 限制每个对象的记录数量
        if (m_objectTimings[data.object].size() > 1000) {
            m_objectTimings[data.object].removeFirst();
        }
    }
    
    // 移除已完成的计时器
    m_activeTimers.remove(timerId);
    
    // 更新趋势数据
    updateTrendData();
}

EventPerformanceAnalyzer::PerformanceMetrics 
EventPerformanceAnalyzer::getEventTypeMetrics(QEvent::Type eventType) const
{
    QMutexLocker locker(&m_dataMutex);
    
    if (!m_eventTimings.contains(eventType)) {
        return PerformanceMetrics();
    }
    
    return calculateMetrics(m_eventTimings[eventType]);
}

EventPerformanceAnalyzer::PerformanceMetrics 
EventPerformanceAnalyzer::getObjectMetrics(QObject* object) const
{
    QMutexLocker locker(&m_dataMutex);
    
    if (!m_objectTimings.contains(object)) {
        return PerformanceMetrics();
    }
    
    return calculateMetrics(m_objectTimings[object]);
}

EventPerformanceAnalyzer::PerformanceMetrics 
EventPerformanceAnalyzer::getOverallMetrics() const
{
    QMutexLocker locker(&m_dataMutex);
    
    QList<qint64> allTimings;
    
    // 收集所有计时数据
    for (auto it = m_eventTimings.begin(); it != m_eventTimings.end(); ++it) {
        allTimings.append(it.value());
    }
    
    return calculateMetrics(allTimings);
}

QList<EventPerformanceAnalyzer::OptimizationSuggestion> 
EventPerformanceAnalyzer::analyzePerformance() const
{
    QList<OptimizationSuggestion> suggestions;
    
    // 分析总体性能
    PerformanceMetrics overallMetrics = getOverallMetrics();
    suggestions.append(detectIssues(overallMetrics));
    
    // 分析各事件类型性能
    QMutexLocker locker(&m_dataMutex);
    for (auto it = m_eventTimings.begin(); it != m_eventTimings.end(); ++it) {
        PerformanceMetrics metrics = calculateMetrics(it.value());
        QList<OptimizationSuggestion> typeIssues = detectIssues(metrics);
        
        // 为每个建议添加事件类型信息
        for (OptimizationSuggestion& suggestion : typeIssues) {
            QString eventTypeName = EventManager::instance()->getEventTypeName(it.key());
            suggestion.description = QString("[%1] %2").arg(eventTypeName, suggestion.description);
        }
        
        suggestions.append(typeIssues);
    }
    
    // 按优先级排序
    std::sort(suggestions.begin(), suggestions.end(), 
              [](const OptimizationSuggestion& a, const OptimizationSuggestion& b) {
                  return a.priority > b.priority;
              });
    
    return suggestions;
}

QList<QPair<QEvent::Type, EventPerformanceAnalyzer::PerformanceMetrics>> 
EventPerformanceAnalyzer::getPerformanceHotspots(int topN) const
{
    QMutexLocker locker(&m_dataMutex);
    
    QList<QPair<QEvent::Type, PerformanceMetrics>> hotspots;
    
    // 计算每种事件类型的性能指标
    for (auto it = m_eventTimings.begin(); it != m_eventTimings.end(); ++it) {
        PerformanceMetrics metrics = calculateMetrics(it.value());
        hotspots.append(qMakePair(it.key(), metrics));
    }
    
    // 按平均处理时间排序
    std::sort(hotspots.begin(), hotspots.end(),
              [](const QPair<QEvent::Type, PerformanceMetrics>& a,
                 const QPair<QEvent::Type, PerformanceMetrics>& b) {
                  return a.second.avgProcessingTime > b.second.avgProcessingTime;
              });
    
    // 返回前topN个
    if (hotspots.size() > topN) {
        hotspots = hotspots.mid(0, topN);
    }
    
    return hotspots;
}

QList<QPair<QDateTime, double>> 
EventPerformanceAnalyzer::getPerformanceTrend(int minutes) const
{
    QMutexLocker locker(&m_dataMutex);
    
    QDateTime cutoffTime = QDateTime::currentDateTime().addSecs(-minutes * 60);
    QList<QPair<QDateTime, double>> recentTrend;
    
    for (const auto& point : m_trendData) {
        if (point.first >= cutoffTime) {
            recentTrend.append(point);
        }
    }
    
    return recentTrend;
}

void EventPerformanceAnalyzer::resetAnalysis()
{
    QMutexLocker locker(&m_dataMutex);
    
    m_activeTimers.clear();
    m_eventTimings.clear();
    m_objectTimings.clear();
    m_trendData.clear();
    m_nextTimerId = 1;
    
    qDebug() << "Performance analysis data reset";
}

void EventPerformanceAnalyzer::setEnabled(bool enabled)
{
    QMutexLocker locker(&m_configMutex);
    m_enabled = enabled;
    qDebug() << "Performance analysis" << (enabled ? "enabled" : "disabled");
}

bool EventPerformanceAnalyzer::isEnabled() const
{
    QMutexLocker locker(&m_configMutex);
    return m_enabled;
}

void EventPerformanceAnalyzer::setPerformanceThresholds(double slowThresholdMs, int highFrequencyThreshold)
{
    QMutexLocker locker(&m_configMutex);
    m_slowThresholdMs = slowThresholdMs;
    m_highFrequencyThreshold = highFrequencyThreshold;
    
    qDebug() << "Performance thresholds updated: slow =" << slowThresholdMs 
             << "ms, high frequency =" << highFrequencyThreshold << "events/sec";
}

void EventPerformanceAnalyzer::performPeriodicAnalysis()
{
    QMutexLocker configLocker(&m_configMutex);
    if (!m_enabled) {
        return;
    }
    configLocker.unlock();

    // 分析性能问题
    QList<OptimizationSuggestion> suggestions = analyzePerformance();
    
    if (!suggestions.isEmpty()) {
        // 只报告高优先级的问题
        QList<OptimizationSuggestion> highPrioritySuggestions;
        for (const OptimizationSuggestion& suggestion : suggestions) {
            if (suggestion.priority >= 7) { // 优先级7以上
                highPrioritySuggestions.append(suggestion);
            }
        }
        
        if (!highPrioritySuggestions.isEmpty()) {
            emit performanceIssueDetected(highPrioritySuggestions);
        }
    }
    
    // 发出性能指标更新信号
    PerformanceMetrics overallMetrics = getOverallMetrics();
    emit metricsUpdated(overallMetrics);
}

EventPerformanceAnalyzer::PerformanceMetrics 
EventPerformanceAnalyzer::calculateMetrics(const QList<qint64>& timings) const
{
    PerformanceMetrics metrics;
    
    if (timings.isEmpty()) {
        return metrics;
    }
    
    metrics.eventCount = timings.size();
    metrics.totalProcessingTime = 0;
    metrics.minProcessingTime = LLONG_MAX;
    metrics.maxProcessingTime = 0;
    
    for (qint64 timing : timings) {
        metrics.totalProcessingTime += timing;
        metrics.minProcessingTime = qMin(metrics.minProcessingTime, timing);
        metrics.maxProcessingTime = qMax(metrics.maxProcessingTime, timing);
    }
    
    metrics.avgProcessingTime = metrics.totalProcessingTime / metrics.eventCount;
    
    // 计算事件频率（简化计算，假设事件在最近1秒内均匀分布）
    metrics.eventsPerSecond = metrics.eventCount; // 简化计算
    
    return metrics;
}

QList<EventPerformanceAnalyzer::OptimizationSuggestion> 
EventPerformanceAnalyzer::detectIssues(const PerformanceMetrics& metrics) const
{
    QList<OptimizationSuggestion> issues;
    
    if (metrics.eventCount == 0) {
        return issues;
    }
    
    QMutexLocker configLocker(&m_configMutex);
    double slowThreshold = m_slowThresholdMs;
    int highFreqThreshold = m_highFrequencyThreshold;
    configLocker.unlock();

    // 检查处理速度慢的问题
    double avgTimeMs = static_cast<double>(metrics.avgProcessingTime) / 1000000.0;
    if (avgTimeMs > slowThreshold) {
        OptimizationSuggestion suggestion(
            SlowProcessing,
            QString("平均处理时间过长: %1ms").arg(avgTimeMs, 0, 'f', 2),
            "考虑优化事件处理逻辑，减少不必要的计算或I/O操作",
            8
        );
        issues.append(suggestion);
    }
    
    // 检查事件频率过高的问题
    if (metrics.eventsPerSecond > highFreqThreshold) {
        OptimizationSuggestion suggestion(
            HighFrequency,
            QString("事件频率过高: %1 事件/秒").arg(metrics.eventsPerSecond),
            "考虑使用事件压缩、批处理或降低事件生成频率",
            7
        );
        issues.append(suggestion);
    }
    
    // 检查性能瓶颈
    double maxTimeMs = static_cast<double>(metrics.maxProcessingTime) / 1000000.0;
    if (maxTimeMs > slowThreshold * 5) { // 最大时间超过阈值5倍
        OptimizationSuggestion suggestion(
            Bottleneck,
            QString("检测到性能瓶颈: 最大处理时间 %1ms").arg(maxTimeMs, 0, 'f', 2),
            "存在偶发的严重性能问题，建议进行详细的性能分析",
            9
        );
        issues.append(suggestion);
    }
    
    return issues;
}

void EventPerformanceAnalyzer::updateTrendData()
{
    // 注意：此方法应在已获取m_dataMutex锁的情况下调用
    QDateTime now = QDateTime::currentDateTime();
    
    // 计算当前的平均处理时间
    PerformanceMetrics currentMetrics = getOverallMetrics();
    double avgTimeMs = static_cast<double>(currentMetrics.avgProcessingTime) / 1000000.0;
    
    m_trendData.append(qMakePair(now, avgTimeMs));
    
    // 清理超过1小时的数据
    QDateTime oneHourAgo = now.addSecs(-3600);
    auto it = m_trendData.begin();
    while (it != m_trendData.end()) {
        if (it->first < oneHourAgo) {
            it = m_trendData.erase(it);
        } else {
            ++it;
        }
    }
}