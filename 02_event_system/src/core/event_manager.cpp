#include "event_manager.h"
#include <QCoreApplication>
#include <QMutexLocker>
#include <QDebug>

// 静态成员初始化
EventManager* EventManager::s_instance = nullptr;
QMutex EventManager::s_mutex;

EventManager::EventManager(QObject* parent)
    : QObject(parent)
{
    // 注册Qt内置的常用事件类型
    registerEventType(QEvent::MouseButtonPress, "MouseButtonPress");
    registerEventType(QEvent::MouseButtonRelease, "MouseButtonRelease");
    registerEventType(QEvent::MouseMove, "MouseMove");
    registerEventType(QEvent::KeyPress, "KeyPress");
    registerEventType(QEvent::KeyRelease, "KeyRelease");
    registerEventType(QEvent::Paint, "Paint");
    registerEventType(QEvent::Resize, "Resize");
    registerEventType(QEvent::Close, "Close");
    registerEventType(QEvent::Show, "Show");
    registerEventType(QEvent::Hide, "Hide");
    registerEventType(QEvent::Timer, "Timer");
    registerEventType(QEvent::FocusIn, "FocusIn");
    registerEventType(QEvent::FocusOut, "FocusOut");
    registerEventType(QEvent::Enter, "Enter");
    registerEventType(QEvent::Leave, "Leave");
    
    qDebug() << "EventManager initialized with built-in event types";
}

EventManager* EventManager::instance()
{
    // 双重检查锁定模式确保线程安全的单例
    if (s_instance == nullptr) {
        QMutexLocker locker(&s_mutex);
        if (s_instance == nullptr) {
            s_instance = new EventManager();
        }
    }
    return s_instance;
}

void EventManager::registerEventType(QEvent::Type type, const QString& name)
{
    QMutexLocker locker(&m_typeNamesMutex);
    m_eventTypeNames.insert(type, name);
    qDebug() << "Registered event type:" << static_cast<int>(type) << "as" << name;
}

QString EventManager::getEventTypeName(QEvent::Type type) const
{
    QMutexLocker locker(&m_typeNamesMutex);
    auto it = m_eventTypeNames.find(type);
    if (it != m_eventTypeNames.end()) {
        return it.value();
    }
    
    // 如果未注册，返回默认格式的名称
    return QString("UnknownEvent_%1").arg(static_cast<int>(type));
}

void EventManager::postCustomEvent(QObject* receiver, QEvent* event)
{
    if (!receiver || !event) {
        qWarning() << "EventManager::postCustomEvent: Invalid receiver or event";
        if (event) {
            delete event; // 清理事件内存
        }
        return;
    }

    QEvent::Type eventType = event->type();
    
    // 发出事件投递信号
    emit eventPosted(receiver, eventType);
    
    // 使用Qt的事件系统异步投递事件
    QCoreApplication::postEvent(receiver, event);
    
    qDebug() << "Posted event" << getEventTypeName(eventType) 
             << "to object" << receiver->objectName();
}

bool EventManager::sendCustomEvent(QObject* receiver, QEvent* event)
{
    if (!receiver || !event) {
        qWarning() << "EventManager::sendCustomEvent: Invalid receiver or event";
        return false;
    }

    QEvent::Type eventType = event->type();
    
    // 使用Qt的事件系统同步发送事件
    bool accepted = QCoreApplication::sendEvent(receiver, event);
    
    // 发出事件处理信号
    emit eventProcessed(receiver, eventType, accepted);
    
    qDebug() << "Sent event" << getEventTypeName(eventType) 
             << "to object" << receiver->objectName() 
             << "- accepted:" << accepted;
    
    return accepted;
}

QHash<QEvent::Type, QString> EventManager::getRegisteredEventTypes() const
{
    QMutexLocker locker(&m_typeNamesMutex);
    return m_eventTypeNames;
}

void EventManager::clearRegisteredEventTypes()
{
    QMutexLocker locker(&m_typeNamesMutex);
    m_eventTypeNames.clear();
    qDebug() << "Cleared all registered event types";
}