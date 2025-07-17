#ifndef KEYBOARD_EVENT_WIDGET_H
#define KEYBOARD_EVENT_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QKeyEvent>
#include <QListWidget>
#include <QPushButton>
#include <QCheckBox>

/**
 * @brief 键盘事件演示控件
 * 
 * 这个控件演示了Qt中各种键盘事件的处理，包括：
 * - 按键按下和释放事件
 * - 按键码和文本的获取
 * - 修饰键的检测和组合
 * - 特殊按键的处理
 * - 键盘输入的捕获和过滤
 * - 快捷键的实现
 */
class KeyboardEventWidget : public QWidget
{
    Q_OBJECT

public:
    explicit KeyboardEventWidget(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void clearKeyHistory();
    void toggleKeyCapture(bool enabled);
    void onTextChanged();

private:
    void setupUI();
    void updateKeyInfo(const QString &eventType, QKeyEvent *event);
    QString getKeyText(int key);
    QString getModifiersText(Qt::KeyboardModifiers modifiers);
    void addKeyToHistory(const QString &eventType, QKeyEvent *event);
    bool isSpecialKey(int key);

    // UI组件
    QVBoxLayout *m_mainLayout;
    QGroupBox *m_infoGroup;
    QGroupBox *m_inputGroup;
    QGroupBox *m_historyGroup;
    
    // 信息显示标签
    QLabel *m_eventTypeLabel;
    QLabel *m_keyCodeLabel;
    QLabel *m_keyTextLabel;
    QLabel *m_modifiersLabel;
    QLabel *m_nativeKeyLabel;
    QLabel *m_autoRepeatLabel;
    QLabel *m_countLabel;
    
    // 输入相关
    QTextEdit *m_inputArea;
    QCheckBox *m_captureCheckBox;
    QLabel *m_inputStatsLabel;
    
    // 历史记录
    QListWidget *m_keyHistoryList;
    QPushButton *m_clearHistoryButton;
    
    // 状态跟踪
    QSet<int> m_pressedKeys;  // 当前按下的按键
    int m_keyPressCount;
    int m_keyReleaseCount;
    int m_totalCharCount;
    bool m_captureEnabled;

signals:
    void keyEventOccurred(const QString &eventType, int keyCode, const QString &keyText);
    void shortcutTriggered(const QString &shortcut);
};

#endif // KEYBOARD_EVENT_WIDGET_H