#ifndef TEST_MOUSE_KEYBOARD_H
#define TEST_MOUSE_KEYBOARD_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QApplication>

#include "../examples/mouse_keyboard/mouse_event_widget.h"
#include "../examples/mouse_keyboard/keyboard_event_widget.h"
#include "../examples/mouse_keyboard/interaction_demo.h"

/**
 * @brief 鼠标键盘事件处理测试类
 * 
 * 测试鼠标和键盘事件的处理功能，包括：
 * - 鼠标事件的正确处理和信号发射
 * - 键盘事件的正确处理和快捷键识别
 * - 事件信息的准确显示和统计
 * - 交互演示的功能验证
 */
class TestMouseKeyboard : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // 鼠标事件测试
    void testMousePressEvent();
    void testMouseReleaseEvent();
    void testMouseMoveEvent();
    void testMouseDoubleClickEvent();
    void testWheelEvent();
    void testMouseEnterLeaveEvents();
    void testMouseEventSignals();
    void testMouseButtonDetection();
    void testMouseModifierKeys();

    // 键盘事件测试
    void testKeyPressEvent();
    void testKeyReleaseEvent();
    void testKeyEventSignals();
    void testShortcutDetection();
    void testSpecialKeys();
    void testModifierKeys();
    void testKeyCapture();
    void testAutoRepeat();

    // 交互演示测试
    void testInteractionDemo();
    void testDrawingModes();
    void testObjectInteraction();
    void testEventStatistics();

private:
    MouseEventWidget *m_mouseWidget;
    KeyboardEventWidget *m_keyboardWidget;
    InteractionDemo *m_interactionDemo;
    
    // 辅助方法
    void simulateMouseEvent(QWidget *widget, QEvent::Type type, 
                           const QPoint &pos, Qt::MouseButton button = Qt::LeftButton,
                           Qt::KeyboardModifiers modifiers = Qt::NoModifier);
    void simulateKeyEvent(QWidget *widget, QEvent::Type type, 
                         int key, const QString &text = QString(),
                         Qt::KeyboardModifiers modifiers = Qt::NoModifier);
};

#endif // TEST_MOUSE_KEYBOARD_H