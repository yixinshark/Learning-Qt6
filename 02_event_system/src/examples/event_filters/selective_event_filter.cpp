#include "selective_event_filter.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QDebug>
#include <QDateTime>

SelectiveEventFilter::SelectiveEventFilter(QObject *parent)
    : QObject(parent)
    , m_enabled(true)
    , m_useAllowedTypes(false)
    , m_allowedMouseButtons(Qt::AllButtons)
    , m_totalEventsProcessed(0)
    , m_eventsFiltered(0)
    , m_eventsBlocked(0)
    , m_eventsTransformed(0)
{
    // 默认阻止一些高频事件
    m_blockedTypes.insert(QEvent::Paint);
    m_blockedTypes.insert(QEvent::UpdateRequest);
    m_blockedTypes.insert(QEvent::MouseMove);
}

SelectiveEventFilter::~SelectiveEventFilter()
{
    clearEventTransformers();
}

bool SelectiveEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    if (!m_enabled) {
        return QObject::eventFilter(watched, event);
    }

    m_totalEventsProcessed++;

    // 检查是否应该处理此事件
    if (!shouldProcessEvent(watched, event)) {
        return QObject::eventFilter(watched, event);
    }

    m_eventsFiltered++;

    // 检查事件类型过滤
    if (!checkEventTypeFilter(event->type())) {
        QString reason = "Event type not allowed";
        // emit eventBlocked(watched, event, reason);
        m_eventsBlocked++;
        return true;  // 阻止事件
    }

    // 检查对象过滤
    if (!checkObjectFilter(watched)) {
        QString reason = "Object not in target list";
        // emit eventBlocked(watched, event, reason);
        m_eventsBlocked++;
        return true;  // 阻止事件
    }

    // 检查条件过滤器
    if (!checkConditionFilters(watched, event)) {
        QString reason = getBlockReason(watched, event);
        // emit eventBlocked(watched, event, reason);
        m_eventsBlocked++;
        return true;  // 阻止事件
    }

    // 应用自定义过滤规则
    if (!applyFilterRules(watched, event)) {
        QString reason = "Custom filter rule blocked";
        // emit eventBlocked(watched, event, reason);
        m_eventsBlocked++;
        return true;  // 阻止事件
    }

    // 尝试转换事件
    QEvent *transformedEvent = transformEvent(watched, event);
    if (transformedEvent && transformedEvent != event) {
        // emit eventTransformed(watched, event, transformedEvent);
        m_eventsTransformed++;
        
        // 注意：这里只是演示概念，实际的事件转换需要更复杂的处理
        qDebug() << QString("[%1] Event transformed: %2 -> %3")
                    .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"))
                    .arg(static_cast<int>(event->type()))
                    .arg(static_cast<int>(transformedEvent->type()));
        
        delete transformedEvent;  // 清理转换后的事件
    }

    // 事件通过所有过滤器
    // emit eventPassed(watched, event);
    
    return QObject::eventFilter(watched, event);
}

void SelectiveEventFilter::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

bool SelectiveEventFilter::isEnabled() const
{
    return m_enabled;
}

void SelectiveEventFilter::addFilterRule(const QString &name, FilterRule rule)
{
    m_filterRules[name] = rule;
}

void SelectiveEventFilter::removeFilterRule(const QString &name)
{
    m_filterRules.remove(name);
}

void SelectiveEventFilter::clearFilterRules()
{
    m_filterRules.clear();
}

QStringList SelectiveEventFilter::filterRuleNames() const
{
    return m_filterRules.keys();
}

void SelectiveEventFilter::setAllowedEventTypes(const QSet<QEvent::Type> &types)
{
    m_allowedTypes = types;
    m_useAllowedTypes = !types.isEmpty();
}

void SelectiveEventFilter::addAllowedEventType(QEvent::Type type)
{
    m_allowedTypes.insert(type);
    m_useAllowedTypes = true;
}

void SelectiveEventFilter::removeAllowedEventType(QEvent::Type type)
{
    m_allowedTypes.remove(type);
    if (m_allowedTypes.isEmpty()) {
        m_useAllowedTypes = false;
    }
}

QSet<QEvent::Type> SelectiveEventFilter::allowedEventTypes() const
{
    return m_allowedTypes;
}

void SelectiveEventFilter::setBlockedEventTypes(const QSet<QEvent::Type> &types)
{
    m_blockedTypes = types;
}

void SelectiveEventFilter::addBlockedEventType(QEvent::Type type)
{
    m_blockedTypes.insert(type);
}

void SelectiveEventFilter::removeBlockedEventType(QEvent::Type type)
{
    m_blockedTypes.remove(type);
}

QSet<QEvent::Type> SelectiveEventFilter::blockedEventTypes() const
{
    return m_blockedTypes;
}

void SelectiveEventFilter::setTargetObjects(const QSet<QObject*> &objects)
{
    // 移除旧的事件过滤器
    for (QObject *obj : m_targetObjects) {
        if (obj) {
            obj->removeEventFilter(this);
        }
    }
    
    m_targetObjects = objects;
    
    // 安装新的事件过滤器
    for (QObject *obj : m_targetObjects) {
        if (obj) {
            obj->installEventFilter(this);
        }
    }
}

void SelectiveEventFilter::addTargetObject(QObject *object)
{
    if (object && !m_targetObjects.contains(object)) {
        m_targetObjects.insert(object);
        object->installEventFilter(this);
    }
}

void SelectiveEventFilter::removeTargetObject(QObject *object)
{
    if (object && m_targetObjects.contains(object)) {
        m_targetObjects.remove(object);
        object->removeEventFilter(this);
    }
}

QSet<QObject*> SelectiveEventFilter::targetObjects() const
{
    return m_targetObjects;
}

void SelectiveEventFilter::addEventTransformer(QEvent::Type sourceType, EventTransformer transformer)
{
    m_eventTransformers[sourceType] = transformer;
}

void SelectiveEventFilter::removeEventTransformer(QEvent::Type sourceType)
{
    m_eventTransformers.remove(sourceType);
}

void SelectiveEventFilter::clearEventTransformers()
{
    m_eventTransformers.clear();
}

void SelectiveEventFilter::setMouseButtonFilter(Qt::MouseButtons buttons)
{
    m_allowedMouseButtons = buttons;
}

void SelectiveEventFilter::setKeyFilter(const QSet<int> &keys)
{
    m_allowedKeys = keys;
}

void SelectiveEventFilter::setObjectNameFilter(const QStringList &names)
{
    m_allowedObjectNames = names;
}

void SelectiveEventFilter::setObjectClassFilter(const QStringList &classNames)
{
    m_allowedObjectClasses = classNames;
}

int SelectiveEventFilter::totalEventsProcessed() const
{
    return m_totalEventsProcessed;
}

int SelectiveEventFilter::eventsFiltered() const
{
    return m_eventsFiltered;
}

int SelectiveEventFilter::eventsBlocked() const
{
    return m_eventsBlocked;
}

int SelectiveEventFilter::eventsTransformed() const
{
    return m_eventsTransformed;
}

void SelectiveEventFilter::resetStatistics()
{
    m_totalEventsProcessed = 0;
    m_eventsFiltered = 0;
    m_eventsBlocked = 0;
    m_eventsTransformed = 0;
}

bool SelectiveEventFilter::shouldProcessEvent(QObject *watched, QEvent *event) const
{
    Q_UNUSED(watched)
    Q_UNUSED(event)
    
    // 基本的处理条件检查
    return true;
}

bool SelectiveEventFilter::checkEventTypeFilter(QEvent::Type type) const
{
    if (m_useAllowedTypes) {
        // 使用白名单模式
        return m_allowedTypes.contains(type);
    } else {
        // 使用黑名单模式
        return !m_blockedTypes.contains(type);
    }
}

bool SelectiveEventFilter::checkObjectFilter(QObject *object) const
{
    if (m_targetObjects.isEmpty()) {
        return true;  // 没有指定目标对象，允许所有对象
    }
    
    return m_targetObjects.contains(object);
}

bool SelectiveEventFilter::checkConditionFilters(QObject *watched, QEvent *event) const
{
    // 检查鼠标按钮过滤
    if (event->type() == QEvent::MouseButtonPress || 
        event->type() == QEvent::MouseButtonRelease) {
        if (QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event)) {
            if (!(m_allowedMouseButtons & mouseEvent->button())) {
                return false;
            }
        }
    }

    // 检查按键过滤
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
        if (!m_allowedKeys.isEmpty()) {
            if (QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event)) {
                if (!m_allowedKeys.contains(keyEvent->key())) {
                    return false;
                }
            }
        }
    }

    // 检查对象名称过滤
    if (!m_allowedObjectNames.isEmpty()) {
        QString objectName = watched ? watched->objectName() : QString();
        if (!m_allowedObjectNames.contains(objectName)) {
            return false;
        }
    }

    // 检查对象类名过滤
    if (!m_allowedObjectClasses.isEmpty()) {
        QString className = watched ? watched->metaObject()->className() : QString();
        if (!m_allowedObjectClasses.contains(className)) {
            return false;
        }
    }

    return true;
}

bool SelectiveEventFilter::applyFilterRules(QObject *watched, QEvent *event) const
{
    // 应用所有自定义过滤规则
    for (auto it = m_filterRules.constBegin(); it != m_filterRules.constEnd(); ++it) {
        if (!it.value()(watched, event)) {
            return false;  // 任何一个规则不通过就阻止事件
        }
    }
    
    return true;
}

QEvent* SelectiveEventFilter::transformEvent(QObject *watched, QEvent *event)
{
    auto it = m_eventTransformers.find(event->type());
    if (it != m_eventTransformers.end()) {
        return it.value()(watched, event);
    }
    
    return nullptr;  // 没有找到对应的转换器
}

QString SelectiveEventFilter::getBlockReason(QObject *watched, QEvent *event) const
{
    // 检查各种阻止原因
    if (event->type() == QEvent::MouseButtonPress || 
        event->type() == QEvent::MouseButtonRelease) {
        if (QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event)) {
            if (!(m_allowedMouseButtons & mouseEvent->button())) {
                return QString("Mouse button %1 not allowed").arg(static_cast<int>(mouseEvent->button()));
            }
        }
    }

    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
        if (!m_allowedKeys.isEmpty()) {
            if (QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event)) {
                if (!m_allowedKeys.contains(keyEvent->key())) {
                    return QString("Key %1 not allowed").arg(keyEvent->key());
                }
            }
        }
    }

    if (!m_allowedObjectNames.isEmpty()) {
        QString objectName = watched ? watched->objectName() : QString();
        if (!m_allowedObjectNames.contains(objectName)) {
            return QString("Object name '%1' not allowed").arg(objectName);
        }
    }

    if (!m_allowedObjectClasses.isEmpty()) {
        QString className = watched ? watched->metaObject()->className() : QString();
        if (!m_allowedObjectClasses.contains(className)) {
            return QString("Object class '%1' not allowed").arg(className);
        }
    }

    return "Unknown reason";
}