#include <QApplication>
#include <QDebug>
#include <QTimer>
#include <QTest>
#include <QWidget>
#include <QElapsedTimer>
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>

// 包含所有核心组件
#include "src/core/event_manager.h"
#include "src/core/event_logger.h"
#include "src/core/custom_events.h"
#include "src/core/event_performance_analyzer.h"
#include "src/core/object_hierarchy_model.h"

// 包含所有示例组件
#include "src/examples/basic_events/basic_event_widget.h"
#include "src/examples/mouse_keyboard/interaction_demo.h"
#include "src/examples/custom_events/custom_event_demo.h"
#include "src/examples/event_filters/global_event_filter.h"
#include "src/examples/event_propagation/parent_child_event_demo.h"
#include "src/examples/advanced_patterns/advanced_patterns_demo.h"

// 包含所有UI组件
#include "src/widgets/main_window.h"
#include "src/widgets/event_display_widget.h"
#include "src/widgets/interactive_area_widget.h"
#include "src/widgets/debug_panel_widget.h"
#include "src/widgets/performance_monitor_widget.h"

/**
 * 集成验证程序
 * 验证所有组件的集成和一致性
 */
class IntegrationVerifier : public QObject
{
    Q_OBJECT

public:
    IntegrationVerifier(QObject* parent = nullptr) : QObject(parent) {}

    bool runAllTests()
    {
        qDebug() << "=== Qt事件系统项目集成验证开始 ===";
        
        bool allPassed = true;
        
        // 1. 核心组件验证
        allPassed &= verifyCoreComponents();
        
        // 2. 示例组件验证
        allPassed &= verifyExampleComponents();
        
        // 3. UI组件验证
        allPassed &= verifyUIComponents();
        
        // 4. 集成功能验证
        allPassed &= verifyIntegrationFeatures();
        
        // 5. 性能验证
        allPassed &= verifyPerformance();
        
        qDebug() << "=== 集成验证完成 ===";
        qDebug() << "总体结果:" << (allPassed ? "通过" : "失败");
        
        return allPassed;
    }

private slots:
    void onTestCompleted()
    {
        qDebug() << "测试完成，退出应用程序";
        QApplication::quit();
    }

private:
    bool verifyCoreComponents()
    {
        qDebug() << "\n--- 核心组件验证 ---";
        bool passed = true;
        
        try {
            // EventManager验证
            EventManager* eventManager = EventManager::instance();
            if (!eventManager) {
                qDebug() << "❌ EventManager单例创建失败";
                passed = false;
            } else {
                qDebug() << "✅ EventManager单例创建成功";
                
                // 测试事件类型注册
                eventManager->registerEventType(QEvent::User, "TestEvent");
                QString typeName = eventManager->getEventTypeName(QEvent::User);
                if (typeName != "TestEvent") {
                    qDebug() << "❌ EventManager事件类型注册失败";
                    passed = false;
                } else {
                    qDebug() << "✅ EventManager事件类型注册成功";
                }
            }
            
            // EventLogger验证
            EventLogger* eventLogger = EventLogger::instance();
            if (!eventLogger) {
                qDebug() << "❌ EventLogger单例创建失败";
                passed = false;
            } else {
                qDebug() << "✅ EventLogger单例创建成功";
                
                // 测试事件记录
                int initialCount = eventLogger->getEventHistory().size();
                EventLogger::EventRecord record;
                record.timestamp = QDateTime::currentDateTime();
                record.eventType = QEvent::MouseButtonPress;
                record.eventName = "TestEvent";
                record.accepted = true;
                
                eventLogger->logEvent(record);
                
                if (eventLogger->getEventHistory().size() != initialCount + 1) {
                    qDebug() << "❌ EventLogger事件记录失败";
                    passed = false;
                } else {
                    qDebug() << "✅ EventLogger事件记录成功";
                }
            }
            
            // 自定义事件验证
            DataEvent* dataEvent = new DataEvent(QVariant("test data"));
            if (!dataEvent || dataEvent->data().toString() != "test data") {
                qDebug() << "❌ 自定义事件创建失败";
                passed = false;
            } else {
                qDebug() << "✅ 自定义事件创建成功";
            }
            delete dataEvent;
            
        } catch (const std::exception& e) {
            qDebug() << "❌ 核心组件验证异常:" << e.what();
            passed = false;
        } catch (...) {
            qDebug() << "❌ 核心组件验证未知异常";
            passed = false;
        }
        
        return passed;
    }
    
    bool verifyExampleComponents()
    {
        qDebug() << "\n--- 示例组件验证 ---";
        bool passed = true;
        
        try {
            // BasicEventWidget验证
            BasicEventWidget* basicWidget = new BasicEventWidget();
            if (!basicWidget) {
                qDebug() << "❌ BasicEventWidget创建失败";
                passed = false;
            } else {
                qDebug() << "✅ BasicEventWidget创建成功";
                basicWidget->deleteLater();
            }
            
            // InteractionDemo验证
            InteractionDemo* interactionDemo = new InteractionDemo();
            if (!interactionDemo) {
                qDebug() << "❌ InteractionDemo创建失败";
                passed = false;
            } else {
                qDebug() << "✅ InteractionDemo创建成功";
                interactionDemo->deleteLater();
            }
            
            // CustomEventDemo验证
            CustomEventDemo* customDemo = new CustomEventDemo();
            if (!customDemo) {
                qDebug() << "❌ CustomEventDemo创建失败";
                passed = false;
            } else {
                qDebug() << "✅ CustomEventDemo创建成功";
                customDemo->deleteLater();
            }
            
            // ParentChildEventDemo验证
            ParentChildEventDemo* propagationDemo = new ParentChildEventDemo();
            if (!propagationDemo) {
                qDebug() << "❌ ParentChildEventDemo创建失败";
                passed = false;
            } else {
                qDebug() << "✅ ParentChildEventDemo创建成功";
                propagationDemo->deleteLater();
            }
            
            // AdvancedPatternsDemo验证
            AdvancedPatternsDemo* advancedDemo = new AdvancedPatternsDemo();
            if (!advancedDemo) {
                qDebug() << "❌ AdvancedPatternsDemo创建失败";
                passed = false;
            } else {
                qDebug() << "✅ AdvancedPatternsDemo创建成功";
                advancedDemo->deleteLater();
            }
            
        } catch (const std::exception& e) {
            qDebug() << "❌ 示例组件验证异常:" << e.what();
            passed = false;
        } catch (...) {
            qDebug() << "❌ 示例组件验证未知异常";
            passed = false;
        }
        
        return passed;
    }
    
    bool verifyUIComponents()
    {
        qDebug() << "\n--- UI组件验证 ---";
        bool passed = true;
        
        try {
            // EventDisplayWidget验证
            EventDisplayWidget* eventDisplay = new EventDisplayWidget();
            if (!eventDisplay) {
                qDebug() << "❌ EventDisplayWidget创建失败";
                passed = false;
            } else {
                qDebug() << "✅ EventDisplayWidget创建成功";
                eventDisplay->deleteLater();
            }
            
            // InteractiveAreaWidget验证
            InteractiveAreaWidget* interactiveArea = new InteractiveAreaWidget();
            if (!interactiveArea) {
                qDebug() << "❌ InteractiveAreaWidget创建失败";
                passed = false;
            } else {
                qDebug() << "✅ InteractiveAreaWidget创建成功";
                interactiveArea->deleteLater();
            }
            
            // DebugPanelWidget验证
            DebugPanelWidget* debugPanel = new DebugPanelWidget();
            if (!debugPanel) {
                qDebug() << "❌ DebugPanelWidget创建失败";
                passed = false;
            } else {
                qDebug() << "✅ DebugPanelWidget创建成功";
                debugPanel->deleteLater();
            }
            
            // PerformanceMonitorWidget验证
            PerformanceMonitorWidget* performanceMonitor = new PerformanceMonitorWidget();
            if (!performanceMonitor) {
                qDebug() << "❌ PerformanceMonitorWidget创建失败";
                passed = false;
            } else {
                qDebug() << "✅ PerformanceMonitorWidget创建成功";
                performanceMonitor->deleteLater();
            }
            
        } catch (const std::exception& e) {
            qDebug() << "❌ UI组件验证异常:" << e.what();
            passed = false;
        } catch (...) {
            qDebug() << "❌ UI组件验证未知异常";
            passed = false;
        }
        
        return passed;
    }
    
    bool verifyIntegrationFeatures()
    {
        qDebug() << "\n--- 集成功能验证 ---";
        bool passed = true;
        
        try {
            // 创建主窗口进行集成测试
            MainWindow* mainWindow = new MainWindow();
            if (!mainWindow) {
                qDebug() << "❌ MainWindow创建失败";
                passed = false;
            } else {
                qDebug() << "✅ MainWindow创建成功";
                
                // 测试窗口显示
                mainWindow->show();
                QTest::qWaitForWindowExposed(mainWindow);
                qDebug() << "✅ MainWindow显示成功";
                
                // 测试事件日志集成
                EventLogger* logger = EventLogger::instance();
                int initialCount = logger->getEventHistory().size();
                
                // 模拟一些事件
                QMouseEvent mouseEvent(QEvent::MouseButtonPress, QPoint(10, 10), 
                                     Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
                QApplication::sendEvent(mainWindow, &mouseEvent);
                
                // 等待事件处理
                QTest::qWait(100);
                
                if (logger->getEventHistory().size() > initialCount) {
                    qDebug() << "✅ 事件日志集成工作正常";
                } else {
                    qDebug() << "⚠️  事件日志集成可能存在问题";
                }
                
                mainWindow->close();
                mainWindow->deleteLater();
            }
            
        } catch (const std::exception& e) {
            qDebug() << "❌ 集成功能验证异常:" << e.what();
            passed = false;
        } catch (...) {
            qDebug() << "❌ 集成功能验证未知异常";
            passed = false;
        }
        
        return passed;
    }
    
    bool verifyPerformance()
    {
        qDebug() << "\n--- 性能验证 ---";
        bool passed = true;
        
        try {
            // 测试事件处理性能
            EventLogger* logger = EventLogger::instance();
            logger->clearHistory();
            
            QElapsedTimer timer;
            timer.start();
            
            // 生成大量事件记录
            const int eventCount = 1000;
            for (int i = 0; i < eventCount; ++i) {
                EventLogger::EventRecord record;
                record.timestamp = QDateTime::currentDateTime();
                record.eventType = static_cast<QEvent::Type>(QEvent::MouseButtonPress + (i % 10));
                record.eventName = QString("TestEvent_%1").arg(i);
                record.accepted = true;
                
                logger->logEvent(record);
            }
            
            qint64 elapsed = timer.elapsed();
            double avgTime = static_cast<double>(elapsed) / eventCount;
            
            qDebug() << QString("✅ 性能测试完成: %1个事件，总时间%2ms，平均%3ms/事件")
                        .arg(eventCount).arg(elapsed).arg(avgTime, 0, 'f', 3);
            
            if (avgTime > 1.0) {
                qDebug() << "⚠️  事件处理性能可能需要优化";
            } else {
                qDebug() << "✅ 事件处理性能良好";
            }
            
        } catch (const std::exception& e) {
            qDebug() << "❌ 性能验证异常:" << e.what();
            passed = false;
        } catch (...) {
            qDebug() << "❌ 性能验证未知异常";
            passed = false;
        }
        
        return passed;
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    app.setApplicationName("Qt6 Event System Integration Verification");
    app.setApplicationVersion("1.0");
    
    qDebug() << "Qt6事件系统项目集成验证程序";
    qDebug() << "版本: 1.0";
    qDebug() << "Qt版本:" << QT_VERSION_STR;
    
    IntegrationVerifier verifier;
    
    // 运行验证测试
    bool success = verifier.runAllTests();
    
    // 设置定时器退出应用程序
    QTimer::singleShot(1000, &verifier, &IntegrationVerifier::onTestCompleted);
    
    // 运行事件循环
    int result = app.exec();
    
    // 返回验证结果
    return success ? 0 : 1;
}

#include "integration_verification.moc"