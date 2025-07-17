#include "basic_event_widget.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QFocusEvent>
#include <QEnterEvent>
#include <QPainter>
#include <QApplication>
#include <QDateTime>
#include <QScrollBar>

BasicEventWidget::BasicEventWidget(QWidget *parent)
    : QWidget(parent)
    , m_mainLayout(nullptr)
    , m_titleLabel(nullptr)
    , m_eventLog(nullptr)
    , m_clearButton(nullptr)
    , m_toggleAcceptButton(nullptr)
    , m_simulateEventButton(nullptr)
    , m_statusLabel(nullptr)
    , m_instructionLabel(nullptr)
    , m_acceptEvents(true)
    , m_eventCounter(0)
    , m_statusTimer(new QTimer(this))
{
    setupUI();
    
    // 设置定时器用于状态更新
    m_statusTimer->setSingleShot(true);
    connect(m_statusTimer, &QTimer::timeout, [this]() {
        m_statusLabel->setText("状态: 等待事件...");
    });
    
    // 设置焦点策略以接收键盘事件
    setFocusPolicy(Qt::StrongFocus);
    
    logEvent("BasicEventWidget 初始化完成 - 开始事件监听");
}

void BasicEventWidget::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    
    // 标题
    m_titleLabel = new QLabel("基础事件处理演示", this);
    m_titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #2c3e50; padding: 10px;");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    
    // 说明文字
    m_instructionLabel = new QLabel(
        "操作说明:\n"
        "• 点击鼠标观察鼠标事件\n"
        "• 按键盘按键观察键盘事件\n"
        "• 移动鼠标进出控件观察进入/离开事件\n"
        "• 点击控件获得/失去焦点观察焦点事件\n"
        "• 调整窗口大小观察调整事件\n"
        "• 使用按钮控制事件接受/忽略行为", this);
    m_instructionLabel->setStyleSheet("background-color: #ecf0f1; padding: 10px; border-radius: 5px;");
    m_instructionLabel->setWordWrap(true);
    
    // 事件日志
    m_eventLog = new QTextEdit(this);
    m_eventLog->setReadOnly(true);
    m_eventLog->setMaximumHeight(300);
    m_eventLog->setStyleSheet("font-family: 'Courier New', monospace; font-size: 12px;");
    
    // 按钮布局
    auto *buttonLayout = new QHBoxLayout();
    
    m_clearButton = new QPushButton("清空日志", this);
    m_clearButton->setStyleSheet("QPushButton { background-color: #3498db; color: white; padding: 8px; border-radius: 4px; }");
    connect(m_clearButton, &QPushButton::clicked, this, &BasicEventWidget::onClearLog);
    
    m_toggleAcceptButton = new QPushButton("切换事件接受模式", this);
    m_toggleAcceptButton->setStyleSheet("QPushButton { background-color: #27ae60; color: white; padding: 8px; border-radius: 4px; }");
    connect(m_toggleAcceptButton, &QPushButton::clicked, this, &BasicEventWidget::onToggleEventAcceptance);
    
    m_simulateEventButton = new QPushButton("模拟自定义事件", this);
    m_simulateEventButton->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; padding: 8px; border-radius: 4px; }");
    connect(m_simulateEventButton, &QPushButton::clicked, this, &BasicEventWidget::onSimulateCustomEvent);
    
    buttonLayout->addWidget(m_clearButton);
    buttonLayout->addWidget(m_toggleAcceptButton);
    buttonLayout->addWidget(m_simulateEventButton);
    buttonLayout->addStretch();
    
    // 状态标签
    m_statusLabel = new QLabel("状态: 等待事件...", this);
    m_statusLabel->setStyleSheet("color: #27ae60; font-weight: bold; padding: 5px;");
    
    // 添加到主布局
    m_mainLayout->addWidget(m_titleLabel);
    m_mainLayout->addWidget(m_instructionLabel);
    m_mainLayout->addWidget(new QLabel("事件日志:", this));
    m_mainLayout->addWidget(m_eventLog);
    m_mainLayout->addLayout(buttonLayout);
    m_mainLayout->addWidget(m_statusLabel);
    
    setLayout(m_mainLayout);
    setMinimumSize(600, 500);
}

bool BasicEventWidget::event(QEvent *event)
{
    // 记录事件信息
    QString eventInfo = QString("[%1] 事件类型: %2 (%3)")
                        .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"))
                        .arg(getEventTypeName(event->type()))
                        .arg(static_cast<int>(event->type()));
    
    // 根据事件接受模式决定是否处理事件
    bool shouldAccept = m_acceptEvents;
    
    // 某些关键事件总是需要处理
    switch (event->type()) {
        case QEvent::Paint:
        case QEvent::Resize:
        case QEvent::Show:
        case QEvent::Hide:
        case QEvent::Close:
            shouldAccept = true;
            break;
        default:
            break;
    }
    
    if (shouldAccept) {
        eventInfo += " [已接受]";
        event->accept();
    } else {
        eventInfo += " [已忽略]";
        event->ignore();
    }
    
    // 只记录我们关心的事件类型
    switch (event->type()) {
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseMove:
        case QEvent::KeyPress:
        case QEvent::KeyRelease:
        case QEvent::Enter:
        case QEvent::Leave:
        case QEvent::FocusIn:
        case QEvent::FocusOut:
        case QEvent::Resize:
        case QEvent::Paint:
        case QEvent::User:
            logEvent(eventInfo);
            m_statusLabel->setText(QString("状态: 处理 %1 事件").arg(getEventTypeName(event->type())));
            m_statusTimer->start(2000);
            break;
        default:
            break;
    }
    
    // 调用基类的事件处理
    return QWidget::event(event);
}

void BasicEventWidget::mousePressEvent(QMouseEvent *event)
{
    QString details = QString("鼠标按下 - 按钮: %1, 位置: (%2, %3), 修饰键: %4")
                      .arg(event->button() == Qt::LeftButton ? "左键" : 
                           event->button() == Qt::RightButton ? "右键" : "中键")
                      .arg(event->pos().x())
                      .arg(event->pos().y())
                      .arg(event->modifiers() == Qt::NoModifier ? "无" : "有");
    
    logEvent(details);
    
    if (m_acceptEvents) {
        QWidget::mousePressEvent(event);
    }
}

void BasicEventWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QString details = QString("鼠标释放 - 按钮: %1, 位置: (%2, %3)")
                      .arg(event->button() == Qt::LeftButton ? "左键" : 
                           event->button() == Qt::RightButton ? "右键" : "中键")
                      .arg(event->pos().x())
                      .arg(event->pos().y());
    
    logEvent(details);
    
    if (m_acceptEvents) {
        QWidget::mouseReleaseEvent(event);
    }
}

void BasicEventWidget::keyPressEvent(QKeyEvent *event)
{
    QString keyText = event->text().isEmpty() ? 
                      QString("特殊键(%1)").arg(event->key()) : 
                      event->text();
    
    QString details = QString("按键按下 - 键: %1, 键码: %2, 修饰键: %3")
                      .arg(keyText)
                      .arg(event->key())
                      .arg(event->modifiers() == Qt::NoModifier ? "无" : "有");
    
    logEvent(details);
    
    if (m_acceptEvents) {
        QWidget::keyPressEvent(event);
    }
}

void BasicEventWidget::paintEvent(QPaintEvent *event)
{
    logEvent(QString("绘制事件 - 区域: (%1, %2, %3x%4)")
             .arg(event->rect().x())
             .arg(event->rect().y())
             .arg(event->rect().width())
             .arg(event->rect().height()));
    
    QWidget::paintEvent(event);
}

void BasicEventWidget::resizeEvent(QResizeEvent *event)
{
    logEvent(QString("调整大小事件 - 从 %1x%2 到 %3x%4")
             .arg(event->oldSize().width())
             .arg(event->oldSize().height())
             .arg(event->size().width())
             .arg(event->size().height()));
    
    QWidget::resizeEvent(event);
}

void BasicEventWidget::enterEvent(QEnterEvent *event)
{
    logEvent(QString("鼠标进入事件 - 位置: (%1, %2)")
             .arg(event->position().x())
             .arg(event->position().y()));
    
    if (m_acceptEvents) {
        QWidget::enterEvent(event);
    }
}

void BasicEventWidget::leaveEvent(QEvent *event)
{
    logEvent("鼠标离开事件");
    
    if (m_acceptEvents) {
        QWidget::leaveEvent(event);
    }
}

void BasicEventWidget::focusInEvent(QFocusEvent *event)
{
    QString reason;
    switch (event->reason()) {
        case Qt::MouseFocusReason: reason = "鼠标点击"; break;
        case Qt::TabFocusReason: reason = "Tab键"; break;
        case Qt::BacktabFocusReason: reason = "Shift+Tab键"; break;
        case Qt::ActiveWindowFocusReason: reason = "窗口激活"; break;
        case Qt::PopupFocusReason: reason = "弹出窗口"; break;
        case Qt::ShortcutFocusReason: reason = "快捷键"; break;
        case Qt::MenuBarFocusReason: reason = "菜单栏"; break;
        case Qt::OtherFocusReason: reason = "其他"; break;
        default: reason = "未知"; break;
    }
    
    logEvent(QString("获得焦点事件 - 原因: %1").arg(reason));
    
    if (m_acceptEvents) {
        QWidget::focusInEvent(event);
    }
}

void BasicEventWidget::focusOutEvent(QFocusEvent *event)
{
    QString reason;
    switch (event->reason()) {
        case Qt::MouseFocusReason: reason = "鼠标点击"; break;
        case Qt::TabFocusReason: reason = "Tab键"; break;
        case Qt::BacktabFocusReason: reason = "Shift+Tab键"; break;
        case Qt::ActiveWindowFocusReason: reason = "窗口激活"; break;
        case Qt::PopupFocusReason: reason = "弹出窗口"; break;
        case Qt::ShortcutFocusReason: reason = "快捷键"; break;
        case Qt::MenuBarFocusReason: reason = "菜单栏"; break;
        case Qt::OtherFocusReason: reason = "其他"; break;
        default: reason = "未知"; break;
    }
    
    logEvent(QString("失去焦点事件 - 原因: %1").arg(reason));
    
    if (m_acceptEvents) {
        QWidget::focusOutEvent(event);
    }
}

void BasicEventWidget::onClearLog()
{
    m_eventLog->clear();
    m_eventCounter = 0;
    logEvent("日志已清空");
}

void BasicEventWidget::onToggleEventAcceptance()
{
    m_acceptEvents = !m_acceptEvents;
    QString mode = m_acceptEvents ? "接受" : "忽略";
    m_toggleAcceptButton->setText(QString("当前模式: %1事件").arg(mode));
    m_toggleAcceptButton->setStyleSheet(m_acceptEvents ? 
        "QPushButton { background-color: #27ae60; color: white; padding: 8px; border-radius: 4px; }" :
        "QPushButton { background-color: #e67e22; color: white; padding: 8px; border-radius: 4px; }");
    
    logEvent(QString("事件处理模式切换为: %1").arg(mode));
}

void BasicEventWidget::onSimulateCustomEvent()
{
    // 创建并发送自定义事件
    QEvent *customEvent = new QEvent(QEvent::User);
    QApplication::postEvent(this, customEvent);
    
    logEvent("模拟自定义事件已发送");
}

void BasicEventWidget::logEvent(const QString &eventInfo)
{
    m_eventCounter++;
    QString logEntry = QString("[%1] %2").arg(m_eventCounter, 3, 10, QChar('0')).arg(eventInfo);
    
    m_eventLog->append(logEntry);
    
    // 自动滚动到底部
    QScrollBar *scrollBar = m_eventLog->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

QString BasicEventWidget::getEventTypeName(QEvent::Type type) const
{
    switch (type) {
        case QEvent::MouseButtonPress: return "鼠标按下";
        case QEvent::MouseButtonRelease: return "鼠标释放";
        case QEvent::MouseMove: return "鼠标移动";
        case QEvent::KeyPress: return "按键按下";
        case QEvent::KeyRelease: return "按键释放";
        case QEvent::Enter: return "鼠标进入";
        case QEvent::Leave: return "鼠标离开";
        case QEvent::FocusIn: return "获得焦点";
        case QEvent::FocusOut: return "失去焦点";
        case QEvent::Paint: return "绘制";
        case QEvent::Resize: return "调整大小";
        case QEvent::Show: return "显示";
        case QEvent::Hide: return "隐藏";
        case QEvent::Close: return "关闭";
        case QEvent::User: return "自定义事件";
        default: return QString("未知事件(%1)").arg(static_cast<int>(type));
    }
}