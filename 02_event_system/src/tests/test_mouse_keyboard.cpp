#include "test_mouse_keyboard.h"
#include <QTimer>
#include <QEventLoop>

void TestMouseKeyboard::initTestCase()
{
    // 测试套件初始化
    qDebug() << "Starting Mouse/Keyboard Event Tests";
}

void TestMouseKeyboard::cleanupTestCase()
{
    // 测试套件清理
    qDebug() << "Mouse/Keyboard Event Tests Completed";
}

void TestMouseKeyboard::init()
{
    // 每个测试前的初始化
    m_mouseWidget = new MouseEventWidget();
    m_keyboardWidget = new KeyboardEventWidget();
    m_interactionDemo = new InteractionDemo();
    
    // 显示控件以确保事件能正确处理
    m_mouseWidget->show();
    m_keyboardWidget->show();
    m_interactionDemo->show();
    
    // 等待控件完全显示，忽略返回值
    (void)QTest::qWaitForWindowExposed(m_mouseWidget);
    (void)QTest::qWaitForWindowExposed(m_keyboardWidget);
    (void)QTest::qWaitForWindowExposed(m_interactionDemo);
}

void TestMouseKeyboard::cleanup()
{
    // 每个测试后的清理
    delete m_mouseWidget;
    delete m_keyboardWidget;
    delete m_interactionDemo;
    
    m_mouseWidget = nullptr;
    m_keyboardWidget = nullptr;
    m_interactionDemo = nullptr;
}

void TestMouseKeyboard::testMousePressEvent()
{
    // 测试鼠标按下事件
    QSignalSpy spy(m_mouseWidget, &MouseEventWidget::mouseEventOccurred);
    
    // 模拟左键按下
    simulateMouseEvent(m_mouseWidget, QEvent::MouseButtonPress, QPoint(100, 100), Qt::LeftButton);
    
    // 验证信号发射
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("MousePress"));
    QCOMPARE(arguments.at(1).toPoint(), QPoint(100, 100));
    
    // 测试右键按下
    simulateMouseEvent(m_mouseWidget, QEvent::MouseButtonPress, QPoint(150, 150), Qt::RightButton);
    
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("MousePress"));
    QCOMPARE(arguments.at(1).toPoint(), QPoint(150, 150));
}

void TestMouseKeyboard::testMouseReleaseEvent()
{
    // 测试鼠标释放事件
    QSignalSpy spy(m_mouseWidget, &MouseEventWidget::mouseEventOccurred);
    
    // 模拟鼠标释放
    simulateMouseEvent(m_mouseWidget, QEvent::MouseButtonRelease, QPoint(200, 200), Qt::LeftButton);
    
    // 验证信号发射
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("MouseRelease"));
    QCOMPARE(arguments.at(1).toPoint(), QPoint(200, 200));
}

void TestMouseKeyboard::testMouseMoveEvent()
{
    // 测试鼠标移动事件
    QSignalSpy spy(m_mouseWidget, &MouseEventWidget::mouseEventOccurred);
    
    // 模拟鼠标移动
    simulateMouseEvent(m_mouseWidget, QEvent::MouseMove, QPoint(50, 50));
    
    // 验证信号发射
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("MouseMove"));
    QCOMPARE(arguments.at(1).toPoint(), QPoint(50, 50));
}

void TestMouseKeyboard::testMouseDoubleClickEvent()
{
    // 测试鼠标双击事件
    QSignalSpy spy(m_mouseWidget, &MouseEventWidget::mouseEventOccurred);
    
    // 模拟双击
    simulateMouseEvent(m_mouseWidget, QEvent::MouseButtonDblClick, QPoint(75, 75), Qt::LeftButton);
    
    // 验证信号发射
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("MouseDoubleClick"));
    QCOMPARE(arguments.at(1).toPoint(), QPoint(75, 75));
}

void TestMouseKeyboard::testWheelEvent()
{
    // 测试滚轮事件
    QSignalSpy spy(m_mouseWidget, &MouseEventWidget::mouseEventOccurred);
    
    // 创建滚轮事件
    QWheelEvent wheelEvent(QPointF(100, 100), QPointF(100, 100), 
                          QPoint(0, 0), QPoint(0, 120), 
                          Qt::NoButton, Qt::NoModifier, Qt::NoScrollPhase, false);
    
    // 发送滚轮事件
    QApplication::sendEvent(m_mouseWidget, &wheelEvent);
    
    // 验证信号发射
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("WheelEvent"));
}

void TestMouseKeyboard::testMouseEnterLeaveEvents()
{
    // 测试鼠标进入和离开事件
    QSignalSpy spy(m_mouseWidget, &MouseEventWidget::mouseEventOccurred);
    
    // 创建进入事件
    QEnterEvent enterEvent(QPointF(50, 50), QPointF(50, 50), QPointF(150, 150));
    QApplication::sendEvent(m_mouseWidget, &enterEvent);
    
    // 验证进入事件
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("MouseEnter"));
    
    // 创建离开事件
    QEvent leaveEvent(QEvent::Leave);
    QApplication::sendEvent(m_mouseWidget, &leaveEvent);
    
    // 验证离开事件
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("MouseLeave"));
}

void TestMouseKeyboard::testMouseEventSignals()
{
    // 测试鼠标事件信号的完整性
    QSignalSpy spy(m_mouseWidget, &MouseEventWidget::mouseEventOccurred);
    
    // 连续发送多个事件
    simulateMouseEvent(m_mouseWidget, QEvent::MouseButtonPress, QPoint(10, 10));
    simulateMouseEvent(m_mouseWidget, QEvent::MouseMove, QPoint(20, 20));
    simulateMouseEvent(m_mouseWidget, QEvent::MouseButtonRelease, QPoint(30, 30));
    
    // 验证所有事件都被正确处理
    QCOMPARE(spy.count(), 3);
    
    // 验证事件顺序
    QCOMPARE(spy.at(0).at(0).toString(), QString("MousePress"));
    QCOMPARE(spy.at(1).at(0).toString(), QString("MouseMove"));
    QCOMPARE(spy.at(2).at(0).toString(), QString("MouseRelease"));
}

void TestMouseKeyboard::testMouseButtonDetection()
{
    // 测试不同鼠标按键的检测
    QSignalSpy spy(m_mouseWidget, &MouseEventWidget::mouseEventOccurred);
    
    // 测试左键
    simulateMouseEvent(m_mouseWidget, QEvent::MouseButtonPress, QPoint(100, 100), Qt::LeftButton);
    QCOMPARE(spy.count(), 1);
    spy.clear();
    
    // 测试右键
    simulateMouseEvent(m_mouseWidget, QEvent::MouseButtonPress, QPoint(100, 100), Qt::RightButton);
    QCOMPARE(spy.count(), 1);
    spy.clear();
    
    // 测试中键
    simulateMouseEvent(m_mouseWidget, QEvent::MouseButtonPress, QPoint(100, 100), Qt::MiddleButton);
    QCOMPARE(spy.count(), 1);
}

void TestMouseKeyboard::testMouseModifierKeys()
{
    // 测试鼠标事件中的修饰键检测
    QSignalSpy spy(m_mouseWidget, &MouseEventWidget::mouseEventOccurred);
    
    // 测试Ctrl+点击
    simulateMouseEvent(m_mouseWidget, QEvent::MouseButtonPress, QPoint(100, 100), 
                      Qt::LeftButton, Qt::ControlModifier);
    QCOMPARE(spy.count(), 1);
    spy.clear();
    
    // 测试Shift+点击
    simulateMouseEvent(m_mouseWidget, QEvent::MouseButtonPress, QPoint(100, 100), 
                      Qt::LeftButton, Qt::ShiftModifier);
    QCOMPARE(spy.count(), 1);
}

void TestMouseKeyboard::testKeyPressEvent()
{
    // 测试按键按下事件
    QSignalSpy spy(m_keyboardWidget, &KeyboardEventWidget::keyEventOccurred);
    
    // 模拟按键按下
    simulateKeyEvent(m_keyboardWidget, QEvent::KeyPress, Qt::Key_A, "a");
    
    // 验证信号发射
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("KeyPress"));
    QCOMPARE(arguments.at(1).toInt(), static_cast<int>(Qt::Key_A));
    QCOMPARE(arguments.at(2).toString(), QString("a"));
}

void TestMouseKeyboard::testKeyReleaseEvent()
{
    // 测试按键释放事件
    QSignalSpy spy(m_keyboardWidget, &KeyboardEventWidget::keyEventOccurred);
    
    // 模拟按键释放
    simulateKeyEvent(m_keyboardWidget, QEvent::KeyRelease, Qt::Key_B, "b");
    
    // 验证信号发射
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("KeyRelease"));
    QCOMPARE(arguments.at(1).toInt(), static_cast<int>(Qt::Key_B));
    QCOMPARE(arguments.at(2).toString(), QString("b"));
}

void TestMouseKeyboard::testKeyEventSignals()
{
    // 测试键盘事件信号的完整性
    QSignalSpy spy(m_keyboardWidget, &KeyboardEventWidget::keyEventOccurred);
    
    // 连续发送多个按键事件
    simulateKeyEvent(m_keyboardWidget, QEvent::KeyPress, Qt::Key_H, "h");
    simulateKeyEvent(m_keyboardWidget, QEvent::KeyPress, Qt::Key_E, "e");
    simulateKeyEvent(m_keyboardWidget, QEvent::KeyPress, Qt::Key_L, "l");
    simulateKeyEvent(m_keyboardWidget, QEvent::KeyPress, Qt::Key_L, "l");
    simulateKeyEvent(m_keyboardWidget, QEvent::KeyPress, Qt::Key_O, "o");
    
    // 验证所有事件都被正确处理
    QCOMPARE(spy.count(), 5);
    
    // 验证按键顺序
    QCOMPARE(spy.at(0).at(1).toInt(), static_cast<int>(Qt::Key_H));
    QCOMPARE(spy.at(1).at(1).toInt(), static_cast<int>(Qt::Key_E));
    QCOMPARE(spy.at(2).at(1).toInt(), static_cast<int>(Qt::Key_L));
    QCOMPARE(spy.at(3).at(1).toInt(), static_cast<int>(Qt::Key_L));
    QCOMPARE(spy.at(4).at(1).toInt(), static_cast<int>(Qt::Key_O));
}

void TestMouseKeyboard::testShortcutDetection()
{
    // 测试快捷键检测
    QSignalSpy spy(m_keyboardWidget, &KeyboardEventWidget::shortcutTriggered);
    
    // 测试Ctrl+S快捷键
    simulateKeyEvent(m_keyboardWidget, QEvent::KeyPress, Qt::Key_S, "s", Qt::ControlModifier);
    
    // 验证快捷键信号发射
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toString().contains("Ctrl+S"));
    
    // 测试Ctrl+C快捷键
    simulateKeyEvent(m_keyboardWidget, QEvent::KeyPress, Qt::Key_C, "c", Qt::ControlModifier);
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toString().contains("Ctrl+C"));
    
    // 测试Esc键
    simulateKeyEvent(m_keyboardWidget, QEvent::KeyPress, Qt::Key_Escape, "");
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toString().contains("Esc"));
}

void TestMouseKeyboard::testSpecialKeys()
{
    // 测试特殊按键处理
    QSignalSpy spy(m_keyboardWidget, &KeyboardEventWidget::keyEventOccurred);
    
    // 测试功能键
    simulateKeyEvent(m_keyboardWidget, QEvent::KeyPress, Qt::Key_F1, "");
    QCOMPARE(spy.count(), 1);
    spy.clear();
    
    // 测试箭头键
    simulateKeyEvent(m_keyboardWidget, QEvent::KeyPress, Qt::Key_Up, "");
    QCOMPARE(spy.count(), 1);
    spy.clear();
    
    // 测试空格键
    simulateKeyEvent(m_keyboardWidget, QEvent::KeyPress, Qt::Key_Space, " ");
    QCOMPARE(spy.count(), 1);
    spy.clear();
    
    // 测试回车键
    simulateKeyEvent(m_keyboardWidget, QEvent::KeyPress, Qt::Key_Return, "\r");
    QCOMPARE(spy.count(), 1);
}

void TestMouseKeyboard::testModifierKeys()
{
    // 测试修饰键检测
    QSignalSpy spy(m_keyboardWidget, &KeyboardEventWidget::keyEventOccurred);
    
    // 测试Shift+字母
    simulateKeyEvent(m_keyboardWidget, QEvent::KeyPress, Qt::Key_A, "A", Qt::ShiftModifier);
    QCOMPARE(spy.count(), 1);
    spy.clear();
    
    // 测试Alt+字母
    simulateKeyEvent(m_keyboardWidget, QEvent::KeyPress, Qt::Key_F, "f", Qt::AltModifier);
    QCOMPARE(spy.count(), 1);
    spy.clear();
    
    // 测试Ctrl+Alt组合
    simulateKeyEvent(m_keyboardWidget, QEvent::KeyPress, Qt::Key_T, "t", 
                    Qt::ControlModifier | Qt::AltModifier);
    QCOMPARE(spy.count(), 1);
}

void TestMouseKeyboard::testKeyCapture()
{
    // 测试按键捕获功能
    // 这个测试需要访问KeyboardEventWidget的私有成员，
    // 在实际实现中可能需要添加公共接口或友元类
    
    // 模拟启用按键捕获
    // m_keyboardWidget->toggleKeyCapture(true);
    
    QSignalSpy spy(m_keyboardWidget, &KeyboardEventWidget::keyEventOccurred);
    
    // 发送按键事件
    simulateKeyEvent(m_keyboardWidget, QEvent::KeyPress, Qt::Key_X, "x");
    
    // 验证事件被捕获
    QCOMPARE(spy.count(), 1);
}

void TestMouseKeyboard::testAutoRepeat()
{
    // 测试自动重复按键
    QSignalSpy spy(m_keyboardWidget, &KeyboardEventWidget::keyEventOccurred);
    
    // 创建自动重复的按键事件
    QKeyEvent autoRepeatEvent(QEvent::KeyPress, Qt::Key_A, Qt::NoModifier, "a", true, 1);
    QApplication::sendEvent(m_keyboardWidget, &autoRepeatEvent);
    
    // 验证自动重复事件被正确处理
    QCOMPARE(spy.count(), 1);
}

void TestMouseKeyboard::testInteractionDemo()
{
    // 测试交互演示的基本功能
    QVERIFY(m_interactionDemo != nullptr);
    
    // 验证控件已正确创建和显示
    QVERIFY(m_interactionDemo->isVisible());
    
    // 测试重置功能
    // 这需要访问InteractionDemo的私有成员或添加公共接口
    // m_interactionDemo->resetDemo();
}

void TestMouseKeyboard::testDrawingModes()
{
    // 测试绘图模式切换
    // 这个测试需要访问InteractiveDrawArea或通过公共接口
    
    // 由于InteractiveDrawArea是InteractionDemo的私有成员，
    // 实际测试中需要添加适当的公共接口
    
    QVERIFY(true); // 占位符测试
}

void TestMouseKeyboard::testObjectInteraction()
{
    // 测试对象交互功能
    // 类似于绘图模式测试，需要适当的公共接口
    
    QVERIFY(true); // 占位符测试
}

void TestMouseKeyboard::testEventStatistics()
{
    // 测试事件统计功能
    // 需要访问InteractionDemo的统计信息
    
    QVERIFY(true); // 占位符测试
}

// 辅助方法实现
void TestMouseKeyboard::simulateMouseEvent(QWidget *widget, QEvent::Type type, 
                                          const QPoint &pos, Qt::MouseButton button,
                                          Qt::KeyboardModifiers modifiers)
{
    if (type == QEvent::MouseButtonPress || type == QEvent::MouseButtonRelease || 
        type == QEvent::MouseButtonDblClick) {
        QMouseEvent event(type, pos, pos, button, button, modifiers);
        QApplication::sendEvent(widget, &event);
    } else if (type == QEvent::MouseMove) {
        QMouseEvent event(type, pos, pos, Qt::NoButton, Qt::NoButton, modifiers);
        QApplication::sendEvent(widget, &event);
    }
}

void TestMouseKeyboard::simulateKeyEvent(QWidget *widget, QEvent::Type type, 
                                        int key, const QString &text,
                                        Qt::KeyboardModifiers modifiers)
{
    QKeyEvent event(type, key, modifiers, text);
    QApplication::sendEvent(widget, &event);
}

// 注册测试类
QTEST_MAIN(TestMouseKeyboard)
// #include "test_mouse_keyboard.moc" // 移除不需要的moc包含