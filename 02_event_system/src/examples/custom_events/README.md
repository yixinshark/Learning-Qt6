# Qt6 自定义事件系统示例

## 概述

本模块深入演示Qt6自定义事件系统的设计和实现，展示如何创建、发送和处理自定义事件，实现组件间的松耦合通信。自定义事件是构建复杂应用程序架构的重要工具。

## 学习目标

- 掌握自定义QEvent子类的创建方法
- 理解事件的发送(postEvent/sendEvent)和接收机制
- 学习事件数据的封装和传递技术
- 掌握跨组件通信的事件模式
- 了解事件系统在应用架构中的作用

## 主要组件

### 1. CustomEventDemo（自定义事件演示）

**文件**: `custom_event_demo.h/cpp`

**功能特性**:
- 展示多种自定义事件类型的实现
- 演示事件数据的封装和传递
- 提供事件发送和接收的完整示例
- 实现事件处理的可视化反馈

**自定义事件类型**:
```cpp
// 数据传输事件
class DataTransferEvent : public QEvent {
public:
    static const QEvent::Type EventType;
    
    explicit DataTransferEvent(const QVariant &data)
        : QEvent(EventType), m_data(data) {}
    
    QVariant data() const { return m_data; }
    
private:
    QVariant m_data;
};

// 状态变化事件
class StatusChangeEvent : public QEvent {
public:
    static const QEvent::Type EventType;
    
    enum Status { Ready, Busy, Error, Finished };
    
    explicit StatusChangeEvent(Status status, const QString &message = QString())
        : QEvent(EventType), m_status(status), m_message(message) {}
    
    Status status() const { return m_status; }
    QString message() const { return m_message; }
    
private:
    Status m_status;
    QString m_message;
};

// 命令执行事件
class CommandEvent : public QEvent {
public:
    static const QEvent::Type EventType;
    
    explicit CommandEvent(const QString &command, const QVariantMap &parameters = QVariantMap())
        : QEvent(EventType), m_command(command), m_parameters(parameters) {}
    
    QString command() const { return m_command; }
    QVariantMap parameters() const { return m_parameters; }
    
private:
    QString m_command;
    QVariantMap m_parameters;
};
```

### 2. CustomEventSender（事件发送器）

**文件**: `custom_event_sender.h/cpp`

**功能特性**:
- 提供统一的事件发送接口
- 支持同步和异步事件发送
- 实现事件优先级管理
- 提供事件发送状态反馈

**核心方法**:
```cpp
class CustomEventSender : public QObject
{
    Q_OBJECT
    
public:
    // 异步发送事件
    void postEvent(QObject *receiver, QEvent *event, int priority = Qt::NormalEventPriority);
    
    // 同步发送事件
    bool sendEvent(QObject *receiver, QEvent *event);
    
    // 批量发送事件
    void postEvents(const QList<QPair<QObject*, QEvent*>> &events);
    
    // 延迟发送事件
    void postDelayedEvent(QObject *receiver, QEvent *event, int delayMs);
    
    // 取消待发送事件
    void cancelPendingEvents(QObject *receiver, QEvent::Type eventType = QEvent::None);
    
signals:
    void eventSent(QObject *receiver, QEvent::Type eventType);
    void eventDelivered(QObject *receiver, QEvent::Type eventType, bool handled);
    
private slots:
    void processPendingEvents();
    void processDelayedEvents();
    
private:
    struct PendingEvent {
        QObject *receiver;
        QEvent *event;
        int priority;
        QDateTime timestamp;
    };
    
    QList<PendingEvent> m_pendingEvents;
    QTimer m_processingTimer;
    QTimer m_delayedTimer;
};
```

### 3. CustomEventReceiver（事件接收器）

**文件**: `custom_event_receiver.h/cpp`

**功能特性**:
- 演示自定义事件的接收和处理
- 实现事件处理器的注册机制
- 提供事件处理结果的反馈
- 支持事件处理链和过滤

**核心实现**:
```cpp
class CustomEventReceiver : public QWidget
{
    Q_OBJECT
    
public:
    // 注册事件处理器
    void registerEventHandler(QEvent::Type eventType, 
                             std::function<bool(QEvent*)> handler);
    
    // 移除事件处理器
    void unregisterEventHandler(QEvent::Type eventType);
    
    // 设置事件过滤器
    void setEventFilter(std::function<bool(QEvent*)> filter);
    
protected:
    bool event(QEvent *event) override;
    
private:
    // 处理数据传输事件
    bool handleDataTransferEvent(DataTransferEvent *event);
    
    // 处理状态变化事件
    bool handleStatusChangeEvent(StatusChangeEvent *event);
    
    // 处理命令执行事件
    bool handleCommandEvent(CommandEvent *event);
    
signals:
    void dataReceived(const QVariant &data);
    void statusChanged(int status, const QString &message);
    void commandExecuted(const QString &command, bool success);
    
private:
    QHash<QEvent::Type, std::function<bool(QEvent*)>> m_eventHandlers;
    std::function<bool(QEvent*)> m_eventFilter;
    QStringList m_eventLog;
};
```

## 核心技术详解

### 1. 自定义事件类设计

#### 事件类型注册
```cpp
// 在头文件中声明
class DataTransferEvent : public QEvent
{
public:
    static const QEvent::Type EventType;
    // ... 其他成员
};

// 在源文件中定义
const QEvent::Type DataTransferEvent::EventType = 
    static_cast<QEvent::Type>(QEvent::registerEventType());
```

#### 复杂数据封装
```cpp
class ComplexDataEvent : public QEvent
{
public:
    static const QEvent::Type EventType;
    
    struct EventData {
        QString id;
        QDateTime timestamp;
        QVariantMap properties;
        QByteArray binaryData;
        
        // 序列化支持
        QByteArray serialize() const;
        void deserialize(const QByteArray &data);
    };
    
    explicit ComplexDataEvent(const EventData &data)
        : QEvent(EventType), m_data(data) {}
    
    const EventData& data() const { return m_data; }
    EventData& data() { return m_data; }
    
    // 便利方法
    QString id() const { return m_data.id; }
    QDateTime timestamp() const { return m_data.timestamp; }
    QVariant property(const QString &key) const { return m_data.properties.value(key); }
    
private:
    EventData m_data;
};
```

#### 事件继承层次
```cpp
// 基础通知事件
class NotificationEvent : public QEvent
{
public:
    enum NotificationType {
        Info,
        Warning,
        Error,
        Success
    };
    
    NotificationEvent(QEvent::Type type, NotificationType notifType, const QString &message)
        : QEvent(type), m_notificationType(notifType), m_message(message) {}
    
    NotificationType notificationType() const { return m_notificationType; }
    QString message() const { return m_message; }
    
private:
    NotificationType m_notificationType;
    QString m_message;
};

// 具体的通知事件类型
class InfoNotificationEvent : public NotificationEvent
{
public:
    static const QEvent::Type EventType;
    
    explicit InfoNotificationEvent(const QString &message)
        : NotificationEvent(EventType, Info, message) {}
};

class ErrorNotificationEvent : public NotificationEvent
{
public:
    static const QEvent::Type EventType;
    
    explicit ErrorNotificationEvent(const QString &message, int errorCode = 0)
        : NotificationEvent(EventType, Error, message), m_errorCode(errorCode) {}
    
    int errorCode() const { return m_errorCode; }
    
private:
    int m_errorCode;
};
```

### 2. 事件发送策略

#### 智能事件发送器
```cpp
class SmartEventSender : public QObject
{
    Q_OBJECT
    
public:
    // 发送策略枚举
    enum SendStrategy {
        Immediate,      // 立即发送
        Queued,         // 队列发送
        Compressed,     // 压缩发送（合并相同类型事件）
        Throttled,      // 限流发送
        Batched         // 批量发送
    };
    
    void setSendStrategy(QEvent::Type eventType, SendStrategy strategy);
    void setThrottleInterval(QEvent::Type eventType, int intervalMs);
    void setBatchSize(QEvent::Type eventType, int batchSize);
    
    void sendEvent(QObject *receiver, QEvent *event);
    
private:
    struct EventConfig {
        SendStrategy strategy = Immediate;
        int throttleInterval = 0;
        int batchSize = 1;
        QDateTime lastSent;
        QList<QEvent*> pendingEvents;
    };
    
    QHash<QEvent::Type, EventConfig> m_eventConfigs;
    QTimer m_processingTimer;
    
    void processThrottledEvents();
    void processBatchedEvents();
    void processCompressedEvents();
};
```

#### 事件路由系统
```cpp
class EventRouter : public QObject
{
    Q_OBJECT
    
public:
    // 注册事件路由规则
    void addRoute(QEvent::Type eventType, 
                  std::function<QList<QObject*>(QEvent*)> routingFunction);
    
    // 广播事件到多个接收者
    void broadcastEvent(QEvent *event);
    
    // 条件路由
    void routeEvent(QEvent *event, std::function<bool(QObject*)> condition);
    
private:
    struct RouteInfo {
        std::function<QList<QObject*>(QEvent*)> routingFunction;
        QList<QObject*> staticReceivers;
    };
    
    QHash<QEvent::Type, RouteInfo> m_routes;
    QList<QObject*> m_globalReceivers;
};
```

### 3. 高级事件处理模式

#### 事件处理链
```cpp
class EventProcessorChain : public QObject
{
    Q_OBJECT
    
public:
    class Processor {
    public:
        virtual ~Processor() = default;
        virtual bool process(QEvent *event) = 0;
        virtual bool canProcess(QEvent *event) const = 0;
    };
    
    void addProcessor(std::unique_ptr<Processor> processor);
    void removeProcessor(Processor *processor);
    
    bool processEvent(QEvent *event);
    
private:
    QList<std::unique_ptr<Processor>> m_processors;
};

// 具体处理器示例
class DataValidationProcessor : public EventProcessorChain::Processor
{
public:
    bool canProcess(QEvent *event) const override {
        return event->type() == DataTransferEvent::EventType;
    }
    
    bool process(QEvent *event) override {
        auto *dataEvent = static_cast<DataTransferEvent*>(event);
        
        // 验证数据
        if (!validateData(dataEvent->data())) {
            qWarning() << "数据验证失败";
            return false;
        }
        
        return true; // 继续处理链
    }
    
private:
    bool validateData(const QVariant &data) const;
};
```

#### 事件状态机
```cpp
class EventStateMachine : public QObject
{
    Q_OBJECT
    
public:
    enum State {
        Idle,
        Processing,
        Waiting,
        Error
    };
    
    void addTransition(State from, QEvent::Type eventType, State to);
    void setInitialState(State state);
    
    bool processEvent(QEvent *event);
    State currentState() const { return m_currentState; }
    
signals:
    void stateChanged(State oldState, State newState);
    void transitionTriggered(State from, QEvent::Type eventType, State to);
    
private:
    struct Transition {
        State fromState;
        QEvent::Type eventType;
        State toState;
        std::function<bool(QEvent*)> condition;
        std::function<void(QEvent*)> action;
    };
    
    QList<Transition> m_transitions;
    State m_currentState = Idle;
    
    bool executeTransition(const Transition &transition, QEvent *event);
};
```

## 实际应用示例

### 1. 插件系统通信
```cpp
// 插件接口
class IPlugin : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IPlugin() = default;
    virtual QString name() const = 0;
    virtual void initialize() = 0;
    virtual void shutdown() = 0;
    
protected:
    bool event(QEvent *event) override;
    
    // 发送插件事件
    void sendPluginEvent(const QString &targetPlugin, QEvent *event);
    
signals:
    void pluginEventSent(const QString &targetPlugin, QEvent *event);
};

// 插件管理器
class PluginManager : public QObject
{
    Q_OBJECT
    
public:
    void registerPlugin(IPlugin *plugin);
    void unregisterPlugin(IPlugin *plugin);
    
    // 插件间事件路由
    void routeEvent(const QString &sourcePlugin, const QString &targetPlugin, QEvent *event);
    
    // 广播事件到所有插件
    void broadcastEvent(QEvent *event);
    
private:
    QHash<QString, IPlugin*> m_plugins;
    EventRouter m_eventRouter;
};

// 具体插件实现
class FileManagerPlugin : public IPlugin
{
    Q_OBJECT
    
public:
    QString name() const override { return "FileManager"; }
    
    void initialize() override {
        // 注册处理的事件类型
        connect(this, &FileManagerPlugin::pluginEventSent,
                PluginManager::instance(), &PluginManager::routeEvent);
    }
    
protected:
    bool event(QEvent *event) override {
        if (event->type() == FileOperationEvent::EventType) {
            return handleFileOperation(static_cast<FileOperationEvent*>(event));
        }
        
        return IPlugin::event(event);
    }
    
private:
    bool handleFileOperation(FileOperationEvent *event);
};
```

### 2. 异步任务系统
```cpp
// 任务事件
class TaskEvent : public QEvent
{
public:
    static const QEvent::Type EventType;
    
    enum TaskType {
        StartTask,
        TaskProgress,
        TaskCompleted,
        TaskFailed,
        TaskCancelled
    };
    
    TaskEvent(TaskType taskType, const QString &taskId, const QVariant &data = QVariant())
        : QEvent(EventType), m_taskType(taskType), m_taskId(taskId), m_data(data) {}
    
    TaskType taskType() const { return m_taskType; }
    QString taskId() const { return m_taskId; }
    QVariant data() const { return m_data; }
    
private:
    TaskType m_taskType;
    QString m_taskId;
    QVariant m_data;
};

// 任务管理器
class TaskManager : public QObject
{
    Q_OBJECT
    
public:
    QString startTask(std::function<QVariant()> taskFunction);
    void cancelTask(const QString &taskId);
    
    // 获取任务状态
    bool isTaskRunning(const QString &taskId) const;
    QVariant getTaskResult(const QString &taskId) const;
    
protected:
    bool event(QEvent *event) override;
    
private:
    struct TaskInfo {
        QString id;
        QThread *thread;
        QVariant result;
        bool completed = false;
        bool cancelled = false;
    };
    
    QHash<QString, TaskInfo> m_tasks;
    
    void handleTaskEvent(TaskEvent *event);
    
signals:
    void taskStarted(const QString &taskId);
    void taskProgress(const QString &taskId, int percentage);
    void taskCompleted(const QString &taskId, const QVariant &result);
    void taskFailed(const QString &taskId, const QString &error);
};

// 工作线程
class WorkerThread : public QThread
{
    Q_OBJECT
    
public:
    WorkerThread(const QString &taskId, std::function<QVariant()> taskFunction)
        : m_taskId(taskId), m_taskFunction(taskFunction) {}
    
protected:
    void run() override {
        try {
            // 发送开始事件
            TaskEvent *startEvent = new TaskEvent(TaskEvent::StartTask, m_taskId);
            QCoreApplication::postEvent(TaskManager::instance(), startEvent);
            
            // 执行任务
            QVariant result = m_taskFunction();
            
            // 发送完成事件
            TaskEvent *completeEvent = new TaskEvent(TaskEvent::TaskCompleted, m_taskId, result);
            QCoreApplication::postEvent(TaskManager::instance(), completeEvent);
            
        } catch (const std::exception &e) {
            // 发送失败事件
            TaskEvent *failEvent = new TaskEvent(TaskEvent::TaskFailed, m_taskId, QString::fromStdString(e.what()));
            QCoreApplication::postEvent(TaskManager::instance(), failEvent);
        }
    }
    
private:
    QString m_taskId;
    std::function<QVariant()> m_taskFunction;
};
```

### 3. 模型-视图通信
```cpp
// 模型变化事件
class ModelChangeEvent : public QEvent
{
public:
    static const QEvent::Type EventType;
    
    enum ChangeType {
        DataChanged,
        RowsInserted,
        RowsRemoved,
        ModelReset
    };
    
    ModelChangeEvent(ChangeType changeType, const QModelIndex &index = QModelIndex(), 
                    int first = -1, int last = -1)
        : QEvent(EventType), m_changeType(changeType), m_index(index), m_first(first), m_last(last) {}
    
    ChangeType changeType() const { return m_changeType; }
    QModelIndex index() const { return m_index; }
    int first() const { return m_first; }
    int last() const { return m_last; }
    
private:
    ChangeType m_changeType;
    QModelIndex m_index;
    int m_first, m_last;
};

// 自定义模型
class CustomModel : public QAbstractItemModel
{
    Q_OBJECT
    
public:
    // 重写标准模型方法
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    
    // 自定义数据修改方法
    void updateData(const QModelIndex &index, const QVariant &value);
    void insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    void removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    
private:
    void notifyViews(ModelChangeEvent::ChangeType changeType, 
                    const QModelIndex &index = QModelIndex(), 
                    int first = -1, int last = -1);
    
    QList<QObject*> m_registeredViews;
    QVariantList m_data;
};

// 自定义视图
class CustomView : public QWidget
{
    Q_OBJECT
    
public:
    void setModel(CustomModel *model);
    
protected:
    bool event(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    
private:
    void handleModelChange(ModelChangeEvent *event);
    void updateView();
    
    CustomModel *m_model = nullptr;
    QRect m_viewRect;
};
```

## 性能优化和最佳实践

### 1. 事件池化
```cpp
template<typename EventType>
class EventPool
{
public:
    static EventPool& instance() {
        static EventPool pool;
        return pool;
    }
    
    EventType* acquire() {
        QMutexLocker locker(&m_mutex);
        
        if (m_pool.isEmpty()) {
            return new EventType();
        }
        
        EventType* event = m_pool.takeLast();
        event->setAccepted(false); // 重置状态
        return event;
    }
    
    void release(EventType* event) {
        QMutexLocker locker(&m_mutex);
        
        if (m_pool.size() < MAX_POOL_SIZE) {
            m_pool.append(event);
        } else {
            delete event;
        }
    }
    
private:
    static const int MAX_POOL_SIZE = 100;
    QList<EventType*> m_pool;
    QMutex m_mutex;
};

// 使用示例
void sendOptimizedEvent(QObject *receiver, const QVariant &data)
{
    DataTransferEvent *event = EventPool<DataTransferEvent>::instance().acquire();
    event->setData(data);
    
    QCoreApplication::postEvent(receiver, event);
    
    // 注意：不要手动删除事件，Qt会自动处理
    // 可以在事件处理完成后通过信号槽机制回收到池中
}
```

### 2. 事件压缩
```cpp
class EventCompressor : public QObject
{
    Q_OBJECT
    
public:
    void compressEvent(QObject *receiver, QEvent *event);
    void setCompressionInterval(int intervalMs);
    
private slots:
    void flushCompressedEvents();
    
private:
    struct CompressedEvent {
        QObject *receiver;
        QEvent *event;
        QDateTime timestamp;
    };
    
    QHash<QPair<QObject*, QEvent::Type>, CompressedEvent> m_compressedEvents;
    QTimer m_flushTimer;
    int m_compressionInterval = 50; // 50ms
};
```

### 3. 内存管理
```cpp
class SafeEventSender : public QObject
{
    Q_OBJECT
    
public:
    ~SafeEventSender() {
        // 清理所有待发送的事件
        cancelAllPendingEvents();
    }
    
    void postEvent(QObject *receiver, QEvent *event) {
        // 检查接收者有效性
        if (!receiver || !isValidReceiver(receiver)) {
            delete event;
            return;
        }
        
        // 记录事件，用于清理
        m_pendingEvents.insert(event, receiver);
        
        // 监听接收者销毁信号
        connect(receiver, &QObject::destroyed, this, &SafeEventSender::receiverDestroyed);
        
        QCoreApplication::postEvent(receiver, event);
    }
    
private slots:
    void receiverDestroyed(QObject *receiver) {
        // 移除指向已销毁对象的事件
        auto it = m_pendingEvents.begin();
        while (it != m_pendingEvents.end()) {
            if (it.value() == receiver) {
                QCoreApplication::removePostedEvents(receiver);
                it = m_pendingEvents.erase(it);
            } else {
                ++it;
            }
        }
    }
    
private:
    QHash<QEvent*, QObject*> m_pendingEvents;
    
    bool isValidReceiver(QObject *receiver) const;
    void cancelAllPendingEvents();
};
```

## 调试和测试

### 1. 事件跟踪器
```cpp
class CustomEventTracker : public QObject
{
    Q_OBJECT
    
public:
    static CustomEventTracker* instance() {
        static CustomEventTracker tracker;
        return &tracker;
    }
    
    void startTracking(const QList<QEvent::Type> &eventTypes = {});
    void stopTracking();
    
    void dumpEventLog() const;
    void clearEventLog();
    
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    
private:
    struct EventRecord {
        QDateTime timestamp;
        QObject *sender;
        QObject *receiver;
        QEvent::Type eventType;
        QString eventData;
        bool handled;
    };
    
    QList<EventRecord> m_eventLog;
    QSet<QEvent::Type> m_trackedTypes;
    bool m_tracking = false;
    
    QString extractEventData(QEvent *event) const;
};
```

### 2. 单元测试
```cpp
class CustomEventTest : public QObject
{
    Q_OBJECT
    
private slots:
    void testEventCreation() {
        DataTransferEvent event(QVariant("test data"));
        QCOMPARE(event.type(), DataTransferEvent::EventType);
        QCOMPARE(event.data().toString(), QString("test data"));
    }
    
    void testEventSending() {
        CustomEventReceiver receiver;
        QSignalSpy spy(&receiver, &CustomEventReceiver::dataReceived);
        
        DataTransferEvent *event = new DataTransferEvent(QVariant("test"));
        QCoreApplication::postEvent(&receiver, event);
        QCoreApplication::processEvents();
        
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.first().first().toString(), QString("test"));
    }
    
    void testEventFiltering() {
        CustomEventReceiver receiver;
        receiver.setEventFilter([](QEvent *event) {
            return event->type() != DataTransferEvent::EventType;
        });
        
        QSignalSpy spy(&receiver, &CustomEventReceiver::dataReceived);
        
        DataTransferEvent *event = new DataTransferEvent(QVariant("filtered"));
        QCoreApplication::postEvent(&receiver, event);
        QCoreApplication::processEvents();
        
        QCOMPARE(spy.count(), 0); // 事件被过滤
    }
};
```

## 学习建议

1. **理解事件生命周期**: 从创建到销毁的完整过程
2. **掌握内存管理**: 正确处理事件对象的内存分配和释放
3. **设计合理的事件层次**: 避免过度复杂的继承关系
4. **考虑性能影响**: 频繁的事件创建和发送可能影响性能
5. **测试事件处理**: 编写单元测试确保事件处理的正确性

## 相关资源

- [Qt官方文档 - 自定义事件](https://doc.qt.io/qt-6/eventsandfilters.html#sending-events)
- [上一个模块: 鼠标键盘事件](../mouse_keyboard/README.md)
- [下一个模块: 事件过滤器](../event_filters/README.md)
- [Qt6_Event_System_Guide_Part2.md](../../Qt6_Event_System_Guide_Part2.md) - 自定义事件详解