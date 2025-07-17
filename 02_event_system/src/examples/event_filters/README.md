# Qt6 事件过滤器系统

## 概述

本模块实现了一个完整的Qt事件过滤器系统，演示了如何使用事件过滤器来监控、拦截和修改应用程序中的事件。

## 主要组件

### 1. GlobalEventFilter（全局事件过滤器）

**功能特性：**
- 监控应用程序中的所有指定类型事件
- 支持事件拦截（阻止事件传递给目标对象）
- 支持事件修改（模拟事件内容的改变）
- 提供详细的统计信息
- 支持动态配置过滤的事件类型和监视对象

**主要方法：**
- `setEnabled(bool)` - 启用/禁用过滤器
- `setInterceptMode(bool)` - 启用/禁用拦截模式
- `setEventModificationEnabled(bool)` - 启用/禁用事件修改
- `addWatchedObject(QObject*)` - 添加监视对象
- `addFilteredEventType(QEvent::Type)` - 添加过滤的事件类型

**信号：**
- `eventFiltered(QObject*, QEvent*, bool)` - 事件被过滤时发出
- `eventModified(QObject*, QEvent::Type, QEvent::Type)` - 事件被修改时发出

### 2. SelectiveEventFilter（选择性事件过滤器）

**功能特性：**
- 基于条件的精细事件过滤控制
- 支持白名单和黑名单模式的事件类型过滤
- 支持基于鼠标按钮、按键、对象名称、对象类名的条件过滤
- 支持自定义过滤规则（使用lambda函数）
- 支持事件转换器
- 提供详细的统计信息

**主要方法：**
- `setAllowedEventTypes(const QSet<QEvent::Type>&)` - 设置允许的事件类型
- `setBlockedEventTypes(const QSet<QEvent::Type>&)` - 设置阻止的事件类型
- `setMouseButtonFilter(Qt::MouseButtons)` - 设置鼠标按钮过滤
- `setKeyFilter(const QSet<int>&)` - 设置按键过滤
- `addFilterRule(const QString&, FilterRule)` - 添加自定义过滤规则
- `addEventTransformer(QEvent::Type, EventTransformer)` - 添加事件转换器

**信号：**
- `eventPassed(QObject*, QEvent*)` - 事件通过过滤器时发出
- `eventBlocked(QObject*, QEvent*, const QString&)` - 事件被阻止时发出
- `eventTransformed(QObject*, QEvent*, QEvent*)` - 事件被转换时发出

### 3. EventModificationDemo（事件修改演示）

**功能特性：**
- 提供完整的图形界面来演示事件过滤器功能
- 实时显示事件过滤日志
- 显示统计信息
- 提供测试区域来触发各种事件
- 支持动态配置过滤器参数

**界面组件：**
- 全局过滤器控制面板
- 选择性过滤器控制面板
- 事件日志显示区域
- 测试区域（按钮和标签）
- 实时统计信息显示

## 使用方法

### 编译和运行

1. 确保已安装Qt6开发环境
2. 进入事件过滤器目录：
   ```bash
   cd 02_event_system/src/examples/event_filters
   ```
3. 编译并运行演示程序：
   ```bash
   # 如果使用CMake构建系统
   mkdir build && cd build
   cmake .. && make
   ./main_event_filters
   
   # 或者直接编译单个文件
   g++ -fPIC -std=c++17 $(pkg-config --cflags Qt6Core Qt6Widgets) \
       main_event_filters.cpp event_modification_demo.cpp \
       global_event_filter.cpp selective_event_filter.cpp \
       $(pkg-config --libs Qt6Core Qt6Widgets) -o event_filter_demo
   ```

### 演示操作

1. **启用全局过滤器**：
   - 勾选"启用全局过滤器"复选框
   - 在测试区域进行鼠标点击、键盘输入等操作
   - 观察事件日志中的过滤信息

2. **测试事件拦截**：
   - 启用全局过滤器后，勾选"拦截模式"
   - 尝试右键点击或按ESC键
   - 这些事件将被拦截，不会传递给目标对象

3. **测试事件修改**：
   - 启用全局过滤器后，勾选"事件修改"
   - 进行左键点击或按A键
   - 观察控制台输出的事件修改信息

4. **配置选择性过滤器**：
   - 勾选"启用选择性过滤器"
   - 配置允许的事件类型、鼠标按钮、按键等
   - 观察只有符合条件的事件才会通过过滤器

5. **查看统计信息**：
   - 统计信息会实时更新，显示处理的事件数量
   - 可以点击"重置统计"按钮清零计数器

## 技术要点

### 事件过滤器工作原理

1. **安装过滤器**：使用`installEventFilter()`方法将过滤器安装到目标对象上
2. **事件处理**：当事件发送给目标对象时，会先调用过滤器的`eventFilter()`方法
3. **过滤决策**：过滤器可以选择处理事件（返回true）或让事件继续传递（返回false）
4. **事件修改**：虽然不能直接修改现有事件，但可以创建新事件并发送

### 过滤器链

- 一个对象可以安装多个事件过滤器
- 后安装的过滤器先被调用
- 如果任何一个过滤器返回true，事件就不会继续传递

### 性能考虑

- 事件过滤器会增加事件处理的开销
- 应该只过滤必要的事件类型
- 避免在过滤器中进行耗时操作

## 扩展功能

### 自定义过滤规则

```cpp
// 添加自定义过滤规则
selectiveFilter->addFilterRule("weekend_block", 
    [](QObject* watched, QEvent* event) -> bool {
        QDateTime now = QDateTime::currentDateTime();
        int dayOfWeek = now.date().dayOfWeek();
        return dayOfWeek < 6;  // 周末阻止操作
    });
```

### 事件转换器

```cpp
// 添加事件转换器
selectiveFilter->addEventTransformer(QEvent::MouseButtonPress,
    [](QObject* watched, QEvent* event) -> QEvent* {
        // 创建新的事件对象
        return new QMouseEvent(QEvent::MouseButtonPress, ...);
    });
```

## 应用场景

1. **输入验证**：限制用户只能输入特定类型的字符
2. **快捷键处理**：全局捕获和处理快捷键
3. **调试和监控**：监控应用程序的事件流
4. **用户界面增强**：添加自定义的交互行为
5. **安全控制**：阻止某些危险操作

## 注意事项

1. 事件过滤器应该尽快处理事件，避免阻塞UI线程
2. 在过滤器中创建新事件时要注意内存管理
3. 过滤器的生命周期应该与被监视对象保持一致
4. 谨慎使用事件拦截，可能会影响正常的用户交互