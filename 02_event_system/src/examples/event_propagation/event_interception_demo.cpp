#include "event_interception_demo.h"
#include <QPainter>
#include <QApplication>
#include <QDebug>
#include <QRandomGenerator>
#include <QFocusEvent>
#include <QPaintEvent>

EventInterceptionDemo::EventInterceptionDemo(QWidget *parent)
    : QWidget(parent)
    , m_currentMode(NoInterception)
    , m_globalFilterEnabled(false)
    , m_modifyEvents(false)
    , m_logAllEvents(false)
    , m_interceptionDelay(0)
    , m_interceptionProbability(100)
    , m_totalEvents(0)
    , m_interceptedEvents(0)
    , m_modifiedEvents(0)
    , m_blockedEvents(0)
    , m_statsUpdateTimer(new QTimer(this))
{
    setWindowTitle("Qt事件拦截与控制演示");
    resize(1200, 800);
    setupUI();
    
    // 设置统计更新定时器
    connect(m_statsUpdateTimer, &QTimer::timeout, [this]() {
        m_probabilityBar->setValue(m_interceptionProbability);
    });
    m_statsUpdateTimer->start(1000);
}

void EventInterceptionDemo::setupUI()
{
    m_mainLayout = new QHBoxLayout(this);
    m_splitter = new QSplitter(Qt::Horizontal, this);
    
    setupInterceptionControls();
    setupTestArea();
    setupLogArea();
    
    // 组装布局
    m_splitter->addWidget(m_controlGroup);
    m_splitter->addWidget(m_testGroup);
    m_splitter->addWidget(m_logGroup);
    m_splitter->setSizes({300, 500, 400});
    
    m_mainLayout->addWidget(m_splitter);
    
    // 初始化日志
    logInterceptionEvent("系统", "初始化", "系统启动", "事件拦截演示已准备就绪");
}

void EventInterceptionDemo::setupInterceptionControls()
{
    m_controlGroup = new QGroupBox("拦截控制面板", this);
    m_controlLayout = new QVBoxLayout(m_controlGroup);
    
    // 拦截模式控制
    m_modeGroup = new QGroupBox("拦截模式", this);
    m_modeLayout = new QVBoxLayout(m_modeGroup);
    
    m_interceptionModeCombo = new QComboBox(this);
    m_interceptionModeCombo->addItems({
        "无拦截",
        "阻止所有事件",
        "阻止选定事件",
        "修改事件",
        "条件性阻止"
    });
    connect(m_interceptionModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &EventInterceptionDemo::onInterceptionModeChanged);
    
    m_globalFilterCheckBox = new QCheckBox("启用全局过滤器", this);
    connect(m_globalFilterCheckBox, &QCheckBox::toggled, 
            this, &EventInterceptionDemo::onToggleGlobalFilter);
    
    m_modifyEventsCheckBox = new QCheckBox("修改事件内容", this);
    connect(m_modifyEventsCheckBox, &QCheckBox::toggled, [this](bool checked) {
        m_modifyEvents = checked;
        logInterceptionEvent("设置", "修改", "事件修改", 
                           QString("事件修改功能已%1").arg(checked ? "启用" : "禁用"));
    });
    
    m_logAllEventsCheckBox = new QCheckBox("记录所有事件", this);
    connect(m_logAllEventsCheckBox, &QCheckBox::toggled, [this](bool checked) {
        m_logAllEvents = checked;
        logInterceptionEvent("设置", "日志", "日志模式", 
                           QString("全事件日志已%1").arg(checked ? "启用" : "禁用"));
    });
    
    m_modeLayout->addWidget(new QLabel("拦截模式:", this));
    m_modeLayout->addWidget(m_interceptionModeCombo);
    m_modeLayout->addWidget(m_globalFilterCheckBox);
    m_modeLayout->addWidget(m_modifyEventsCheckBox);
    m_modeLayout->addWidget(m_logAllEventsCheckBox);
    
    // 事件类型过滤
    m_filterGroup = new QGroupBox("事件类型过滤", this);
    m_filterLayout = new QVBoxLayout(m_filterGroup);
    
    m_mouseEventsCheckBox = new QCheckBox("鼠标事件", this);
    m_mouseEventsCheckBox->setChecked(true);
    m_keyboardEventsCheckBox = new QCheckBox("键盘事件", this);
    m_keyboardEventsCheckBox->setChecked(true);
    m_focusEventsCheckBox = new QCheckBox("焦点事件", this);
    m_paintEventsCheckBox = new QCheckBox("绘制事件", this);
    
    connect(m_mouseEventsCheckBox, &QCheckBox::toggled, 
            this, &EventInterceptionDemo::onEventTypeFilterChanged);
    connect(m_keyboardEventsCheckBox, &QCheckBox::toggled, 
            this, &EventInterceptionDemo::onEventTypeFilterChanged);
    connect(m_focusEventsCheckBox, &QCheckBox::toggled, 
            this, &EventInterceptionDemo::onEventTypeFilterChanged);
    connect(m_paintEventsCheckBox, &QCheckBox::toggled, 
            this, &EventInterceptionDemo::onEventTypeFilterChanged);
    
    m_filterLayout->addWidget(m_mouseEventsCheckBox);
    m_filterLayout->addWidget(m_keyboardEventsCheckBox);
    m_filterLayout->addWidget(m_focusEventsCheckBox);
    m_filterLayout->addWidget(m_paintEventsCheckBox);
    
    // 拦截参数
    m_paramGroup = new QGroupBox("拦截参数", this);
    m_paramLayout = new QVBoxLayout(m_paramGroup);
    
    m_delayLabel = new QLabel("拦截延迟 (ms):", this);
    m_delaySpinBox = new QSpinBox(this);
    m_delaySpinBox->setRange(0, 5000);
    m_delaySpinBox->setValue(0);
    connect(m_delaySpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
        m_interceptionDelay = value;
        logInterceptionEvent("设置", "参数", "延迟设置", 
                           QString("拦截延迟设置为 %1 毫秒").arg(value));
    });
    
    m_probabilityLabel = new QLabel("拦截概率 (%):", this);
    m_probabilitySlider = new QSlider(Qt::Horizontal, this);
    m_probabilitySlider->setRange(0, 100);
    m_probabilitySlider->setValue(100);
    m_probabilityBar = new QProgressBar(this);
    m_probabilityBar->setRange(0, 100);
    m_probabilityBar->setValue(100);
    
    connect(m_probabilitySlider, &QSlider::valueChanged, [this](int value) {
        m_interceptionProbability = value;
        m_probabilityBar->setValue(value);
        logInterceptionEvent("设置", "参数", "概率设置", 
                           QString("拦截概率设置为 %1%").arg(value));
    });
    
    m_paramLayout->addWidget(m_delayLabel);
    m_paramLayout->addWidget(m_delaySpinBox);
    m_paramLayout->addWidget(m_probabilityLabel);
    m_paramLayout->addWidget(m_probabilitySlider);
    m_paramLayout->addWidget(m_probabilityBar);
    
    // 控制按钮
    m_resetButton = new QPushButton("重置演示", this);
    connect(m_resetButton, &QPushButton::clicked, this, &EventInterceptionDemo::onResetDemo);
    
    m_clearLogButton = new QPushButton("清空日志", this);
    connect(m_clearLogButton, &QPushButton::clicked, this, &EventInterceptionDemo::onClearLog);
    
    // 组装控制面板
    m_controlLayout->addWidget(m_modeGroup);
    m_controlLayout->addWidget(m_filterGroup);
    m_controlLayout->addWidget(m_paramGroup);
    m_controlLayout->addWidget(m_resetButton);
    m_controlLayout->addWidget(m_clearLogButton);
    m_controlLayout->addStretch();
}

void EventInterceptionDemo::setupTestArea()
{
    m_testGroup = new QGroupBox("测试区域", this);
    m_testLayout = new QVBoxLayout(m_testGroup);
    
    m_testTabs = new QTabWidget(this);
    
    // 鼠标测试标签页
    m_mouseTestWidget = new QWidget();
    QVBoxLayout *mouseLayout = new QVBoxLayout(m_mouseTestWidget);
    
    InterceptableWidget *mouseWidget1 = new InterceptableWidget("鼠标测试区域1", m_mouseTestWidget);
    mouseWidget1->setMinimumHeight(100);
    mouseWidget1->setStyleSheet("background-color: #e6f3ff; border: 2px solid #0066cc;");
    
    InterceptableWidget *mouseWidget2 = new InterceptableWidget("鼠标测试区域2", m_mouseTestWidget);
    mouseWidget2->setMinimumHeight(100);
    mouseWidget2->setStyleSheet("background-color: #ffe6f3; border: 2px solid #cc0066;");
    
    connect(mouseWidget1, &InterceptableWidget::eventProcessed,
            [this](const QString &name, const QString &type, const QString &details, bool intercepted) {
                logInterceptionEvent(name, intercepted ? "拦截" : "处理", type, details);
            });
    
    connect(mouseWidget2, &InterceptableWidget::eventProcessed,
            [this](const QString &name, const QString &type, const QString &details, bool intercepted) {
                logInterceptionEvent(name, intercepted ? "拦截" : "处理", type, details);
            });
    
    mouseLayout->addWidget(new QLabel("点击下面的区域测试鼠标事件拦截:", m_mouseTestWidget));
    mouseLayout->addWidget(mouseWidget1);
    mouseLayout->addWidget(mouseWidget2);
    mouseLayout->addStretch();
    
    // 键盘测试标签页
    m_keyboardTestWidget = new QWidget();
    QVBoxLayout *keyboardLayout = new QVBoxLayout(m_keyboardTestWidget);
    
    InterceptableWidget *keyWidget = new InterceptableWidget("键盘测试区域", m_keyboardTestWidget);
    keyWidget->setMinimumHeight(150);
    keyWidget->setStyleSheet("background-color: #e6ffe6; border: 2px solid #00cc00;");
    keyWidget->setFocusPolicy(Qt::StrongFocus);
    
    connect(keyWidget, &InterceptableWidget::eventProcessed,
            [this](const QString &name, const QString &type, const QString &details, bool intercepted) {
                logInterceptionEvent(name, intercepted ? "拦截" : "处理", type, details);
            });
    
    keyboardLayout->addWidget(new QLabel("点击下面的区域获得焦点，然后按键测试:", m_keyboardTestWidget));
    keyboardLayout->addWidget(keyWidget);
    keyboardLayout->addStretch();
    
    // 复合测试标签页
    m_complexTestWidget = new QWidget();
    QGridLayout *complexLayout = new QGridLayout(m_complexTestWidget);
    
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            QString name = QString("复合测试区域%1-%2").arg(i+1).arg(j+1);
            InterceptableWidget *widget = new InterceptableWidget(name, m_complexTestWidget);
            widget->setMinimumSize(120, 80);
            
            QColor bgColor;
            switch ((i*3 + j) % 6) {
            case 0: bgColor = QColor(255, 230, 230); break;
            case 1: bgColor = QColor(230, 255, 230); break;
            case 2: bgColor = QColor(230, 230, 255); break;
            case 3: bgColor = QColor(255, 255, 230); break;
            case 4: bgColor = QColor(255, 230, 255); break;
            case 5: bgColor = QColor(230, 255, 255); break;
            }
            
            widget->setStyleSheet(QString("background-color: %1; border: 1px solid gray;")
                                 .arg(bgColor.name()));
            
            connect(widget, &InterceptableWidget::eventProcessed,
                    [this](const QString &name, const QString &type, const QString &details, bool intercepted) {
                        logInterceptionEvent(name, intercepted ? "拦截" : "处理", type, details);
                    });
            
            complexLayout->addWidget(widget, i, j);
        }
    }
    
    // 添加标签页
    m_testTabs->addTab(m_mouseTestWidget, "鼠标事件测试");
    m_testTabs->addTab(m_keyboardTestWidget, "键盘事件测试");
    m_testTabs->addTab(m_complexTestWidget, "复合事件测试");
    
    m_testLayout->addWidget(m_testTabs);
}

void EventInterceptionDemo::setupLogArea()
{
    m_logGroup = new QGroupBox("事件拦截日志", this);
    m_logLayout = new QVBoxLayout(m_logGroup);
    
    m_logDisplay = new QTextEdit(this);
    m_logDisplay->setReadOnly(true);
    m_logDisplay->setFont(QFont("Consolas", 9));
    
    // 添加统计信息显示
    QLabel *statsLabel = new QLabel(this);
    statsLabel->setText("统计信息将在这里显示");
    
    m_logLayout->addWidget(m_logDisplay);
    m_logLayout->addWidget(statsLabel);
}

bool EventInterceptionDemo::eventFilter(QObject *watched, QEvent *event)
{
    if (!m_globalFilterEnabled) {
        return QWidget::eventFilter(watched, event);
    }
    
    m_totalEvents++;
    
    if (shouldInterceptEvent(event, watched)) {
        m_interceptedEvents++;
        
        QString objectName = watched->objectName().isEmpty() ? 
                           watched->metaObject()->className() : watched->objectName();
        QString eventType = QString("事件类型: %1").arg(event->type());
        
        if (m_modifyEvents) {
            QEvent *modifiedEvent = modifyEvent(event);
            if (modifiedEvent != event) {
                m_modifiedEvents++;
                logInterceptionEvent("全局过滤器", "修改", eventType, 
                                   QString("对象: %1, 事件已修改").arg(objectName));
            }
        }
        
        if (m_currentMode == BlockAll || m_currentMode == BlockSelected) {
            m_blockedEvents++;
            logInterceptionEvent("全局过滤器", "阻止", eventType, 
                               QString("对象: %1, 事件已阻止").arg(objectName));
            return true; // 阻止事件
        }
        
        if (m_logAllEvents) {
            logInterceptionEvent("全局过滤器", "通过", eventType, 
                               QString("对象: %1, 事件已通过").arg(objectName));
        }
    }
    
    return QWidget::eventFilter(watched, event);
}

bool EventInterceptionDemo::shouldInterceptEvent(QEvent *event, QObject *source)
{
    // 检查事件类型过滤
    bool typeMatches = false;
    
    switch (event->type()) {
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseMove:
        typeMatches = m_mouseEventsCheckBox->isChecked();
        break;
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
        typeMatches = m_keyboardEventsCheckBox->isChecked();
        break;
    case QEvent::FocusIn:
    case QEvent::FocusOut:
        typeMatches = m_focusEventsCheckBox->isChecked();
        break;
    case QEvent::Paint:
        typeMatches = m_paintEventsCheckBox->isChecked();
        break;
    default:
        typeMatches = false;
        break;
    }
    
    if (!typeMatches) {
        return false;
    }
    
    // 检查拦截概率
    if (m_interceptionProbability < 100) {
        int random = QRandomGenerator::global()->bounded(100);
        if (random >= m_interceptionProbability) {
            return false;
        }
    }
    
    // 应用延迟
    if (m_interceptionDelay > 0) {
        QEventLoop loop;
        QTimer::singleShot(m_interceptionDelay, &loop, &QEventLoop::quit);
        loop.exec();
    }
    
    return true;
}

QEvent* EventInterceptionDemo::modifyEvent(QEvent *event)
{
    // 这里可以实现事件修改逻辑
    // 注意：实际修改事件内容需要非常小心，因为可能影响系统稳定性
    return event;
}

void EventInterceptionDemo::logInterceptionEvent(const QString &source, const QString &action, 
                                                const QString &eventType, const QString &details)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    QString logEntry = QString("[%1] %2 | %3 | %4\n    → %5\n")
                      .arg(timestamp)
                      .arg(source)
                      .arg(action)
                      .arg(eventType)
                      .arg(details);
    
    m_logDisplay->append(logEntry);
    
    // 自动滚动到底部
    QTextCursor cursor = m_logDisplay->textCursor();
    cursor.movePosition(QTextCursor::End);
    m_logDisplay->setTextCursor(cursor);
}

void EventInterceptionDemo::onInterceptionModeChanged()
{
    m_currentMode = static_cast<InterceptionMode>(m_interceptionModeCombo->currentIndex());
    
    QString modeDescription;
    switch (m_currentMode) {
    case NoInterception:
        modeDescription = "无拦截 - 所有事件正常传播";
        break;
    case BlockAll:
        modeDescription = "阻止所有事件 - 匹配的事件将被完全阻止";
        break;
    case BlockSelected:
        modeDescription = "阻止选定事件 - 只阻止选中类型的事件";
        break;
    case ModifyEvents:
        modeDescription = "修改事件 - 在传播前修改事件内容";
        break;
    case ConditionalBlock:
        modeDescription = "条件性阻止 - 根据概率和条件阻止事件";
        break;
    }
    
    logInterceptionEvent("设置", "模式切换", "拦截模式", modeDescription);
}

void EventInterceptionDemo::onEventTypeFilterChanged()
{
    QStringList enabledTypes;
    if (m_mouseEventsCheckBox->isChecked()) enabledTypes << "鼠标事件";
    if (m_keyboardEventsCheckBox->isChecked()) enabledTypes << "键盘事件";
    if (m_focusEventsCheckBox->isChecked()) enabledTypes << "焦点事件";
    if (m_paintEventsCheckBox->isChecked()) enabledTypes << "绘制事件";
    
    QString filterDescription = enabledTypes.isEmpty() ? "无过滤" : enabledTypes.join(", ");
    logInterceptionEvent("设置", "过滤器", "事件类型过滤", 
                       QString("已启用过滤: %1").arg(filterDescription));
}

void EventInterceptionDemo::onClearLog()
{
    m_logDisplay->clear();
    m_totalEvents = 0;
    m_interceptedEvents = 0;
    m_modifiedEvents = 0;
    m_blockedEvents = 0;
    logInterceptionEvent("系统", "清空", "日志清空", "事件日志和统计信息已重置");
}

void EventInterceptionDemo::onResetDemo()
{
    // 重置所有设置到默认值
    m_interceptionModeCombo->setCurrentIndex(0);
    m_globalFilterCheckBox->setChecked(false);
    m_modifyEventsCheckBox->setChecked(false);
    m_logAllEventsCheckBox->setChecked(false);
    
    m_mouseEventsCheckBox->setChecked(true);
    m_keyboardEventsCheckBox->setChecked(true);
    m_focusEventsCheckBox->setChecked(false);
    m_paintEventsCheckBox->setChecked(false);
    
    m_delaySpinBox->setValue(0);
    m_probabilitySlider->setValue(100);
    
    onClearLog();
    logInterceptionEvent("系统", "重置", "演示重置", "所有设置已恢复到默认值");
}

void EventInterceptionDemo::onToggleGlobalFilter()
{
    m_globalFilterEnabled = m_globalFilterCheckBox->isChecked();
    
    if (m_globalFilterEnabled) {
        installEventFilters();
        logInterceptionEvent("系统", "启用", "全局过滤器", "全局事件过滤器已安装");
    } else {
        removeEventFilters();
        logInterceptionEvent("系统", "禁用", "全局过滤器", "全局事件过滤器已移除");
    }
}

void EventInterceptionDemo::installEventFilters()
{
    // 为测试控件安装事件过滤器
    QList<InterceptableWidget*> widgets = findChildren<InterceptableWidget*>();
    for (InterceptableWidget *widget : widgets) {
        widget->installEventFilter(this);
    }
}

void EventInterceptionDemo::removeEventFilters()
{
    // 移除事件过滤器
    QList<InterceptableWidget*> widgets = findChildren<InterceptableWidget*>();
    for (InterceptableWidget *widget : widgets) {
        widget->removeEventFilter(this);
    }
}

// InterceptableWidget 实现
InterceptableWidget::InterceptableWidget(const QString &name, QWidget *parent)
    : QWidget(parent)
    , m_name(name)
    , m_interceptionEnabled(false)
    , m_backgroundColor(Qt::lightGray)
    , m_eventCount(0)
{
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_Hover, true);
}

bool InterceptableWidget::event(QEvent *event)
{
    m_eventCount++;
    
    QString eventType;
    QString details = getEventDetails(event);
    bool shouldLog = false;
    
    switch (event->type()) {
    case QEvent::MouseButtonPress:
        eventType = "鼠标按下";
        shouldLog = true;
        break;
    case QEvent::KeyPress:
        eventType = "按键按下";
        shouldLog = true;
        break;
    case QEvent::FocusIn:
        eventType = "获得焦点";
        shouldLog = true;
        m_backgroundColor = Qt::yellow;
        update();
        break;
    case QEvent::FocusOut:
        eventType = "失去焦点";
        shouldLog = true;
        m_backgroundColor = Qt::lightGray;
        update();
        break;
    case QEvent::Enter:
        m_backgroundColor = QColor(173, 216, 230); // Light blue
        update();
        break;
    case QEvent::Leave:
        if (!hasFocus()) {
            m_backgroundColor = Qt::lightGray;
            update();
        }
        break;
    default:
        break;
    }
    
    if (shouldLog) {
        emit eventProcessed(m_name, eventType, details, m_interceptionEnabled);
        
        if (m_interceptionEnabled) {
            event->ignore();
            return false;
        }
    }
    
    return QWidget::event(event);
}

void InterceptableWidget::mousePressEvent(QMouseEvent *event)
{
    QString details = QString("位置: (%1, %2), 按钮: %3, 事件计数: %4")
                     .arg(event->pos().x())
                     .arg(event->pos().y())
                     .arg(event->button())
                     .arg(m_eventCount);
    
    emit eventProcessed(m_name, "鼠标事件处理", details, false);
    event->accept();
}

void InterceptableWidget::keyPressEvent(QKeyEvent *event)
{
    QString details = QString("按键: '%1', 键码: %2, 修饰键: %3, 事件计数: %4")
                     .arg(event->text())
                     .arg(event->key())
                     .arg(event->modifiers())
                     .arg(m_eventCount);
    
    emit eventProcessed(m_name, "键盘事件处理", details, false);
    event->accept();
}

void InterceptableWidget::focusInEvent(QFocusEvent *event)
{
    emit eventProcessed(m_name, "焦点事件处理", "获得焦点", false);
    QWidget::focusInEvent(event);
}

void InterceptableWidget::focusOutEvent(QFocusEvent *event)
{
    emit eventProcessed(m_name, "焦点事件处理", "失去焦点", false);
    QWidget::focusOutEvent(event);
}

void InterceptableWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(rect(), m_backgroundColor);
    
    // 绘制控件名称和状态
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(rect().adjusted(5, 5, -5, -5), Qt::AlignTop | Qt::AlignLeft, m_name);
    
    // 绘制事件计数
    painter.setFont(QFont("Arial", 8));
    painter.drawText(rect().adjusted(5, 5, -5, -5), Qt::AlignBottom | Qt::AlignRight, 
                    QString("事件: %1").arg(m_eventCount));
    
    // 绘制边框
    painter.setPen(QPen(hasFocus() ? Qt::blue : Qt::gray, 2));
    painter.drawRect(rect().adjusted(1, 1, -1, -1));
    
    QWidget::paintEvent(event);
}

QString InterceptableWidget::getEventDetails(QEvent *event)
{
    switch (event->type()) {
    case QEvent::MouseButtonPress: {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        return QString("按钮: %1, 位置: (%2, %3)")
               .arg(mouseEvent->button())
               .arg(mouseEvent->pos().x())
               .arg(mouseEvent->pos().y());
    }
    case QEvent::KeyPress: {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        return QString("按键: '%1', 键码: %2").arg(keyEvent->text()).arg(keyEvent->key());
    }
    case QEvent::FocusIn:
    case QEvent::FocusOut: {
        QFocusEvent *focusEvent = static_cast<QFocusEvent*>(event);
        return QString("原因: %1").arg(focusEvent->reason());
    }
    default:
        return QString("事件类型: %1").arg(event->type());
    }
}

#include "event_interception_demo.moc"