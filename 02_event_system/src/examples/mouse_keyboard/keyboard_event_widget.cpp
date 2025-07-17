#include "keyboard_event_widget.h"
#include <QApplication>
#include <QDebug>
#include <QDateTime>

KeyboardEventWidget::KeyboardEventWidget(QWidget *parent)
    : QWidget(parent)
    , m_mainLayout(nullptr)
    , m_infoGroup(nullptr)
    , m_inputGroup(nullptr)
    , m_historyGroup(nullptr)
    , m_eventTypeLabel(nullptr)
    , m_keyCodeLabel(nullptr)
    , m_keyTextLabel(nullptr)
    , m_modifiersLabel(nullptr)
    , m_nativeKeyLabel(nullptr)
    , m_autoRepeatLabel(nullptr)
    , m_countLabel(nullptr)
    , m_inputArea(nullptr)
    , m_captureCheckBox(nullptr)
    , m_inputStatsLabel(nullptr)
    , m_keyHistoryList(nullptr)
    , m_clearHistoryButton(nullptr)
    , m_keyPressCount(0)
    , m_keyReleaseCount(0)
    , m_totalCharCount(0)
    , m_captureEnabled(false)
{
    setupUI();
    
    // 设置焦点策略，确保能接收键盘事件
    setFocusPolicy(Qt::StrongFocus);
    
    // 为输入区域安装事件过滤器
    m_inputArea->installEventFilter(this);
}

void KeyboardEventWidget::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    
    // 创建信息显示组
    m_infoGroup = new QGroupBox("键盘事件信息", this);
    QVBoxLayout *infoLayout = new QVBoxLayout(m_infoGroup);
    
    m_eventTypeLabel = new QLabel("事件类型: 无", this);
    m_keyCodeLabel = new QLabel("按键码: 0", this);
    m_keyTextLabel = new QLabel("按键文本: 无", this);
    m_modifiersLabel = new QLabel("修饰键: 无", this);
    m_nativeKeyLabel = new QLabel("原生按键码: 0", this);
    m_autoRepeatLabel = new QLabel("自动重复: 否", this);
    m_countLabel = new QLabel("统计: 按下0次, 释放0次", this);
    
    infoLayout->addWidget(m_eventTypeLabel);
    infoLayout->addWidget(m_keyCodeLabel);
    infoLayout->addWidget(m_keyTextLabel);
    infoLayout->addWidget(m_modifiersLabel);
    infoLayout->addWidget(m_nativeKeyLabel);
    infoLayout->addWidget(m_autoRepeatLabel);
    infoLayout->addWidget(m_countLabel);
    
    // 创建输入测试组
    m_inputGroup = new QGroupBox("键盘输入测试", this);
    QVBoxLayout *inputLayout = new QVBoxLayout(m_inputGroup);
    
    m_captureCheckBox = new QCheckBox("启用按键捕获（阻止正常输入）", this);
    connect(m_captureCheckBox, &QCheckBox::toggled, this, &KeyboardEventWidget::toggleKeyCapture);
    
    m_inputArea = new QTextEdit(this);
    m_inputArea->setPlaceholderText("在此处输入文本测试键盘事件...\n支持的快捷键:\nCtrl+S: 保存快捷键\nCtrl+C: 复制快捷键\nCtrl+V: 粘贴快捷键\nEsc: 清除输入");
    m_inputArea->setMaximumHeight(100);
    connect(m_inputArea, &QTextEdit::textChanged, this, &KeyboardEventWidget::onTextChanged);
    
    m_inputStatsLabel = new QLabel("输入统计: 字符数0", this);
    
    inputLayout->addWidget(m_captureCheckBox);
    inputLayout->addWidget(m_inputArea);
    inputLayout->addWidget(m_inputStatsLabel);
    
    // 创建历史记录组
    m_historyGroup = new QGroupBox("按键历史记录", this);
    QVBoxLayout *historyLayout = new QVBoxLayout(m_historyGroup);
    
    m_keyHistoryList = new QListWidget(this);
    m_keyHistoryList->setMaximumHeight(120);
    
    m_clearHistoryButton = new QPushButton("清除历史", this);
    connect(m_clearHistoryButton, &QPushButton::clicked, this, &KeyboardEventWidget::clearKeyHistory);
    
    historyLayout->addWidget(m_keyHistoryList);
    historyLayout->addWidget(m_clearHistoryButton);
    
    m_mainLayout->addWidget(m_infoGroup);
    m_mainLayout->addWidget(m_inputGroup);
    m_mainLayout->addWidget(m_historyGroup);
    
    // 设置初始焦点
    setFocus();
}

void KeyboardEventWidget::keyPressEvent(QKeyEvent *event)
{
    m_keyPressCount++;
    m_pressedKeys.insert(event->key());
    
    updateKeyInfo("按键按下", event);
    addKeyToHistory("按下", event);
    
    // 检查快捷键
    if (event->modifiers() & Qt::ControlModifier) {
        switch (event->key()) {
            case Qt::Key_S:
                emit shortcutTriggered("Ctrl+S (保存)");
                event->accept();
                return;
            case Qt::Key_C:
                emit shortcutTriggered("Ctrl+C (复制)");
                break;
            case Qt::Key_V:
                emit shortcutTriggered("Ctrl+V (粘贴)");
                break;
        }
    }
    
    // 特殊按键处理
    if (event->key() == Qt::Key_Escape) {
        m_inputArea->clear();
        emit shortcutTriggered("Esc (清除输入)");
        event->accept();
        return;
    }
    
    emit keyEventOccurred("KeyPress", event->key(), event->text());
    
    // 如果启用了按键捕获，则不传递给基类
    if (m_captureEnabled) {
        event->accept();
    } else {
        QWidget::keyPressEvent(event);
    }
}

void KeyboardEventWidget::keyReleaseEvent(QKeyEvent *event)
{
    m_keyReleaseCount++;
    m_pressedKeys.remove(event->key());
    
    updateKeyInfo("按键释放", event);
    addKeyToHistory("释放", event);
    
    emit keyEventOccurred("KeyRelease", event->key(), event->text());
    
    if (m_captureEnabled) {
        event->accept();
    } else {
        QWidget::keyReleaseEvent(event);
    }
}

bool KeyboardEventWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_inputArea && (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease)) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        
        if (event->type() == QEvent::KeyPress) {
            keyPressEvent(keyEvent);
        } else {
            keyReleaseEvent(keyEvent);
        }
        
        // 如果启用了捕获，阻止事件传递给输入区域
        return m_captureEnabled;
    }
    
    return QWidget::eventFilter(obj, event);
}

void KeyboardEventWidget::clearKeyHistory()
{
    m_keyHistoryList->clear();
    m_keyPressCount = 0;
    m_keyReleaseCount = 0;
    m_countLabel->setText("统计: 按下0次, 释放0次");
}

void KeyboardEventWidget::toggleKeyCapture(bool enabled)
{
    m_captureEnabled = enabled;
    if (enabled) {
        m_inputArea->setPlaceholderText("按键捕获已启用 - 按键将被拦截，不会显示在输入框中");
    } else {
        m_inputArea->setPlaceholderText("在此处输入文本测试键盘事件...\n支持的快捷键:\nCtrl+S: 保存快捷键\nCtrl+C: 复制快捷键\nCtrl+V: 粘贴快捷键\nEsc: 清除输入");
    }
}

void KeyboardEventWidget::onTextChanged()
{
    m_totalCharCount = m_inputArea->toPlainText().length();
    m_inputStatsLabel->setText(QString("输入统计: 字符数%1").arg(m_totalCharCount));
}

void KeyboardEventWidget::updateKeyInfo(const QString &eventType, QKeyEvent *event)
{
    m_eventTypeLabel->setText(QString("事件类型: %1").arg(eventType));
    m_keyCodeLabel->setText(QString("按键码: %1 (0x%2)").arg(event->key()).arg(event->key(), 0, 16));
    m_keyTextLabel->setText(QString("按键文本: \"%1\" (%2)")
                           .arg(event->text().isEmpty() ? "无" : event->text())
                           .arg(getKeyText(event->key())));
    m_modifiersLabel->setText(QString("修饰键: %1").arg(getModifiersText(event->modifiers())));
    m_nativeKeyLabel->setText(QString("原生按键码: %1").arg(event->nativeScanCode()));
    m_autoRepeatLabel->setText(QString("自动重复: %1").arg(event->isAutoRepeat() ? "是" : "否"));
    m_countLabel->setText(QString("统计: 按下%1次, 释放%2次").arg(m_keyPressCount).arg(m_keyReleaseCount));
}

QString KeyboardEventWidget::getKeyText(int key)
{
    // 常用按键的中文名称映射
    static QMap<int, QString> keyNames = {
        {Qt::Key_Space, "空格"},
        {Qt::Key_Tab, "Tab"},
        {Qt::Key_Return, "回车"},
        {Qt::Key_Enter, "小键盘回车"},
        {Qt::Key_Escape, "Esc"},
        {Qt::Key_Backspace, "退格"},
        {Qt::Key_Delete, "删除"},
        {Qt::Key_Insert, "插入"},
        {Qt::Key_Home, "Home"},
        {Qt::Key_End, "End"},
        {Qt::Key_PageUp, "Page Up"},
        {Qt::Key_PageDown, "Page Down"},
        {Qt::Key_Up, "上箭头"},
        {Qt::Key_Down, "下箭头"},
        {Qt::Key_Left, "左箭头"},
        {Qt::Key_Right, "右箭头"},
        {Qt::Key_Shift, "Shift"},
        {Qt::Key_Control, "Ctrl"},
        {Qt::Key_Alt, "Alt"},
        {Qt::Key_Meta, "Meta"},
        {Qt::Key_CapsLock, "Caps Lock"},
        {Qt::Key_NumLock, "Num Lock"},
        {Qt::Key_ScrollLock, "Scroll Lock"},
        {Qt::Key_F1, "F1"}, {Qt::Key_F2, "F2"}, {Qt::Key_F3, "F3"}, {Qt::Key_F4, "F4"},
        {Qt::Key_F5, "F5"}, {Qt::Key_F6, "F6"}, {Qt::Key_F7, "F7"}, {Qt::Key_F8, "F8"},
        {Qt::Key_F9, "F9"}, {Qt::Key_F10, "F10"}, {Qt::Key_F11, "F11"}, {Qt::Key_F12, "F12"}
    };
    
    if (keyNames.contains(key)) {
        return keyNames[key];
    }
    
    // 对于字母和数字，直接返回字符
    if ((key >= Qt::Key_A && key <= Qt::Key_Z) || (key >= Qt::Key_0 && key <= Qt::Key_9)) {
        return QChar(key).toLower();
    }
    
    return QString("未知按键");
}

QString KeyboardEventWidget::getModifiersText(Qt::KeyboardModifiers modifiers)
{
    QStringList modifierList;
    if (modifiers & Qt::ShiftModifier) modifierList << "Shift";
    if (modifiers & Qt::ControlModifier) modifierList << "Ctrl";
    if (modifiers & Qt::AltModifier) modifierList << "Alt";
    if (modifiers & Qt::MetaModifier) modifierList << "Meta";
    if (modifiers & Qt::KeypadModifier) modifierList << "小键盘";
    if (modifiers & Qt::GroupSwitchModifier) modifierList << "组切换";
    
    return modifierList.isEmpty() ? "无" : modifierList.join(" + ");
}

void KeyboardEventWidget::addKeyToHistory(const QString &eventType, QKeyEvent *event)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    QString keyInfo = QString("[%1] %2: %3 (%4)")
                     .arg(timestamp)
                     .arg(eventType)
                     .arg(getKeyText(event->key()))
                     .arg(event->key());
    
    if (!event->text().isEmpty() && !isSpecialKey(event->key())) {
        keyInfo += QString(" 文本:\"%1\"").arg(event->text());
    }
    
    if (event->modifiers() != Qt::NoModifier) {
        keyInfo += QString(" 修饰键:%1").arg(getModifiersText(event->modifiers()));
    }
    
    if (event->isAutoRepeat()) {
        keyInfo += " [重复]";
    }
    
    m_keyHistoryList->addItem(keyInfo);
    
    // 限制历史记录数量
    if (m_keyHistoryList->count() > 50) {
        delete m_keyHistoryList->takeItem(0);
    }
    
    // 自动滚动到最新项
    m_keyHistoryList->scrollToBottom();
}

bool KeyboardEventWidget::isSpecialKey(int key)
{
    return key == Qt::Key_Shift || key == Qt::Key_Control || 
           key == Qt::Key_Alt || key == Qt::Key_Meta ||
           key == Qt::Key_CapsLock || key == Qt::Key_NumLock ||
           key == Qt::Key_ScrollLock || key == Qt::Key_Tab ||
           key == Qt::Key_Return || key == Qt::Key_Enter ||
           key == Qt::Key_Escape || key == Qt::Key_Backspace ||
           key == Qt::Key_Delete || (key >= Qt::Key_F1 && key <= Qt::Key_F12);
}