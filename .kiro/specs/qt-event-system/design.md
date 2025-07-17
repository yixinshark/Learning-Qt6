# Qt事件系统学习项目设计文档

## 概述

本项目通过构建一系列递进式的实践示例来深入学习Qt事件系统。设计采用模块化架构，每个模块专注于事件系统的特定方面，从基础概念到高级应用逐步深入。

项目将创建一个综合性的演示应用程序，包含多个独立的示例模块，每个模块都可以单独运行和学习，同时也能组合展示事件系统的完整工作流程。

## 架构设计

### 整体架构

```
02_event_system/
├── src/
│   ├── core/                    # 核心事件系统组件
│   │   ├── event_base.h/cpp     # 基础事件类和工具
│   │   ├── custom_events.h/cpp  # 自定义事件定义
│   │   └── event_logger.h/cpp   # 事件日志和调试工具
│   ├── examples/                # 示例模块
│   │   ├── basic_events/        # 基础事件处理示例
│   │   ├── mouse_keyboard/      # 鼠标键盘事件示例
│   │   ├── custom_events/       # 自定义事件示例
│   │   ├── event_filters/       # 事件过滤器示例
│   │   ├── event_propagation/   # 事件传播示例
│   │   └── advanced_patterns/   # 高级模式示例
│   ├── widgets/                 # 自定义控件
│   │   ├── event_display.h/cpp  # 事件显示控件
│   │   ├── interactive_area.h/cpp # 交互区域控件
│   │   └── debug_panel.h/cpp    # 调试面板控件
│   └── main.cpp                 # 主程序入口
├── CMakeLists.txt
└── README.md
```

### 核心组件设计

#### 1. 事件基础框架 (EventBase)

**职责：**
- 提供事件系统的基础工具类
- 实现事件类型注册和管理
- 提供事件创建和发送的便利方法

**接口设计：**
```cpp
class EventManager : public QObject {
    Q_OBJECT
public:
    static EventManager* instance();
    
    // 事件注册和管理
    void registerEventType(QEvent::Type type, const QString& name);
    QString getEventTypeName(QEvent::Type type) const;
    
    // 事件发送便利方法
    void postCustomEvent(QObject* receiver, QEvent* event);
    void sendCustomEvent(QObject* receiver, QEvent* event);
    
signals:
    void eventPosted(QObject* receiver, QEvent::Type type);
    void eventProcessed(QObject* receiver, QEvent::Type type);
};
```

#### 2. 自定义事件系统 (CustomEvents)

**职责：**
- 定义项目中使用的自定义事件类型
- 提供数据传递和事件创建的标准模式
- 实现事件数据的序列化和反序列化

**事件类型设计：**
```cpp
// 基础自定义事件
class BaseCustomEvent : public QEvent {
public:
    BaseCustomEvent(Type type);
    virtual ~BaseCustomEvent() = default;
    
    // 事件数据接口
    virtual QVariant data() const = 0;
    virtual void setData(const QVariant& data) = 0;
};

// 数据传递事件
class DataEvent : public BaseCustomEvent {
public:
    DataEvent(const QVariant& data);
    QVariant data() const override;
    void setData(const QVariant& data) override;
};

// 命令事件
class CommandEvent : public BaseCustomEvent {
public:
    CommandEvent(const QString& command, const QVariantMap& params = {});
    QString command() const;
    QVariantMap parameters() const;
};
```

#### 3. 事件日志系统 (EventLogger)

**职责：**
- 记录和跟踪事件的产生、传递和处理过程
- 提供事件流的可视化和分析工具
- 支持事件过滤和搜索功能

**设计特点：**
```cpp
class EventLogger : public QObject {
    Q_OBJECT
public:
    struct EventRecord {
        QDateTime timestamp;
        QObject* sender;
        QObject* receiver;
        QEvent::Type eventType;
        QString eventName;
        QString details;
        bool accepted;
    };
    
    void logEvent(const EventRecord& record);
    QList<EventRecord> getEventHistory() const;
    void clearHistory();
    
    // 过滤和搜索
    void setEventTypeFilter(const QSet<QEvent::Type>& types);
    void setObjectFilter(QObject* object);
    
signals:
    void eventLogged(const EventRecord& record);
};
```

### 示例模块设计

#### 1. 基础事件处理 (BasicEvents)

**学习目标：**
- 理解QEvent基类和事件类型
- 掌握event()函数的重写方法
- 学习事件的接受和忽略机制

**组件：**
- `BasicEventWidget`: 演示基础事件处理的控件
- `EventTypeDemo`: 展示不同事件类型的特点
- `EventHandlerDemo`: 演示事件处理器的实现

#### 2. 鼠标键盘事件 (MouseKeyboard)

**学习目标：**
- 掌握QMouseEvent和QKeyEvent的使用
- 理解事件属性的获取和判断
- 学习组合键和修饰键的处理

**组件：**
- `MouseEventWidget`: 鼠标事件演示控件
- `KeyboardEventWidget`: 键盘事件演示控件
- `InteractionDemo`: 鼠标键盘交互演示

#### 3. 自定义事件 (CustomEvents)

**学习目标：**
- 学习自定义事件类的创建
- 掌握事件的发送和接收机制
- 理解事件数据的传递方法

**组件：**
- `CustomEventSender`: 自定义事件发送器
- `CustomEventReceiver`: 自定义事件接收器
- `EventCommunicationDemo`: 事件通信演示

#### 4. 事件过滤器 (EventFilters)

**学习目标：**
- 掌握eventFilter()函数的使用
- 理解全局事件监控的实现
- 学习事件拦截和修改技术

**组件：**
- `GlobalEventFilter`: 全局事件过滤器
- `SelectiveEventFilter`: 选择性事件过滤器
- `EventModificationDemo`: 事件修改演示

#### 5. 事件传播 (EventPropagation)

**学习目标：**
- 理解事件在对象层次中的传播
- 掌握事件的接受和忽略机制
- 学习事件传播的控制方法

**组件：**
- `ParentChildEventDemo`: 父子对象事件传播演示
- `EventPropagationChain`: 事件传播链演示
- `EventInterceptionDemo`: 事件拦截演示

#### 6. 高级模式 (AdvancedPatterns)

**学习目标：**
- 掌握事件压缩和优化技术
- 理解跨线程事件通信
- 学习事件队列管理和调试

**组件：**
- `EventCompressionDemo`: 事件压缩演示
- `ThreadEventDemo`: 跨线程事件演示
- `EventQueueMonitor`: 事件队列监控器

### 用户界面设计

#### 主窗口布局

```
┌─────────────────────────────────────────────────────────────┐
│ Qt事件系统学习演示                                            │
├─────────────────────────────────────────────────────────────┤
│ [基础事件] [鼠标键盘] [自定义事件] [事件过滤] [事件传播] [高级] │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌─────────────────┐  ┌─────────────────────────────────┐   │
│  │   示例区域      │  │        事件日志区域             │   │
│  │                 │  │                                 │   │
│  │  [当前示例的    │  │  时间    | 事件类型 | 源对象    │   │
│  │   交互界面]     │  │  ──────────────────────────────  │   │
│  │                 │  │  14:30:25 | MousePress | Button │   │
│  │                 │  │  14:30:25 | MouseRelease| Button│   │
│  │                 │  │  14:30:26 | CustomEvent | Widget │   │
│  └─────────────────┘  └─────────────────────────────────┘   │
│                                                             │
├─────────────────────────────────────────────────────────────┤
│ 状态栏: 当前示例 | 事件计数 | 性能信息                      │
└─────────────────────────────────────────────────────────────┘
```

#### 调试面板设计

- **事件跟踪器**: 实时显示事件流
- **对象树视图**: 显示对象层次和事件传播路径
- **性能监控**: 显示事件处理性能统计
- **过滤控制**: 允许用户过滤特定类型的事件

### 数据模型

#### 事件记录模型

```cpp
class EventRecordModel : public QAbstractTableModel {
    Q_OBJECT
public:
    enum Column {
        Timestamp,
        EventType,
        Source,
        Target,
        Accepted,
        Details
    };
    
    void addEventRecord(const EventLogger::EventRecord& record);
    void clearRecords();
    void setFilter(const EventFilter& filter);
};
```

#### 对象层次模型

```cpp
class ObjectHierarchyModel : public QAbstractItemModel {
    Q_OBJECT
public:
    void setRootObject(QObject* root);
    void highlightEventPath(QObject* source, QObject* target);
    QModelIndex findObject(QObject* object) const;
};
```

### 错误处理

#### 事件处理错误

- **事件创建失败**: 提供详细的错误信息和建议
- **事件发送失败**: 检查目标对象的有效性
- **事件处理异常**: 捕获并记录处理过程中的异常

#### 调试支持

- **断言检查**: 在关键点添加断言验证
- **日志记录**: 详细记录事件处理的各个阶段
- **错误恢复**: 提供错误情况下的恢复机制

### 测试策略

#### 单元测试

- **事件创建测试**: 验证各种事件类型的正确创建
- **事件处理测试**: 测试事件处理器的正确性
- **事件传播测试**: 验证事件传播机制的正确性

#### 集成测试

- **端到端事件流测试**: 测试完整的事件处理流程
- **性能测试**: 验证事件系统的性能表现
- **并发测试**: 测试多线程环境下的事件处理

#### 用户验收测试

- **交互测试**: 验证用户交互的正确响应
- **可视化测试**: 确保事件日志和调试信息的准确性
- **学习效果测试**: 验证示例的教学效果

### 性能考虑

#### 事件处理优化

- **事件池**: 重用事件对象减少内存分配
- **批量处理**: 合并相似事件减少处理开销
- **延迟处理**: 对非关键事件使用延迟处理

#### 内存管理

- **智能指针**: 使用智能指针管理事件对象生命周期
- **内存池**: 为频繁创建的事件类型使用内存池
- **垃圾回收**: 定期清理过期的事件记录

#### 渲染优化

- **增量更新**: 只更新变化的UI部分
- **虚拟化**: 对大量事件记录使用虚拟化显示
- **缓存机制**: 缓存频繁访问的显示数据