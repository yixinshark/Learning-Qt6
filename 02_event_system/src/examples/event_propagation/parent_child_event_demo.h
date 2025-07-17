#ifndef PARENT_CHILD_EVENT_DEMO_H
#define PARENT_CHILD_EVENT_DEMO_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QGroupBox>
#include <QCheckBox>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QEvent>
#include <QTimer>
#include <QDateTime>

/**
 * @brief 父子对象事件传播演示组件
 * 
 * 这个组件演示了Qt中事件在父子对象层次结构中的传播机制。
 * 包括事件的向上传播、事件的接受和忽略、以及传播路径的可视化显示。
 */
class ParentChildEventDemo : public QWidget
{
    Q_OBJECT

public:
    explicit ParentChildEventDemo(QWidget *parent = nullptr);
    ~ParentChildEventDemo() = default;

protected:
    // 重写事件处理函数以演示事件传播
    bool event(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onClearLog();
    void onToggleEventAcceptance();

private:
    void setupUI();
    void logEvent(const QString &source, const QString &eventType, 
                  const QString &details, bool accepted = true);
    
    // UI组件
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_contentLayout;
    
    // 左侧控制面板
    QGroupBox *m_controlGroup;
    QVBoxLayout *m_controlLayout;
    QPushButton *m_clearLogButton;
    QCheckBox *m_acceptEventsCheckBox;
    QLabel *m_instructionLabel;
    
    // 中间交互区域
    QGroupBox *m_interactionGroup;
    QVBoxLayout *m_interactionLayout;
    
    // 右侧日志显示
    QGroupBox *m_logGroup;
    QVBoxLayout *m_logLayout;
    QTextEdit *m_logDisplay;
    
    // 嵌套的子控件用于演示事件传播
    QWidget *m_childWidget;
    QWidget *m_grandChildWidget;
    QPushButton *m_testButton;
    
    // 状态控制
    bool m_acceptEvents;
};

/**
 * @brief 自定义子控件类，用于演示事件传播
 */
class PropagationChildWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PropagationChildWidget(const QString &name, QWidget *parent = nullptr);
    
    void setAcceptEvents(bool accept) { m_acceptEvents = accept; }
    bool acceptsEvents() const { return m_acceptEvents; }
    
    const QString& widgetName() const { return m_name; }

signals:
    void eventReceived(const QString &widgetName, const QString &eventType, 
                      const QString &details, bool accepted);

protected:
    bool event(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QString m_name;
    bool m_acceptEvents;
    QColor m_backgroundColor;
};

#endif // PARENT_CHILD_EVENT_DEMO_H