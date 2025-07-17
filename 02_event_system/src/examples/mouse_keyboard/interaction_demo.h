#ifndef INTERACTION_DEMO_H
#define INTERACTION_DEMO_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QProgressBar>
#include <QTimer>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QGraphicsEffect>

#include "mouse_event_widget.h"
#include "keyboard_event_widget.h"

/**
 * @brief 鼠标键盘交互演示控件
 * 
 * 这个控件综合演示了鼠标和键盘事件的交互应用，包括：
 * - 鼠标键盘组合操作
 * - 拖拽操作的实现
 * - 绘图和交互式操作
 * - 游戏式的交互控制
 * - 实时反馈和动画效果
 */
class InteractionDemo : public QWidget
{
    Q_OBJECT

public:
    explicit InteractionDemo(QWidget *parent = nullptr);

private slots:
    void onMouseEventOccurred(const QString &eventType, const QPoint &position);
    void onKeyEventOccurred(const QString &eventType, int keyCode, const QString &keyText);
    void onShortcutTriggered(const QString &shortcut);
    void resetDemo();
    void toggleAnimation(bool enabled);

private:
    void setupUI();
    void setupInteractiveArea();
    void setupControlPanel();
    void updateInteractionStats();

    // UI组件
    QVBoxLayout *m_mainLayout;
    QTabWidget *m_tabWidget;
    
    // 基础事件演示标签页
    QWidget *m_basicTab;
    MouseEventWidget *m_mouseWidget;
    KeyboardEventWidget *m_keyboardWidget;
    
    // 交互演示标签页
    QWidget *m_interactionTab;
    QWidget *m_interactiveArea;
    QGroupBox *m_controlGroup;
    QGroupBox *m_statsGroup;
    
    // 控制面板
    QPushButton *m_resetButton;
    QCheckBox *m_animationCheckBox;
    QSlider *m_speedSlider;
    QSpinBox *m_sizeSpinBox;
    QComboBox *m_modeComboBox;
    
    // 统计显示
    QLabel *m_mouseStatsLabel;
    QLabel *m_keyboardStatsLabel;
    QLabel *m_interactionStatsLabel;
    QProgressBar *m_activityProgress;
    
    // 交互状态
    int m_mouseEventCount;
    int m_keyboardEventCount;
    int m_interactionCount;
    QTimer *m_activityTimer;
    bool m_animationEnabled;

signals:
    void interactionCompleted(const QString &type);
};

/**
 * @brief 交互式绘图区域
 * 
 * 支持鼠标绘图、键盘控制、拖拽等交互操作
 */
class InteractiveDrawArea : public QWidget
{
    Q_OBJECT

public:
    explicit InteractiveDrawArea(QWidget *parent = nullptr);
    
    void setDrawMode(int mode);
    void setBrushSize(int size);
    void setAnimationEnabled(bool enabled);
    void clearCanvas();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private slots:
    void updateAnimation();

private:
    void drawPoint(const QPoint &point, const QColor &color);
    void drawLine(const QPoint &start, const QPoint &end, const QColor &color);
    QColor getCurrentColor();

    // 绘图相关
    QPixmap m_canvas;
    QPoint m_lastPoint;
    bool m_drawing;
    int m_drawMode;  // 0: 点, 1: 线, 2: 矩形, 3: 圆
    int m_brushSize;
    QColor m_currentColor;
    
    // 动画相关
    QTimer *m_animationTimer;
    bool m_animationEnabled;
    int m_animationStep;
    
    // 交互对象
    struct InteractiveObject {
        QRect rect;
        QColor color;
        bool selected;
        QPoint dragOffset;
    };
    QList<InteractiveObject> m_objects;
    int m_selectedObject;
    bool m_dragging;

signals:
    void drawingAction(const QString &action, const QPoint &position);
    void objectInteraction(const QString &action, int objectIndex);
};

#endif // INTERACTION_DEMO_H