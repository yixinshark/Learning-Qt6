#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QDebug>

#include "src/examples/basic_events/basic_event_widget.h"
#include "src/examples/basic_events/event_type_demo.h"

/**
 * @brief 简单的测试运行器，用于验证基础事件处理示例的功能
 */
class TestRunner : public QWidget
{
    Q_OBJECT

public:
    explicit TestRunner(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setWindowTitle("Qt6 Event System - Basic Events Test Runner");
        setMinimumSize(800, 700);
        
        setupUI();
        
        qDebug() << "Test Runner initialized successfully";
    }

private:
    void setupUI()
    {
        auto *mainLayout = new QVBoxLayout(this);
        
        // 创建标签页控件
        auto *tabWidget = new QTabWidget(this);
        
        // 添加BasicEventWidget标签页
        m_basicEventWidget = new BasicEventWidget();
        tabWidget->addTab(m_basicEventWidget, "基础事件处理演示");
        
        // 添加EventTypeDemo标签页
        m_eventTypeDemo = new EventTypeDemo();
        tabWidget->addTab(m_eventTypeDemo, "事件类型演示");
        
        mainLayout->addWidget(tabWidget);
        
        setLayout(mainLayout);
    }

private:
    BasicEventWidget *m_basicEventWidget;
    EventTypeDemo *m_eventTypeDemo;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    qDebug() << "Starting Qt6 Event System - Basic Events Test Runner...";
    
    TestRunner testRunner;
    testRunner.show();
    
    qDebug() << "Test Runner started successfully";
    qDebug() << "Instructions:";
    qDebug() << "1. Switch between tabs to test different components";
    qDebug() << "2. In 'Basic Event Widget' tab:";
    qDebug() << "   - Click mouse buttons to test mouse events";
    qDebug() << "   - Press keyboard keys to test keyboard events";
    qDebug() << "   - Move mouse in/out to test enter/leave events";
    qDebug() << "   - Use buttons to test event acceptance/ignore mechanism";
    qDebug() << "3. In 'Event Type Demo' tab:";
    qDebug() << "   - Select different event types from dropdown";
    qDebug() << "   - Click 'Simulate Event' to test event simulation";
    qDebug() << "   - Observe event properties and descriptions";
    
    return app.exec();
}

#include "test_runner.moc"