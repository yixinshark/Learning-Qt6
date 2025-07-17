#include "interaction_demo.h"
#include <QApplication>
#include <QDebug>
#include <QSplitter>

InteractionDemo::InteractionDemo(QWidget *parent)
    : QWidget(parent)
    , m_mainLayout(nullptr)
    , m_tabWidget(nullptr)
    , m_basicTab(nullptr)
    , m_mouseWidget(nullptr)
    , m_keyboardWidget(nullptr)
    , m_interactionTab(nullptr)
    , m_interactiveArea(nullptr)
    , m_controlGroup(nullptr)
    , m_statsGroup(nullptr)
    , m_resetButton(nullptr)
    , m_animationCheckBox(nullptr)
    , m_speedSlider(nullptr)
    , m_sizeSpinBox(nullptr)
    , m_modeComboBox(nullptr)
    , m_mouseStatsLabel(nullptr)
    , m_keyboardStatsLabel(nullptr)
    , m_interactionStatsLabel(nullptr)
    , m_activityProgress(nullptr)
    , m_mouseEventCount(0)
    , m_keyboardEventCount(0)
    , m_interactionCount(0)
    , m_activityTimer(new QTimer(this))
    , m_animationEnabled(true)
{
    setupUI();
    
    // 设置活动监控定时器
    m_activityTimer->setSingleShot(true);
    connect(m_activityTimer, &QTimer::timeout, [this]() {
        m_activityProgress->setValue(0);
    });
}

void InteractionDemo::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_tabWidget = new QTabWidget(this);
    
    // 创建基础事件演示标签页
    m_basicTab = new QWidget();
    QHBoxLayout *basicLayout = new QHBoxLayout(m_basicTab);
    
    m_mouseWidget = new MouseEventWidget(m_basicTab);
    m_keyboardWidget = new KeyboardEventWidget(m_basicTab);
    
    basicLayout->addWidget(m_mouseWidget);
    basicLayout->addWidget(m_keyboardWidget);
    
    // 连接信号
    connect(m_mouseWidget, &MouseEventWidget::mouseEventOccurred,
            this, &InteractionDemo::onMouseEventOccurred);
    connect(m_keyboardWidget, &KeyboardEventWidget::keyEventOccurred,
            this, &InteractionDemo::onKeyEventOccurred);
    connect(m_keyboardWidget, &KeyboardEventWidget::shortcutTriggered,
            this, &InteractionDemo::onShortcutTriggered);
    
    m_tabWidget->addTab(m_basicTab, "基础事件演示");
    
    setupInteractiveArea();
    setupControlPanel();
    
    m_mainLayout->addWidget(m_tabWidget);
    
    // 设置最小尺寸
    setMinimumSize(800, 600);
}

void InteractionDemo::setupInteractiveArea()
{
    // 创建交互演示标签页
    m_interactionTab = new QWidget();
    QVBoxLayout *interactionLayout = new QVBoxLayout(m_interactionTab);
    
    // 创建分割器
    QSplitter *splitter = new QSplitter(Qt::Horizontal, m_interactionTab);
    
    // 创建交互式绘图区域
    m_interactiveArea = new InteractiveDrawArea(splitter);
    
    // 创建控制面板容器
    QWidget *controlContainer = new QWidget(splitter);
    controlContainer->setMaximumWidth(250);
    QVBoxLayout *controlLayout = new QVBoxLayout(controlContainer);
    
    // 控制组
    m_controlGroup = new QGroupBox("绘图控制", controlContainer);
    QVBoxLayout *controlGroupLayout = new QVBoxLayout(m_controlGroup);
    
    // 绘图模式选择
    QLabel *modeLabel = new QLabel("绘图模式:", m_controlGroup);
    m_modeComboBox = new QComboBox(m_controlGroup);
    m_modeComboBox->addItems({"点绘制", "线绘制", "矩形", "圆形"});
    connect(m_modeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int index) {
                if (auto drawArea = qobject_cast<InteractiveDrawArea*>(m_interactiveArea)) {
                    drawArea->setDrawMode(index);
                }
            });
    
    // 画笔大小
    QLabel *sizeLabel = new QLabel("画笔大小:", m_controlGroup);
    m_sizeSpinBox = new QSpinBox(m_controlGroup);
    m_sizeSpinBox->setRange(1, 20);
    m_sizeSpinBox->setValue(3);
    connect(m_sizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int value) {
                if (auto drawArea = qobject_cast<InteractiveDrawArea*>(m_interactiveArea)) {
                    drawArea->setBrushSize(value);
                }
            });
    
    // 动画控制
    m_animationCheckBox = new QCheckBox("启用动画效果", m_controlGroup);
    m_animationCheckBox->setChecked(true);
    connect(m_animationCheckBox, &QCheckBox::toggled, this, &InteractionDemo::toggleAnimation);
    
    // 速度控制
    QLabel *speedLabel = new QLabel("动画速度:", m_controlGroup);
    m_speedSlider = new QSlider(Qt::Horizontal, m_controlGroup);
    m_speedSlider->setRange(1, 10);
    m_speedSlider->setValue(5);
    
    // 重置按钮
    m_resetButton = new QPushButton("清除画布", m_controlGroup);
    connect(m_resetButton, &QPushButton::clicked, this, &InteractionDemo::resetDemo);
    
    controlGroupLayout->addWidget(modeLabel);
    controlGroupLayout->addWidget(m_modeComboBox);
    controlGroupLayout->addWidget(sizeLabel);
    controlGroupLayout->addWidget(m_sizeSpinBox);
    controlGroupLayout->addWidget(m_animationCheckBox);
    controlGroupLayout->addWidget(speedLabel);
    controlGroupLayout->addWidget(m_speedSlider);
    controlGroupLayout->addWidget(m_resetButton);
    
    // 统计组
    m_statsGroup = new QGroupBox("事件统计", controlContainer);
    QVBoxLayout *statsLayout = new QVBoxLayout(m_statsGroup);
    
    m_mouseStatsLabel = new QLabel("鼠标事件: 0", m_statsGroup);
    m_keyboardStatsLabel = new QLabel("键盘事件: 0", m_statsGroup);
    m_interactionStatsLabel = new QLabel("交互次数: 0", m_statsGroup);
    
    m_activityProgress = new QProgressBar(m_statsGroup);
    m_activityProgress->setRange(0, 100);
    m_activityProgress->setValue(0);
    QLabel *activityLabel = new QLabel("活动强度:", m_statsGroup);
    
    statsLayout->addWidget(m_mouseStatsLabel);
    statsLayout->addWidget(m_keyboardStatsLabel);
    statsLayout->addWidget(m_interactionStatsLabel);
    statsLayout->addWidget(activityLabel);
    statsLayout->addWidget(m_activityProgress);
    
    controlLayout->addWidget(m_controlGroup);
    controlLayout->addWidget(m_statsGroup);
    controlLayout->addStretch();
    
    splitter->addWidget(m_interactiveArea);
    splitter->addWidget(controlContainer);
    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 1);
    
    interactionLayout->addWidget(splitter);
    
    m_tabWidget->addTab(m_interactionTab, "交互式演示");
}
void InteractionDemo::setupControlPanel()
{
    // 控制面板已在setupInteractiveArea中设置
}

void InteractionDemo::onMouseEventOccurred(const QString &eventType, const QPoint &position)
{
    m_mouseEventCount++;
    m_interactionCount++;
    updateInteractionStats();
    
    // 更新活动强度
    int activity = qMin(100, m_activityProgress->value() + 10);
    m_activityProgress->setValue(activity);
    m_activityTimer->start(2000);  // 2秒后重置活动强度
    
    qDebug() << "Mouse event:" << eventType << "at" << position;
}

void InteractionDemo::onKeyEventOccurred(const QString &eventType, int keyCode, const QString &keyText)
{
    m_keyboardEventCount++;
    m_interactionCount++;
    updateInteractionStats();
    
    // 更新活动强度
    int activity = qMin(100, m_activityProgress->value() + 5);
    m_activityProgress->setValue(activity);
    m_activityTimer->start(2000);
    
    qDebug() << "Keyboard event:" << eventType << "key:" << keyCode << "text:" << keyText;
}

void InteractionDemo::onShortcutTriggered(const QString &shortcut)
{
    qDebug() << "Shortcut triggered:" << shortcut;
    
    // 特殊快捷键处理
    if (shortcut.contains("Ctrl+S")) {
        // 模拟保存操作
        qDebug() << "Save operation triggered";
    }
}

void InteractionDemo::resetDemo()
{
    if (auto drawArea = qobject_cast<InteractiveDrawArea*>(m_interactiveArea)) {
        drawArea->clearCanvas();
    }
    
    m_mouseEventCount = 0;
    m_keyboardEventCount = 0;
    m_interactionCount = 0;
    m_activityProgress->setValue(0);
    updateInteractionStats();
}

void InteractionDemo::toggleAnimation(bool enabled)
{
    m_animationEnabled = enabled;
    if (auto drawArea = qobject_cast<InteractiveDrawArea*>(m_interactiveArea)) {
        drawArea->setAnimationEnabled(enabled);
    }
}

void InteractionDemo::updateInteractionStats()
{
    m_mouseStatsLabel->setText(QString("鼠标事件: %1").arg(m_mouseEventCount));
    m_keyboardStatsLabel->setText(QString("键盘事件: %1").arg(m_keyboardEventCount));
    m_interactionStatsLabel->setText(QString("交互次数: %1").arg(m_interactionCount));
}

// InteractiveDrawArea 实现
InteractiveDrawArea::InteractiveDrawArea(QWidget *parent)
    : QWidget(parent)
    , m_canvas(400, 300)
    , m_drawing(false)
    , m_drawMode(1)  // 默认线绘制模式
    , m_brushSize(3)
    , m_currentColor(Qt::black)
    , m_animationTimer(new QTimer(this))
    , m_animationEnabled(true)
    , m_animationStep(0)
    , m_selectedObject(-1)
    , m_dragging(false)
{
    setMinimumSize(400, 300);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    
    // 初始化画布
    m_canvas.fill(Qt::white);
    
    // 设置动画定时器
    m_animationTimer->setInterval(50);  // 20 FPS
    connect(m_animationTimer, &QTimer::timeout, this, &InteractiveDrawArea::updateAnimation);
    
    if (m_animationEnabled) {
        m_animationTimer->start();
    }
    
    // 创建一些初始交互对象
    InteractiveObject obj1;
    obj1.rect = QRect(50, 50, 60, 40);
    obj1.color = Qt::red;
    obj1.selected = false;
    m_objects.append(obj1);
    
    InteractiveObject obj2;
    obj2.rect = QRect(150, 100, 80, 60);
    obj2.color = Qt::blue;
    obj2.selected = false;
    m_objects.append(obj2);
}

void InteractiveDrawArea::setDrawMode(int mode)
{
    m_drawMode = mode;
    update();
}

void InteractiveDrawArea::setBrushSize(int size)
{
    m_brushSize = size;
}

void InteractiveDrawArea::setAnimationEnabled(bool enabled)
{
    m_animationEnabled = enabled;
    if (enabled) {
        m_animationTimer->start();
    } else {
        m_animationTimer->stop();
    }
}

void InteractiveDrawArea::clearCanvas()
{
    m_canvas.fill(Qt::white);
    update();
}

void InteractiveDrawArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_lastPoint = event->pos();
        m_drawing = true;
        
        // 检查是否点击了交互对象
        m_selectedObject = -1;
        for (int i = 0; i < m_objects.size(); ++i) {
            if (m_objects[i].rect.contains(event->pos())) {
                m_selectedObject = i;
                m_objects[i].selected = true;
                m_objects[i].dragOffset = event->pos() - m_objects[i].rect.topLeft();
                m_dragging = true;
                emit objectInteraction("Selected", i);
                break;
            } else {
                m_objects[i].selected = false;
            }
        }
        
        if (m_selectedObject == -1) {
            // 在画布上绘制
            switch (m_drawMode) {
                case 0: // 点绘制
                    drawPoint(event->pos(), getCurrentColor());
                    break;
                case 1: // 线绘制
                    // 线绘制在移动时处理
                    break;
                case 2: // 矩形
                case 3: // 圆形
                    // 形状绘制在释放时处理
                    break;
            }
            emit drawingAction("Start", event->pos());
        }
    }
    
    update();
    QWidget::mousePressEvent(event);
}

void InteractiveDrawArea::mouseMoveEvent(QMouseEvent *event)
{
    if (m_drawing && (event->buttons() & Qt::LeftButton)) {
        if (m_dragging && m_selectedObject >= 0) {
            // 拖拽对象
            QPoint newPos = event->pos() - m_objects[m_selectedObject].dragOffset;
            m_objects[m_selectedObject].rect.moveTo(newPos);
            emit objectInteraction("Dragged", m_selectedObject);
        } else if (m_drawMode == 1) {
            // 线绘制
            drawLine(m_lastPoint, event->pos(), getCurrentColor());
            m_lastPoint = event->pos();
            emit drawingAction("Draw", event->pos());
        }
    }
    
    update();
    QWidget::mouseMoveEvent(event);
}

void InteractiveDrawArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_drawing) {
        if (!m_dragging) {
            switch (m_drawMode) {
                case 2: // 矩形
                {
                    QPainter painter(&m_canvas);
                    painter.setPen(QPen(getCurrentColor(), m_brushSize));
                    painter.drawRect(QRect(m_lastPoint, event->pos()).normalized());
                    break;
                }
                case 3: // 圆形
                {
                    QPainter painter(&m_canvas);
                    painter.setPen(QPen(getCurrentColor(), m_brushSize));
                    QRect rect(m_lastPoint, event->pos());
                    painter.drawEllipse(rect.normalized());
                    break;
                }
            }
            emit drawingAction("End", event->pos());
        }
        
        m_drawing = false;
        m_dragging = false;
    }
    
    update();
    QWidget::mouseReleaseEvent(event);
}

void InteractiveDrawArea::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Delete:
            // 删除选中的对象
            if (m_selectedObject >= 0) {
                m_objects.removeAt(m_selectedObject);
                m_selectedObject = -1;
                emit objectInteraction("Deleted", -1);
                update();
            }
            break;
        case Qt::Key_C:
            // 改变颜色
            if (event->modifiers() & Qt::ControlModifier) {
                // Ctrl+C: 复制功能（这里简化为改变颜色）
                m_currentColor = (m_currentColor == Qt::black) ? Qt::red : Qt::black;
            }
            break;
        case Qt::Key_Plus:
        case Qt::Key_Equal:
            // 增加画笔大小
            m_brushSize = qMin(20, m_brushSize + 1);
            break;
        case Qt::Key_Minus:
            // 减少画笔大小
            m_brushSize = qMax(1, m_brushSize - 1);
            break;
        case Qt::Key_Space:
            // 空格键添加新对象
            {
                InteractiveObject newObj;
                newObj.rect = QRect(100 + m_objects.size() * 20, 50 + m_objects.size() * 15, 50, 30);
                newObj.color = getCurrentColor();
                newObj.selected = false;
                m_objects.append(newObj);
                emit objectInteraction("Created", m_objects.size() - 1);
                update();
            }
            break;
        case Qt::Key_Escape:
            // 取消选择
            for (auto &obj : m_objects) {
                obj.selected = false;
            }
            m_selectedObject = -1;
            update();
            break;
    }
    
    QWidget::keyPressEvent(event);
}

void InteractiveDrawArea::wheelEvent(QWheelEvent *event)
{
    // 滚轮控制画笔大小
    QPoint delta = event->angleDelta();
    if (delta.y() > 0) {
        m_brushSize = qMin(20, m_brushSize + 1);
    } else if (delta.y() < 0) {
        m_brushSize = qMax(1, m_brushSize - 1);
    }
    
    QWidget::wheelEvent(event);
}

void InteractiveDrawArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    
    // 绘制画布内容
    painter.drawPixmap(0, 0, m_canvas);
    
    // 绘制交互对象
    for (int i = 0; i < m_objects.size(); ++i) {
        const auto &obj = m_objects[i];
        
        // 设置颜色和样式
        QColor color = obj.color;
        if (obj.selected) {
            painter.setPen(QPen(Qt::yellow, 3, Qt::DashLine));
            painter.setBrush(QBrush(color, Qt::Dense4Pattern));
        } else {
            painter.setPen(QPen(color, 2));
            painter.setBrush(QBrush(color, Qt::SolidPattern));
        }
        
        // 绘制对象
        painter.drawRect(obj.rect);
        
        // 绘制对象编号
        painter.setPen(Qt::white);
        painter.drawText(obj.rect, Qt::AlignCenter, QString::number(i));
    }
    
    // 绘制当前绘制模式提示
    painter.setPen(Qt::black);
    QStringList modeNames = {"点绘制", "线绘制", "矩形绘制", "圆形绘制"};
    QString modeText = QString("模式: %1 | 画笔: %2px").arg(modeNames[m_drawMode]).arg(m_brushSize);
    painter.drawText(10, height() - 10, modeText);
    
    // 绘制动画效果
    if (m_animationEnabled) {
        painter.setPen(QPen(QColor(255, 0, 0, 100), 2));
        int radius = (m_animationStep % 50) + 10;
        painter.drawEllipse(width() - 50, 20, radius, radius);
    }
}

void InteractiveDrawArea::updateAnimation()
{
    m_animationStep++;
    if (m_animationStep > 1000) {
        m_animationStep = 0;
    }
    
    // 只更新动画区域
    update(QRect(width() - 70, 10, 80, 80));
}

void InteractiveDrawArea::drawPoint(const QPoint &point, const QColor &color)
{
    QPainter painter(&m_canvas);
    painter.setPen(QPen(color, m_brushSize, Qt::SolidLine, Qt::RoundCap));
    painter.drawPoint(point);
}

void InteractiveDrawArea::drawLine(const QPoint &start, const QPoint &end, const QColor &color)
{
    QPainter painter(&m_canvas);
    painter.setPen(QPen(color, m_brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(start, end);
}

QColor InteractiveDrawArea::getCurrentColor()
{
    // 根据动画步骤改变颜色
    if (m_animationEnabled) {
        int hue = (m_animationStep * 2) % 360;
        return QColor::fromHsv(hue, 200, 200);
    }
    return m_currentColor;
}