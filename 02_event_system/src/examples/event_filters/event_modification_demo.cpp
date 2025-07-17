#include "event_modification_demo.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QDateTime>
#include <QDebug>
#include <QApplication>

EventModificationDemo::EventModificationDemo(QWidget *parent)
    : QWidget(parent)
    , m_mainLayout(nullptr)
    , m_globalFilterGroup(nullptr)
    , m_globalFilterEnabled(nullptr)
    , m_interceptMode(nullptr)
    , m_eventModification(nullptr)
    , m_globalFilterReset(nullptr)
    , m_globalStatsLabel(nullptr)
    , m_selectiveFilterGroup(nullptr)
    , m_selectiveFilterEnabled(nullptr)
    , m_eventTypeCombo(nullptr)
    , m_mouseButtonCombo(nullptr)
    , m_keyCodeSpin(nullptr)
    , m_selectiveFilterReset(nullptr)
    , m_selectiveStatsLabel(nullptr)
    , m_eventDisplayGroup(nullptr)
    , m_eventLog(nullptr)
    , m_clearLogButton(nullptr)
    , m_testAreaGroup(nullptr)
    , m_testButton(nullptr)
    , m_testLabel(nullptr)
    , m_generateEventsButton(nullptr)
    , m_globalFilter(nullptr)
    , m_selectiveFilter(nullptr)
    , m_statsTimer(nullptr)
{
    setupUI();
    
    // 创建过滤器实例
    m_globalFilter = new GlobalEventFilter(this);
    m_selectiveFilter = new SelectiveEventFilter(this);
    
    connectSignals();
    setupCustomFilterRules();
    
    // 设置统计更新定时器
    m_statsTimer = new QTimer(this);
    connect(m_statsTimer, &QTimer::timeout, this, &EventModificationDemo::updateStatistics);
    m_statsTimer->start(1000);  // 每秒更新一次统计信息
    
    // 设置窗口属性
    setWindowTitle("事件过滤器演示");
    setMinimumSize(800, 600);
    setFocusPolicy(Qt::StrongFocus);  // 确保能接收键盘事件
}

EventModificationDemo::~EventModificationDemo()
{
    // 清理资源
    if (m_globalFilter) {
        m_globalFilter->clearWatchedObjects();
    }
    if (m_selectiveFilter) {
        m_selectiveFilter->setTargetObjects({});
    }
}

void EventModificationDemo::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    
    setupGlobalFilterControls();
    setupSelectiveFilterControls();
    setupEventDisplay();
    setupTestArea();
    
    setLayout(m_mainLayout);
}

void EventModificationDemo::setupGlobalFilterControls()
{
    m_globalFilterGroup = new QGroupBox("全局事件过滤器", this);
    QVBoxLayout *layout = new QVBoxLayout(m_globalFilterGroup);
    
    // 控制选项
    m_globalFilterEnabled = new QCheckBox("启用全局过滤器", this);
    m_globalFilterEnabled->setChecked(true);
    
    m_interceptMode = new QCheckBox("拦截模式", this);
    m_interceptMode->setToolTip("启用后将拦截特定事件（如右键点击、ESC键）");
    
    m_eventModification = new QCheckBox("事件修改", this);
    m_eventModification->setToolTip("启用后将尝试修改某些事件");
    
    m_globalFilterReset = new QPushButton("重置统计", this);
    
    m_globalStatsLabel = new QLabel("统计信息：处理: 0, 拦截: 0, 修改: 0", this);
    
    layout->addWidget(m_globalFilterEnabled);
    layout->addWidget(m_interceptMode);
    layout->addWidget(m_eventModification);
    layout->addWidget(m_globalFilterReset);
    layout->addWidget(m_globalStatsLabel);
    
    m_mainLayout->addWidget(m_globalFilterGroup);
}

void EventModificationDemo::setupSelectiveFilterControls()
{
    m_selectiveFilterGroup = new QGroupBox("选择性事件过滤器", this);
    QGridLayout *layout = new QGridLayout(m_selectiveFilterGroup);
    
    // 启用选项
    m_selectiveFilterEnabled = new QCheckBox("启用选择性过滤器", this);
    layout->addWidget(m_selectiveFilterEnabled, 0, 0, 1, 2);
    
    // 事件类型选择
    layout->addWidget(new QLabel("允许的事件类型:", this), 1, 0);
    m_eventTypeCombo = new QComboBox(this);
    m_eventTypeCombo->addItem("所有事件", static_cast<int>(QEvent::None));
    m_eventTypeCombo->addItem("鼠标按下", static_cast<int>(QEvent::MouseButtonPress));
    m_eventTypeCombo->addItem("鼠标释放", static_cast<int>(QEvent::MouseButtonRelease));
    m_eventTypeCombo->addItem("按键按下", static_cast<int>(QEvent::KeyPress));
    m_eventTypeCombo->addItem("按键释放", static_cast<int>(QEvent::KeyRelease));
    m_eventTypeCombo->addItem("滚轮事件", static_cast<int>(QEvent::Wheel));
    layout->addWidget(m_eventTypeCombo, 1, 1);
    
    // 鼠标按钮过滤
    layout->addWidget(new QLabel("允许的鼠标按钮:", this), 2, 0);
    m_mouseButtonCombo = new QComboBox(this);
    m_mouseButtonCombo->addItem("所有按钮", static_cast<int>(Qt::AllButtons));
    m_mouseButtonCombo->addItem("左键", static_cast<int>(Qt::LeftButton));
    m_mouseButtonCombo->addItem("右键", static_cast<int>(Qt::RightButton));
    m_mouseButtonCombo->addItem("中键", static_cast<int>(Qt::MiddleButton));
    layout->addWidget(m_mouseButtonCombo, 2, 1);
    
    // 按键过滤
    layout->addWidget(new QLabel("允许的按键码:", this), 3, 0);
    m_keyCodeSpin = new QSpinBox(this);
    m_keyCodeSpin->setRange(0, 0xFFFF);
    m_keyCodeSpin->setValue(0);  // 0表示允许所有按键
    m_keyCodeSpin->setToolTip("0表示允许所有按键，其他值表示只允许特定按键");
    layout->addWidget(m_keyCodeSpin, 3, 1);
    
    // 重置按钮和统计
    m_selectiveFilterReset = new QPushButton("重置统计", this);
    layout->addWidget(m_selectiveFilterReset, 4, 0);
    
    m_selectiveStatsLabel = new QLabel("统计信息：处理: 0, 通过: 0, 阻止: 0, 转换: 0", this);
    layout->addWidget(m_selectiveStatsLabel, 4, 1);
    
    m_mainLayout->addWidget(m_selectiveFilterGroup);
}

void EventModificationDemo::setupEventDisplay()
{
    m_eventDisplayGroup = new QGroupBox("事件日志", this);
    QVBoxLayout *layout = new QVBoxLayout(m_eventDisplayGroup);
    
    m_eventLog = new QTextEdit(this);
    m_eventLog->setMaximumHeight(200);
    m_eventLog->setReadOnly(true);
    m_eventLog->setFont(QFont("monospace", 9));
    
    m_clearLogButton = new QPushButton("清空日志", this);
    
    layout->addWidget(m_eventLog);
    layout->addWidget(m_clearLogButton);
    
    m_mainLayout->addWidget(m_eventDisplayGroup);
}

void EventModificationDemo::setupTestArea()
{
    m_testAreaGroup = new QGroupBox("测试区域", this);
    QHBoxLayout *layout = new QHBoxLayout(m_testAreaGroup);
    
    m_testButton = new QPushButton("测试按钮", this);
    m_testButton->setToolTip("点击此按钮测试事件过滤");
    
    m_testLabel = new QLabel("在此区域进行鼠标和键盘操作来测试事件过滤器", this);
    m_testLabel->setStyleSheet("QLabel { border: 1px solid gray; padding: 10px; background-color: #f0f0f0; }");
    m_testLabel->setMinimumHeight(80);
    m_testLabel->setAlignment(Qt::AlignCenter);
    
    m_generateEventsButton = new QPushButton("生成测试事件", this);
    m_generateEventsButton->setToolTip("程序化生成一些测试事件");
    
    layout->addWidget(m_testButton);
    layout->addWidget(m_testLabel);
    layout->addWidget(m_generateEventsButton);
    
    // 将测试控件添加到列表中
    m_testWidgets.append(m_testButton);
    m_testWidgets.append(m_testLabel);
    m_testWidgets.append(m_generateEventsButton);
    
    m_mainLayout->addWidget(m_testAreaGroup);
}

void EventModificationDemo::connectSignals()
{
    // 全局过滤器信号连接
    connect(m_globalFilterEnabled, &QCheckBox::toggled, 
            this, &EventModificationDemo::onGlobalFilterToggled);
    connect(m_interceptMode, &QCheckBox::toggled, 
            this, &EventModificationDemo::onInterceptModeToggled);
    connect(m_eventModification, &QCheckBox::toggled, 
            this, &EventModificationDemo::onEventModificationToggled);
    connect(m_globalFilterReset, &QPushButton::clicked, 
            this, &EventModificationDemo::onGlobalFilterReset);
    
    // 选择性过滤器信号连接
    connect(m_selectiveFilterEnabled, &QCheckBox::toggled, 
            this, &EventModificationDemo::onSelectiveFilterToggled);
    connect(m_eventTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &EventModificationDemo::onAllowedEventTypeChanged);
    connect(m_mouseButtonCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &EventModificationDemo::onMouseButtonFilterChanged);
    connect(m_keyCodeSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &EventModificationDemo::onKeyFilterChanged);
    connect(m_selectiveFilterReset, &QPushButton::clicked, 
            this, &EventModificationDemo::onSelectiveFilterReset);
    
    // 事件日志
    connect(m_clearLogButton, &QPushButton::clicked, 
            this, &EventModificationDemo::clearEventLog);
    
    // 测试按钮
    connect(m_testButton, &QPushButton::clicked, 
            this, &EventModificationDemo::onTestButtonClicked);
    connect(m_generateEventsButton, &QPushButton::clicked, 
            this, &EventModificationDemo::generateTestEvents);
    
    // 暂时注释掉信号连接，避免编译问题
    // TODO: 修复MOC问题后重新启用
    /*
    if (m_globalFilter) {
        connect(m_globalFilter, &GlobalEventFilter::eventFiltered,
                this, &EventModificationDemo::onGlobalEventFiltered);
        connect(m_globalFilter, &GlobalEventFilter::eventModified,
                this, &EventModificationDemo::onGlobalEventModified);
    }
    
    if (m_selectiveFilter) {
        connect(m_selectiveFilter, &SelectiveEventFilter::eventPassed,
                this, &EventModificationDemo::onSelectiveEventPassed);
        connect(m_selectiveFilter, &SelectiveEventFilter::eventBlocked,
                this, &EventModificationDemo::onSelectiveEventBlocked);
        connect(m_selectiveFilter, &SelectiveEventFilter::eventTransformed,
                this, &EventModificationDemo::onSelectiveEventTransformed);
    }
    */
}

void EventModificationDemo::setupCustomFilterRules()
{
    // 为选择性过滤器添加一些自定义规则
    
    // 规则1：阻止在周末进行的操作（示例规则）
    m_selectiveFilter->addFilterRule("weekend_block", 
        [](QObject* watched, QEvent* event) -> bool {
            Q_UNUSED(watched)
            Q_UNUSED(event)
            QDateTime now = QDateTime::currentDateTime();
            int dayOfWeek = now.date().dayOfWeek();
            // 在实际应用中可能不会这样做，这里只是演示
            return dayOfWeek < 6;  // 周一到周五允许，周末阻止
        });
    
    // 规则2：限制快速连续点击
    static QDateTime lastClickTime;
    m_selectiveFilter->addFilterRule("click_rate_limit", 
        [](QObject* watched, QEvent* event) -> bool {
            Q_UNUSED(watched)
            if (event->type() == QEvent::MouseButtonPress) {
                QDateTime now = QDateTime::currentDateTime();
                if (lastClickTime.isValid() && 
                    lastClickTime.msecsTo(now) < 200) {  // 200ms内的连续点击被阻止
                    return false;
                }
                lastClickTime = now;
            }
            return true;
        });
}

void EventModificationDemo::onGlobalFilterToggled(bool enabled)
{
    if (m_globalFilter) {
        m_globalFilter->setEnabled(enabled);
        
        if (enabled) {
            // 为所有测试控件安装全局过滤器
            for (QWidget *widget : m_testWidgets) {
                m_globalFilter->addWatchedObject(widget);
            }
            // 也为主窗口安装过滤器
            m_globalFilter->addWatchedObject(this);
            
            logEvent("全局事件过滤器已启用");
        } else {
            m_globalFilter->clearWatchedObjects();
            logEvent("全局事件过滤器已禁用");
        }
    }
}

void EventModificationDemo::onInterceptModeToggled(bool enabled)
{
    if (m_globalFilter) {
        m_globalFilter->setInterceptMode(enabled);
        logEvent(enabled ? "拦截模式已启用" : "拦截模式已禁用");
    }
}

void EventModificationDemo::onEventModificationToggled(bool enabled)
{
    if (m_globalFilter) {
        m_globalFilter->setEventModificationEnabled(enabled);
        logEvent(enabled ? "事件修改已启用" : "事件修改已禁用");
    }
}

void EventModificationDemo::onGlobalFilterReset()
{
    if (m_globalFilter) {
        m_globalFilter->resetStatistics();
        logEvent("全局过滤器统计已重置");
    }
}

void EventModificationDemo::onSelectiveFilterToggled(bool enabled)
{
    if (m_selectiveFilter) {
        m_selectiveFilter->setEnabled(enabled);
        
        if (enabled) {
            // 为测试控件设置选择性过滤器
            QSet<QObject*> targets;
            for (QWidget *widget : m_testWidgets) {
                targets.insert(widget);
            }
            targets.insert(this);
            m_selectiveFilter->setTargetObjects(targets);
            
            logEvent("选择性事件过滤器已启用");
        } else {
            m_selectiveFilter->setTargetObjects({});
            logEvent("选择性事件过滤器已禁用");
        }
    }
}

void EventModificationDemo::onAllowedEventTypeChanged()
{
    if (!m_selectiveFilter || !m_eventTypeCombo) return;
    
    int eventType = m_eventTypeCombo->currentData().toInt();
    if (eventType == static_cast<int>(QEvent::None)) {
        // 允许所有事件类型
        m_selectiveFilter->setAllowedEventTypes({});
    } else {
        // 只允许选定的事件类型
        QSet<QEvent::Type> allowedTypes;
        allowedTypes.insert(static_cast<QEvent::Type>(eventType));
        m_selectiveFilter->setAllowedEventTypes(allowedTypes);
    }
    
    logEvent(QString("事件类型过滤已更新: %1").arg(m_eventTypeCombo->currentText()));
}

void EventModificationDemo::onMouseButtonFilterChanged()
{
    if (!m_selectiveFilter || !m_mouseButtonCombo) return;
    
    Qt::MouseButtons buttons = static_cast<Qt::MouseButtons>(m_mouseButtonCombo->currentData().toInt());
    m_selectiveFilter->setMouseButtonFilter(buttons);
    
    logEvent(QString("鼠标按钮过滤已更新: %1").arg(m_mouseButtonCombo->currentText()));
}

void EventModificationDemo::onKeyFilterChanged()
{
    if (!m_selectiveFilter || !m_keyCodeSpin) return;
    
    int keyCode = m_keyCodeSpin->value();
    if (keyCode == 0) {
        // 允许所有按键
        m_selectiveFilter->setKeyFilter({});
    } else {
        // 只允许特定按键
        QSet<int> allowedKeys;
        allowedKeys.insert(keyCode);
        m_selectiveFilter->setKeyFilter(allowedKeys);
    }
    
    logEvent(QString("按键过滤已更新: %1").arg(keyCode == 0 ? "所有按键" : QString::number(keyCode)));
}

void EventModificationDemo::onSelectiveFilterReset()
{
    if (m_selectiveFilter) {
        m_selectiveFilter->resetStatistics();
        logEvent("选择性过滤器统计已重置");
    }
}

void EventModificationDemo::onGlobalEventFiltered(QObject *watched, QEvent *event, bool intercepted)
{
    QString message = QString("全局过滤: %1 %2")
                      .arg(formatEventInfo(watched, event))
                      .arg(intercepted ? "[已拦截]" : "[已通过]");
    logEvent(message);
}

void EventModificationDemo::onGlobalEventModified(QObject *watched, QEvent::Type originalType, QEvent::Type modifiedType)
{
    Q_UNUSED(watched)
    QString message = QString("事件修改: %1 -> %2")
                      .arg(static_cast<int>(originalType))
                      .arg(static_cast<int>(modifiedType));
    logEvent(message);
}

void EventModificationDemo::onSelectiveEventPassed(QObject *watched, QEvent *event)
{
    QString message = QString("选择性过滤通过: %1").arg(formatEventInfo(watched, event));
    logEvent(message);
}

void EventModificationDemo::onSelectiveEventBlocked(QObject *watched, QEvent *event, const QString &reason)
{
    QString message = QString("选择性过滤阻止: %1 [原因: %2]")
                      .arg(formatEventInfo(watched, event))
                      .arg(reason);
    logEvent(message);
}

void EventModificationDemo::onSelectiveEventTransformed(QObject *watched, QEvent *originalEvent, QEvent *transformedEvent)
{
    Q_UNUSED(watched)
    QString message = QString("事件转换: %1 -> %2")
                      .arg(static_cast<int>(originalEvent->type()))
                      .arg(static_cast<int>(transformedEvent->type()));
    logEvent(message);
}

void EventModificationDemo::updateStatistics()
{
    if (m_globalFilter && m_globalStatsLabel) {
        QString globalStats = QString("统计信息：处理: %1, 拦截: %2, 修改: %3")
                              .arg(m_globalFilter->totalEventsProcessed())
                              .arg(m_globalFilter->eventsIntercepted())
                              .arg(m_globalFilter->eventsModified());
        m_globalStatsLabel->setText(globalStats);
    }
    
    if (m_selectiveFilter && m_selectiveStatsLabel) {
        QString selectiveStats = QString("统计信息：处理: %1, 通过: %2, 阻止: %3, 转换: %4")
                                 .arg(m_selectiveFilter->totalEventsProcessed())
                                 .arg(m_selectiveFilter->eventsFiltered())
                                 .arg(m_selectiveFilter->eventsBlocked())
                                 .arg(m_selectiveFilter->eventsTransformed());
        m_selectiveStatsLabel->setText(selectiveStats);
    }
}

void EventModificationDemo::clearEventLog()
{
    if (m_eventLog) {
        m_eventLog->clear();
    }
}

void EventModificationDemo::onTestButtonClicked()
{
    logEvent("测试按钮被点击");
}

void EventModificationDemo::generateTestEvents()
{
    logEvent("开始生成测试事件...");
    
    // 模拟一些鼠标事件（使用新的构造函数）
    QMouseEvent *pressEvent = new QMouseEvent(QEvent::MouseButtonPress, 
                                              QPointF(100, 100), 
                                              QPointF(100, 100),
                                              Qt::LeftButton, 
                                              Qt::LeftButton, 
                                              Qt::NoModifier);
    QApplication::postEvent(m_testButton, pressEvent);
    
    QMouseEvent *releaseEvent = new QMouseEvent(QEvent::MouseButtonRelease, 
                                                QPointF(100, 100), 
                                                QPointF(100, 100),
                                                Qt::LeftButton, 
                                                Qt::LeftButton, 
                                                Qt::NoModifier);
    QApplication::postEvent(m_testButton, releaseEvent);
    
    // 模拟一些键盘事件
    QKeyEvent *keyPressEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_A, Qt::NoModifier, "a");
    QApplication::postEvent(this, keyPressEvent);
    
    QKeyEvent *keyReleaseEvent = new QKeyEvent(QEvent::KeyRelease, Qt::Key_A, Qt::NoModifier, "a");
    QApplication::postEvent(this, keyReleaseEvent);
    
    logEvent("测试事件生成完成");
}

void EventModificationDemo::logEvent(const QString &message)
{
    if (m_eventLog) {
        QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
        QString logMessage = QString("[%1] %2").arg(timestamp).arg(message);
        m_eventLog->append(logMessage);
        
        // 自动滚动到底部
        QTextCursor cursor = m_eventLog->textCursor();
        cursor.movePosition(QTextCursor::End);
        m_eventLog->setTextCursor(cursor);
    }
}

QString EventModificationDemo::formatEventInfo(QObject *watched, QEvent *event) const
{
    QString objectName = watched ? watched->objectName() : "Unknown";
    if (objectName.isEmpty()) {
        objectName = watched ? watched->metaObject()->className() : "Unknown";
    }
    
    QString eventTypeName;
    switch (event->type()) {
    case QEvent::MouseButtonPress: eventTypeName = "MousePress"; break;
    case QEvent::MouseButtonRelease: eventTypeName = "MouseRelease"; break;
    case QEvent::MouseMove: eventTypeName = "MouseMove"; break;
    case QEvent::KeyPress: eventTypeName = "KeyPress"; break;
    case QEvent::KeyRelease: eventTypeName = "KeyRelease"; break;
    case QEvent::Wheel: eventTypeName = "Wheel"; break;
    default: eventTypeName = QString("Type%1").arg(static_cast<int>(event->type())); break;
    }
    
    return QString("%1 on %2").arg(eventTypeName).arg(objectName);
}

// 重写事件处理函数以演示事件处理
void EventModificationDemo::mousePressEvent(QMouseEvent *event)
{
    logEvent(QString("主窗口鼠标按下: 按钮=%1, 位置=(%2,%3)")
             .arg(static_cast<int>(event->button()))
             .arg(event->position().x())
             .arg(event->position().y()));
    QWidget::mousePressEvent(event);
}

void EventModificationDemo::mouseReleaseEvent(QMouseEvent *event)
{
    logEvent(QString("主窗口鼠标释放: 按钮=%1, 位置=(%2,%3)")
             .arg(static_cast<int>(event->button()))
             .arg(event->position().x())
             .arg(event->position().y()));
    QWidget::mouseReleaseEvent(event);
}

void EventModificationDemo::keyPressEvent(QKeyEvent *event)
{
    logEvent(QString("主窗口按键按下: 键=%1, 文本='%2'")
             .arg(event->key())
             .arg(event->text()));
    QWidget::keyPressEvent(event);
}

void EventModificationDemo::keyReleaseEvent(QKeyEvent *event)
{
    logEvent(QString("主窗口按键释放: 键=%1, 文本='%2'")
             .arg(event->key())
             .arg(event->text()));
    QWidget::keyReleaseEvent(event);
}

void EventModificationDemo::wheelEvent(QWheelEvent *event)
{
    logEvent(QString("主窗口滚轮事件: 角度=%1, 位置=(%2,%3)")
             .arg(event->angleDelta().y())
             .arg(event->position().x())
             .arg(event->position().y()));
    QWidget::wheelEvent(event);
}