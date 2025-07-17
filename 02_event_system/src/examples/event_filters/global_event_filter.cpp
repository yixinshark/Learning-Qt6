#include "global_event_filter.h"
#include <QApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QTimer>

GlobalEventFilter::GlobalEventFilter(QObject *parent)
    : QObject(parent)
    , m_enabled(true)
    , m_interceptMode(false)
    , m_eventModificationEnabled(false)
    , m_totalEventsProcessed(0)
    , m_eventsIntercepted(0)
    , m_eventsModified(0)
{
    // 默认过滤一些常见的事件类型
    m_filteredTypes.insert(QEvent::MouseButtonPress);
    m_filteredTypes.insert(QEvent::MouseButtonRelease);
    m_filteredTypes.insert(QEvent::KeyPress);
    m_filteredTypes.insert(QEvent::KeyRelease);
    m_filteredTypes.insert(QEvent::Wheel);
}

GlobalEventFilter::~GlobalEventFilter()
{
    // 清理资源
    clearWatchedObjects();
}

bool GlobalEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    if (!m_enabled) {
        return QObject::eventFilter(watched, event);
    }

    m_totalEventsProcessed++;

    // 检查是否需要过滤此事件
    if (!shouldFilterEvent(watched, event)) {
        return QObject::eventFilter(watched, event);
    }

    bool intercepted = false;

    // 记录事件信息
    logEventInfo(watched, event, false);

    // 尝试修改事件
    if (m_eventModificationEnabled) {
        if (modifyEvent(watched, event)) {
            m_eventsModified++;
            // emit eventModified(watched, event->type(), event->type());
        }
    }

    // 决定是否拦截事件
    if (m_interceptMode) {
        // 根据事件类型决定是否拦截
        switch (event->type()) {
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
            // 拦截鼠标事件的示例逻辑
            if (QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event)) {
                if (mouseEvent->button() == Qt::RightButton) {
                    intercepted = true;  // 拦截右键点击
                }
            }
            break;
        case QEvent::KeyPress:
            // 拦截特定按键的示例逻辑
            if (QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event)) {
                if (keyEvent->key() == Qt::Key_Escape) {
                    intercepted = true;  // 拦截ESC键
                }
            }
            break;
        default:
            break;
        }
    }

    if (intercepted) {
        m_eventsIntercepted++;
        logEventInfo(watched, event, true);
    }

    // 发出过滤信号 (暂时注释掉)
    // emit eventFiltered(watched, event, intercepted);

    // 返回true表示事件被处理，不再传递给目标对象
    // 返回false表示事件继续传递
    return intercepted ? true : QObject::eventFilter(watched, event);
}

void GlobalEventFilter::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

bool GlobalEventFilter::isEnabled() const
{
    return m_enabled;
}

void GlobalEventFilter::addFilteredEventType(QEvent::Type type)
{
    m_filteredTypes.insert(type);
}

void GlobalEventFilter::removeFilteredEventType(QEvent::Type type)
{
    m_filteredTypes.remove(type);
}

void GlobalEventFilter::clearFilteredEventTypes()
{
    m_filteredTypes.clear();
}

QSet<QEvent::Type> GlobalEventFilter::filteredEventTypes() const
{
    return m_filteredTypes;
}

void GlobalEventFilter::addWatchedObject(QObject *object)
{
    if (object) {
        m_watchedObjects.insert(object);
        object->installEventFilter(this);
    }
}

void GlobalEventFilter::removeWatchedObject(QObject *object)
{
    if (object && m_watchedObjects.contains(object)) {
        m_watchedObjects.remove(object);
        object->removeEventFilter(this);
    }
}

void GlobalEventFilter::clearWatchedObjects()
{
    for (QObject *obj : m_watchedObjects) {
        if (obj) {
            obj->removeEventFilter(this);
        }
    }
    m_watchedObjects.clear();
}

QSet<QObject*> GlobalEventFilter::watchedObjects() const
{
    return m_watchedObjects;
}

void GlobalEventFilter::setInterceptMode(bool intercept)
{
    m_interceptMode = intercept;
}

bool GlobalEventFilter::isInterceptMode() const
{
    return m_interceptMode;
}

void GlobalEventFilter::setEventModificationEnabled(bool enabled)
{
    m_eventModificationEnabled = enabled;
}

bool GlobalEventFilter::isEventModificationEnabled() const
{
    return m_eventModificationEnabled;
}

int GlobalEventFilter::totalEventsProcessed() const
{
    return m_totalEventsProcessed;
}

int GlobalEventFilter::eventsIntercepted() const
{
    return m_eventsIntercepted;
}

int GlobalEventFilter::eventsModified() const
{
    return m_eventsModified;
}

void GlobalEventFilter::resetStatistics()
{
    m_totalEventsProcessed = 0;
    m_eventsIntercepted = 0;
    m_eventsModified = 0;
}

bool GlobalEventFilter::shouldFilterEvent(QObject *watched, QEvent *event) const
{
    // 如果指定了监视对象，只过滤这些对象的事件
    if (!m_watchedObjects.isEmpty()) {
        if (!m_watchedObjects.contains(watched)) {
            return false;
        }
    }

    // 检查事件类型是否在过滤列表中
    return m_filteredTypes.contains(event->type());
}

bool GlobalEventFilter::modifyEvent(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched)
    
    bool modified = false;

    // 事件修改的示例逻辑
    switch (event->type()) {
    case QEvent::MouseButtonPress:
        if (QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event)) {
            // 示例：将左键点击修改为中键点击
            if (mouseEvent->button() == Qt::LeftButton) {
                // 注意：QMouseEvent是只读的，这里只是演示概念
                // 实际修改需要创建新的事件对象
                qDebug() << "模拟修改：左键点击 -> 中键点击";
                modified = true;
            }
        }
        break;
    case QEvent::KeyPress:
        if (QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event)) {
            // 示例：记录按键修改
            if (keyEvent->key() == Qt::Key_A) {
                qDebug() << "模拟修改：A键 -> B键";
                modified = true;
            }
        }
        break;
    default:
        break;
    }

    return modified;
}

QString GlobalEventFilter::eventTypeToString(QEvent::Type type) const
{
    switch (type) {
    case QEvent::MouseButtonPress: return "MouseButtonPress";
    case QEvent::MouseButtonRelease: return "MouseButtonRelease";
    case QEvent::MouseMove: return "MouseMove";
    case QEvent::KeyPress: return "KeyPress";
    case QEvent::KeyRelease: return "KeyRelease";
    case QEvent::Wheel: return "Wheel";
    case QEvent::Paint: return "Paint";
    case QEvent::Resize: return "Resize";
    case QEvent::Show: return "Show";
    case QEvent::Hide: return "Hide";
    case QEvent::Close: return "Close";
    case QEvent::Timer: return "Timer";
    default: return QString("Unknown(%1)").arg(static_cast<int>(type));
    }
}

void GlobalEventFilter::logEventInfo(QObject *watched, QEvent *event, bool intercepted) const
{
    QString objectName = watched ? watched->objectName() : "Unknown";
    if (objectName.isEmpty()) {
        objectName = watched ? watched->metaObject()->className() : "Unknown";
    }

    QString status = intercepted ? "[INTERCEPTED]" : "[FILTERED]";
    
    qDebug() << QString("%1 %2: %3 on %4")
                .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"))
                .arg(status)
                .arg(eventTypeToString(event->type()))
                .arg(objectName);
}