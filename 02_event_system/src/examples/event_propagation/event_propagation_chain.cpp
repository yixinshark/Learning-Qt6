#include "event_propagation_chain.h"
#include <QPainter>
#include <QApplication>
#include <QDebug>
#include <QHeaderView>
#include <QEnterEvent>

EventPropagationChain::EventPropagationChain(QWidget *parent)
    : QWidget(parent)
    , m_visualizationEnabled(true)
    , m_highlightTimer(new QTimer(this))
    , m_highlightAnimation(new QPropertyAnimation(this))
{
    setWindowTitle("Qt事件传播链演示");
    resize(1000, 700);
    setupUI();
    createHierarchy();
    updateHierarchyTree();
}

void EventPropagationChain::setupUI()
{
    m_mainLayout = new QHBoxLayout(this);
    m_splitter = new QSplitter(Qt::Horizontal, this);
    
    // 左侧层次结构显示
    m_hierarchyGroup = new QGroupBox("对象层次结构", this);
    m_hierarchyLayout = new QVBoxLayout(m_hierarchyGroup);
    
    m_hierarchyTree = new QTreeWidget(this);
    m_hierarchyTree->setHeaderLabels({"控件名称", "层级", "状态"});
    m_hierarchyTree->header()->setStretchLastSection(false);
    m_hierarchyTree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_hierarchyTree->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_hierarchyTree->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    
    m_resetButton = new QPushButton("重置演示", this);
    connect(m_resetButton, &QPushButton::clicked, this, &EventPropagationChain::onResetDemo);
    
    m_visualizeButton = new QPushButton("启用可视化", this);
    m_visualizeButton->setCheckable(true);
    m_visualizeButton->setChecked(true);
    connect(m_visualizeButton, &QPushButton::toggled, this, &EventPropagationChain::onToggleVisualization);
    
    m_hierarchyLayout->addWidget(m_hierarchyTree);
    m_hierarchyLayout->addWidget(m_resetButton);
    m_hierarchyLayout->addWidget(m_visualizeButton);
    
    // 中间交互区域
    m_interactionGroup = new QGroupBox("交互区域 - 多层级事件传播", this);
    m_interactionLayout = new QVBoxLayout(m_interactionGroup);
    
    // 右侧日志区域
    m_logGroup = new QGroupBox("事件传播日志", this);
    m_logLayout = new QVBoxLayout(m_logGroup);
    
    m_logDisplay = new QTextEdit(this);
    m_logDisplay->setReadOnly(true);
    m_logDisplay->setFont(QFont("Consolas", 9));
    
    m_clearLogButton = new QPushButton("清空日志", this);
    connect(m_clearLogButton, &QPushButton::clicked, this, &EventPropagationChain::onClearLog);
    
    m_logLayout->addWidget(m_logDisplay);
    m_logLayout->addWidget(m_clearLogButton);
    
    // 组装布局
    m_splitter->addWidget(m_hierarchyGroup);
    m_splitter->addWidget(m_interactionGroup);
    m_splitter->addWidget(m_logGroup);
    m_splitter->setSizes({250, 400, 350});
    
    m_mainLayout->addWidget(m_splitter);
    
    // 初始化日志
    logPropagationStep("系统初始化", "事件传播链演示已启动");
}

void EventPropagationChain::createHierarchy()
{
    // 创建根控件
    m_rootWidget = new QWidget(m_interactionGroup);
    m_rootWidget->setMinimumSize(350, 500);
    m_rootWidget->setStyleSheet("background-color: #f0f0f0; border: 2px solid #333;");
    m_interactionLayout->addWidget(m_rootWidget);
    
    // 创建多层级的控件链
    m_chainWidgets.clear();
    
    // 第1层 - 容器控件
    ChainWidget *level1 = new ChainWidget("容器控件", 1, m_rootWidget);
    level1->setGeometry(25, 25, 300, 450);
    level1->setStyleSheet("background-color: #e6f3ff; border: 2px solid #0066cc;");
    m_chainWidgets.append(level1);
    
    // 第2层 - 面板控件
    ChainWidget *level2 = new ChainWidget("面板控件", 2, level1);
    level2->setGeometry(25, 25, 250, 400);
    level2->setStyleSheet("background-color: #fff2e6; border: 2px solid #ff6600;");
    m_chainWidgets.append(level2);
    
    // 第3层 - 组控件
    ChainWidget *level3 = new ChainWidget("组控件", 3, level2);
    level3->setGeometry(25, 25, 200, 350);
    level3->setStyleSheet("background-color: #e6ffe6; border: 2px solid #00cc00;");
    m_chainWidgets.append(level3);
    
    // 第4层 - 内容控件
    ChainWidget *level4 = new ChainWidget("内容控件", 4, level3);
    level4->setGeometry(25, 25, 150, 300);
    level4->setStyleSheet("background-color: #ffe6f3; border: 2px solid #cc0066;");
    m_chainWidgets.append(level4);
    
    // 第5层 - 按钮控件
    ChainWidget *level5 = new ChainWidget("按钮控件", 5, level4);
    level5->setGeometry(25, 25, 100, 250);
    level5->setStyleSheet("background-color: #f3e6ff; border: 2px solid #6600cc;");
    m_chainWidgets.append(level5);
    
    // 第6层 - 最内层控件
    ChainWidget *level6 = new ChainWidget("核心控件", 6, level5);
    level6->setGeometry(10, 10, 80, 230);
    level6->setStyleSheet("background-color: #ffffe6; border: 2px solid #cccc00;");
    m_chainWidgets.append(level6);
    
    // 连接所有控件的事件信号
    for (ChainWidget *widget : m_chainWidgets) {
        connect(widget, &ChainWidget::eventPropagated,
                [this, widget](const QString &name, int level, const QString &eventType, 
                       const QString &details, bool accepted, bool willPropagate) {
                    QString propagationInfo = QString("Level %1: %2").arg(level).arg(name);
                    QString fullDetails = QString("%1 | %2 | 接受: %3 | 继续传播: %4")
                                        .arg(eventType)
                                        .arg(details)
                                        .arg(accepted ? "是" : "否")
                                        .arg(willPropagate ? "是" : "否");
                    logPropagationStep(propagationInfo, fullDetails);
                    
                    // 可视化传播路径
                    if (m_visualizationEnabled) {
                        widget->highlightWidget(true);
                        QTimer::singleShot(500, [widget]() {
                            widget->highlightWidget(false);
                        });
                    }
                });
    }
}

void EventPropagationChain::updateHierarchyTree()
{
    m_hierarchyTree->clear();
    
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(m_hierarchyTree);
    rootItem->setText(0, "EventPropagationChain (根)");
    rootItem->setText(1, "0");
    rootItem->setText(2, "活动");
    rootItem->setExpanded(true);
    
    QTreeWidgetItem *parentItem = rootItem;
    
    for (int i = 0; i < m_chainWidgets.size(); ++i) {
        ChainWidget *widget = m_chainWidgets[i];
        QTreeWidgetItem *item = new QTreeWidgetItem(parentItem);
        item->setText(0, widget->widgetName());
        item->setText(1, QString::number(widget->hierarchyLevel()));
        item->setText(2, widget->acceptsEvents() ? "接受事件" : "忽略事件");
        item->setExpanded(true);
        
        // 设置颜色标识
        QColor levelColor;
        switch (widget->hierarchyLevel()) {
        case 1: levelColor = QColor(230, 243, 255); break;
        case 2: levelColor = QColor(255, 242, 230); break;
        case 3: levelColor = QColor(230, 255, 230); break;
        case 4: levelColor = QColor(255, 230, 243); break;
        case 5: levelColor = QColor(243, 230, 255); break;
        case 6: levelColor = QColor(255, 255, 230); break;
        default: levelColor = Qt::white; break;
        }
        
        for (int col = 0; col < 3; ++col) {
            item->setBackground(col, QBrush(levelColor));
        }
        
        parentItem = item;
    }
}

void EventPropagationChain::highlightPropagationPath(const QStringList &path)
{
    // 在层次树中高亮显示传播路径
    QTreeWidgetItemIterator it(m_hierarchyTree);
    while (*it) {
        QTreeWidgetItem *item = *it;
        if (path.contains(item->text(0))) {
            for (int col = 0; col < 3; ++col) {
                item->setBackground(col, QBrush(Qt::yellow));
            }
        }
        ++it;
    }
    
    // 延迟恢复原色
    QTimer::singleShot(1000, [this]() {
        updateHierarchyTree();
    });
}

void EventPropagationChain::logPropagationStep(const QString &step, const QString &details)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    QString logEntry = QString("[%1] %2\n    → %3\n")
                      .arg(timestamp)
                      .arg(step)
                      .arg(details);
    
    m_logDisplay->append(logEntry);
    
    // 自动滚动到底部
    QTextCursor cursor = m_logDisplay->textCursor();
    cursor.movePosition(QTextCursor::End);
    m_logDisplay->setTextCursor(cursor);
}

void EventPropagationChain::onResetDemo()
{
    // 重置所有控件状态
    for (ChainWidget *widget : m_chainWidgets) {
        widget->setAcceptEvents(true);
        widget->highlightWidget(false);
    }
    
    updateHierarchyTree();
    logPropagationStep("系统重置", "所有控件状态已重置，准备新的演示");
}

void EventPropagationChain::onToggleVisualization()
{
    m_visualizationEnabled = m_visualizeButton->isChecked();
    m_visualizeButton->setText(m_visualizationEnabled ? "禁用可视化" : "启用可视化");
    
    logPropagationStep("可视化设置", 
                      QString("事件传播可视化已%1").arg(m_visualizationEnabled ? "启用" : "禁用"));
}

void EventPropagationChain::onClearLog()
{
    m_logDisplay->clear();
    logPropagationStep("日志清空", "事件传播日志已清空");
}

// ChainWidget 实现
ChainWidget::ChainWidget(const QString &name, int level, QWidget *parent)
    : QWidget(parent)
    , m_name(name)
    , m_level(level)
    , m_acceptEvents(true)
    , m_isHighlighted(false)
    , m_baseColor(Qt::lightGray)
    , m_highlightColor(Qt::yellow)
    , m_currentColor(m_baseColor)
{
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_Hover, true);
    
    // 设置阴影效果
    m_shadowEffect = new QGraphicsDropShadowEffect(this);
    m_shadowEffect->setBlurRadius(5);
    m_shadowEffect->setOffset(2, 2);
    m_shadowEffect->setColor(QColor(0, 0, 0, 100));
    setGraphicsEffect(m_shadowEffect);
    
    // 设置颜色动画
    m_colorAnimation = new QPropertyAnimation(this, "color");
    m_colorAnimation->setDuration(300);
}

bool ChainWidget::event(QEvent *event)
{
    QString eventType;
    QString details;
    bool shouldLog = false;
    bool willPropagate = !m_acceptEvents;
    
    switch (event->type()) {
    case QEvent::MouseButtonPress: {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        eventType = "鼠标按下";
        details = QString("按钮: %1, 位置: (%2, %3)")
                 .arg(mouseEvent->button())
                 .arg(mouseEvent->pos().x())
                 .arg(mouseEvent->pos().y());
        shouldLog = true;
        break;
    }
    case QEvent::KeyPress: {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        eventType = "按键按下";
        details = QString("按键: '%1', 键码: %2, 修饰键: %3")
                 .arg(keyEvent->text())
                 .arg(keyEvent->key())
                 .arg(keyEvent->modifiers());
        shouldLog = true;
        break;
    }
    case QEvent::Enter:
        eventType = "鼠标进入";
        details = "鼠标指针进入控件区域";
        shouldLog = true;
        break;
    case QEvent::Leave:
        eventType = "鼠标离开";
        details = "鼠标指针离开控件区域";
        shouldLog = true;
        break;
    default:
        break;
    }
    
    if (shouldLog) {
        emit eventPropagated(m_name, m_level, eventType, details, m_acceptEvents, willPropagate);
        
        if (!m_acceptEvents) {
            event->ignore();
            return false;
        } else {
            event->accept();
        }
    }
    
    return QWidget::event(event);
}

void ChainWidget::mousePressEvent(QMouseEvent *event)
{
    QString details = QString("鼠标事件处理 - 按钮: %1, 本地坐标: (%2, %3), 全局坐标: (%4, %5)")
                     .arg(event->button())
                     .arg(event->pos().x())
                     .arg(event->pos().y())
                     .arg(event->globalPosition().toPoint().x())
                     .arg(event->globalPosition().toPoint().y());
    
    emit eventPropagated(m_name, m_level, "鼠标事件处理", details, m_acceptEvents, !m_acceptEvents);
    
    if (m_acceptEvents) {
        event->accept();
    } else {
        event->ignore();
    }
}

void ChainWidget::keyPressEvent(QKeyEvent *event)
{
    QString details = QString("键盘事件处理 - 按键: '%1', 键码: %2, 修饰键: %3")
                     .arg(event->text())
                     .arg(event->key())
                     .arg(event->modifiers());
    
    emit eventPropagated(m_name, m_level, "键盘事件处理", details, m_acceptEvents, !m_acceptEvents);
    
    if (m_acceptEvents) {
        event->accept();
    } else {
        event->ignore();
    }
}

void ChainWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 绘制背景
    QColor bgColor = m_isHighlighted ? m_highlightColor : m_currentColor;
    painter.fillRect(rect(), bgColor);
    
    // 绘制控件信息
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    
    QString displayText = QString("%1\n(Level %2)").arg(m_name).arg(m_level);
    painter.drawText(rect().adjusted(5, 5, -5, -5), Qt::AlignTop | Qt::AlignLeft, displayText);
    
    // 绘制状态信息
    painter.setFont(QFont("Arial", 8));
    QString statusText = QString("事件: %1").arg(m_acceptEvents ? "接受" : "忽略");
    painter.drawText(rect().adjusted(5, 5, -5, -5), Qt::AlignBottom | Qt::AlignLeft, statusText);
    
    // 绘制层级指示器
    painter.setPen(QPen(Qt::darkGray, 1));
    for (int i = 0; i < m_level; ++i) {
        painter.drawRect(rect().adjusted(i*2, i*2, -i*2, -i*2));
    }
    
    QWidget::paintEvent(event);
}

void ChainWidget::enterEvent(QEnterEvent *event)
{
    m_currentColor = m_highlightColor.lighter(150);
    update();
    QWidget::enterEvent(event);
}

void ChainWidget::leaveEvent(QEvent *event)
{
    m_currentColor = m_baseColor;
    update();
    QWidget::leaveEvent(event);
}

void ChainWidget::highlightWidget(bool highlight)
{
    m_isHighlighted = highlight;
    update();
    
    if (highlight) {
        animateHighlight();
    }
}

void ChainWidget::setHighlightColor(const QColor &color)
{
    m_highlightColor = color;
    if (m_isHighlighted) {
        update();
    }
}

QString ChainWidget::getEventDescription(QEvent *event)
{
    switch (event->type()) {
    case QEvent::MouseButtonPress:
        return "鼠标按下事件";
    case QEvent::MouseButtonRelease:
        return "鼠标释放事件";
    case QEvent::KeyPress:
        return "按键按下事件";
    case QEvent::KeyRelease:
        return "按键释放事件";
    case QEvent::Enter:
        return "鼠标进入事件";
    case QEvent::Leave:
        return "鼠标离开事件";
    default:
        return QString("其他事件 (类型: %1)").arg(event->type());
    }
}

void ChainWidget::animateHighlight()
{
    // 创建高亮动画效果
    m_shadowEffect->setColor(QColor(255, 255, 0, 200));
    m_shadowEffect->setBlurRadius(15);
    
    QTimer::singleShot(300, [this]() {
        m_shadowEffect->setColor(QColor(0, 0, 0, 100));
        m_shadowEffect->setBlurRadius(5);
    });
}

// #include "event_propagation_chain.moc" // 移除不需要的moc包含