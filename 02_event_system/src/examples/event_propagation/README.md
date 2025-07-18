# Qt6 事件传播机制示例

## 概述

本模块深入演示Qt6事件传播和冒泡机制，展示事件在对象层次结构中的传递路径、拦截技术和传播控制。理解事件传播是掌握Qt事件系统的关键环节。

## 学习目标

- 理解Qt事件传播的完整路径和机制
- 掌握父子对象间的事件传播规则
- 学习事件拦截和阻止传播的技术
- 了解事件冒泡和捕获模式的区别
- 掌握复杂界面中的事件路由控制

## 主要组件

### 1. EventPropagationChain（事件传播链）

**文件**: `event_propagation_chain.h/cpp`

**功能特性**:
- 可视化展示事件传播路径
- 演示事件在对象层次中的流向
- 提供传播过程的详细日志
- 支持传播路径的自定义配置

**核心实现**:
```cpp
class EventPropagationChain : public QWidget
{
    Q_OBJECT
    
public:
    struct PropagationNode {
        QObject *object;
        QString name;
        bool intercepted = false;
        bool handled = false;
        QDateTime timestamp;
        QString eventInfo;
    };
    
    // 记录事件传播路径
    void recordPropagation(QObject *object, QEvent *event, bool handled);
    
    // 获取传播链
    QList<PropagationNode> getPropagationChain() const;
    
    // 清除传播记录
    void clearPropagationChain();
    
    // 设置传播可视化
    void setVisualizationEnabled(bool enabled);
    
protected:
    void paintEvent(QPaintEvent *event) override;
    
private:
    QList<PropagationNode> m_propagationChain;
    bool m_visualizationEnabled = true;
    
    void drawPropagationPath(QPainter *painter);
    void drawPropagationNode(QPainter *painter, const PropagationNode &node, const QPoint &position);
    
signals:
    void propagationRecorded(const PropagationNode &node);
    void propagationChainCompleted();
};
```

### 2. ParentChildEventDemo（父子事件演示）

**文件**: `parent_child_event_demo.h/cpp`

**功能特性**:
- 演示父子控件间的事件传播
- 展示事件接受和忽略的影响
- 提供交互式的传播控制
- 实现传播路径的实时监控

**层次结构示例**:
```cpp
class ParentWidget : public QWidget
{
    Q_OBJECT
    
public:
    ParentWidget(QWidget *parent = nullptr);
    
    // 设置事件传播策略
    void setPropagationStrategy(PropagationStrategy strategy);
    
    // 设置事件拦截规则
    void setInterceptionRules(const QHash<QEvent::Type, bool> &rules);
    
protected:
    bool event(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    
private:
    enum PropagationStrategy {
        AlwaysPropagate,    // 总是传播
        SelectivePropagate, // 选择性传播
        NeverPropagate,     // 从不传播
        ConditionalPropagate // 条件传播
    };
    
    PropagationStrategy m_strategy = AlwaysPropagate;
    QHash<QEvent::Type, bool> m_interceptionRules;
    
    bool shouldPropagateEvent(QEvent *event) const;
    void logEventHandling(QEvent *event, bool handled, bool propagated);
    
signals:
    void eventHandled(QObject *handler, QEvent::Type eventType, bool propagated);
};

class ChildWidget : public QWidget
{
    Q_OBJECT
    
public:
    ChildWidget(QWidget *parent = nullptr);
    
    // 设置事件处理模式
    void setEventHandlingMode(EventHandlingMode mode);
    
protected:
    bool event(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    
private:
    enum EventHandlingMode {
        HandleAll,      // 处理所有事件
        HandleNone,     // 不处理任何事件
        HandleSelective // 选择性处理
    };
    
    EventHandlingMode m_handlingMode = HandleSelective;
    QSet<QEvent::Type> m_handledEventTypes;
    
    bool shouldHandleEvent(QEvent *event) const;
};
```

### 3. EventInterceptionDemo（事件拦截演示）

**文件**: `event_interception_demo.h/cpp`

**功能特性**:
- 演示各种事件拦截技术
- 展示事件过滤器的拦截能力
- 实现条件拦截和智能路由
- 提供拦截统计和分析

**拦截器实现**:
```cpp
class EventInterceptor : public QObject
{
    Q_OBJECT
    
public:
    struct InterceptionRule {
        QEvent::Type eventType;
        std::function<bool(QObject*, QEvent*)> condition;
        std::function<void(QObject*, QEvent*)> action;
        bool stopPropagation = true;
        QString description;
    };
    
    // 添加拦截规则
    void addInterceptionRule(const InterceptionRule &rule);
    
    // 移除拦截规则
    void removeInterceptionRule(QEvent::Type eventType);
    
    // 启用/禁用拦截器
    void setEnabled(bool enabled);
    
    // 获取拦截统计
    QHash<QEvent::Type, int> getInterceptionStats() const;
    
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    
private:
    QList<InterceptionRule> m_rules;
    bool m_enabled = true;
    QHash<QEvent::Type, int> m_interceptionStats;
    
    bool processInterceptionRules(QObject *watched, QEvent *event);
    void recordInterception(QEvent::Type eventType);
    
signals:
    void eventIntercepted(QObject *target, QEvent::Type eventType, const QString &reason);
    void interceptionStatsChanged();
};

// 智能事件路由器
class SmartEventRouter : public QObject
{
    Q_OBJECT
    
public:
    struct RoutingRule {
        std::function<bool(QObject*, QEvent*)> matcher;
        std::function<QList<QObject*>(QObject*, QEvent*)> router;
        int priority = 0;
        QString name;
    };
    
    // 添加路由规则
    void addRoutingRule(const RoutingRule &rule);
    
    // 路由事件
    bool routeEvent(QObject *source, QEvent *event);
    
    // 设置默认路由
    void setDefaultRouter(std::function<QList<QObject*>(QObject*, QEvent*)> router);
    
private:
    QList<RoutingRule> m_routingRules;
    std::function<QList<QObject*>(QObject*, QEvent*)> m_defaultRouter;
    
    QList<QObject*> findTargets(QObject *source, QEvent *event);
    
signals:
    void eventRouted(QObject *source, const QList<QObject*> &targets, QEvent::Type eventType);
};
```

## 核心技术详解

### 1. 事件传播路径

#### 标准传播流程
```cpp
// Qt事件传播的标准路径：
// 1. 事件过滤器 (eventFilter)
// 2. 目标对象的event()函数
// 3. 特定事件处理函数 (如mousePressEvent)
// 4. 父对象的事件处理 (如果事件未被接受)

bool MyWidget::event(QEvent *event)
{
    // 记录事件到达
    logEventArrival(event);
    
    // 检查是否应该处理此事件
    if (shouldHandleEvent(event)) {
        bool handled = handleSpecificEvent(event);
        if (handled) {
            logEventHandled(event, true);
            return true;
        }
    }
    
    // 调用基类处理
    bool baseHandled = QWidget::event(event);
    logEventHandled(event, baseHandled);
    
    return baseHandled;
}
```

#### 传播控制机制
```cpp
class PropagationController : public QObject
{
public:
    enum PropagationMode {
        Bubble,     // 冒泡模式：从子到父
        Capture,    // 捕获模式：从父到子
        Direct,     // 直接模式：只处理目标对象
        Broadcast   // 广播模式：发送给所有相关对象
    };
    
    void setPropagationMode(QEvent::Type eventType, PropagationMode mode);
    
    bool controlPropagation(QObject *source, QEvent *event) {
        PropagationMode mode = m_propagationModes.value(event->type(), Bubble);
        
        switch (mode) {
        case Bubble:
            return handleBubblePropagation(source, event);
        case Capture:
            return handleCapturePropagation(source, event);
        case Direct:
            return handleDirectPropagation(source, event);
        case Broadcast:
            return handleBroadcastPropagation(source, event);
        }
        
        return false;
    }
    
private:
    QHash<QEvent::Type, PropagationMode> m_propagationModes;
    
    bool handleBubblePropagation(QObject *source, QEvent *event);
    bool handleCapturePropagation(QObject *source, QEvent *event);
    bool handleDirectPropagation(QObject *source, QEvent *event);
    bool handleBroadcastPropagation(QObject *source, QEvent *event);
};
```

### 2. 高级拦截技术

#### 条件拦截器
```cpp
class ConditionalInterceptor : public QObject
{
    Q_OBJECT
    
public:
    // 基于时间的拦截
    void addTimeBasedRule(QEvent::Type eventType, const QTime &startTime, const QTime &endTime);
    
    // 基于状态的拦截
    void addStateBasedRule(QEvent::Type eventType, std::function<bool()> stateChecker);
    
    // 基于频率的拦截
    void addFrequencyBasedRule(QEvent::Type eventType, int maxEventsPerSecond);
    
    // 基于用户权限的拦截
    void addPermissionBasedRule(QEvent::Type eventType, const QString &requiredPermission);
    
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    
private:
    struct TimeRule {
        QTime startTime, endTime;
    };
    
    struct FrequencyRule {
        int maxEvents;
        QQueue<QDateTime> eventTimes;
    };
    
    QHash<QEvent::Type, TimeRule> m_timeRules;
    QHash<QEvent::Type, std::function<bool()>> m_stateRules;
    QHash<QEvent::Type, FrequencyRule> m_frequencyRules;
    QHash<QEvent::Type, QString> m_permissionRules;
    
    bool checkTimeRule(QEvent::Type eventType) const;
    bool checkStateRule(QEvent::Type eventType) const;
    bool checkFrequencyRule(QEvent::Type eventType);
    bool checkPermissionRule(QEvent::Type eventType) const;
};
```

#### 智能事件分发器
```cpp
class SmartEventDispatcher : public QObject
{
    Q_OBJECT
    
public:
    struct DispatchRule {
        std::function<bool(QEvent*)> matcher;
        std::function<QList<QObject*>(QEvent*)> targetSelector;
        std::function<QEvent*(QEvent*)> eventTransformer;
        int priority = 0;
    };
    
    void addDispatchRule(const DispatchRule &rule);
    void removeDispatchRule(int index);
    
    // 智能分发事件
    bool dispatchEvent(QEvent *event);
    
    // 设置分发策略
    void setDispatchStrategy(DispatchStrategy strategy);
    
private:
    enum DispatchStrategy {
        FirstMatch,     // 第一个匹配的规则
        AllMatches,     // 所有匹配的规则
        BestMatch,      // 最佳匹配的规则
        PriorityBased   // 基于优先级的匹配
    };
    
    QList<DispatchRule> m_dispatchRules;
    DispatchStrategy m_strategy = FirstMatch;
    
    QList<int> findMatchingRules(QEvent *event) const;
    int findBestMatch(QEvent *event) const;
    bool executeDispatchRule(const DispatchRule &rule, QEvent *event);
    
signals:
    void eventDispatched(QEvent *event, const QList<QObject*> &targets);
    void dispatchFailed(QEvent *event, const QString &reason);
};
```

### 3. 复杂传播场景

#### 多层嵌套控件
```cpp
class NestedWidgetDemo : public QWidget
{
    Q_OBJECT
    
public:
    NestedWidgetDemo(QWidget *parent = nullptr) {
        setupNestedStructure();
        installPropagationMonitor();
    }
    
private:
    void setupNestedStructure() {
        // 创建多层嵌套结构
        // Level 1: 主容器
        auto *mainContainer = new QWidget(this);
        
        // Level 2: 分组容器
        auto *groupContainer = new QWidget(mainContainer);
        
        // Level 3: 内容容器
        auto *contentContainer = new QWidget(groupContainer);
        
        // Level 4: 交互控件
        auto *button = new QPushButton("Click Me", contentContainer);
        auto *lineEdit = new QLineEdit(contentContainer);
        
        // 为每个层级安装事件监控
        installEventMonitor(mainContainer, "MainContainer");
        installEventMonitor(groupContainer, "GroupContainer");
        installEventMonitor(contentContainer, "ContentContainer");
        installEventMonitor(button, "Button");
        installEventMonitor(lineEdit, "LineEdit");
    }
    
    void installEventMonitor(QWidget *widget, const QString &name) {
        auto *monitor = new EventMonitor(name, this);
        widget->installEventFilter(monitor);
        
        connect(monitor, &EventMonitor::eventProcessed,
                this, &NestedWidgetDemo::onEventProcessed);
    }
    
    void installPropagationMonitor() {
        m_propagationMonitor = new PropagationMonitor(this);
        qApp->installEventFilter(m_propagationMonitor);
    }
    
private slots:
    void onEventProcessed(const QString &widgetName, QEvent::Type eventType, bool handled) {
        m_eventLog.append(QString("[%1] %2: %3 (handled: %4)")
                         .arg(QTime::currentTime().toString("hh:mm:ss.zzz"))
                         .arg(widgetName)
                         .arg(eventType)
                         .arg(handled));
        
        updateEventDisplay();
    }
    
private:
    PropagationMonitor *m_propagationMonitor;
    QStringList m_eventLog;
};
```

#### 跨窗口事件传播
```cpp
class CrossWindowPropagator : public QObject
{
    Q_OBJECT
    
public:
    // 注册窗口到传播网络
    void registerWindow(QWidget *window, const QString &windowId);
    
    // 注销窗口
    void unregisterWindow(const QString &windowId);
    
    // 跨窗口发送事件
    void propagateEvent(const QString &sourceWindowId, const QString &targetWindowId, QEvent *event);
    
    // 广播事件到所有窗口
    void broadcastEvent(const QString &sourceWindowId, QEvent *event);
    
    // 设置传播规则
    void setPropagationRule(const QString &sourceWindowId, const QString &targetWindowId, 
                           std::function<bool(QEvent*)> rule);
    
private:
    struct WindowInfo {
        QWidget *widget;
        QString id;
        QHash<QString, std::function<bool(QEvent*)>> propagationRules;
    };
    
    QHash<QString, WindowInfo> m_windows;
    
    bool canPropagate(const QString &sourceId, const QString &targetId, QEvent *event) const;
    
signals:
    void eventPropagated(const QString &sourceId, const QString &targetId, QEvent::Type eventType);
    void propagationBlocked(const QString &sourceId, const QString &targetId, QEvent::Type eventType, const QString &reason);
};
```

## 实际应用示例

### 1. 模态对话框系统
```cpp
class ModalDialogSystem : public QObject
{
    Q_OBJECT
    
public:
    // 显示模态对话框
    int showModal(QDialog *dialog, QWidget *parent = nullptr);
    
    // 设置模态级别
    void setModalLevel(QDialog *dialog, int level);
    
    // 检查事件是否应该被模态对话框拦截
    bool shouldInterceptForModal(QObject *target, QEvent *event) const;
    
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    
private:
    struct ModalInfo {
        QDialog *dialog;
        int level;
        QWidget *parent;
        QDateTime showTime;
    };
    
    QList<ModalInfo> m_modalStack;
    
    bool isEventAllowedForModal(QEvent *event) const;
    QDialog* getTopModalDialog() const;
    bool isWidgetUnderModal(QWidget *widget, QDialog *modal) const;
    
signals:
    void modalShown(QDialog *dialog);
    void modalHidden(QDialog *dialog);
    void eventInterceptedByModal(QObject *target, QEvent::Type eventType);
};
```

### 2. 拖拽系统
```cpp
class DragDropPropagationSystem : public QObject
{
    Q_OBJECT
    
public:
    // 开始拖拽操作
    void startDrag(QWidget *source, const QMimeData *data, Qt::DropActions supportedActions);
    
    // 设置拖拽传播规则
    void setDragPropagationRule(std::function<bool(QWidget*, QDragEnterEvent*)> rule);
    
    // 设置放置传播规则
    void setDropPropagationRule(std::function<bool(QWidget*, QDropEvent*)> rule);
    
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    
private:
    bool handleDragEnterEvent(QWidget *widget, QDragEnterEvent *event);
    bool handleDragMoveEvent(QWidget *widget, QDragMoveEvent *event);
    bool handleDragLeaveEvent(QWidget *widget, QDragLeaveEvent *event);
    bool handleDropEvent(QWidget *widget, QDropEvent *event);
    
    void propagateDragEvent(QWidget *source, QEvent *event);
    QList<QWidget*> findDragTargets(QWidget *source, QEvent *event) const;
    
    std::function<bool(QWidget*, QDragEnterEvent*)> m_dragPropagationRule;
    std::function<bool(QWidget*, QDropEvent*)> m_dropPropagationRule;
    
    QWidget *m_currentDragSource = nullptr;
    QList<QWidget*> m_dragTargets;
    
signals:
    void dragStarted(QWidget *source);
    void dragEntered(QWidget *target);
    void dragLeft(QWidget *target);
    void dropCompleted(QWidget *target, Qt::DropAction action);
};
```

### 3. 快捷键系统
```cpp
class ShortcutPropagationSystem : public QObject
{
    Q_OBJECT
    
public:
    struct ShortcutInfo {
        QKeySequence sequence;
        std::function<void()> action;
        QWidget *context = nullptr;  // nullptr表示全局快捷键
        bool propagateAfterAction = false;
        int priority = 0;
    };
    
    // 注册快捷键
    void registerShortcut(const ShortcutInfo &shortcut);
    
    // 注销快捷键
    void unregisterShortcut(const QKeySequence &sequence, QWidget *context = nullptr);
    
    // 设置快捷键传播策略
    void setShortcutPropagationStrategy(PropagationStrategy strategy);
    
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    
private:
    enum PropagationStrategy {
        StopOnFirstMatch,   // 第一个匹配后停止
        ExecuteAllMatches,  // 执行所有匹配的快捷键
        PriorityBased      // 基于优先级执行
    };
    
    QList<ShortcutInfo> m_shortcuts;
    PropagationStrategy m_strategy = StopOnFirstMatch;
    QKeySequence m_currentSequence;
    QTimer m_sequenceTimer;
    
    bool handleKeyEvent(QKeyEvent *event);
    QList<ShortcutInfo> findMatchingShortcuts(const QKeySequence &sequence, QWidget *context) const;
    bool executeShortcut(const ShortcutInfo &shortcut);
    bool isShortcutApplicable(const ShortcutInfo &shortcut, QWidget *focusWidget) const;
    
signals:
    void shortcutExecuted(const QKeySequence &sequence, QWidget *context);
    void shortcutBlocked(const QKeySequence &sequence, const QString &reason);
};
```

## 性能优化

### 1. 传播路径优化
```cpp
class OptimizedPropagationManager : public QObject
{
    Q_OBJECT
    
public:
    // 缓存传播路径
    void cachePropagationPath(QObject *source, QEvent::Type eventType, const QList<QObject*> &path);
    
    // 获取缓存的传播路径
    QList<QObject*> getCachedPropagationPath(QObject *source, QEvent::Type eventType) const;
    
    // 清除传播路径缓存
    void clearPropagationCache();
    
    // 优化传播顺序
    void optimizePropagationOrder(const QList<QObject*> &objects);
    
private:
    struct PathCacheKey {
        QObject *source;
        QEvent::Type eventType;
        
        bool operator==(const PathCacheKey &other) const {
            return source == other.source && eventType == other.eventType;
        }
    };
    
    QHash<PathCacheKey, QList<QObject*>> m_pathCache;
    QHash<QObject*, int> m_propagationPriority;
    
    uint qHash(const PathCacheKey &key) const {
        return qHash(key.source) ^ qHash(key.eventType);
    }
};
```

### 2. 事件批处理
```cpp
class BatchedPropagationProcessor : public QObject
{
    Q_OBJECT
    
public:
    // 添加事件到批处理队列
    void addEventToBatch(QObject *target, QEvent *event);
    
    // 设置批处理大小
    void setBatchSize(int size);
    
    // 设置批处理间隔
    void setBatchInterval(int intervalMs);
    
    // 立即处理所有批处理事件
    void flushBatch();
    
private slots:
    void processBatch();
    
private:
    struct BatchedEvent {
        QObject *target;
        QEvent *event;
        QDateTime timestamp;
    };
    
    QList<BatchedEvent> m_eventBatch;
    QTimer m_batchTimer;
    int m_batchSize = 10;
    int m_batchInterval = 50; // 50ms
    
    void processBatchedEvents(const QList<BatchedEvent> &events);
    bool canBatchEvents(const BatchedEvent &event1, const BatchedEvent &event2) const;
};
```

## 调试和分析工具

### 1. 传播路径可视化
```cpp
class PropagationVisualizer : public QWidget
{
    Q_OBJECT
    
public:
    PropagationVisualizer(QWidget *parent = nullptr);
    
    // 设置要可视化的对象层次
    void setObjectHierarchy(QObject *root);
    
    // 记录事件传播
    void recordPropagation(QObject *source, QObject *target, QEvent::Type eventType);
    
    // 清除可视化数据
    void clearVisualization();
    
protected:
    void paintEvent(QPaintEvent *event) override;
    
private:
    struct VisualNode {
        QObject *object;
        QString name;
        QPoint position;
        QList<VisualNode*> children;
        QList<QPair<QEvent::Type, QDateTime>> recentEvents;
    };
    
    VisualNode *m_rootNode = nullptr;
    QList<QPair<QObject*, QObject*>> m_propagationPaths;
    
    void buildVisualHierarchy(QObject *object, VisualNode *parentNode);
    void layoutNodes(VisualNode *node, const QRect &rect);
    void drawNode(QPainter *painter, const VisualNode *node);
    void drawPropagationPath(QPainter *painter, QObject *source, QObject *target);
};
```

### 2. 性能分析器
```cpp
class PropagationPerformanceAnalyzer : public QObject
{
    Q_OBJECT
    
public:
    struct PropagationMetrics {
        QEvent::Type eventType;
        int totalPropagations = 0;
        qint64 totalTime = 0;
        qint64 averageTime = 0;
        qint64 maxTime = 0;
        qint64 minTime = LLONG_MAX;
        int maxDepth = 0;
        QDateTime lastPropagation;
    };
    
    // 开始性能分析
    void startAnalysis();
    
    // 停止性能分析
    void stopAnalysis();
    
    // 记录传播性能
    void recordPropagation(QEvent::Type eventType, qint64 duration, int depth);
    
    // 获取性能指标
    PropagationMetrics getMetrics(QEvent::Type eventType) const;
    
    // 获取所有指标
    QHash<QEvent::Type, PropagationMetrics> getAllMetrics() const;
    
    // 生成性能报告
    QString generatePerformanceReport() const;
    
private:
    QHash<QEvent::Type, PropagationMetrics> m_metrics;
    bool m_analyzing = false;
    QElapsedTimer m_timer;
    
    void updateMetrics(QEvent::Type eventType, qint64 duration, int depth);
    
signals:
    void metricsUpdated(QEvent::Type eventType, const PropagationMetrics &metrics);
    void performanceThresholdExceeded(QEvent::Type eventType, qint64 duration);
};
```

## 学习建议

1. **理解传播机制**: 深入理解Qt事件传播的完整流程
2. **实践传播控制**: 通过实际代码练习事件拦截和路由
3. **分析复杂场景**: 研究多层嵌套控件中的事件传播
4. **优化传播性能**: 学习减少不必要的事件传播
5. **调试传播问题**: 掌握传播问题的诊断和解决方法

## 相关资源

- [Qt官方文档 - 事件传播](https://doc.qt.io/qt-6/eventsandfilters.html#event-propagation)
- [上一个模块: 自定义事件](../custom_events/README.md)
- [下一个模块: 高级事件模式](../advanced_patterns/README.md)
- [Qt6_Event_System_Guide_Part2.md](../../Qt6_Event_System_Guide_Part2.md) - 事件传播详解