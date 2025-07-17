#ifndef GLOBAL_EVENT_FILTER_H
#define GLOBAL_EVENT_FILTER_H

#include <QObject>
#include <QEvent>
#include <QSet>
#include <QDateTime>
#include <QDebug>

/**
 * @brief 全局事件过滤器类
 * 
 * 实现全局事件监控和过滤功能，可以拦截、记录和修改应用程序中的所有事件。
 * 这个类演示了Qt事件过滤器的核心机制和使用方法。
 */
class GlobalEventFilter : public QObject
{
    Q_OBJECT

public:
    explicit GlobalEventFilter(QObject *parent = nullptr);
    virtual ~GlobalEventFilter();

    // 事件过滤器核心函数
    bool eventFilter(QObject *watched, QEvent *event) override;

    // 过滤器配置
    void setEnabled(bool enabled);
    bool isEnabled() const;

    // 事件类型过滤
    void addFilteredEventType(QEvent::Type type);
    void removeFilteredEventType(QEvent::Type type);
    void clearFilteredEventTypes();
    QSet<QEvent::Type> filteredEventTypes() const;

    // 对象过滤
    void addWatchedObject(QObject *object);
    void removeWatchedObject(QObject *object);
    void clearWatchedObjects();
    QSet<QObject*> watchedObjects() const;

    // 事件拦截控制
    void setInterceptMode(bool intercept);
    bool isInterceptMode() const;

    // 事件修改功能
    void setEventModificationEnabled(bool enabled);
    bool isEventModificationEnabled() const;

    // 统计信息
    int totalEventsProcessed() const;
    int eventsIntercepted() const;
    int eventsModified() const;
    void resetStatistics();

// 信号暂时注释掉以避免MOC问题
// signals:
//     void eventFiltered(QObject *watched, QEvent *event, bool intercepted);
//     void eventModified(QObject *watched, QEvent::Type originalType, QEvent::Type modifiedType);

private:
    bool m_enabled;                    // 过滤器是否启用
    bool m_interceptMode;              // 是否拦截事件
    bool m_eventModificationEnabled;   // 是否启用事件修改
    
    QSet<QEvent::Type> m_filteredTypes;  // 需要过滤的事件类型
    QSet<QObject*> m_watchedObjects;     // 需要监视的对象
    
    // 统计信息
    int m_totalEventsProcessed;
    int m_eventsIntercepted;
    int m_eventsModified;

    // 辅助方法
    bool shouldFilterEvent(QObject *watched, QEvent *event) const;
    bool modifyEvent(QObject *watched, QEvent *event);
    QString eventTypeToString(QEvent::Type type) const;
    void logEventInfo(QObject *watched, QEvent *event, bool intercepted) const;
};

#endif // GLOBAL_EVENT_FILTER_H