# Qt6 事件系统深度学习项目

## 项目概述

本项目是一个专门用于深入学习Qt6事件系统的综合性演示程序。通过一系列递进式的实践示例，帮助开发者全面掌握Qt事件系统的核心概念、工作原理和实际应用。

## 当前状态

✅ **任务1已完成**: 项目基础结构和核心框架
- CMake项目配置，包含Qt6依赖
- 核心目录结构已建立
- EventManager单例类已实现
- 基础测试应用已创建并验证

## 学习目标

- 🎯 **理解事件系统架构**: 掌握Qt事件系统的基础架构和工作原理
- 🔧 **掌握事件处理机制**: 学会使用各种事件处理方法和技巧
- 🎨 **创建自定义事件**: 能够设计和实现自定义事件系统
- 🔍 **使用事件过滤器**: 掌握全局事件监控和过滤技术
- 📡 **理解事件传播**: 深入了解事件在对象层次中的传播机制
- ⚡ **优化事件性能**: 学习高级事件处理模式和性能优化技巧

## 项目结构

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
├── CMakeLists.txt               # CMake构建配置
├── README.md                    # 项目说明文档
└── Qt6_Event_System_Guide.md    # 详细技术解析文档
```

## 核心学习模块

### 1. 基础事件处理 (Basic Events)
- QEvent基类和事件类型系统
- event()函数的重写和事件分发
- 事件的接受(accept)和忽略(ignore)机制
- 事件处理的基本流程

### 2. 鼠标键盘事件 (Mouse & Keyboard)
- QMouseEvent: 鼠标按键、移动、滚轮事件
- QKeyEvent: 键盘按键、组合键、修饰键
- 事件属性的获取和状态判断
- 交互式事件处理演示

### 3. 自定义事件 (Custom Events)
- 创建自定义QEvent子类
- 事件的发送(postEvent/sendEvent)和接收
- 事件数据的封装和传递
- 跨组件通信的事件模式

### 4. 事件过滤器 (Event Filters)
- eventFilter()函数的实现和使用
- 全局事件监控和拦截
- 事件的修改和转发
- 选择性事件过滤技术

### 5. 事件传播 (Event Propagation)
- 父子对象间的事件传播机制
- 事件传播链和传递路径
- 事件的拦截和阻止传播
- 事件冒泡和捕获模式

### 6. 高级模式 (Advanced Patterns)
- 事件压缩和批量处理
- 延迟事件和定时器事件
- 跨线程事件通信
- 事件队列管理和性能优化

## 快速开始

### 环境要求

- Qt 6.2 或更高版本
- CMake 3.16 或更高版本
- C++17 兼容的编译器 (GCC 7+, Clang 6+, MSVC 2019+)

### 编译和运行

```bash
# 克隆或下载项目
cd 02_event_system

# 创建构建目录
mkdir build && cd build

# 配置CMake
cmake ..

# 编译项目
cmake --build .

# 运行演示程序
./Qt6EventSystemDemo
```

### 使用说明

1. **启动程序**: 运行可执行文件后，会看到主界面包含多个标签页
2. **选择示例**: 点击不同的标签页切换到相应的学习模块
3. **交互测试**: 在交互区域进行鼠标点击、键盘输入等操作
4. **查看日志**: 右侧的事件日志面板会实时显示事件信息
5. **调试分析**: 使用调试面板分析事件流和性能数据

## 学习路径建议

### 初学者路径
1. 📚 先阅读 `Qt6_Event_System_Guide.md` 了解理论基础
2. 🔧 从"基础事件处理"模块开始实践
3. 🖱️ 学习"鼠标键盘事件"掌握常用交互
4. 🎨 尝试"自定义事件"理解事件创建

### 进阶路径
1. 🔍 深入"事件过滤器"学习全局监控
2. 📡 掌握"事件传播"理解传递机制
3. ⚡ 探索"高级模式"优化性能
4. 🛠️ 结合实际项目应用所学知识

## 关键概念速览

### Qt事件系统核心组件

```cpp
// 事件基类 - 所有事件的基础
class QEvent {
public:
    enum Type {
        MouseButtonPress = 2,
        MouseButtonRelease = 3,
        KeyPress = 6,
        KeyRelease = 7,
        // ... 更多事件类型
        User = 1000  // 用户自定义事件起始值
    };
};

// 事件处理的核心方法
class QObject {
protected:
    virtual bool event(QEvent *event);        // 事件分发器
    virtual bool eventFilter(QObject *watched, QEvent *event); // 事件过滤器
};

// 事件发送方法
QCoreApplication::postEvent(receiver, event);  // 异步发送
QCoreApplication::sendEvent(receiver, event);  // 同步发送
```

### 常用事件类型

| 事件类型 | 说明 | 主要用途 |
|---------|------|----------|
| QMouseEvent | 鼠标事件 | 处理鼠标点击、移动、滚轮 |
| QKeyEvent | 键盘事件 | 处理按键输入、快捷键 |
| QPaintEvent | 绘制事件 | 控件重绘和自定义绘制 |
| QResizeEvent | 尺寸变化事件 | 响应窗口大小改变 |
| QCloseEvent | 关闭事件 | 处理窗口关闭逻辑 |
| QTimerEvent | 定时器事件 | 定时任务和动画 |

## 实际应用场景

### 1. 用户界面交互
- 按钮点击响应
- 拖拽操作实现
- 快捷键处理
- 上下文菜单

### 2. 自定义控件开发
- 重写事件处理器
- 实现特殊交互逻辑
- 控件状态管理
- 绘制优化

### 3. 应用程序架构
- 组件间通信
- 状态变化通知
- 异步操作处理
- 插件系统

### 4. 性能优化
- 事件批量处理
- 绘制事件优化
- 内存管理
- 响应性提升

## 调试和故障排除

### 常见问题

1. **事件未被处理**: 检查event()函数的返回值和事件类型判断
2. **事件过滤器不工作**: 确认installEventFilter()的调用和对象生命周期
3. **自定义事件接收失败**: 验证事件类型注册和接收对象的event()实现
4. **性能问题**: 使用事件日志分析事件频率和处理时间

### 调试技巧

```cpp
// 启用事件调试输出
bool MyWidget::event(QEvent *event) {
    qDebug() << "Event type:" << event->type() 
             << "for object:" << objectName();
    return QWidget::event(event);
}

// 使用事件过滤器进行全局监控
bool MyEventFilter::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        qDebug() << "Mouse press on:" << obj->objectName();
    }
    return QObject::eventFilter(obj, event);
}
```

## 贡献和反馈

欢迎提交问题报告、功能建议或代码贡献。如果你在学习过程中遇到问题或有改进建议，请通过以下方式联系：

- 📧 提交Issue描述问题
- 💡 分享学习心得和最佳实践
- 🔧 贡献代码示例和改进

## 许可证

本项目采用MIT许可证，详情请参阅LICENSE文件。

---

**开始你的Qt事件系统学习之旅吧！** 🚀