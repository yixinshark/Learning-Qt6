# Qt6 事件系统技术解析 - 第二部分

## 5. 事件发送和接收机制

### 5.1 事件发送方式

Qt提供了两种主要的事件发送方式：

#### 异步发送 (postEvent)
```cpp
// 异步发送事件 - 事件被添加到队列中，稍后处理
QCoreApplication::postEvent(receiver, new QMouseEvent(
    QEvent::MouseButtonPress, 
    QPoint(100, 100), 
    Qt::LeftButton, 
    Qt::LeftButton, 
    Qt::NoModifier
));

// 带优先级的异步发送
QCoreApplication::postEvent(receiver, event, Qt::HighEventPriority);
```

#### 同步发送 (sendEvent)
```cpp
// 同步发送事件 - 立即处理，阻塞当前线程
QMouseEvent mouseEvent(QEvent::MouseButtonPress, 
                      QPoint(100, 100), 
                      Qt::LeftButton, 
                      Qt::LeftButton, 
                      Qt::NoModifier);
bool handled = QCoreApplication::sendEvent(receiver, &mouseEvent);
```

### 5.2 事件队列管理

```cpp
// 移除特定对象的所有待处理事件
QCoreApplication::removePostedEvents(receiver);

// 移除特定类型的事件
QCoreApplication::removePostedEvents(receiver, QEvent::MouseButtonPress);

// 立即处理所有待处理事件
QCoreApplication::processEvents();

// 处理事件，但最多处理指定时间
QCoreApplication::processEvents(QEventLoop::AllEvents, 100); // 100ms
```

### 5.3 事件优先级

Qt事件系统支持事件优先级，高优先级事件会被优先处理：

```cpp
enum Priority {
    HighEventPriority = 1,
    NormalEventPriority = 0,
    LowEventPriority = -1
};

// 发送高优先级事件
QCoreApplication::postEvent(receiver, event, Qt::HighEventPriority);
```

## 6. 自定义事件详解

### 6.1 创建自定义事件类

```cpp
// 自定义事件类示例
class CustomDataEvent : public QEvent {
public:
    // 注册自定义事件类型
    static const QEvent::Type EventType;
    
    explicit CustomDataEvent(const QString& data)
        : QEvent(EventType), m_data(data) {}
    
    QString data() const { return m_data; }
    
private:
    QString m_data;
};

// 在源文件中定义事件类型
const QEvent::Type CustomDataEvent::EventType = 
    static_cast<QEvent::Type>(QEvent::registerEventType());
```

### 6.2 发送自定义事件

```cpp
// 发送自定义事件的工具类
class EventSender : public QObject {
    Q_OBJECT
    
public slots:
    void sendCustomData(const QString& data) {
        // 创建并发送自定义事件
        CustomDataEvent* event = new CustomDataEvent(data);
        QCoreApplication::postEvent(m_receiver, event);
    }
    
private:
    QObject* m_receiver;
};
```

### 6.3 接收自定义事件

```cpp
// 接收自定义事件的控件
class CustomEventReceiver : public QWidget {
    Q_OBJECT
    
protected:
    bool event(QEvent* event) override {
        if (event->type() == CustomDataEvent::EventType) {
            // 处理自定义事件
            CustomDataEvent* customEvent = 
                static_cast<CustomDataEvent*>(event);
            
            qDebug() << "接收到自定义数据:" << customEvent->data();
            
            // 触发信号通知其他组件
            emit customDataReceived(customEvent->data());
            
            return true; // 事件已处理
        }
        
        return QWidget::event(event);
    }
    
signals:
    void customDataReceived(const QString& data);
};
```

## 7. 事件传播和冒泡机制

### 7.1 事件传播路径

Qt中的事件传播遵循以下路径：
1. 事件过滤器 (eventFilter)
2. 目标对象的event()函数
3. 特定事件处理函数 (如mousePressEvent)
4. 父对象的事件处理 (如果事件未被接受)

```cpp
// 演示事件传播的示例
class ParentWidget : public QWidget {
protected:
    void mousePressEvent(QMouseEvent* event) override {
        qDebug() << "父控件收到鼠标事件";
        // 如果不调用accept()，事件会继续传播
        event->ignore(); // 明确忽略事件
    }
};

class ChildWidget : public QWidget {
protected:
    void mousePressEvent(QMouseEvent* event) override {
        qDebug() << "子控件收到鼠标事件";
        
        if (shouldHandleEvent()) {
            // 处理事件并阻止进一步传播
            event->accept();
        } else {
            // 让父控件处理
            event->ignore();
        }
    }
};
```

### 7.2 控制事件传播

```cpp
// 事件传播控制示例
class EventPropagationDemo : public QWidget {
protected:
    bool event(QEvent* event) override {
        qDebug() << "事件类型:" << event->type();
        
        // 根据条件决定是否传播事件
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            
            if (keyEvent->key() == Qt::Key_Escape) {
                // 处理ESC键，阻止传播
                handleEscapeKey();
                event->accept();
                return true;
            }
        }
        
        // 调用基类处理其他事件
        return QWidget::event(event);
    }
    
private:
    void handleEscapeKey() {
        qDebug() << "ESC键被处理，事件传播已停止";
    }
};
```

## 8. 常用事件类型详解

### 8.1 鼠标事件 (QMouseEvent)

```cpp
// 鼠标事件处理示例
class MouseEventDemo : public QWidget {
protected:
    void mousePressEvent(QMouseEvent* event) override {
        // 获取鼠标位置
        QPoint localPos = event->pos();        // 相对于控件的位置
        QPoint globalPos = event->globalPos(); // 屏幕绝对位置
        
        // 获取按键信息
        Qt::MouseButton button = event->button();      // 触发事件的按键
        Qt::MouseButtons buttons = event->buttons();   // 当前按下的所有按键
        
        // 获取修饰键
        Qt::KeyboardModifiers modifiers = event->modifiers();
        
        qDebug() << "鼠标按下:" 
                 << "位置(" << localPos << ")"
                 << "按键(" << button << ")"
                 << "修饰键(" << modifiers << ")";
        
        // 根据按键执行不同操作
        switch (button) {
        case Qt::LeftButton:
            handleLeftClick(localPos);
            break;
        case Qt::RightButton:
            showContextMenu(globalPos);
            break;
        case Qt::MiddleButton:
            handleMiddleClick(localPos);
            break;
        }
    }
    
    void mouseMoveEvent(QMouseEvent* event) override {
        // 只有在鼠标按下时才跟踪移动
        if (event->buttons() & Qt::LeftButton) {
            // 实现拖拽功能
            handleDrag(event->pos());
        }
        
        // 更新鼠标悬停状态
        updateHoverState(event->pos());
    }
    
    void mouseDoubleClickEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            handleDoubleClick(event->pos());
        }
    }
};
```

### 8.2 键盘事件 (QKeyEvent)

```cpp
// 键盘事件处理示例
class KeyboardEventDemo : public QWidget {
protected:
    void keyPressEvent(QKeyEvent* event) override {
        // 获取按键信息
        int key = event->key();                    // 按键码
        QString text = event->text();              // 按键对应的文本
        Qt::KeyboardModifiers modifiers = event->modifiers(); // 修饰键
        
        qDebug() << "按键按下:" 
                 << "键码(" << key << ")"
                 << "文本(" << text << ")"
                 << "修饰键(" << modifiers << ")";
        
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
        }
        
        // 处理组合键
        if (modifiers & Qt::ControlModifier) {
            switch (key) {
            case Qt::Key_C:
                handleCopy();
                event->accept();
                return;
            case Qt::Key_V:
                handlePaste();
                event->accept();
                return;
            case Qt::Key_S:
                handleSave();
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
    
    void keyReleaseEvent(QKeyEvent* event) override {
        // 处理按键释放事件
        qDebug() << "按键释放:" << event->key();
        QWidget::keyReleaseEvent(event);
    }
};
```

### 8.3 绘制事件 (QPaintEvent)

```cpp
// 绘制事件处理示例
class PaintEventDemo : public QWidget {
protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        
        // 获取需要重绘的区域
        QRect updateRect = event->rect();
        qDebug() << "重绘区域:" << updateRect;
        
        // 设置抗锯齿
        painter.setRenderHint(QPainter::Antialiasing);
        
        // 绘制背景
        painter.fillRect(rect(), Qt::white);
        
        // 只绘制需要更新的部分以提高性能
        if (updateRect.intersects(m_circleRect)) {
            drawCircle(&painter);
        }
        
        if (updateRect.intersects(m_textRect)) {
            drawText(&painter);
        }
    }
    
    void resizeEvent(QResizeEvent* event) override {
        // 窗口大小改变时更新绘制区域
        updateDrawingAreas();
        QWidget::resizeEvent(event);
    }
    
private:
    QRect m_circleRect;
    QRect m_textRect;
    
    void drawCircle(QPainter* painter) {
        painter->setBrush(Qt::blue);
        painter->drawEllipse(m_circleRect);
    }
    
    void drawText(QPainter* painter) {
        painter->setPen(Qt::black);
        painter->drawText(m_textRect, Qt::AlignCenter, "Hello Qt!");
    }
    
    void updateDrawingAreas() {
        m_circleRect = QRect(10, 10, 100, 100);
        m_textRect = QRect(10, 120, width() - 20, 50);
    }
};
```

### 8.4 定时器事件 (QTimerEvent)

```cpp
// 定时器事件处理示例
class TimerEventDemo : public QWidget {
public:
    TimerEventDemo(QWidget* parent = nullptr) : QWidget(parent) {
        // 启动定时器
        m_timerId = startTimer(1000); // 1秒间隔
        m_fastTimerId = startTimer(100); // 100ms间隔
    }
    
    ~TimerEventDemo() {
        // 停止定时器
        if (m_timerId) {
            killTimer(m_timerId);
        }
        if (m_fastTimerId) {
            killTimer(m_fastTimerId);
        }
    }
    
protected:
    void timerEvent(QTimerEvent* event) override {
        int timerId = event->timerId();
        
        if (timerId == m_timerId) {
            // 处理慢速定时器
            handleSlowTimer();
        } else if (timerId == m_fastTimerId) {
            // 处理快速定时器
            handleFastTimer();
        }
    }
    
private:
    int m_timerId = 0;
    int m_fastTimerId = 0;
    int m_counter = 0;
    
    void handleSlowTimer() {
        m_counter++;
        qDebug() << "慢速定时器触发，计数:" << m_counter;
        
        // 更新UI
        update();
    }
    
    void handleFastTimer() {
        // 处理动画或快速更新
        updateAnimation();
    }
    
    void updateAnimation() {
        // 动画逻辑
        static int animationStep = 0;
        animationStep = (animationStep + 1) % 360;
        
        // 触发重绘
        update();
    }
};
```