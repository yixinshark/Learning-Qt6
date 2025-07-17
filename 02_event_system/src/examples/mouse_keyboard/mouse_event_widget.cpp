#include "mouse_event_widget.h"
#include <QApplication>
#include <QDebug>

MouseEventWidget::MouseEventWidget(QWidget *parent)
    : QWidget(parent)
    , m_mainLayout(nullptr)
    , m_infoGroup(nullptr)
    , m_visualGroup(nullptr)
    , m_eventTypeLabel(nullptr)
    , m_positionLabel(nullptr)
    , m_globalPositionLabel(nullptr)
    , m_buttonLabel(nullptr)
    , m_buttonsLabel(nullptr)
    , m_modifiersLabel(nullptr)
    , m_wheelDeltaLabel(nullptr)
    , m_trailTimer(new QTimer(this))
    , m_mouseInside(false)
    , m_clickCount(0)
    , m_moveCount(0)
    , m_wheelCount(0)
{
    setupUI();
    
    // 启用鼠标跟踪，即使没有按下鼠标按键也能接收移动事件
    setMouseTracking(true);
    
    // 设置定时器清除鼠标轨迹
    m_trailTimer->setSingleShot(true);
    connect(m_trailTimer, &QTimer::timeout, this, &MouseEventWidget::clearTrail);
    
    // 设置最小尺寸
    setMinimumSize(400, 300);
}

void MouseEventWidget::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    
    // 创建信息显示组
    m_infoGroup = new QGroupBox("鼠标事件信息", this);
    QVBoxLayout *infoLayout = new QVBoxLayout(m_infoGroup);
    
    m_eventTypeLabel = new QLabel("事件类型: 无", this);
    m_positionLabel = new QLabel("本地位置: (0, 0)", this);
    m_globalPositionLabel = new QLabel("全局位置: (0, 0)", this);
    m_buttonLabel = new QLabel("触发按键: 无", this);
    m_buttonsLabel = new QLabel("按下的按键: 无", this);
    m_modifiersLabel = new QLabel("修饰键: 无", this);
    m_wheelDeltaLabel = new QLabel("滚轮增量: 0", this);
    
    infoLayout->addWidget(m_eventTypeLabel);
    infoLayout->addWidget(m_positionLabel);
    infoLayout->addWidget(m_globalPositionLabel);
    infoLayout->addWidget(m_buttonLabel);
    infoLayout->addWidget(m_buttonsLabel);
    infoLayout->addWidget(m_modifiersLabel);
    infoLayout->addWidget(m_wheelDeltaLabel);
    
    // 创建可视化显示组
    m_visualGroup = new QGroupBox("鼠标轨迹可视化 (统计: 点击0次, 移动0次, 滚轮0次)", this);
    m_visualGroup->setMinimumHeight(150);
    
    m_mainLayout->addWidget(m_infoGroup);
    m_mainLayout->addWidget(m_visualGroup);
}

void MouseEventWidget::mousePressEvent(QMouseEvent *event)
{
    m_clickCount++;
    m_lastClickPos = event->pos();
    updateEventInfo("鼠标按下", event);
    
    // 添加到轨迹
    m_mouseTrail.append(event->pos());
    if (m_mouseTrail.size() > 50) {  // 限制轨迹点数量
        m_mouseTrail.removeFirst();
    }
    
    update();  // 触发重绘
    
    emit mouseEventOccurred("MousePress", event->pos());
    
    // 调用基类处理
    QWidget::mousePressEvent(event);
}

void MouseEventWidget::mouseReleaseEvent(QMouseEvent *event)
{
    updateEventInfo("鼠标释放", event);
    update();
    
    emit mouseEventOccurred("MouseRelease", event->pos());
    
    QWidget::mouseReleaseEvent(event);
}

void MouseEventWidget::mouseMoveEvent(QMouseEvent *event)
{
    m_moveCount++;
    updateEventInfo("鼠标移动", event);
    
    // 添加到轨迹（仅当鼠标按下时）
    if (event->buttons() != Qt::NoButton) {
        m_mouseTrail.append(event->pos());
        if (m_mouseTrail.size() > 100) {
            m_mouseTrail.removeFirst();
        }
        update();
    }
    
    // 重启清除定时器
    m_trailTimer->start(3000);  // 3秒后清除轨迹
    
    emit mouseEventOccurred("MouseMove", event->pos());
    
    QWidget::mouseMoveEvent(event);
}

void MouseEventWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    updateEventInfo("鼠标双击", event);
    
    // 双击时添加特殊标记
    m_lastClickPos = event->pos();
    update();
    
    emit mouseEventOccurred("MouseDoubleClick", event->pos());
    
    QWidget::mouseDoubleClickEvent(event);
}

void MouseEventWidget::wheelEvent(QWheelEvent *event)
{
    m_wheelCount++;
    updateWheelInfo(event);
    
    emit mouseEventOccurred("WheelEvent", event->position().toPoint());
    
    QWidget::wheelEvent(event);
}

void MouseEventWidget::enterEvent(QEnterEvent *event)
{
    m_mouseInside = true;
    m_eventTypeLabel->setText("事件类型: 鼠标进入");
    updatePositionInfo(event->position().toPoint());
    
    // 改变背景色表示鼠标进入
    setStyleSheet("MouseEventWidget { background-color: #f0f8ff; }");
    
    emit mouseEventOccurred("MouseEnter", event->position().toPoint());
    
    QWidget::enterEvent(event);
}

void MouseEventWidget::leaveEvent(QEvent *event)
{
    m_mouseInside = false;
    m_eventTypeLabel->setText("事件类型: 鼠标离开");
    
    // 恢复背景色
    setStyleSheet("");
    
    emit mouseEventOccurred("MouseLeave", QPoint(-1, -1));
    
    QWidget::leaveEvent(event);
}

void MouseEventWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 绘制鼠标轨迹
    if (!m_mouseTrail.isEmpty()) {
        painter.setPen(QPen(Qt::blue, 2));
        for (int i = 1; i < m_mouseTrail.size(); ++i) {
            painter.drawLine(m_mouseTrail[i-1], m_mouseTrail[i]);
        }
        
        // 绘制轨迹点
        painter.setPen(QPen(Qt::red, 1));
        painter.setBrush(Qt::red);
        for (const QPoint &point : m_mouseTrail) {
            painter.drawEllipse(point, 2, 2);
        }
    }
    
    // 绘制最后点击位置
    if (!m_lastClickPos.isNull()) {
        painter.setPen(QPen(Qt::green, 3));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(m_lastClickPos, 10, 10);
        painter.drawLine(m_lastClickPos.x() - 5, m_lastClickPos.y(),
                        m_lastClickPos.x() + 5, m_lastClickPos.y());
        painter.drawLine(m_lastClickPos.x(), m_lastClickPos.y() - 5,
                        m_lastClickPos.x(), m_lastClickPos.y() + 5);
    }
    
    // 更新统计信息
    m_visualGroup->setTitle(QString("鼠标轨迹可视化 (统计: 点击%1次, 移动%2次, 滚轮%3次)")
                           .arg(m_clickCount).arg(m_moveCount).arg(m_wheelCount));
}

void MouseEventWidget::clearTrail()
{
    m_mouseTrail.clear();
    update();
}

void MouseEventWidget::updateEventInfo(const QString &eventType, QMouseEvent *event)
{
    m_eventTypeLabel->setText(QString("事件类型: %1").arg(eventType));
    updatePositionInfo(event->pos());
    m_globalPositionLabel->setText(QString("全局位置: (%1, %2)")
                                  .arg(event->globalPosition().toPoint().x())
                                  .arg(event->globalPosition().toPoint().y()));
    
    // 显示触发事件的按键
    QString buttonText = "无";
    switch (event->button()) {
        case Qt::LeftButton: buttonText = "左键"; break;
        case Qt::RightButton: buttonText = "右键"; break;
        case Qt::MiddleButton: buttonText = "中键"; break;
        case Qt::BackButton: buttonText = "后退键"; break;
        case Qt::ForwardButton: buttonText = "前进键"; break;
        default: buttonText = "其他"; break;
    }
    m_buttonLabel->setText(QString("触发按键: %1").arg(buttonText));
    
    // 显示当前按下的所有按键
    m_buttonsLabel->setText(QString("按下的按键: %1").arg(getButtonsText(event->buttons())));
    
    // 显示修饰键
    m_modifiersLabel->setText(QString("修饰键: %1").arg(getModifiersText(event->modifiers())));
}

void MouseEventWidget::updateWheelInfo(QWheelEvent *event)
{
    m_eventTypeLabel->setText("事件类型: 滚轮事件");
    updatePositionInfo(event->position().toPoint());
    m_globalPositionLabel->setText(QString("全局位置: (%1, %2)")
                                  .arg(event->globalPosition().toPoint().x())
                                  .arg(event->globalPosition().toPoint().y()));
    
    QPoint delta = event->angleDelta();
    m_wheelDeltaLabel->setText(QString("滚轮增量: x=%1, y=%2").arg(delta.x()).arg(delta.y()));
    
    m_modifiersLabel->setText(QString("修饰键: %1").arg(getModifiersText(event->modifiers())));
}

void MouseEventWidget::updatePositionInfo(const QPoint &position)
{
    m_positionLabel->setText(QString("本地位置: (%1, %2)").arg(position.x()).arg(position.y()));
}

QString MouseEventWidget::getButtonsText(Qt::MouseButtons buttons)
{
    QStringList buttonList;
    if (buttons & Qt::LeftButton) buttonList << "左键";
    if (buttons & Qt::RightButton) buttonList << "右键";
    if (buttons & Qt::MiddleButton) buttonList << "中键";
    if (buttons & Qt::BackButton) buttonList << "后退键";
    if (buttons & Qt::ForwardButton) buttonList << "前进键";
    
    return buttonList.isEmpty() ? "无" : buttonList.join(", ");
}

QString MouseEventWidget::getModifiersText(Qt::KeyboardModifiers modifiers)
{
    QStringList modifierList;
    if (modifiers & Qt::ShiftModifier) modifierList << "Shift";
    if (modifiers & Qt::ControlModifier) modifierList << "Ctrl";
    if (modifiers & Qt::AltModifier) modifierList << "Alt";
    if (modifiers & Qt::MetaModifier) modifierList << "Meta";
    
    return modifierList.isEmpty() ? "无" : modifierList.join(" + ");
}