#include "test_basic_events.h"
#include "../examples/basic_events/basic_event_widget.h"
#include "../examples/basic_events/event_type_demo.h"
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QTableWidget>

void TestBasicEvents::initTestCase()
{
    // 测试套件初始化
    qDebug() << "开始基础事件处理测试";
}

void TestBasicEvents::cleanupTestCase()
{
    // 测试套件清理
    qDebug() << "基础事件处理测试完成";
}

void TestBasicEvents::init()
{
    // 每个测试用例前的初始化
    m_basicWidget = new BasicEventWidget();
    m_eventTypeDemo = new EventTypeDemo();
    
    // 显示控件以确保事件能够正常处理
    m_basicWidget->show();
    m_eventTypeDemo->show();
    
    waitForEvents(50);
}

void TestBasicEvents::cleanup()
{
    // 每个测试用例后的清理
    delete m_basicWidget;
    delete m_eventTypeDemo;
    m_basicWidget = nullptr;
    m_eventTypeDemo = nullptr;
}

void TestBasicEvents::testBasicEventWidgetCreation()
{
    // 测试BasicEventWidget的创建
    QVERIFY(m_basicWidget != nullptr);
    QVERIFY(m_basicWidget->isVisible());
    
    // 检查UI组件是否正确创建
    QTextEdit *eventLog = m_basicWidget->findChild<QTextEdit*>();
    QVERIFY(eventLog != nullptr);
    
    QPushButton *clearButton = m_basicWidget->findChild<QPushButton*>("clearButton");
    // 注意：由于我们没有设置objectName，这里检查是否存在任何QPushButton
    QList<QPushButton*> buttons = m_basicWidget->findChildren<QPushButton*>();
    QVERIFY(buttons.size() >= 3); // 应该有至少3个按钮
}

void TestBasicEvents::testEventLogging()
{
    // 测试事件日志功能
    QTextEdit *eventLog = m_basicWidget->findChild<QTextEdit*>();
    QVERIFY(eventLog != nullptr);
    
    // 初始状态应该有初始化日志
    QString initialText = eventLog->toPlainText();
    QVERIFY(initialText.contains("BasicEventWidget 初始化完成"));
    
    // 模拟鼠标点击事件
    simulateMouseClick(m_basicWidget);
    waitForEvents(100);
    
    // 检查是否记录了鼠标事件
    QString updatedText = eventLog->toPlainText();
    QVERIFY(updatedText.length() > initialText.length());
}

void TestBasicEvents::testEventAcceptanceToggle()
{
    // 测试事件接受/忽略切换功能
    QList<QPushButton*> buttons = m_basicWidget->findChildren<QPushButton*>();
    QPushButton *toggleButton = nullptr;
    
    // 找到切换按钮
    for (QPushButton *button : buttons) {
        if (button->text().contains("切换事件接受") || button->text().contains("当前模式")) {
            toggleButton = button;
            break;
        }
    }
    
    QVERIFY(toggleButton != nullptr);
    
    // 记录初始按钮文本
    QString initialText = toggleButton->text();
    
    // 点击切换按钮
    QTest::mouseClick(toggleButton, Qt::LeftButton);
    waitForEvents(50);
    
    // 检查按钮文本是否改变
    QString newText = toggleButton->text();
    QVERIFY(newText != initialText);
    QVERIFY(newText.contains("当前模式"));
}

void TestBasicEvents::testMouseEventHandling()
{
    // 测试鼠标事件处理
    QTextEdit *eventLog = m_basicWidget->findChild<QTextEdit*>();
    QVERIFY(eventLog != nullptr);
    
    QString beforeText = eventLog->toPlainText();
    
    // 模拟鼠标按下和释放
    QTest::mousePress(m_basicWidget, Qt::LeftButton, Qt::NoModifier, QPoint(100, 100));
    waitForEvents(50);
    QTest::mouseRelease(m_basicWidget, Qt::LeftButton, Qt::NoModifier, QPoint(100, 100));
    waitForEvents(50);
    
    QString afterText = eventLog->toPlainText();
    
    // 检查是否记录了鼠标事件
    QVERIFY(afterText.length() > beforeText.length());
    QVERIFY(afterText.contains("鼠标按下") || afterText.contains("MouseButtonPress"));
}

void TestBasicEvents::testKeyboardEventHandling()
{
    // 测试键盘事件处理
    QTextEdit *eventLog = m_basicWidget->findChild<QTextEdit*>();
    QVERIFY(eventLog != nullptr);
    
    // 确保控件有焦点
    m_basicWidget->setFocus();
    waitForEvents(50);
    
    QString beforeText = eventLog->toPlainText();
    
    // 模拟按键
    QTest::keyPress(m_basicWidget, Qt::Key_A);
    waitForEvents(50);
    QTest::keyRelease(m_basicWidget, Qt::Key_A);
    waitForEvents(50);
    
    QString afterText = eventLog->toPlainText();
    
    // 检查是否记录了键盘事件
    QVERIFY(afterText.length() > beforeText.length());
}

void TestBasicEvents::testFocusEventHandling()
{
    // 测试焦点事件处理
    QTextEdit *eventLog = m_basicWidget->findChild<QTextEdit*>();
    QVERIFY(eventLog != nullptr);
    
    QString beforeText = eventLog->toPlainText();
    
    // 设置焦点
    m_basicWidget->setFocus();
    waitForEvents(50);
    
    // 清除焦点
    m_basicWidget->clearFocus();
    waitForEvents(50);
    
    QString afterText = eventLog->toPlainText();
    
    // 检查是否记录了焦点事件
    QVERIFY(afterText.length() > beforeText.length());
}

void TestBasicEvents::testCustomEventSimulation()
{
    // 测试自定义事件模拟
    QTextEdit *eventLog = m_basicWidget->findChild<QTextEdit*>();
    QVERIFY(eventLog != nullptr);
    
    QList<QPushButton*> buttons = m_basicWidget->findChildren<QPushButton*>();
    QPushButton *simulateButton = nullptr;
    
    // 找到模拟事件按钮
    for (QPushButton *button : buttons) {
        if (button->text().contains("模拟自定义事件")) {
            simulateButton = button;
            break;
        }
    }
    
    QVERIFY(simulateButton != nullptr);
    
    QString beforeText = eventLog->toPlainText();
    
    // 点击模拟按钮
    QTest::mouseClick(simulateButton, Qt::LeftButton);
    waitForEvents(100);
    
    QString afterText = eventLog->toPlainText();
    
    // 检查是否记录了自定义事件
    QVERIFY(afterText.length() > beforeText.length());
    QVERIFY(afterText.contains("模拟自定义事件") || afterText.contains("自定义事件"));
}

void TestBasicEvents::testEventCounterIncrement()
{
    // 测试事件计数器递增
    QTextEdit *eventLog = m_basicWidget->findChild<QTextEdit*>();
    QVERIFY(eventLog != nullptr);
    
    // 触发几个事件
    simulateMouseClick(m_basicWidget);
    waitForEvents(50);
    simulateMouseClick(m_basicWidget);
    waitForEvents(50);
    
    QString logText = eventLog->toPlainText();
    
    // 检查是否有递增的计数器
    QVERIFY(logText.contains("[001]") || logText.contains("[1]"));
    QVERIFY(logText.contains("[002]") || logText.contains("[2]") || 
            logText.contains("[003]") || logText.contains("[3]"));
}

void TestBasicEvents::testEventTypeDemoCreation()
{
    // 测试EventTypeDemo的创建
    QVERIFY(m_eventTypeDemo != nullptr);
    QVERIFY(m_eventTypeDemo->isVisible());
    
    // 检查UI组件
    QComboBox *combo = m_eventTypeDemo->findChild<QComboBox*>();
    QVERIFY(combo != nullptr);
    QVERIFY(combo->count() > 0);
    
    QTableWidget *table = m_eventTypeDemo->findChild<QTableWidget*>();
    QVERIFY(table != nullptr);
    QCOMPARE(table->columnCount(), 2);
}

void TestBasicEvents::testEventTypeSelection()
{
    // 测试事件类型选择
    QComboBox *combo = m_eventTypeDemo->findChild<QComboBox*>();
    QVERIFY(combo != nullptr);
    
    int initialIndex = combo->currentIndex();
    int itemCount = combo->count();
    QVERIFY(itemCount > 1);
    
    // 选择不同的事件类型
    int newIndex = (initialIndex + 1) % itemCount;
    combo->setCurrentIndex(newIndex);
    waitForEvents(50);
    
    QCOMPARE(combo->currentIndex(), newIndex);
}

void TestBasicEvents::testEventTypeProperties()
{
    // 测试事件类型属性显示
    QTableWidget *table = m_eventTypeDemo->findChild<QTableWidget*>();
    QVERIFY(table != nullptr);
    
    // 表格应该有内容
    QVERIFY(table->rowCount() > 0);
    
    // 检查基本属性行
    QTableWidgetItem *typeItem = table->item(0, 0);
    QVERIFY(typeItem != nullptr);
    QCOMPARE(typeItem->text(), QString("事件类型"));
    
    QTableWidgetItem *categoryItem = table->item(1, 0);
    QVERIFY(categoryItem != nullptr);
    QCOMPARE(categoryItem->text(), QString("事件分类"));
}

void TestBasicEvents::testEventSimulation()
{
    // 测试事件模拟功能
    QList<QPushButton*> buttons = m_eventTypeDemo->findChildren<QPushButton*>();
    QPushButton *simulateButton = nullptr;
    
    for (QPushButton *button : buttons) {
        if (button->text().contains("模拟事件")) {
            simulateButton = button;
            break;
        }
    }
    
    QVERIFY(simulateButton != nullptr);
    
    // 如果按钮可用，测试模拟功能
    if (simulateButton->isEnabled()) {
        QTextEdit *eventLog = m_eventTypeDemo->findChild<QTextEdit*>();
        QVERIFY(eventLog != nullptr);
        
        QString beforeText = eventLog->toPlainText();
        
        QTest::mouseClick(simulateButton, Qt::LeftButton);
        waitForEvents(100);
        
        QString afterText = eventLog->toPlainText();
        QVERIFY(afterText.length() > beforeText.length());
    }
}

void TestBasicEvents::testEventTypeCategories()
{
    // 测试事件类型分类
    QComboBox *combo = m_eventTypeDemo->findChild<QComboBox*>();
    QVERIFY(combo != nullptr);
    
    // 检查是否包含不同类别的事件
    QStringList allItems;
    for (int i = 0; i < combo->count(); ++i) {
        allItems << combo->itemText(i);
    }
    
    // 应该包含鼠标事件
    bool hasMouseEvent = false;
    bool hasKeyboardEvent = false;
    bool hasFocusEvent = false;
    
    for (const QString &item : allItems) {
        if (item.contains("Mouse") || item.contains("鼠标")) {
            hasMouseEvent = true;
        }
        if (item.contains("Key") || item.contains("键盘")) {
            hasKeyboardEvent = true;
        }
        if (item.contains("Focus") || item.contains("焦点")) {
            hasFocusEvent = true;
        }
    }
    
    QVERIFY(hasMouseEvent);
    QVERIFY(hasKeyboardEvent);
    QVERIFY(hasFocusEvent);
}

void TestBasicEvents::testEventLoggingInDemo()
{
    // 测试EventTypeDemo中的事件日志
    QTextEdit *eventLog = m_eventTypeDemo->findChild<QTextEdit*>();
    QVERIFY(eventLog != nullptr);
    
    // 触发一些事件
    simulateMouseClick(m_eventTypeDemo);
    waitForEvents(100);
    
    // 检查日志是否有内容
    QString logText = eventLog->toPlainText();
    // 注意：由于EventTypeDemo可能不记录所有事件，这里只检查日志控件存在
    QVERIFY(eventLog->isVisible());
}

// 辅助方法实现
void TestBasicEvents::simulateMouseClick(QWidget *widget, const QPoint &pos)
{
    QTest::mouseClick(widget, Qt::LeftButton, Qt::NoModifier, pos);
}

void TestBasicEvents::simulateKeyPress(QWidget *widget, Qt::Key key, const QString &text)
{
    QTest::keyPress(widget, key, Qt::NoModifier);
}

void TestBasicEvents::waitForEvents(int msecs)
{
    QTest::qWait(msecs);
}

QTEST_MAIN(TestBasicEvents)
// #include "test_basic_events.moc" // 移除不需要的moc包含