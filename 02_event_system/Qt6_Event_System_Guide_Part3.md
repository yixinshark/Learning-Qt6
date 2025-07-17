# Qt6 事件系统技术解析 - 第三部分

## 9. 高级事件处理模式

### 9.1 事件压缩和批量处理

在高频事件场景下，事件压缩可以显著提高性能：

```cpp
// 事件压缩示例 - 合并重复的绘制事件
class OptimizedWidget : public QWidget {
public:
    OptimizedWidget(QWidget* parent = nullptr) : QWidget(parent) {
        // 启用事件压缩
        setAttribute(Qt::WA_OpaquePaintEvent);
        setAttribute(Qt::WA_NoSystemBackground);
    }
    
protected:
    void paintEvent(QPaintEvent* event) override {
        // 批量处理所有待绘制区域
        QPainter painter(this);
        
        // 获取所有需要更新的区域
        QRegion updateRegion = event->region();
        
        // 一次性处理所有区域，避免重复绘制
        for (const QRect& rect : updateRegion) {
            drawContent(&painter, rect);
        }
    }
    
    // 自定义更新策略
    void scheduleUpdate() {
        if (!m_updatePending) {
            m_updatePending = true;
            // 使用单次定时器延迟更新，合并多个更新请求
            QTimer::singleShot(0, this, [this]() {
                m_updatePending = false;
                update();
            });
        }
    }
    
private:
    bool m_updatePending = false;
    
    void drawContent(QPainter* painter, const QRect& rect) {
        // 只绘制指定区域的内容
        painter->fillRect(rect, Qt::lightGray);
    }
};
```

### 9.2 延迟事件处理

```cpp
// 延迟事件处理示例
class DelayedEventProcessor : public QObject {
    Q_OBJECT
    
public:
    // 延迟处理事件，避免频繁操作
    void scheduleDelayedAction(const QString& action) {
        m_pendingActions.append(action);
        
        // 重置定时器，实现防抖效果
        m_delayTimer.stop();
        m_delayTimer.start();
    }
    
private slots:
    void processDelayedActions() {
        if (m_pendingActions.isEmpty()) return;
        
        qDebug() << "批量处理延迟操作:" << m_pendingActions.size() << "个";
        
        // 批量处理所有待处理的操作
        for (const QString& action : m_pendingActions) {
            processAction(action);
        }
        
        m_pendingActions.clear();
    }
    
private:
    QTimer m_delayTimer;
    QStringList m_pendingActions;
    
    void setupTimer() {
        m_delayTimer.setSingleShot(true);
        m_delayTimer.setInterval(500); // 500ms延迟
        connect(&m_delayTimer, &QTimer::timeout, 
                this, &DelayedEventProcessor::processDelayedActions);
    }
    
    void processAction(const QString& action) {
        // 实际的操作处理逻辑
        qDebug() << "处理操作:" << action;
    }
};
```

### 9.3 跨线程事件通信

```cpp
// 跨线程事件通信示例
class ThreadSafeEventSender : public QObject {
    Q_OBJECT
    
public:
    // 线程安全的事件发送
    void sendEventToMainThread(const QString& data) {
        // 创建自定义事件
        CustomDataEvent* event = new CustomDataEvent(data);
        
        // 发送到主线程的目标对象
        QCoreApplication::postEvent(m_mainThreadReceiver, event);
    }
    
    // 从工作线程发送信号到主线程
    void sendSignalToMainThread(const QString& message) {
        // 使用Qt::QueuedConnection确保线程安全
        emit messageReady(message);
    }
    
signals:
    void messageReady(const QString& message);
    
private:
    QObject* m_mainThreadReceiver;
};

// 工作线程示例
class WorkerThread : public QThread {
    Q_OBJECT
    
protected:
    void run() override {
        ThreadSafeEventSender sender;
        
        // 连接信号到主线程槽函数
        connect(&sender, &ThreadSafeEventSender::messageReady,
                this, &WorkerThread::messageProcessed,
                Qt::QueuedConnection);
        
        // 模拟工作
        for (int i = 0; i < 10; ++i) {
            QString data = QString("工作数据 %1").arg(i);
            
            // 发送事件到主线程
            sender.sendEventToMainThread(data);
            
            // 发送信号到主线程
            sender.sendSignalToMainThread(data);
            
            msleep(1000); // 模拟工作延迟
        }
    }
    
signals:
    void messageProcessed(const QString& message);
};
```

## 10. 事件调试和性能优化

### 10.1 事件跟踪和日志

```cpp
// 事件跟踪器
class EventTracker : public QObject {
    Q_OBJECT
    
public:
    static EventTracker* instance() {
        static EventTracker tracker;
        return &tracker;
    }
    
    void installGlobalFilter() {
        qApp->installEventFilter(this);
    }
    
    void setEventTypeFilter(const QSet<QEvent::Type>& types) {
        m_filteredTypes = types;
    }
    
    void setObjectFilter(const QString& objectName) {
        m_filteredObjectName = objectName;
    }
    
protected:
    bool eventFilter(QObject* watched, QEvent* event) override {
        // 应用过滤条件
        if (!m_filteredTypes.isEmpty() && 
            !m_filteredTypes.contains(event->type())) {
            return false;
        }
        
        if (!m_filteredObjectName.isEmpty() && 
            watched->objectName() != m_filteredObjectName) {
            return false;
        }
        
        // 记录事件信息
        logEvent(watched, event);
        
        return false; // 不拦截事件
    }
    
private:
    QSet<QEvent::Type> m_filteredTypes;
    QString m_filteredObjectName;
    QElapsedTimer m_timer;
    
    void logEvent(QObject* watched, QEvent* event) {
        qint64 timestamp = m_timer.elapsed();
        
        qDebug() << QString("[%1ms] 事件: %2, 对象: %3, 类型: %4")
                    .arg(timestamp)
                    .arg(event->type())
                    .arg(watched->objectName())
                    .arg(watched->metaObject()->className());
    }
};
```

### 10.2 性能监控

```cpp
// 事件性能监控器
class EventPerformanceMonitor : public QObject {
    Q_OBJECT
    
public:
    struct EventStats {
        int count = 0;
        qint64 totalTime = 0;
        qint64 maxTime = 0;
        qint64 minTime = LLONG_MAX;
        
        double averageTime() const {
            return count > 0 ? static_cast<double>(totalTime) / count : 0.0;
        }
    };
    
    void startMonitoring(QObject* target) {
        target->installEventFilter(this);
    }
    
    void printStatistics() {
        qDebug() << "=== 事件性能统计 ===";
        for (auto it = m_stats.begin(); it != m_stats.end(); ++it) {
            const EventStats& stats = it.value();
            qDebug() << QString("事件类型 %1: 次数=%2, 平均耗时=%.2fms, 最大耗时=%3ms")
                        .arg(it.key())
                        .arg(stats.count)
                        .arg(stats.averageTime())
                        .arg(stats.maxTime);
        }
    }
    
protected:
    bool eventFilter(QObject* watched, QEvent* event) override {
        QElapsedTimer timer;
        timer.start();
        
        // 让事件正常处理
        bool result = QObject::eventFilter(watched, event);
        
        // 记录处理时间
        qint64 elapsed = timer.nsecsElapsed() / 1000000; // 转换为毫秒
        updateStats(event->type(), elapsed);
        
        return result;
    }
    
private:
    QHash<QEvent::Type, EventStats> m_stats;
    
    void updateStats(QEvent::Type type, qint64 elapsed) {
        EventStats& stats = m_stats[type];
        stats.count++;
        stats.totalTime += elapsed;
        stats.maxTime = qMax(stats.maxTime, elapsed);
        stats.minTime = qMin(stats.minTime, elapsed);
    }
};
```

### 10.3 内存优化

```cpp
// 事件对象池，减少内存分配
template<typename EventType>
class EventPool {
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
        
        return m_pool.takeLast();
    }
    
    void release(EventType* event) {
        QMutexLocker locker(&m_mutex);
        
        // 重置事件状态
        event->setAccepted(false);
        
        // 限制池大小，避免内存泄漏
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

// 使用事件池的示例
class OptimizedEventSender : public QObject {
public:
    void sendOptimizedEvent(QObject* receiver, const QString& data) {
        // 从池中获取事件对象
        CustomDataEvent* event = EventPool<CustomDataEvent>::instance().acquire();
        
        // 设置事件数据
        event->setData(data);
        
        // 发送事件
        QCoreApplication::postEvent(receiver, event);
        
        // 注意：不要手动删除事件，Qt会自动处理
        // 在事件处理完成后，可以通过自定义机制回收到池中
    }
};
```

## 11. 实际应用场景和最佳实践

### 11.1 拖拽操作实现

```cpp
// 拖拽操作的完整实现
class DragDropWidget : public QWidget {
    Q_OBJECT
    
public:
    DragDropWidget(QWidget* parent = nullptr) : QWidget(parent) {
        setAcceptDrops(true); // 启用拖放接收
    }
    
protected:
    // 开始拖拽
    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            m_dragStartPosition = event->pos();
        }
    }
    
    void mouseMoveEvent(QMouseEvent* event) override {
        if (!(event->buttons() & Qt::LeftButton)) return;
        
        // 检查是否达到拖拽距离阈值
        if ((event->pos() - m_dragStartPosition).manhattanLength() 
            < QApplication::startDragDistance()) {
            return;
        }
        
        // 开始拖拽操作
        startDrag();
    }
    
    // 拖拽进入
    void dragEnterEvent(QDragEnterEvent* event) override {
        if (event->mimeData()->hasText()) {
            event->acceptProposedAction();
            setStyleSheet("background-color: lightblue;");
        }
    }
    
    // 拖拽移动
    void dragMoveEvent(QDragMoveEvent* event) override {
        if (event->mimeData()->hasText()) {
            event->acceptProposedAction();
        }
    }
    
    // 拖拽离开
    void dragLeaveEvent(QDragLeaveEvent* event) override {
        setStyleSheet("");
        event->accept();
    }
    
    // 放置操作
    void dropEvent(QDropEvent* event) override {
        if (event->mimeData()->hasText()) {
            QString text = event->mimeData()->text();
            handleDroppedText(text);
            
            setStyleSheet("");
            event->acceptProposedAction();
        }
    }
    
private:
    QPoint m_dragStartPosition;
    
    void startDrag() {
        QDrag* drag = new QDrag(this);
        QMimeData* mimeData = new QMimeData;
        
        // 设置拖拽数据
        mimeData->setText("拖拽的文本数据");
        drag->setMimeData(mimeData);
        
        // 设置拖拽图标
        QPixmap pixmap(100, 50);
        pixmap.fill(Qt::lightGray);
        drag->setPixmap(pixmap);
        
        // 执行拖拽
        Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);
        
        if (dropAction == Qt::MoveAction) {
            qDebug() << "拖拽移动完成";
        } else if (dropAction == Qt::CopyAction) {
            qDebug() << "拖拽复制完成";
        }
    }
    
    void handleDroppedText(const QString& text) {
        qDebug() << "接收到拖放文本:" << text;
        // 处理放置的数据
    }
};
```

### 11.2 自定义控件的事件处理

```cpp
// 自定义按钮控件，演示完整的事件处理
class CustomButton : public QWidget {
    Q_OBJECT
    
public:
    CustomButton(const QString& text, QWidget* parent = nullptr)
        : QWidget(parent), m_text(text) {
        setFixedSize(120, 40);
        setFocusPolicy(Qt::StrongFocus);
    }
    
signals:
    void clicked();
    void pressed();
    void released();
    
protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        
        // 根据状态绘制不同样式
        QColor bgColor = Qt::lightGray;
        if (m_pressed) {
            bgColor = Qt::darkGray;
        } else if (m_hovered) {
            bgColor = Qt::gray;
        }
        
        // 绘制按钮背景
        painter.setBrush(bgColor);
        painter.setPen(Qt::black);
        painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 5, 5);
        
        // 绘制文本
        painter.setPen(Qt::black);
        painter.drawText(rect(), Qt::AlignCenter, m_text);
        
        // 绘制焦点指示器
        if (hasFocus()) {
            painter.setPen(QPen(Qt::blue, 2, Qt::DashLine));
            painter.drawRoundedRect(rect().adjusted(2, 2, -2, -2), 3, 3);
        }
    }
    
    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            m_pressed = true;
            update();
            emit pressed();
        }
    }
    
    void mouseReleaseEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton && m_pressed) {
            m_pressed = false;
            update();
            emit released();
            
            // 只有在按钮区域内释放才触发点击
            if (rect().contains(event->pos())) {
                emit clicked();
            }
        }
    }
    
    void enterEvent(QEnterEvent* event) override {
        m_hovered = true;
        update();
        QWidget::enterEvent(event);
    }
    
    void leaveEvent(QEvent* event) override {
        m_hovered = false;
        m_pressed = false; // 鼠标离开时取消按下状态
        update();
        QWidget::leaveEvent(event);
    }
    
    void keyPressEvent(QKeyEvent* event) override {
        if (event->key() == Qt::Key_Space || event->key() == Qt::Key_Return) {
            m_pressed = true;
            update();
            emit pressed();
        } else {
            QWidget::keyPressEvent(event);
        }
    }
    
    void keyReleaseEvent(QKeyEvent* event) override {
        if ((event->key() == Qt::Key_Space || event->key() == Qt::Key_Return) 
            && m_pressed) {
            m_pressed = false;
            update();
            emit released();
            emit clicked();
        } else {
            QWidget::keyReleaseEvent(event);
        }
    }
    
    void focusInEvent(QFocusEvent* event) override {
        update();
        QWidget::focusInEvent(event);
    }
    
    void focusOutEvent(QFocusEvent* event) override {
        m_pressed = false; // 失去焦点时取消按下状态
        update();
        QWidget::focusOutEvent(event);
    }
    
private:
    QString m_text;
    bool m_pressed = false;
    bool m_hovered = false;
};
```