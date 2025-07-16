#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <QDir>
#include <QFileInfo>
#include "demo_runner.h"
#include "person.h"
#include "company.h"

/**
 * @brief Qt6 元对象系统演示程序主入口
 * 
 * 这个程序展示了Qt元对象系统的各种特性和用法，
 * 包括信号槽、属性系统、反射、动态调用等。
 * 
 * 编译后，MOC生成的文件会被拷贝到src/generated目录，
 * 可以查看这些文件来理解元对象系统的内部实现。
 */

void printWelcomeMessage()
{
    qDebug() << R"(
╔══════════════════════════════════════════════════════════════╗
║                Qt6 元对象系统深度演示程序                      ║
║                                                              ║
║  本程序演示以下Qt元对象系统特性:                               ║
║  • 信号槽机制 (Signals & Slots)                              ║
║  • 属性系统 (Property System)                                ║
║  • 反射和内省 (Reflection & Introspection)                   ║
║  • 动态方法调用 (Dynamic Method Invocation)                  ║
║  • 元对象编译器 (MOC) 生成的代码分析                          ║
║  • 性能测试和最佳实践                                         ║
║                                                              ║
║  编译完成后，请查看 src/generated/ 目录中MOC生成的文件        ║
╚══════════════════════════════════════════════════════════════╝
    )";
}

void printMocFileInfo()
{
    qDebug() << "\n=== MOC生成文件信息 ===";
    
    QDir generatedDir("src/generated");
    if (generatedDir.exists()) {
        qDebug() << "MOC生成的文件位置: src/generated/";
        
        QStringList filters;
        filters << "moc_*.cpp" << "*.moc";
        
        QFileInfoList files = generatedDir.entryInfoList(filters, QDir::Files);
        for (const QFileInfo &fileInfo : files) {
            qDebug() << "  -" << fileInfo.fileName() 
                     << "(" << fileInfo.size() << "字节)";
        }
        
        if (files.isEmpty()) {
            qDebug() << "  注意: MOC文件将在编译完成后生成";
        }
    } else {
        qDebug() << "  MOC生成目录尚未创建，将在编译后生成";
    }
}

void demonstrateBasicMetaObjectFeatures()
{
    qDebug() << "\n=== 基础元对象特性演示 ===";
    
    // 创建对象
    Person person("演示用户", 25);
    
    // 获取元对象信息
    const QMetaObject *metaObj = person.metaObject();
    qDebug() << "类名:" << metaObj->className();
    qDebug() << "父类:" << (metaObj->superClass() ? metaObj->superClass()->className() : "无");
    
    // 检查继承关系
    qDebug() << "是否继承自QObject:" << person.inherits("QObject");
    qDebug() << "是否是Person类型:" << (qobject_cast<Person*>(&person) != nullptr);
    
    // 动态属性演示
    person.setProperty("customProperty", "这是一个动态属性");
    qDebug() << "动态属性值:" << person.property("customProperty").toString();
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    // 设置应用程序信息
    app.setApplicationName("Qt6 MetaObject System Demo");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Qt Learning");
    
    // 打印欢迎信息
    printWelcomeMessage();
    
    // 显示MOC文件信息
    printMocFileInfo();
    
    // 基础特性演示
    demonstrateBasicMetaObjectFeatures();
    
    // 创建演示运行器
    DemoRunner *demoRunner = new DemoRunner(&app);
    
    // 设置程序退出条件
    QObject::connect(demoRunner, &DemoRunner::allDemosCompleted, [&app]() {
        qDebug() << "\n🎉 所有演示完成！程序将在3秒后退出...";
        QTimer::singleShot(3000, &app, &QCoreApplication::quit);
    });
    
    // 延迟启动演示，给用户时间阅读欢迎信息
    QTimer::singleShot(2000, [demoRunner]() {
        qDebug() << "\n⏰ 演示即将开始...";
        demoRunner->runAllDemos();
    });
    
    // 运行事件循环
    int result = app.exec();
    
    qDebug() << "\n👋 感谢使用Qt6元对象系统演示程序！";
    qDebug() << "请查看生成的MOC文件以深入理解元对象系统的实现原理。";
    
    return result;
}

/**
 * 编译说明:
 * 
 * 1. 确保已安装Qt6开发环境
 * 2. 在项目根目录执行:
 *    mkdir build && cd build
 *    cmake ..
 *    make (或 cmake --build .)
 * 
 * 3. 运行程序:
 *    ./Qt6MetaObjectDemo
 * 
 * 4. 查看MOC生成的文件:
 *    ls -la src/generated/
 * 
 * MOC文件分析:
 * - moc_person.cpp: Person类的元对象实现
 * - moc_company.cpp: Company类的元对象实现  
 * - moc_demo_runner.cpp: DemoRunner类的元对象实现
 * 
 * 这些文件包含:
 * - 静态元对象数据表
 * - 信号发射函数
 * - 属性访问函数
 * - 方法调用分发函数
 * - 字符串表和索引表
 */