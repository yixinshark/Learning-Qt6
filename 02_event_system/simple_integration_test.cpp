#include <QApplication>
#include <QDebug>
#include <QTimer>
#include <iostream>

// 包含核心组件
#include "src/core/event_manager.h"
#include "src/core/event_logger.h"
#include "src/core/custom_events.h"

// 包含主窗口
#include "src/widgets/main_window.h"

/**
 * 简单的集成测试程序
 * 验证核心功能是否正常工作
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    std::cout << "=== Qt6事件系统简单集成测试 ===" << std::endl;
    
    bool allPassed = true;
    
    try {
        // 1. 测试核心组件
        std::cout << "\n1. 测试核心组件..." << std::endl;
        
        EventManager* eventManager = EventManager::instance();
        if (eventManager) {
            std::cout << "✅ EventManager创建成功" << std::endl;
        } else {
            std::cout << "❌ EventManager创建失败" << std::endl;
            allPassed = false;
        }
        
        EventLogger* eventLogger = EventLogger::instance();
        if (eventLogger) {
            std::cout << "✅ EventLogger创建成功" << std::endl;
        } else {
            std::cout << "❌ EventLogger创建失败" << std::endl;
            allPassed = false;
        }
        
        // 2. 测试自定义事件
        std::cout << "\n2. 测试自定义事件..." << std::endl;
        
        DataEvent* dataEvent = new DataEvent(QVariant("test data"));
        if (dataEvent && dataEvent->data().toString() == "test data") {
            std::cout << "✅ 自定义事件创建成功" << std::endl;
        } else {
            std::cout << "❌ 自定义事件创建失败" << std::endl;
            allPassed = false;
        }
        delete dataEvent;
        
        // 3. 测试主窗口创建
        std::cout << "\n3. 测试主窗口创建..." << std::endl;
        
        MainWindow* mainWindow = new MainWindow();
        if (mainWindow) {
            std::cout << "✅ MainWindow创建成功" << std::endl;
            
            // 简单显示测试
            mainWindow->show();
            std::cout << "✅ MainWindow显示成功" << std::endl;
            
            // 立即关闭
            mainWindow->close();
            mainWindow->deleteLater();
        } else {
            std::cout << "❌ MainWindow创建失败" << std::endl;
            allPassed = false;
        }
        
        // 4. 测试事件记录
        std::cout << "\n4. 测试事件记录..." << std::endl;
        
        int initialCount = eventLogger->getEventHistory().size();
        
        EventLogger::EventRecord record;
        record.timestamp = QDateTime::currentDateTime();
        record.eventType = QEvent::MouseButtonPress;
        record.eventName = "TestEvent";
        record.accepted = true;
        
        eventLogger->logEvent(record);
        
        if (eventLogger->getEventHistory().size() == initialCount + 1) {
            std::cout << "✅ 事件记录功能正常" << std::endl;
        } else {
            std::cout << "❌ 事件记录功能异常" << std::endl;
            allPassed = false;
        }
        
    } catch (const std::exception& e) {
        std::cout << "❌ 测试过程中发生异常: " << e.what() << std::endl;
        allPassed = false;
    } catch (...) {
        std::cout << "❌ 测试过程中发生未知异常" << std::endl;
        allPassed = false;
    }
    
    std::cout << "\n=== 测试结果 ===" << std::endl;
    if (allPassed) {
        std::cout << "✅ 所有测试通过！项目集成成功。" << std::endl;
    } else {
        std::cout << "❌ 部分测试失败，请检查项目配置。" << std::endl;
    }
    
    return allPassed ? 0 : 1;
}