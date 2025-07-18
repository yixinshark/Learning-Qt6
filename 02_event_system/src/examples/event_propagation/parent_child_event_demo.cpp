#include "parent_child_event_demo.h"
#include <QPainter>
#include <QApplication>
#include <QDebug>

ParentChildEventDemo::ParentChildEventDemo(QWidget *parent)
    : QWidget(parent)
    , m_acceptEvents(true)
{
    setWindowTitle("Qt事件传播演示 - 父子对象");
    resize(800, 600);
    setupUI();
}

void ParentChildEventDemo::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_contentLayout = new QHBoxLayout();
    
    // 左侧控制面板
    m_controlGroup = new QGroupBox("控制面板", this);
    m_controlLayout = new QVBoxLayout(m_controlGroup);
    
    m_instructionLabel = new QLabel(
        "说明：\n"
        "• 点击不同区域观察事件传播\n"
        "• 按键盘按键测试键盘事件传播\n"
        "• 取消勾选可阻止事件传播\n"
        "• 观察右侧日志了解传播路径", this);
    m_instructionLabel->setWordWrap(true);
    
    m_acceptEventsCheckBox = new QCheckBox("接受事件传播", this);
    m_acceptEventsCheckBox->setChecked(true);
    connect(m_acceptEventsCheckBox, &QCheckBox::toggled, 
            this, &ParentChildEventDemo::onToggleEventAcceptance);
    
    m_clearLogButton = new QPushButton("清空日志", this);
    connect(m_clearLogButton, &QPushButton::clicked, 
            this, &ParentChildEventDemo::onClearLog);
    
    m_controlLayout->addWidget(m_instructionLabel);
    m_controlLayout->addWidget(m_acceptEventsCheckBox);
    m_controlLayout->addWidget(m_clearLogButton);
    m_controlLayout->addStretch();
    
    // 中间交互区域 - 创建嵌套的控件层次
    m_interactionGroup = new QGroupBox("交互区域 (点击测试)", this);
    m_interactionLayout = new QVBoxLayout(m_interactionGroup);
    
    // 创建嵌套的子控件
    m_childWidget = new PropagationChildWidget("子控件", m_interactionGroup);
    m_childWidget->setMinimumSize(300, 200);
    m_childWidget->setStyleSheet("background-color: lightblue; border: 2px solid blue;");
    
    m_grandChildWidget = new PropagationChildWidget("孙子控件", m_childWidget);
    m_grandChildWidget->setGeometry(50, 50, 200, 100);
    m_grandChildWidget->setStyleSheet("background-color: lightgreen; border: 2px solid green;");
    
    m_testButton = new QPushButton("测试按钮", m_grandChildWidget);
    m_testButton->setGeometry(50, 30, 100, 40);
    
    // 连接子控件的事件信号
    connect(static_cast<PropagationChildWidget*>(m_childWidget), 
            &PropagationChildWidget::eventReceived,
            [this](const QString &name, const QString &type, const QString &details, bool accepted) {
                logEvent(name, type, details, accepted);
            });
    
    connect(static_cast<PropagationChildWidget*>(m_grandChildWidget), 
            &PropagationChildWidget::eventReceived,
            [this](const QString &name, const QString &type, const QString &details, bool accepted) {
                logEvent(name, type, details, accepted);
            });
    
    m_interactionLayout->addWidget(m_childWidget);
    
    // 右侧日志显示
    m_logGroup = new QGroupBox("事件传播日志", this);
    m_logLayout = new QVBoxLayout(m_logGroup);
    
    m_logDisplay = new QTextEdit(this);
    m_logDisplay->setReadOnly(true);
    m_logDisplay->setFont(QFont("Consolas", 9));
    m_logLayout->addWidget(m_logDisplay);
    
    // 布局组装
    m_contentLayout->addWidget(m_controlGroup, 1);
    m_contentLayout->addWidget(m_interactionGroup, 2);
    m_contentLayout->addWidget(m_logGroup, 2);
    
    m_mainLayout->addLayout(m_contentLayout);
    
    // 初始日志
    logEvent("系统", "初始化", "事件传播演示已启动，准备接收事件");
}

bool ParentChildEventDemo::event(QEvent *event)
{
    QString eventType;
    QString details;
    bool shouldLog = false;
    
    switch (event->type()) {
    case QEvent::MouseButtonPress:
        eventType = "鼠标按下";
        details = QString("按钮: %1").arg(static_cast<QMouseEvent*>(event)->button());
        shouldLog = true;
        break;
    case QEvent::KeyPress:
        eventType = "按键按下";
        details = QString("按键: %1").arg(static_cast<QKeyEvent*>(event)->text());
        shouldLog = true;
        break;
    default:
        break;
    }
    
    if (shouldLog) {
        logEvent("父控件(ParentChildEventDemo)", eventType, 
                QString("事件详情: %1, 接受状态: %2").arg(details).arg(m_acceptEvents ? "接受" : "忽略"));
        
        if (!m_acceptEvents) {
            event->ignore();
            return false;
        } else {
            event->accept();
        }
    }
    
    return QWidget::event(event);
}

void ParentChildEventDemo::mousePressEvent(QMouseEvent *event)
{
    logEvent("父控件(ParentChildEventDemo)", "鼠标事件处理", 
            QString("位置: (%1, %2), 按钮: %3")
            .arg(event->pos().x())
            .arg(event->pos().y())
            .arg(event->button()));
    
    if (m_acceptEvents) {
        event->accept();
    } else {
        event->ignore();
    }
}

void ParentChildEventDemo::keyPressEvent(QKeyEvent *event)
{
    logEvent("父控件(ParentChildEventDemo)", "键盘事件处理", 
            QString("按键: %1, 修饰键: %2")
            .arg(event->text())
            .arg(event->modifiers()));
    
    if (m_acceptEvents) {
        event->accept();
    } else {
        event->ignore();
    }
}

void ParentChildEventDemo::onClearLog()
{
    m_logDisplay->clear();
    logEvent("系统", "日志清空", "事件日志已清空");
}

void ParentChildEventDemo::onToggleEventAcceptance()
{
    m_acceptEvents = m_acceptEventsCheckBox->isChecked();
    
    // 同时设置子控件的事件接受状态
    if (m_childWidget) {
        static_cast<PropagationChildWidget*>(m_childWidget)->setAcceptEvents(m_acceptEvents);
    }
    if (m_grandChildWidget) {
        static_cast<PropagationChildWidget*>(m_grandChildWidget)->setAcceptEvents(m_acceptEvents);
    }
    
    logEvent("系统", "设置更改", 
            QString("事件接受状态已设置为: %1").arg(m_acceptEvents ? "接受" : "忽略"));
}

void ParentChildEventDemo::logEvent(const QString &source, const QString &eventType, 
                                   const QString &details, bool accepted)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    QString acceptedStr = accepted ? "✓" : "✗";
    QString logEntry = QString("[%1] %2 | %3 | %4 | %5")
                      .arg(timestamp)
                      .arg(acceptedStr)
                      .arg(source)
                      .arg(eventType)
                      .arg(details);
    
    m_logDisplay->append(logEntry);
    
    // 自动滚动到底部
    QTextCursor cursor = m_logDisplay->textCursor();
    cursor.movePosition(QTextCursor::End);
    m_logDisplay->setTextCursor(cursor);
}

// PropagationChildWidget 实现
PropagationChildWidget::PropagationChildWidget(const QString &name, QWidget *parent)
    : QWidget(parent)
    , m_name(name)
    , m_acceptEvents(true)
    , m_backgroundColor(Qt::lightGray)
{
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_Hover, true);
}

bool PropagationChildWidget::event(QEvent *event)
{
    QString eventType;
    QString details;
    bool shouldLog = false;
    
    switch (event->type()) {
    case QEvent::MouseButtonPress:
        eventType = "鼠标按下事件";
        details = QString("按钮: %1, 位置: (%2, %3)")
                 .arg(static_cast<QMouseEvent*>(event)->button())
                 .arg(static_cast<QMouseEvent*>(event)->pos().x())
                 .arg(static_cast<QMouseEvent*>(event)->pos().y());
        shouldLog = true;
        break;
    case QEvent::KeyPress:
        eventType = "按键按下事件";
        details = QString("按键: '%1', 键码: %2")
                 .arg(static_cast<QKeyEvent*>(event)->text())
                 .arg(static_cast<QKeyEvent*>(event)->key());
        shouldLog = true;
        break;
    case QEvent::HoverEnter:
        m_backgroundColor = Qt::yellow;
        update();
        break;
    case QEvent::HoverLeave:
        m_backgroundColor = Qt::lightGray;
        update();
        break;
    default:
        break;
    }
    
    if (shouldLog) {
        emit eventReceived(m_name, eventType, details, m_acceptEvents);
        
        if (!m_acceptEvents) {
            event->ignore();
            return false;
        } else {
            event->accept();
        }
    }
    
    return QWidget::event(event);
}

void PropagationChildWidget::mousePressEvent(QMouseEvent *event)
{
    QString details = QString("鼠标事件处理完成 - 位置: (%1, %2), 全局位置: (%3, %4)")
                     .arg(event->pos().x())
                     .arg(event->pos().y())
                     .arg(event->globalPosition().toPoint().x())
                     .arg(event->globalPosition().toPoint().y());
    
    emit eventReceived(m_name, "鼠标事件处理", details, m_acceptEvents);
    
    if (m_acceptEvents) {
        event->accept();
    } else {
        event->ignore();
        // 如果不接受事件，它会传播到父控件
    }
}

void PropagationChildWidget::keyPressEvent(QKeyEvent *event)
{
    QString details = QString("键盘事件处理完成 - 按键: '%1', 修饰键: %2")
                     .arg(event->text())
                     .arg(event->modifiers());
    
    emit eventReceived(m_name, "键盘事件处理", details, m_acceptEvents);
    
    if (m_acceptEvents) {
        event->accept();
    } else {
        event->ignore();
        // 如果不接受事件，它会传播到父控件
    }
}

void PropagationChildWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(rect(), m_backgroundColor);
    
    // 绘制控件名称
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.drawText(rect(), Qt::AlignCenter, m_name);
    
    // 绘制边框
    painter.setPen(QPen(Qt::darkGray, 2));
    painter.drawRect(rect().adjusted(1, 1, -1, -1));
    
    QWidget::paintEvent(event);
}

// #include "parent_child_event_demo.moc" // 移除不需要的moc包含