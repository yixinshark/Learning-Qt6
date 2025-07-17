#include "event_type_demo.h"
#include <QHeaderView>
#include <QApplication>
#include <QDateTime>
#include <QScrollBar>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QFocusEvent>
#include <QEnterEvent>

EventTypeDemo::EventTypeDemo(QWidget *parent)
    : QWidget(parent)
    , m_mainLayout(nullptr)
    , m_titleLabel(nullptr)
    , m_eventTypeCombo(nullptr)
    , m_eventPropertiesTable(nullptr)
    , m_eventDescription(nullptr)
    , m_simulateButton(nullptr)
    , m_clearButton(nullptr)
    , m_eventLog(nullptr)
    , m_statusLabel(nullptr)
    , m_statusTimer(new QTimer(this))
    , m_eventCounter(0)
{
    setupUI();
    initializeEventTypes();
    
    // 设置定时器
    m_statusTimer->setSingleShot(true);
    connect(m_statusTimer, &QTimer::timeout, [this]() {
        m_statusLabel->setText("状态: 等待操作...");
    });
    
    // 设置焦点策略
    setFocusPolicy(Qt::StrongFocus);
}

void EventTypeDemo::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    
    // 标题
    m_titleLabel = new QLabel("事件类型演示", this);
    m_titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #2c3e50; padding: 10px;");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    
    // 事件类型选择
    auto *selectionLayout = new QHBoxLayout();
    selectionLayout->addWidget(new QLabel("选择事件类型:", this));
    
    m_eventTypeCombo = new QComboBox(this);
    m_eventTypeCombo->setMinimumWidth(200);
    connect(m_eventTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &EventTypeDemo::onEventTypeSelected);
    
    m_simulateButton = new QPushButton("模拟事件", this);
    m_simulateButton->setStyleSheet("QPushButton { background-color: #3498db; color: white; padding: 8px; border-radius: 4px; }");
    connect(m_simulateButton, &QPushButton::clicked, this, &EventTypeDemo::onSimulateEvent);
    
    selectionLayout->addWidget(m_eventTypeCombo);
    selectionLayout->addWidget(m_simulateButton);
    selectionLayout->addStretch();
    
    // 事件属性表格
    m_eventPropertiesTable = new QTableWidget(this);
    m_eventPropertiesTable->setColumnCount(2);
    m_eventPropertiesTable->setHorizontalHeaderLabels({"属性", "值/描述"});
    m_eventPropertiesTable->horizontalHeader()->setStretchLastSection(true);
    m_eventPropertiesTable->setMaximumHeight(200);
    m_eventPropertiesTable->setAlternatingRowColors(true);
    
    // 事件描述
    m_eventDescription = new QTextEdit(this);
    m_eventDescription->setReadOnly(true);
    m_eventDescription->setMaximumHeight(120);
    m_eventDescription->setStyleSheet("background-color: #f8f9fa; border: 1px solid #dee2e6; border-radius: 4px;");
    
    // 控制按钮
    auto *controlLayout = new QHBoxLayout();
    m_clearButton = new QPushButton("清空日志", this);
    m_clearButton->setStyleSheet("QPushButton { background-color: #6c757d; color: white; padding: 8px; border-radius: 4px; }");
    connect(m_clearButton, &QPushButton::clicked, this, &EventTypeDemo::onClearDetails);
    
    controlLayout->addWidget(m_clearButton);
    controlLayout->addStretch();
    
    // 事件日志
    m_eventLog = new QTextEdit(this);
    m_eventLog->setReadOnly(true);
    m_eventLog->setMaximumHeight(200);
    m_eventLog->setStyleSheet("font-family: 'Courier New', monospace; font-size: 12px;");
    
    // 状态标签
    m_statusLabel = new QLabel("状态: 等待操作...", this);
    m_statusLabel->setStyleSheet("color: #28a745; font-weight: bold; padding: 5px;");
    
    // 添加到主布局
    m_mainLayout->addWidget(m_titleLabel);
    m_mainLayout->addLayout(selectionLayout);
    m_mainLayout->addWidget(new QLabel("事件属性:", this));
    m_mainLayout->addWidget(m_eventPropertiesTable);
    m_mainLayout->addWidget(new QLabel("事件描述:", this));
    m_mainLayout->addWidget(m_eventDescription);
    m_mainLayout->addLayout(controlLayout);
    m_mainLayout->addWidget(new QLabel("事件日志:", this));
    m_mainLayout->addWidget(m_eventLog);
    m_mainLayout->addWidget(m_statusLabel);
    
    setLayout(m_mainLayout);
    setMinimumSize(700, 600);
}

void EventTypeDemo::initializeEventTypes()
{
    // 鼠标事件
    m_eventTypes[QEvent::MouseButtonPress] = {
        QEvent::MouseButtonPress, "MouseButtonPress", "鼠标事件",
        "当鼠标按钮被按下时触发。包含按钮信息、位置坐标和修饰键状态。",
        {"按钮类型", "位置坐标", "修饰键", "全局坐标"}, true
    };
    
    m_eventTypes[QEvent::MouseButtonRelease] = {
        QEvent::MouseButtonRelease, "MouseButtonRelease", "鼠标事件",
        "当鼠标按钮被释放时触发。包含按钮信息和位置坐标。",
        {"按钮类型", "位置坐标", "修饰键", "全局坐标"}, true
    };
    
    m_eventTypes[QEvent::MouseMove] = {
        QEvent::MouseMove, "MouseMove", "鼠标事件",
        "当鼠标在控件上移动时触发。包含当前位置和按钮状态。",
        {"位置坐标", "按钮状态", "修饰键", "全局坐标"}, false
    };
    
    // 键盘事件
    m_eventTypes[QEvent::KeyPress] = {
        QEvent::KeyPress, "KeyPress", "键盘事件",
        "当键盘按键被按下时触发。包含按键码、文本和修饰键信息。",
        {"按键码", "文本内容", "修饰键", "自动重复"}, true
    };
    
    m_eventTypes[QEvent::KeyRelease] = {
        QEvent::KeyRelease, "KeyRelease", "键盘事件",
        "当键盘按键被释放时触发。包含按键码和修饰键信息。",
        {"按键码", "文本内容", "修饰键", "自动重复"}, true
    };
    
    // 焦点事件
    m_eventTypes[QEvent::FocusIn] = {
        QEvent::FocusIn, "FocusIn", "焦点事件",
        "当控件获得键盘焦点时触发。包含焦点获得的原因。",
        {"焦点原因", "前一个焦点控件"}, true
    };
    
    m_eventTypes[QEvent::FocusOut] = {
        QEvent::FocusOut, "FocusOut", "焦点事件",
        "当控件失去键盘焦点时触发。包含焦点失去的原因。",
        {"焦点原因", "下一个焦点控件"}, true
    };
    
    // 绘制事件
    m_eventTypes[QEvent::Paint] = {
        QEvent::Paint, "Paint", "绘制事件",
        "当控件需要重新绘制时触发。包含需要更新的区域信息。",
        {"更新区域", "绘制设备", "绘制引擎"}, false
    };
    
    // 几何事件
    m_eventTypes[QEvent::Resize] = {
        QEvent::Resize, "Resize", "几何事件",
        "当控件大小发生变化时触发。包含新旧尺寸信息。",
        {"新尺寸", "旧尺寸", "尺寸变化"}, true
    };
    
    m_eventTypes[QEvent::Move] = {
        QEvent::Move, "Move", "几何事件",
        "当控件位置发生变化时触发。包含新旧位置信息。",
        {"新位置", "旧位置", "位置变化"}, false
    };
    
    // 鼠标进入/离开事件
    m_eventTypes[QEvent::Enter] = {
        QEvent::Enter, "Enter", "鼠标事件",
        "当鼠标指针进入控件区域时触发。",
        {"进入位置", "相对坐标", "全局坐标"}, false
    };
    
    m_eventTypes[QEvent::Leave] = {
        QEvent::Leave, "Leave", "鼠标事件",
        "当鼠标指针离开控件区域时触发。",
        {"离开时间", "控件状态"}, false
    };
    
    // 显示/隐藏事件
    m_eventTypes[QEvent::Show] = {
        QEvent::Show, "Show", "控件事件",
        "当控件变为可见时触发。在控件首次显示或从隐藏状态恢复时发生。",
        {"显示状态", "父控件状态", "窗口状态"}, true
    };
    
    m_eventTypes[QEvent::Hide] = {
        QEvent::Hide, "Hide", "控件事件",
        "当控件变为不可见时触发。在控件被隐藏时发生。",
        {"隐藏状态", "父控件状态", "窗口状态"}, true
    };
    
    // 自定义事件
    m_eventTypes[QEvent::User] = {
        QEvent::User, "User", "自定义事件",
        "用户定义的自定义事件类型。可以携带自定义数据。",
        {"事件类型ID", "自定义数据", "发送者", "接收者"}, true
    };
    
    // 填充下拉框
    for (auto it = m_eventTypes.begin(); it != m_eventTypes.end(); ++it) {
        QString displayText = QString("%1 (%2)").arg(it->name).arg(it->category);
        m_eventTypeCombo->addItem(displayText, static_cast<int>(it->type));
    }
    
    // 选择第一个项目
    if (m_eventTypeCombo->count() > 0) {
        m_eventTypeCombo->setCurrentIndex(0);
        onEventTypeSelected();
    }
}

void EventTypeDemo::onEventTypeSelected()
{
    int currentData = m_eventTypeCombo->currentData().toInt();
    QEvent::Type selectedType = static_cast<QEvent::Type>(currentData);
    
    updateEventDetails(selectedType);
    
    // 更新模拟按钮状态
    bool canSimulate = m_eventTypes[selectedType].canSimulate;
    m_simulateButton->setEnabled(canSimulate);
    m_simulateButton->setText(canSimulate ? "模拟事件" : "无法模拟");
    
    m_statusLabel->setText(QString("已选择: %1").arg(m_eventTypes[selectedType].name));
    m_statusTimer->start(3000);
}

void EventTypeDemo::updateEventDetails(QEvent::Type eventType)
{
    const EventTypeInfo &info = m_eventTypes[eventType];
    
    // 更新属性表格
    m_eventPropertiesTable->setRowCount(info.properties.size() + 3);
    
    // 基本信息
    m_eventPropertiesTable->setItem(0, 0, new QTableWidgetItem("事件类型"));
    m_eventPropertiesTable->setItem(0, 1, new QTableWidgetItem(info.name));
    
    m_eventPropertiesTable->setItem(1, 0, new QTableWidgetItem("事件分类"));
    m_eventPropertiesTable->setItem(1, 1, new QTableWidgetItem(info.category));
    
    m_eventPropertiesTable->setItem(2, 0, new QTableWidgetItem("类型枚举值"));
    m_eventPropertiesTable->setItem(2, 1, new QTableWidgetItem(QString::number(static_cast<int>(eventType))));
    
    // 特定属性
    for (int i = 0; i < info.properties.size(); ++i) {
        m_eventPropertiesTable->setItem(i + 3, 0, new QTableWidgetItem(info.properties[i]));
        m_eventPropertiesTable->setItem(i + 3, 1, new QTableWidgetItem("(运行时获取)"));
    }
    
    // 调整列宽
    m_eventPropertiesTable->resizeColumnsToContents();
    
    // 更新描述
    m_eventDescription->setText(info.description);
}

void EventTypeDemo::onSimulateEvent()
{
    simulateSelectedEvent();
}

void EventTypeDemo::simulateSelectedEvent()
{
    int currentData = m_eventTypeCombo->currentData().toInt();
    QEvent::Type selectedType = static_cast<QEvent::Type>(currentData);
    
    QEvent *event = nullptr;
    
    switch (selectedType) {
        case QEvent::MouseButtonPress: {
            QMouseEvent *mouseEvent = new QMouseEvent(
                QEvent::MouseButtonPress,
                QPointF(100, 100),
                QPointF(200, 200),
                Qt::LeftButton,
                Qt::LeftButton,
                Qt::NoModifier
            );
            event = mouseEvent;
            break;
        }
        case QEvent::MouseButtonRelease: {
            QMouseEvent *mouseEvent = new QMouseEvent(
                QEvent::MouseButtonRelease,
                QPointF(100, 100),
                QPointF(200, 200),
                Qt::LeftButton,
                Qt::NoButton,
                Qt::NoModifier
            );
            event = mouseEvent;
            break;
        }
        case QEvent::KeyPress: {
            QKeyEvent *keyEvent = new QKeyEvent(
                QEvent::KeyPress,
                Qt::Key_A,
                Qt::NoModifier,
                "a"
            );
            event = keyEvent;
            break;
        }
        case QEvent::KeyRelease: {
            QKeyEvent *keyEvent = new QKeyEvent(
                QEvent::KeyRelease,
                Qt::Key_A,
                Qt::NoModifier,
                "a"
            );
            event = keyEvent;
            break;
        }
        case QEvent::FocusIn: {
            QFocusEvent *focusEvent = new QFocusEvent(
                QEvent::FocusIn,
                Qt::OtherFocusReason
            );
            event = focusEvent;
            break;
        }
        case QEvent::FocusOut: {
            QFocusEvent *focusEvent = new QFocusEvent(
                QEvent::FocusOut,
                Qt::OtherFocusReason
            );
            event = focusEvent;
            break;
        }
        case QEvent::Resize: {
            QResizeEvent *resizeEvent = new QResizeEvent(
                QSize(400, 300),
                QSize(350, 250)
            );
            event = resizeEvent;
            break;
        }
        case QEvent::Show:
            event = new QEvent(QEvent::Show);
            break;
        case QEvent::Hide:
            event = new QEvent(QEvent::Hide);
            break;
        case QEvent::User:
            event = new QEvent(QEvent::User);
            break;
        default:
            return;
    }
    
    if (event) {
        QApplication::postEvent(this, event);
        
        QString logEntry = QString("[模拟] %1 事件已发送")
                          .arg(m_eventTypes[selectedType].name);
        logEventOccurrence(nullptr, logEntry);
        
        m_statusLabel->setText("模拟事件已发送");
        m_statusTimer->start(2000);
    }
}

bool EventTypeDemo::event(QEvent *event)
{
    // 记录接收到的事件
    if (m_eventTypes.contains(event->type())) {
        logEventOccurrence(event);
    }
    
    return QWidget::event(event);
}

void EventTypeDemo::logEventOccurrence(QEvent *event, const QString &customMessage)
{
    m_eventCounter++;
    QString logEntry;
    
    if (!customMessage.isEmpty()) {
        logEntry = QString("[%1] %2")
                  .arg(m_eventCounter, 3, 10, QChar('0'))
                  .arg(customMessage);
    } else if (event && m_eventTypes.contains(event->type())) {
        const EventTypeInfo &info = m_eventTypes[event->type()];
        logEntry = QString("[%1] [%2] 接收到 %3 事件")
                  .arg(m_eventCounter, 3, 10, QChar('0'))
                  .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"))
                  .arg(info.name);
        
        // 添加事件特定信息
        switch (event->type()) {
            case QEvent::MouseButtonPress:
            case QEvent::MouseButtonRelease: {
                QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
                logEntry += QString(" - 按钮: %1, 位置: (%2, %3)")
                           .arg(mouseEvent->button())
                           .arg(mouseEvent->pos().x())
                           .arg(mouseEvent->pos().y());
                break;
            }
            case QEvent::KeyPress:
            case QEvent::KeyRelease: {
                QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
                logEntry += QString(" - 键: %1, 文本: '%2'")
                           .arg(keyEvent->key())
                           .arg(keyEvent->text());
                break;
            }
            case QEvent::Resize: {
                QResizeEvent *resizeEvent = static_cast<QResizeEvent*>(event);
                logEntry += QString(" - 尺寸: %1x%2")
                           .arg(resizeEvent->size().width())
                           .arg(resizeEvent->size().height());
                break;
            }
            default:
                break;
        }
    }
    
    if (!logEntry.isEmpty()) {
        m_eventLog->append(logEntry);
        
        // 自动滚动到底部
        QScrollBar *scrollBar = m_eventLog->verticalScrollBar();
        scrollBar->setValue(scrollBar->maximum());
    }
}

void EventTypeDemo::onClearDetails()
{
    m_eventLog->clear();
    m_eventCounter = 0;
    
    QString logEntry = QString("[%1] 日志已清空")
                      .arg(++m_eventCounter, 3, 10, QChar('0'));
    m_eventLog->append(logEntry);
    
    m_statusLabel->setText("日志已清空");
    m_statusTimer->start(2000);
}