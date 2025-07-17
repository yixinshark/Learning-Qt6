#ifndef SELECTIVE_EVENT_FILTER_H
#define SELECTIVE_EVENT_FILTER_H

#include <QObject>
#include <QEvent>
#include <QSet>
#include <QMap>
#include <QVariant>
#include <functional>

/**
 * @brief 选择性事件过滤器类
 * 
 * 提供更精细的事件过滤控制，支持基于条件的事件过滤、
 * 事件转换和自定义过滤规则。
 */
class SelectiveEventFilter : public QObject
{
    Q_OBJECT

public:
    // 过滤规则函数类型
    using FilterRule = std::function<bool(QObject*, QEvent*)>;
    
    // 事件转换函数类型
    using EventTransformer = std::function<QEvent*(QObject*, QEvent*)>;

    explicit SelectiveEventFilter(QObject *parent = nullptr);
    virtual ~SelectiveEventFilter();

    // 事件过滤器核心函数
    bool eventFilter(QObject *watched, QEvent *event) override;

    // 基本控制
    void setEnabled(bool enabled);
    bool isEnabled() const;

    // 规则管理
    void addFilterRule(const QString &name, FilterRule rule);
    void removeFilterRule(const QString &name);
    void clearFilterRules();
    QStringList filterRuleNames() const;

    // 事件类型选择
    void setAllowedEventTypes(const QSet<QEvent::Type> &types);
    void addAllowedEventType(QEvent::Type type);
    void removeAllowedEventType(QEvent::Type type);
    QSet<QEvent::Type> allowedEventTypes() const;

    void setBlockedEventTypes(const QSet<QEvent::Type> &types);
    void addBlockedEventType(QEvent::Type type);
    void removeBlockedEventType(QEvent::Type type);
    QSet<QEvent::Type> blockedEventTypes() const;

    // 对象选择
    void setTargetObjects(const QSet<QObject*> &objects);
    void addTargetObject(QObject *object);
    void removeTargetObject(QObject *object);
    QSet<QObject*> targetObjects() const;

    // 事件转换
    void addEventTransformer(QEvent::Type sourceType, EventTransformer transformer);
    void removeEventTransformer(QEvent::Type sourceType);
    void clearEventTransformers();

    // 条件过滤
    void setMouseButtonFilter(Qt::MouseButtons buttons);
    void setKeyFilter(const QSet<int> &keys);
    void setObjectNameFilter(const QStringList &names);
    void setObjectClassFilter(const QStringList &classNames);

    // 统计信息
    int totalEventsProcessed() const;
    int eventsFiltered() const;
    int eventsBlocked() const;
    int eventsTransformed() const;
    void resetStatistics();

// 信号暂时注释掉以避免MOC问题
// signals:
//     void eventPassed(QObject *watched, QEvent *event);
//     void eventBlocked(QObject *watched, QEvent *event, const QString &reason);
//     void eventTransformed(QObject *watched, QEvent *originalEvent, QEvent *transformedEvent);

private:
    bool m_enabled;
    
    // 过滤规则
    QMap<QString, FilterRule> m_filterRules;
    
    // 事件类型控制
    QSet<QEvent::Type> m_allowedTypes;
    QSet<QEvent::Type> m_blockedTypes;
    bool m_useAllowedTypes;  // true使用白名单，false使用黑名单
    
    // 目标对象
    QSet<QObject*> m_targetObjects;
    
    // 事件转换器
    QMap<QEvent::Type, EventTransformer> m_eventTransformers;
    
    // 条件过滤器
    Qt::MouseButtons m_allowedMouseButtons;
    QSet<int> m_allowedKeys;
    QStringList m_allowedObjectNames;
    QStringList m_allowedObjectClasses;
    
    // 统计信息
    int m_totalEventsProcessed;
    int m_eventsFiltered;
    int m_eventsBlocked;
    int m_eventsTransformed;

    // 辅助方法
    bool shouldProcessEvent(QObject *watched, QEvent *event) const;
    bool checkEventTypeFilter(QEvent::Type type) const;
    bool checkObjectFilter(QObject *object) const;
    bool checkConditionFilters(QObject *watched, QEvent *event) const;
    bool applyFilterRules(QObject *watched, QEvent *event) const;
    QEvent* transformEvent(QObject *watched, QEvent *event);
    QString getBlockReason(QObject *watched, QEvent *event) const;
};

#endif // SELECTIVE_EVENT_FILTER_H