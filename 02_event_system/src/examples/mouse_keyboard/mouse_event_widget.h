#ifndef MOUSE_EVENT_WIDGET_H
#define MOUSE_EVENT_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>

/**
 * @brief 鼠标事件演示控件
 * 
 * 这个控件演示了Qt中各种鼠标事件的处理，包括：
 * - 鼠标按下、释放、移动事件
 * - 鼠标滚轮事件
 * - 鼠标进入、离开事件
 * - 双击事件
 * - 鼠标位置和按键状态的获取
 * - 修饰键的检测
 */
class MouseEventWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MouseEventWidget(QWidget *parent = nullptr);

protected:
    // 鼠标事件处理函数
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private slots:
    void clearTrail();

private:
    void setupUI();
    void updateEventInfo(const QString &eventType, QMouseEvent *event);
    void updateWheelInfo(QWheelEvent *event);
    void updatePositionInfo(const QPoint &position);
    QString getButtonsText(Qt::MouseButtons buttons);
    QString getModifiersText(Qt::KeyboardModifiers modifiers);

    // UI组件
    QVBoxLayout *m_mainLayout;
    QGroupBox *m_infoGroup;
    QGroupBox *m_visualGroup;
    
    // 信息显示标签
    QLabel *m_eventTypeLabel;
    QLabel *m_positionLabel;
    QLabel *m_globalPositionLabel;
    QLabel *m_buttonLabel;
    QLabel *m_buttonsLabel;
    QLabel *m_modifiersLabel;
    QLabel *m_wheelDeltaLabel;
    
    // 可视化相关
    QList<QPoint> m_mouseTrail;  // 鼠标轨迹
    QPoint m_lastClickPos;       // 最后点击位置
    QTimer *m_trailTimer;        // 轨迹清除定时器
    bool m_mouseInside;          // 鼠标是否在控件内
    
    // 统计信息
    int m_clickCount;
    int m_moveCount;
    int m_wheelCount;

signals:
    void mouseEventOccurred(const QString &eventType, const QPoint &position);
};

#endif // MOUSE_EVENT_WIDGET_H