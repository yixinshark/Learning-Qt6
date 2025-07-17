#include "interactive_area_widget.h"
#include "../core/event_logger.h"
#include "../core/custom_events.h"
#include <QPainter>
#include <QApplication>
#include <QDebug>
#include <QRandomGenerator>

InteractiveAreaWidget::InteractiveAreaWidget(QWidget* parent)
    : QWidget(parent)
    , m_mainLayout(nullptr)
    , m_basicControlsGroup(nullptr)
    , m_eventTestGroup(nullptr)
    , m_eventGeneratorGroup(nullptr)
    , m_mousePressed(false)
    , m_eventStormTimer(nullptr)
    , m_eventStormCount(0)
    , m_backgroundColor(QColor(240, 248, 255))
{
    setMinimumSize(300, 400);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    
    setupUI();
    
    // 设置事件风暴定时器
    m_eventStormTimer = new QTimer(this);
    connect(m_eventStormTimer, &QTimer::timeout, this, &InteractiveAreaWidget::generateEventStorm);
    
    qDebug() << "InteractiveAreaWidget initialized";
}

void InteractiveAreaWidget::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(5, 5, 5, 5);
    m_mainLayout->setSpacing(5);
    
    // 标题
    QLabel* titleLabel = new QLabel("交互测试区域");
    titleLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #2c3e50;");
    m_mainLayout->addWidget(titleLabel);
    
    setupBasicControls();
    setupEventTestArea();
    setupEventGenerators();
    
    m_mainLayout->addStretch();
}

void InteractiveAreaWidget::setupBasicControls()
{
    m_basicControlsGroup = new QGroupBox("基础控件测试");
    QGridLayout* layout = new QGridLayout(m_basicControlsGroup);
    
    // 按钮
    m_testButton = new QPushButton("测试按钮");
    connect(m_testButton, &QPushButton::clicked, this, &InteractiveAreaWidget::onButtonClicked);
    layout->addWidget(new QLabel("按钮:"), 0, 0);
    layout->addWidget(m_testButton, 0, 1);
    
    // 文本输入
    m_testLineEdit = new QLineEdit();
    m_testLineEdit->setPlaceholderText("输入文本测试键盘事件...");
    connect(m_testLineEdit, &QLineEdit::textChanged, this, &InteractiveAreaWidget::onTextChanged);
    layout->addWidget(new QLabel("文本:"), 1, 0);
    layout->addWidget(m_testLineEdit, 1, 1);
    
    // 滑块
    m_testSlider = new QSlider(Qt::Horizontal);
    m_testSlider->setRange(0, 100);
    m_testSlider->setValue(50);
    connect(m_testSlider, &QSlider::valueChanged, this, &InteractiveAreaWidget::onSliderValueChanged);
    layout->addWidget(new QLabel("滑块:"), 2, 0);
    layout->addWidget(m_testSlider, 2, 1);
    
    // 数值框
    m_testSpinBox = new QSpinBox();
    m_testSpinBox->setRange(0, 1000);
    m_testSpinBox->setValue(50);
    layout->addWidget(new QLabel("数值:"), 3, 0);
    layout->addWidget(m_testSpinBox, 3, 1);
    
    // 复选框
    m_testCheckBox = new QCheckBox("启用测试模式");
    connect(m_testCheckBox, &QCheckBox::toggled, this, &InteractiveAreaWidget::onCheckBoxToggled);
    layout->addWidget(m_testCheckBox, 4, 0, 1, 2);
    
    // 下拉框
    m_testComboBox = new QComboBox();
    m_testComboBox->addItems({"选项1", "选项2", "选项3", "选项4"});
    connect(m_testComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &InteractiveAreaWidget::onComboBoxChanged);
    layout->addWidget(new QLabel("选择:"), 5, 0);
    layout->addWidget(m_testComboBox, 5, 1);
    
    // 进度条
    m_testProgressBar = new QProgressBar();
    m_testProgressBar->setValue(50);
    layout->addWidget(new QLabel("进度:"), 6, 0);
    layout->addWidget(m_testProgressBar, 6, 1);
    
    m_mainLayout->addWidget(m_basicControlsGroup);
}

void InteractiveAreaWidget::setupEventTestArea()
{
    m_eventTestGroup = new QGroupBox("事件测试区域");
    QVBoxLayout* layout = new QVBoxLayout(m_eventTestGroup);
    
    // 交互信息显示
    m_interactionLabel = new QLabel("在此区域进行鼠标和键盘交互");
    m_interactionLabel->setStyleSheet("background-color: #ecf0f1; padding: 10px; border-radius: 5px;");
    m_interactionLabel->setMinimumHeight(60);
    m_interactionLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_interactionLabel);
    
    // 鼠标位置显示
    m_mousePositionLabel = new QLabel("鼠标位置: (0, 0)");
    layout->addWidget(m_mousePositionLabel);
    
    // 按键信息显示
    m_keyInfoLabel = new QLabel("按键信息: 无");
    layout->addWidget(m_keyInfoLabel);
    
    // 事件日志
    QLabel* logLabel = new QLabel("最近事件:");
    layout->addWidget(logLabel);
    
    m_eventLogText = new QTextEdit();
    m_eventLogText->setMaximumHeight(100);
    m_eventLogText->setReadOnly(true);
    layout->addWidget(m_eventLogText);
    
    m_mainLayout->addWidget(m_eventTestGroup);
}

void InteractiveAreaWidget::setupEventGenerators()
{
    m_eventGeneratorGroup = new QGroupBox("事件生成器");
    QGridLayout* layout = new QGridLayout(m_eventGeneratorGroup);
    
    // 自定义事件按钮
    m_customEventButton = new QPushButton("发送自定义事件");
    connect(m_customEventButton, &QPushButton::clicked, this, &InteractiveAreaWidget::triggerCustomEvent);
    layout->addWidget(m_customEventButton, 0, 0);
    
    // 模拟鼠标事件按钮
    m_mouseEventButton = new QPushButton("模拟鼠标事件");
    connect(m_mouseEventButton, &QPushButton::clicked, this, &InteractiveAreaWidget::simulateMouseEvent);
    layout->addWidget(m_mouseEventButton, 0, 1);
    
    // 模拟键盘事件按钮
    m_keyEventButton = new QPushButton("模拟键盘事件");
    connect(m_keyEventButton, &QPushButton::clicked, this, &InteractiveAreaWidget::simulateKeyEvent);
    layout->addWidget(m_keyEventButton, 1, 0);
    
    // 事件风暴控制
    QHBoxLayout* stormLayout = new QHBoxLayout();
    m_eventStormButton = new QPushButton("开始事件风暴");
    connect(m_eventStormButton, &QPushButton::clicked, this, &InteractiveAreaWidget::startEventStorm);
    
    m_stopStormButton = new QPushButton("停止");
    m_stopStormButton->setEnabled(false);
    connect(m_stopStormButton, &QPushButton::clicked, this, &InteractiveAreaWidget::stopEventStorm);
    
    stormLayout->addWidget(m_eventStormButton);
    stormLayout->addWidget(m_stopStormButton);
    layout->addLayout(stormLayout, 1, 1);
    
    // 事件间隔设置
    QHBoxLayout* intervalLayout = new QHBoxLayout();
    intervalLayout->addWidget(new QLabel("间隔(ms):"));
    m_stormIntervalSpinBox = new QSpinBox();
    m_stormIntervalSpinBox->setRange(1, 1000);
    m_stormIntervalSpinBox->setValue(100);
    intervalLayout->addWidget(m_stormIntervalSpinBox);
    layout->addLayout(intervalLayout, 2, 0, 1, 2);
    
    m_mainLayout->addWidget(m_eventGeneratorGroup);
}

void InteractiveAreaWidget::setCurrentExample(const QString& exampleName)
{
    m_currentExample = exampleName;
    updateInteractionInfo(QString("当前示例: %1").arg(exampleName));
}

void InteractiveAreaWidget::mousePressEvent(QMouseEvent* event)
{
    m_mousePressed = true;
    m_lastMousePos = event->pos();
    m_mouseTrail.clear();
    m_mouseTrail.append(event->pos());
    
    QString info = QString("鼠标按下: (%1, %2) 按钮: %3")
                   .arg(event->pos().x())
                   .arg(event->pos().y())
                   .arg(event->button() == Qt::LeftButton ? "左键" : 
                        event->button() == Qt::RightButton ? "右键" : "中键");
    
    updateInteractionInfo(info);
    logInteractionEvent("MousePress", info);
    
    update(); // 触发重绘
    QWidget::mousePressEvent(event);
}

void InteractiveAreaWidget::mouseMoveEvent(QMouseEvent* event)
{
    m_lastMousePos = event->pos();
    m_mousePositionLabel->setText(QString("鼠标位置: (%1, %2)")
                                 .arg(event->pos().x())
                                 .arg(event->pos().y()));
    
    if (m_mousePressed) {
        m_mouseTrail.append(event->pos());
        if (m_mouseTrail.size() > 50) { // 限制轨迹点数量
            m_mouseTrail.removeFirst();
        }
        update();
    }
    
    QWidget::mouseMoveEvent(event);
}

void InteractiveAreaWidget::mouseReleaseEvent(QMouseEvent* event)
{
    m_mousePressed = false;
    
    QString info = QString("鼠标释放: (%1, %2)")
                   .arg(event->pos().x())
                   .arg(event->pos().y());
    
    updateInteractionInfo(info);
    logInteractionEvent("MouseRelease", info);
    
    QWidget::mouseReleaseEvent(event);
}

void InteractiveAreaWidget::keyPressEvent(QKeyEvent* event)
{
    QString keyText = event->text().isEmpty() ? 
                     QString("特殊键: %1").arg(event->key()) : 
                     QString("字符: %1").arg(event->text());
    
    QString modifiers;
    if (event->modifiers() & Qt::ControlModifier) modifiers += "Ctrl+";
    if (event->modifiers() & Qt::ShiftModifier) modifiers += "Shift+";
    if (event->modifiers() & Qt::AltModifier) modifiers += "Alt+";
    
    QString info = QString("按键按下: %1%2").arg(modifiers).arg(keyText);
    m_keyInfoLabel->setText(info);
    updateInteractionInfo(info);
    logInteractionEvent("KeyPress", info);
    
    QWidget::keyPressEvent(event);
}

void InteractiveAreaWidget::keyReleaseEvent(QKeyEvent* event)
{
    QString info = QString("按键释放: %1").arg(event->key());
    logInteractionEvent("KeyRelease", info);
    
    QWidget::keyReleaseEvent(event);
}

void InteractiveAreaWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 绘制背景
    painter.fillRect(rect(), m_backgroundColor);
    
    // 绘制鼠标轨迹
    if (m_mouseTrail.size() > 1) {
        painter.setPen(QPen(QColor(52, 152, 219), 2));
        for (int i = 1; i < m_mouseTrail.size(); ++i) {
            painter.drawLine(m_mouseTrail[i-1], m_mouseTrail[i]);
        }
        
        // 绘制当前鼠标位置
        if (!m_mouseTrail.isEmpty()) {
            painter.setPen(QPen(QColor(231, 76, 60), 3));
            painter.drawEllipse(m_mouseTrail.last(), 5, 5);
        }
    }
    
    QWidget::paintEvent(event);
}

void InteractiveAreaWidget::enterEvent(QEnterEvent* event)
{
    m_backgroundColor = QColor(245, 255, 250);
    update();
    updateInteractionInfo("鼠标进入交互区域");
    logInteractionEvent("Enter", "鼠标进入");
    QWidget::enterEvent(event);
}

void InteractiveAreaWidget::leaveEvent(QEvent* event)
{
    m_backgroundColor = QColor(240, 248, 255);
    update();
    updateInteractionInfo("鼠标离开交互区域");
    logInteractionEvent("Leave", "鼠标离开");
    QWidget::leaveEvent(event);
}

void InteractiveAreaWidget::triggerCustomEvent()
{
    // 创建自定义数据事件
    QVariantMap data;
    data["source"] = "InteractiveAreaWidget";
    data["action"] = "custom_trigger";
    data["timestamp"] = QDateTime::currentDateTime();
    data["value"] = QRandomGenerator::global()->bounded(100);
    
    DataEvent* customEvent = new DataEvent(data);
    QApplication::postEvent(this, customEvent);
    
    logInteractionEvent("CustomEvent", "发送自定义数据事件");
}

void InteractiveAreaWidget::simulateMouseEvent()
{
    // 模拟鼠标点击事件
    QPoint center = rect().center();
    QMouseEvent* pressEvent = new QMouseEvent(QEvent::MouseButtonPress, center, center,
                                            Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QMouseEvent* releaseEvent = new QMouseEvent(QEvent::MouseButtonRelease, center, center,
                                              Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    
    QApplication::postEvent(this, pressEvent);
    QApplication::postEvent(this, releaseEvent);
    
    logInteractionEvent("SimulatedMouse", "模拟鼠标点击事件");
}

void InteractiveAreaWidget::simulateKeyEvent()
{
    // 模拟按键事件
    QKeyEvent* pressEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier, " ");
    QKeyEvent* releaseEvent = new QKeyEvent(QEvent::KeyRelease, Qt::Key_Space, Qt::NoModifier, " ");
    
    QApplication::postEvent(this, pressEvent);
    QApplication::postEvent(this, releaseEvent);
    
    logInteractionEvent("SimulatedKey", "模拟空格键事件");
}

void InteractiveAreaWidget::startEventStorm()
{
    m_eventStormCount = 0;
    m_eventStormTimer->start(m_stormIntervalSpinBox->value());
    m_eventStormButton->setEnabled(false);
    m_stopStormButton->setEnabled(true);
    
    logInteractionEvent("EventStorm", "开始事件风暴");
}

void InteractiveAreaWidget::stopEventStorm()
{
    m_eventStormTimer->stop();
    m_eventStormButton->setEnabled(true);
    m_stopStormButton->setEnabled(false);
    
    logInteractionEvent("EventStorm", QString("停止事件风暴 (共生成 %1 个事件)").arg(m_eventStormCount));
}

void InteractiveAreaWidget::generateEventStorm()
{
    // 随机生成不同类型的事件
    int eventType = QRandomGenerator::global()->bounded(3);
    
    switch (eventType) {
    case 0: // 鼠标事件
        {
            QPoint randomPos(QRandomGenerator::global()->bounded(width()),
                           QRandomGenerator::global()->bounded(height()));
            QMouseEvent* event = new QMouseEvent(QEvent::MouseButtonPress, randomPos, randomPos,
                                               Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
            QApplication::postEvent(this, event);
        }
        break;
    case 1: // 键盘事件
        {
            int key = Qt::Key_A + QRandomGenerator::global()->bounded(26);
            QKeyEvent* event = new QKeyEvent(QEvent::KeyPress, key, Qt::NoModifier);
            QApplication::postEvent(this, event);
        }
        break;
    case 2: // 自定义事件
        {
            QVariantMap data;
            data["storm_event"] = true;
            data["count"] = m_eventStormCount;
            DataEvent* event = new DataEvent(data);
            QApplication::postEvent(this, event);
        }
        break;
    }
    
    m_eventStormCount++;
    
    // 限制事件风暴数量
    if (m_eventStormCount >= 1000) {
        stopEventStorm();
    }
}

void InteractiveAreaWidget::onButtonClicked()
{
    logInteractionEvent("ButtonClick", "测试按钮被点击");
    m_testProgressBar->setValue((m_testProgressBar->value() + 10) % 101);
}

void InteractiveAreaWidget::onSliderValueChanged(int value)
{
    logInteractionEvent("SliderChange", QString("滑块值变为: %1").arg(value));
    m_testProgressBar->setValue(value);
    m_testSpinBox->setValue(value);
}

void InteractiveAreaWidget::onTextChanged()
{
    QString text = m_testLineEdit->text();
    logInteractionEvent("TextChange", QString("文本变为: %1").arg(text));
}

void InteractiveAreaWidget::onComboBoxChanged()
{
    QString text = m_testComboBox->currentText();
    logInteractionEvent("ComboChange", QString("选择变为: %1").arg(text));
}

void InteractiveAreaWidget::onCheckBoxToggled(bool checked)
{
    logInteractionEvent("CheckBoxToggle", QString("复选框: %1").arg(checked ? "选中" : "未选中"));
}

void InteractiveAreaWidget::updateInteractionInfo(const QString& info)
{
    m_interactionLabel->setText(info);
}

void InteractiveAreaWidget::logInteractionEvent(const QString& eventType, const QString& details)
{
    QString logEntry = QString("[%1] %2: %3")
                      .arg(QTime::currentTime().toString("hh:mm:ss.zzz"))
                      .arg(eventType)
                      .arg(details);
    
    m_eventLogText->append(logEntry);
    
    // 限制日志行数
    QTextDocument* doc = m_eventLogText->document();
    if (doc->blockCount() > 50) {
        QTextCursor cursor = m_eventLogText->textCursor();
        cursor.movePosition(QTextCursor::Start);
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.removeSelectedText();
        cursor.deleteChar(); // 删除换行符
    }
    
    // 滚动到底部
    m_eventLogText->moveCursor(QTextCursor::End);
}