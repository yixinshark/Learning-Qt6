#include <QApplication>
#include <QDebug>

#include "widgets/main_window.h"
#include "core/event_manager.h"
#include "core/event_logger.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    app.setApplicationName("Qt6 Event System Demo");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("Qt Learning Project");
    
    qDebug() << "Starting Qt6 Event System Demo...";
    
    // 初始化核心组件
    EventManager* eventManager = EventManager::instance();
    EventLogger* eventLogger = EventLogger::instance();
    Q_UNUSED(eventManager)
    Q_UNUSED(eventLogger)
    
    // 创建并显示主窗口
    MainWindow window;
    window.show();
    
    qDebug() << "Qt6 Event System Demo started successfully";
    
    return app.exec();
}