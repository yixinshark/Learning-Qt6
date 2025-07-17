#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <QObject>
#include <QEvent>
#include <QHash>
#include <QString>
#include <QMutex>

/**
 * @brief EventManager 单例类，提供事件类型注册和管理功能
 * 
 * 这个类负责：
 * - 管理自定义事件类型的注册
 * - 提供事件类型名称的映射
 * - 提供便利的事件发送方法
 * - 跟踪事件的发送和处理状态
 */
class EventManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 获取EventManager的单例实例
     * @return EventManager的单例指针
     */
    static EventManager* instance();

    /**
     * @brief 注册事件类型及其名称
     * @param type 事件类型
     * @param name 事件类型的可读名称
     */
    void registerEventType(QEvent::Type type, const QString& name);

    /**
     * @brief 获取事件类型的名称
     * @param type 事件类型
     * @return 事件类型的可读名称，如果未注册则返回默认名称
     */
    QString getEventTypeName(QEvent::Type type) const;

    /**
     * @brief 异步发送自定义事件
     * @param receiver 接收事件的对象
     * @param event 要发送的事件（EventManager会获取所有权）
     */
    void postCustomEvent(QObject* receiver, QEvent* event);

    /**
     * @brief 同步发送自定义事件
     * @param receiver 接收事件的对象
     * @param event 要发送的事件
     * @return 事件是否被接受
     */
    bool sendCustomEvent(QObject* receiver, QEvent* event);

    /**
     * @brief 获取所有已注册的事件类型
     * @return 事件类型到名称的映射
     */
    QHash<QEvent::Type, QString> getRegisteredEventTypes() const;

    /**
     * @brief 清除所有注册的事件类型
     */
    void clearRegisteredEventTypes();

signals:
    /**
     * @brief 当事件被投递时发出的信号
     * @param receiver 接收事件的对象
     * @param type 事件类型
     */
    void eventPosted(QObject* receiver, QEvent::Type type);

    /**
     * @brief 当事件被处理时发出的信号
     * @param receiver 处理事件的对象
     * @param type 事件类型
     * @param accepted 事件是否被接受
     */
    void eventProcessed(QObject* receiver, QEvent::Type type, bool accepted);

private:
    /**
     * @brief 私有构造函数，确保单例模式
     */
    explicit EventManager(QObject* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~EventManager() override = default;

    // 禁用拷贝构造和赋值操作
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;

    // 静态实例指针
    static EventManager* s_instance;
    static QMutex s_mutex;

    // 事件类型到名称的映射
    QHash<QEvent::Type, QString> m_eventTypeNames;
    mutable QMutex m_typeNamesMutex;
};

#endif // EVENT_MANAGER_H