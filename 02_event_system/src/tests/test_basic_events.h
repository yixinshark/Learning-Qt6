#ifndef TEST_BASIC_EVENTS_H
#define TEST_BASIC_EVENTS_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QApplication>

class BasicEventWidget;
class EventTypeDemo;

/**
 * TestBasicEvents - 基础事件处理的单元测试
 * 
 * 测试内容：
 * 1. BasicEventWidget的事件处理功能
 * 2. EventTypeDemo的事件类型展示功能
 * 3. 事件接受和忽略机制
 * 4. 事件日志记录功能
 */
class TestBasicEvents : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    
    // BasicEventWidget测试
    void testBasicEventWidgetCreation();
    void testEventLogging();
    void testEventAcceptanceToggle();
    void testMouseEventHandling();
    void testKeyboardEventHandling();
    void testFocusEventHandling();
    void testCustomEventSimulation();
    void testEventCounterIncrement();
    
    // EventTypeDemo测试
    void testEventTypeDemoCreation();
    void testEventTypeSelection();
    void testEventTypeProperties();
    void testEventSimulation();
    void testEventTypeCategories();
    void testEventLoggingInDemo();

private:
    BasicEventWidget *m_basicWidget;
    EventTypeDemo *m_eventTypeDemo;
    
    // 辅助方法
    void simulateMouseClick(QWidget *widget, const QPoint &pos = QPoint(50, 50));
    void simulateKeyPress(QWidget *widget, Qt::Key key, const QString &text = QString());
    void waitForEvents(int msecs = 100);
};

#endif // TEST_BASIC_EVENTS_H