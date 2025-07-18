# Qt6 鼠标键盘事件处理示例

## 概述

本模块专门演示Qt6中鼠标和键盘事件的处理技术，涵盖了从基础的点击、按键响应到复杂的拖拽、组合键处理等各种交互场景。这是构建用户友好界面的核心技能。

## 学习目标

- 掌握QMouseEvent的各种属性和处理方法
- 学习QKeyEvent的按键识别和修饰键处理
- 理解鼠标拖拽操作的实现原理
- 掌握键盘快捷键和组合键的处理技巧
- 学习交互反馈和用户体验优化

## 主要组件

### 1. MouseEventWidget（鼠标事件控件）

**文件**: `mouse_event_widget.h/cpp`

**功能特性**:
- 完整的鼠标事件处理演示
- 支持单击、双击、拖拽操作
- 实时显示鼠标位置和按键状态
- 可视化鼠标轨迹和交互区域

**核心事件处理**:
```cpp
// 鼠标按下事件
void mousePressEvent(QMouseEvent *event) override;

// 鼠标移动事件
void mouseMoveEvent(QMouseEvent *event) override;

// 鼠标释放事件
void mouseReleaseEvent(QMouseEvent *event) override;

// 鼠标双击事件
void mouseDoubleClickEvent(QMouseEvent *event) override;

// 鼠标滚轮事件
void wheelEvent(QWheelEvent *event) override;
```

**演示功能**:
- 左键点击创建圆形标记
- 右键点击显示上下文菜单
- 中键点击清除所有标记
- 拖拽操作移动图形元素
- 滚轮缩放视图内容

### 2. KeyboardEventWidget（键盘事件控件）

**文件**: `keyboard_event_widget.h/cpp`

**功能特性**:
- 全面的键盘事件处理演示
- 支持普通按键、功能键、组合键
- 实时显示按键状态和修饰键
- 文本输入和编辑功能演示

**核心事件处理**:
```cpp
// 按键按下事件
void keyPressEvent(QKeyEvent *event) override;

// 按键释放事件
void keyReleaseEvent(QKeyEvent *event) override;

// 输入法事件处理
void inputMethodEvent(QInputMethodEvent *event) override;
```

**演示功能**:
- 方向键控制光标移动
- Ctrl+组合键实现快捷操作
- 功能键触发特殊功能
- 文本输入和字符过滤
- 按键序列识别和处理

### 3. InteractionDemo（交互演示）

**文件**: `interaction_demo.h/cpp`

**功能特性**:
- 综合的鼠标键盘交互演示
- 实现复杂的用户交互场景
- 展示最佳实践和设计模式
- 提供性能优化示例

**交互场景**:
- 绘图工具：鼠标绘制，键盘切换工具
- 游戏控制：WASD移动，鼠标瞄准
- 文本编辑：选择、复制、粘贴操作
- 图形操作：选择、移动、缩放、旋转

## 核心技术详解

### 1. 鼠标事件处理

#### 基础鼠标事件
```cpp
void MouseEventWidget::mousePressEvent(QMouseEvent *event)
{
    // 获取鼠标位置信息
    QPoint localPos = event->pos();        // 相对于控件的位置
    QPoint globalPos = event->globalPos(); // 屏幕绝对位置
    
    // 获取按键信息
    Qt::MouseButton button = event->button();      // 触发事件的按键
    Qt::MouseButtons buttons = event->buttons();   // 当前按下的所有按键
    
    // 获取修饰键状态
    Qt::KeyboardModifiers modifiers = event->modifiers();
    
    // 根据按键执行不同操作
    switch (button) {
    case Qt::LeftButton:
        if (modifiers & Qt::ControlModifier) {
            handleCtrlLeftClick(localPos);
        } else {
            handleLeftClick(localPos);
        }
        break;
        
    case Qt::RightButton:
        showContextMenu(globalPos);
        break;
        
    case Qt::MiddleButton:
        handleMiddleClick(localPos);
        break;
    }
    
    // 记录按下位置，用于拖拽检测
    m_lastPressPos = localPos;
    m_dragStarted = false;
    
    update(); // 触发重绘
}
```

#### 拖拽操作实现
```cpp
void MouseEventWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint currentPos = event->pos();
    
    // 检查是否开始拖拽
    if (event->buttons() & Qt::LeftButton) {
        if (!m_dragStarted) {
            // 检查拖拽距离阈值
            int dragDistance = (currentPos - m_lastPressPos).manhattanLength();
            if (dragDistance >= QApplication::startDragDistance()) {
                m_dragStarted = true;
                startDragOperation(m_lastPressPos);
            }
        }
        
        if (m_dragStarted) {
            updateDragOperation(currentPos);
        }
    }
    
    // 更新鼠标悬停状态
    updateHoverState(currentPos);
    
    // 实时更新位置显示
    emit mousePositionChanged(currentPos);
}
```

#### 滚轮事件处理
```cpp
void MouseEventWidget::wheelEvent(QWheelEvent *event)
{
    // 获取滚轮增量
    QPoint angleDelta = event->angleDelta();
    QPoint pixelDelta = event->pixelDelta();
    
    // 检查修饰键
    Qt::KeyboardModifiers modifiers = event->modifiers();
    
    if (modifiers & Qt::ControlModifier) {
        // Ctrl+滚轮：缩放操作
        double scaleFactor = 1.0;
        if (angleDelta.y() > 0) {
            scaleFactor = 1.1;  // 放大
        } else if (angleDelta.y() < 0) {
            scaleFactor = 0.9;  // 缩小
        }
        
        applyScale(scaleFactor, event->position());
        
    } else {
        // 普通滚轮：滚动操作
        if (!pixelDelta.isNull()) {
            // 高精度滚动（触摸板）
            scrollBy(pixelDelta);
        } else {
            // 传统滚轮
            QPoint scrollDelta = angleDelta / 8 / 15 * 20; // 转换为像素
            scrollBy(scrollDelta);
        }
    }
    
    event->accept();
}
```

### 2. 键盘事件处理

#### 基础按键处理
```cpp
void KeyboardEventWidget::keyPressEvent(QKeyEvent *event)
{
    // 获取按键信息
    int key = event->key();                    // 按键码
    QString text = event->text();              // 按键对应的文本
    Qt::KeyboardModifiers modifiers = event->modifiers(); // 修饰键
    
    // 记录按键状态
    m_pressedKeys.insert(key);
    
    // 处理特殊按键
    switch (key) {
    case Qt::Key_Return:
    case Qt::Key_Enter:
        handleEnterKey();
        event->accept();
        return;
        
    case Qt::Key_Escape:
        handleEscapeKey();
        event->accept();
        return;
        
    case Qt::Key_Delete:
        if (modifiers & Qt::ShiftModifier) {
            handleShiftDelete();
        } else {
            handleDelete();
        }
        event->accept();
        return;
        
    case Qt::Key_Backspace:
        handleBackspace();
        event->accept();
        return;
    }
    
    // 处理方向键
    if (key >= Qt::Key_Left && key <= Qt::Key_Down) {
        handleArrowKey(key, modifiers);
        event->accept();
        return;
    }
    
    // 处理功能键
    if (key >= Qt::Key_F1 && key <= Qt::Key_F12) {
        handleFunctionKey(key);
        event->accept();
        return;
    }
    
    // 处理组合键
    if (modifiers & Qt::ControlModifier) {
        if (handleControlCombination(key)) {
            event->accept();
            return;
        }
    }
    
    if (modifiers & Qt::AltModifier) {
        if (handleAltCombination(key)) {
            event->accept();
            return;
        }
    }
    
    // 处理字符输入
    if (!text.isEmpty() && text[0].isPrint()) {
        handleTextInput(text);
        event->accept();
    } else {
        // 未处理的按键传递给基类
        QWidget::keyPressEvent(event);
    }
}
```

#### 组合键处理
```cpp
bool KeyboardEventWidget::handleControlCombination(int key)
{
    switch (key) {
    case Qt::Key_C:
        copyToClipboard();
        return true;
        
    case Qt::Key_V:
        pasteFromClipboard();
        return true;
        
    case Qt::Key_X:
        cutToClipboard();
        return true;
        
    case Qt::Key_Z:
        undo();
        return true;
        
    case Qt::Key_Y:
        redo();
        return true;
        
    case Qt::Key_A:
        selectAll();
        return true;
        
    case Qt::Key_S:
        save();
        return true;
        
    case Qt::Key_O:
        open();
        return true;
        
    case Qt::Key_N:
        newDocument();
        return true;
        
    default:
        return false;
    }
}
```

#### 按键序列识别
```cpp
void KeyboardEventWidget::handleKeySequence(QKeyEvent *event)
{
    // 添加按键到序列
    m_keySequence.append(event->key());
    
    // 限制序列长度
    if (m_keySequence.size() > MAX_SEQUENCE_LENGTH) {
        m_keySequence.removeFirst();
    }
    
    // 检查预定义的按键序列
    if (checkSequence({Qt::Key_Up, Qt::Key_Up, Qt::Key_Down, Qt::Key_Down})) {
        activateCheatCode();
        m_keySequence.clear();
    } else if (checkSequence({Qt::Key_Escape, Qt::Key_Escape})) {
        emergencyExit();
        m_keySequence.clear();
    }
    
    // 启动序列重置定时器
    m_sequenceTimer.start(SEQUENCE_TIMEOUT);
}
```

### 3. 高级交互技术

#### 多点触控支持
```cpp
void InteractionDemo::touchEvent(QTouchEvent *event)
{
    const QList<QTouchEvent::TouchPoint> &touchPoints = event->touchPoints();
    
    switch (event->type()) {
    case QEvent::TouchBegin:
        handleTouchBegin(touchPoints);
        break;
        
    case QEvent::TouchUpdate:
        if (touchPoints.count() == 1) {
            // 单点触控：移动
            handleSingleTouchMove(touchPoints.first());
        } else if (touchPoints.count() == 2) {
            // 双点触控：缩放和旋转
            handleTwoFingerGesture(touchPoints);
        }
        break;
        
    case QEvent::TouchEnd:
        handleTouchEnd(touchPoints);
        break;
    }
    
    event->accept();
}
```

#### 手势识别
```cpp
void InteractionDemo::handleTwoFingerGesture(const QList<QTouchEvent::TouchPoint> &points)
{
    if (points.size() != 2) return;
    
    const QTouchEvent::TouchPoint &p1 = points[0];
    const QTouchEvent::TouchPoint &p2 = points[1];
    
    // 计算当前距离和角度
    QPointF center = (p1.pos() + p2.pos()) / 2;
    qreal currentDistance = QLineF(p1.pos(), p2.pos()).length();
    qreal currentAngle = QLineF(p1.pos(), p2.pos()).angle();
    
    if (m_gestureState == GestureActive) {
        // 计算缩放比例
        qreal scaleFactor = currentDistance / m_lastGestureDistance;
        
        // 计算旋转角度
        qreal rotationDelta = currentAngle - m_lastGestureAngle;
        
        // 应用变换
        applyGestureTransform(center, scaleFactor, rotationDelta);
    }
    
    // 更新手势状态
    m_lastGestureDistance = currentDistance;
    m_lastGestureAngle = currentAngle;
    m_gestureState = GestureActive;
}
```

## 实际应用示例

### 1. 绘图应用
```cpp
class DrawingWidget : public QWidget
{
public:
    DrawingWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setMouseTracking(true);  // 启用鼠标跟踪
        m_currentTool = PenTool;
        m_penColor = Qt::black;
        m_penWidth = 2;
    }
    
protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            m_drawing = true;
            m_lastPoint = event->pos();
            
            switch (m_currentTool) {
            case PenTool:
                startDrawing(event->pos());
                break;
            case EraserTool:
                startErasing(event->pos());
                break;
            case SelectTool:
                startSelection(event->pos());
                break;
            }
        }
    }
    
    void mouseMoveEvent(QMouseEvent *event) override {
        if (m_drawing && (event->buttons() & Qt::LeftButton)) {
            switch (m_currentTool) {
            case PenTool:
                drawLineTo(event->pos());
                break;
            case EraserTool:
                eraseAt(event->pos());
                break;
            case SelectTool:
                updateSelection(event->pos());
                break;
            }
        }
        
        // 更新光标
        updateCursor(event->pos());
    }
    
    void keyPressEvent(QKeyEvent *event) override {
        switch (event->key()) {
        case Qt::Key_P:
            m_currentTool = PenTool;
            break;
        case Qt::Key_E:
            m_currentTool = EraserTool;
            break;
        case Qt::Key_S:
            m_currentTool = SelectTool;
            break;
        case Qt::Key_Plus:
            m_penWidth = qMin(m_penWidth + 1, 20);
            break;
        case Qt::Key_Minus:
            m_penWidth = qMax(m_penWidth - 1, 1);
            break;
        }
        
        update();
    }
    
private:
    enum Tool { PenTool, EraserTool, SelectTool };
    Tool m_currentTool;
    bool m_drawing = false;
    QPoint m_lastPoint;
    QColor m_penColor;
    int m_penWidth;
    QPixmap m_canvas;
};
```

### 2. 游戏控制
```cpp
class GameWidget : public QWidget
{
public:
    GameWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setFocusPolicy(Qt::StrongFocus);
        
        // 启动游戏循环定时器
        m_gameTimer.setInterval(16); // 60 FPS
        connect(&m_gameTimer, &QTimer::timeout, this, &GameWidget::updateGame);
        m_gameTimer.start();
    }
    
protected:
    void keyPressEvent(QKeyEvent *event) override {
        m_pressedKeys.insert(event->key());
        
        // 处理即时响应的按键
        switch (event->key()) {
        case Qt::Key_Space:
            if (!event->isAutoRepeat()) {
                fireWeapon();
            }
            break;
        case Qt::Key_P:
            togglePause();
            break;
        }
    }
    
    void keyReleaseEvent(QKeyEvent *event) override {
        m_pressedKeys.remove(event->key());
    }
    
    void mouseMoveEvent(QMouseEvent *event) override {
        // 鼠标控制瞄准方向
        QPoint center = rect().center();
        QPoint delta = event->pos() - center;
        m_aimDirection = qAtan2(delta.y(), delta.x());
        
        // 隐藏鼠标光标（游戏模式）
        setCursor(Qt::BlankCursor);
    }
    
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            fireWeapon();
        } else if (event->button() == Qt::RightButton) {
            useSecondaryWeapon();
        }
    }
    
private slots:
    void updateGame() {
        // 处理持续按键状态
        QPointF movement(0, 0);
        
        if (m_pressedKeys.contains(Qt::Key_W) || m_pressedKeys.contains(Qt::Key_Up)) {
            movement.setY(-1);
        }
        if (m_pressedKeys.contains(Qt::Key_S) || m_pressedKeys.contains(Qt::Key_Down)) {
            movement.setY(1);
        }
        if (m_pressedKeys.contains(Qt::Key_A) || m_pressedKeys.contains(Qt::Key_Left)) {
            movement.setX(-1);
        }
        if (m_pressedKeys.contains(Qt::Key_D) || m_pressedKeys.contains(Qt::Key_Right)) {
            movement.setX(1);
        }
        
        // 标准化移动向量
        if (movement.manhattanLength() > 0) {
            qreal length = sqrt(movement.x() * movement.x() + movement.y() * movement.y());
            movement /= length;
            movePlayer(movement * m_playerSpeed);
        }
        
        update();
    }
    
private:
    QSet<int> m_pressedKeys;
    QTimer m_gameTimer;
    qreal m_aimDirection = 0;
    qreal m_playerSpeed = 5.0;
    bool m_paused = false;
};
```

## 性能优化技巧

### 1. 事件过滤优化
```cpp
// 只在需要时启用鼠标跟踪
void OptimizedWidget::setTrackingEnabled(bool enabled)
{
    setMouseTracking(enabled);
    if (enabled) {
        // 安装高精度定时器用于平滑更新
        m_updateTimer.start(16); // 60 FPS
    } else {
        m_updateTimer.stop();
    }
}
```

### 2. 事件批处理
```cpp
void OptimizedWidget::mouseMoveEvent(QMouseEvent *event)
{
    // 将鼠标移动事件添加到队列
    m_mouseEventQueue.enqueue(event->pos());
    
    // 限制队列大小
    while (m_mouseEventQueue.size() > MAX_QUEUE_SIZE) {
        m_mouseEventQueue.dequeue();
    }
    
    // 使用定时器批量处理
    if (!m_processingTimer.isActive()) {
        m_processingTimer.start(10); // 100 FPS处理频率
    }
}
```

### 3. 内存优化
```cpp
// 使用对象池减少内存分配
class EventPool
{
public:
    static QMouseEvent* acquireMouseEvent() {
        if (s_mouseEventPool.isEmpty()) {
            return new QMouseEvent(QEvent::MouseMove, QPoint(), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
        }
        return s_mouseEventPool.takeLast();
    }
    
    static void releaseMouseEvent(QMouseEvent* event) {
        if (s_mouseEventPool.size() < MAX_POOL_SIZE) {
            s_mouseEventPool.append(event);
        } else {
            delete event;
        }
    }
    
private:
    static QList<QMouseEvent*> s_mouseEventPool;
    static const int MAX_POOL_SIZE = 100;
};
```

## 调试和测试

### 1. 事件日志记录
```cpp
class EventLogger : public QObject
{
public:
    static void logMouseEvent(QMouseEvent *event, const QString &handler) {
        qDebug() << QString("[%1] %2: pos(%3,%4) button=%5 buttons=%6 modifiers=%7")
                    .arg(QTime::currentTime().toString("hh:mm:ss.zzz"))
                    .arg(handler)
                    .arg(event->pos().x())
                    .arg(event->pos().y())
                    .arg(event->button())
                    .arg(event->buttons())
                    .arg(event->modifiers());
    }
    
    static void logKeyEvent(QKeyEvent *event, const QString &handler) {
        qDebug() << QString("[%1] %2: key=%3 text='%4' modifiers=%5 autoRepeat=%6")
                    .arg(QTime::currentTime().toString("hh:mm:ss.zzz"))
                    .arg(handler)
                    .arg(event->key())
                    .arg(event->text())
                    .arg(event->modifiers())
                    .arg(event->isAutoRepeat());
    }
};
```

### 2. 单元测试示例
```cpp
class MouseEventTest : public QObject
{
    Q_OBJECT
    
private slots:
    void testMouseClick() {
        MouseEventWidget widget;
        
        // 模拟鼠标点击
        QMouseEvent clickEvent(QEvent::MouseButtonPress, QPoint(100, 100), 
                              Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
        
        QApplication::sendEvent(&widget, &clickEvent);
        
        // 验证结果
        QVERIFY(widget.hasClickAt(QPoint(100, 100)));
    }
    
    void testKeyboardShortcut() {
        KeyboardEventWidget widget;
        
        // 模拟Ctrl+S按键
        QKeyEvent keyEvent(QEvent::KeyPress, Qt::Key_S, Qt::ControlModifier, "s");
        
        QSignalSpy spy(&widget, &KeyboardEventWidget::saveRequested);
        QApplication::sendEvent(&widget, &keyEvent);
        
        QCOMPARE(spy.count(), 1);
    }
};
```

## 学习建议

1. **循序渐进**: 从简单的点击响应开始，逐步学习复杂交互
2. **实践为主**: 多写代码，多做实验，理解事件处理的细节
3. **调试技巧**: 使用qDebug()输出事件信息，观察事件流
4. **性能意识**: 注意事件处理的性能影响，避免阻塞UI线程
5. **用户体验**: 考虑用户的使用习惯，提供直观的交互反馈

## 相关资源

- [Qt官方文档 - 鼠标和键盘事件](https://doc.qt.io/qt-6/eventsandfilters.html)
- [上一个模块: 基础事件处理](../basic_events/README.md)
- [下一个模块: 自定义事件](../custom_events/README.md)
- [Qt6_Event_System_Guide_Part2.md](../../Qt6_Event_System_Guide_Part2.md) - 详细技术解析