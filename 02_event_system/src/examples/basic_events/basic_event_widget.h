#ifndef BASIC_EVENT_WIDGET_H
#define BASIC_EVENT_WIDGET_H

#include <QWidget>
#include <QEvent>
#include <QLabel>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QTimer>

/**
 * BasicEventWidget - 演示QEvent基类和基础事件处理
 * 
 * 这个类展示了：
 * 1. event()函数的重写和事件分发机制
 * 2. 不同事件类型的识别和处理
 * 3. 事件接受和忽略机制
 * 4. 事件处理的基本流程
 */
class BasicEventWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BasicEventWidget(QWidget *parent = nullptr);
    ~BasicEventWidget() override = default;

protected:
    // 重写event()函数来演示事件分发机制
    bool event(QEvent *event) override;
    
    // 重写特定事件处理函数
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private slots:
    void onClearLog();
    void onToggleEventAcceptance();
    void onSimulateCustomEvent();

private:
    void setupUI();
    void logEvent(const QString &eventInfo);
    QString getEventTypeName(QEvent::Type type) const;
    
    // UI组件
    QVBoxLayout *m_mainLayout;
    QLabel *m_titleLabel;
    QTextEdit *m_eventLog;
    QPushButton *m_clearButton;
    QPushButton *m_toggleAcceptButton;
    QPushButton *m_simulateEventButton;
    QLabel *m_statusLabel;
    QLabel *m_instructionLabel;
    
    // 状态变量
    bool m_acceptEvents;
    int m_eventCounter;
    QTimer *m_statusTimer;
};

#endif // BASIC_EVENT_WIDGET_H