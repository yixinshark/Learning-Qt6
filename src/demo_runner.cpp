#include "demo_runner.h"
#include <QDebug>
#include <QElapsedTimer>
#include <QMetaEnum>
#include <QVariant>
#include <QCoreApplication>

/**
 * @brief DemoRunner类的实现
 * 
 * 这个类提供了完整的Qt元对象系统演示，
 * 通过实际的代码执行来展示各种特性。
 */

DemoRunner::DemoRunner(QObject *parent)
    : QObject(parent)
    , m_person1(nullptr)
    , m_person2(nullptr)
    , m_company(nullptr)
    , m_demoTimer(new QTimer(this))
    , m_currentDemo(0)
{
    qDebug() << "=== Qt6 元对象系统演示程序启动 ===";
    
    // 设置演示定时器
    m_demoTimer->setSingleShot(true);
    connect(m_demoTimer, &QTimer::timeout, this, &DemoRunner::onDemoTimer);
    
    setupObjects();
}

DemoRunner::~DemoRunner()
{
    cleanupObjects();
    qDebug() << "=== 演示程序结束 ===";
}

void DemoRunner::setupObjects()
{
    /**
     * 创建演示对象并建立连接
     */
    qDebug() << "正在创建演示对象...";
    
    // 创建Person对象
    m_person1 = new Person("张三", 25, this);
    m_person1->setEmail("zhangsan@example.com");
    m_person1->setSalary(8000.0);
    
    m_person2 = new Person("李四", 17, this);
    m_person2->setEmail("lisi@example.com");
    m_person2->setSalary(6000.0);
    
    // 创建Company对象
    m_company = new Company("创新科技有限公司", "北京市海淀区", this);
    m_company->setType(Company::Technology);
    
    // 建立信号槽连接
    connect(m_person1, &Person::statusChanged, 
            this, &DemoRunner::onPersonStatusChanged);
    connect(m_person2, &Person::statusChanged, 
            this, &DemoRunner::onPersonStatusChanged);
    connect(m_company, &Company::reportGenerated, 
            this, &DemoRunner::onCompanyReportGenerated);
    
    qDebug() << "演示对象创建完成";
}

void DemoRunner::cleanupObjects()
{
    /**
     * 清理演示对象
     * 由于使用了对象树，这里主要是显式清理
     */
    // 对象会通过父子关系自动清理
    qDebug() << "清理演示对象";
}

void DemoRunner::runAllDemos()
{
    /**
     * 运行所有演示
     */
    printSeparator("开始运行所有演示");
    
    runBasicSignalSlotDemo();
    runPropertySystemDemo();
    runReflectionDemo();
    runDynamicInvocationDemo();
    runCompanyDemo();
    runMetaObjectAnalysis();
    runPerformanceTest();
    
    emit allDemosCompleted();
    printSeparator("所有演示完成");
}

void DemoRunner::runBasicSignalSlotDemo()
{
    /**
     * 基本信号槽演示
     */
    printSeparator("基本信号槽演示");
    emit demoStarted("基本信号槽演示");
    
    qDebug() << "1. 测试属性变化信号";
    m_person1->setName("张三丰");
    m_person1->setAge(30);
    
    qDebug() << "\n2. 测试方法调用";
    m_person1->introduce();
    
    qDebug() << "\n3. 测试生日庆祝";
    m_person1->celebrateBirthday();
    
    qDebug() << "\n4. 测试成年转换";
    m_person2->setAge(18);  // 从17岁变为18岁，触发成年信号
    
    qDebug() << "\n5. 测试加薪";
    m_person1->giveRaise(0.15);  // 加薪15%
    
    emit demoFinished("基本信号槽演示");
}

void DemoRunner::runPropertySystemDemo()
{
    /**
     * 属性系统演示
     */
    printSeparator("属性系统演示");
    emit demoStarted("属性系统演示");
    
    qDebug() << "1. 通过属性系统访问对象属性";
    
    // 使用属性系统读取属性
    QString name = m_person1->property("name").toString();
    int age = m_person1->property("age").toInt();
    bool isAdult = m_person1->property("isAdult").toBool();
    
    qDebug() << "通过属性系统读取:";
    qDebug() << "  姓名:" << name;
    qDebug() << "  年龄:" << age;
    qDebug() << "  是否成年:" << (isAdult ? "是" : "否");
    
    qDebug() << "\n2. 通过属性系统修改属性";
    m_person1->setProperty("name", "王五");
    m_person1->setProperty("age", 35);
    m_person1->setProperty("salary", 12000.0);
    
    qDebug() << "修改后的信息:" << m_person1->getInfo();
    
    qDebug() << "\n3. 动态属性演示";
    m_person1->setProperty("department", "研发部");
    m_person1->setProperty("level", "高级工程师");
    m_person1->setProperty("workYears", 8);
    
    qDebug() << "动态属性:";
    qDebug() << "  部门:" << m_person1->property("department").toString();
    qDebug() << "  级别:" << m_person1->property("level").toString();
    qDebug() << "  工作年限:" << m_person1->property("workYears").toInt();
    
    emit demoFinished("属性系统演示");
}

void DemoRunner::runReflectionDemo()
{
    /**
     * 反射和内省演示
     */
    printSeparator("反射和内省演示");
    emit demoStarted("反射和内省演示");
    
    qDebug() << "1. 分析Person类的元对象信息";
    analyzeMetaObject(m_person1->metaObject());
    
    qDebug() << "\n2. 分析Company类的元对象信息";
    analyzeMetaObject(m_company->metaObject());
    
    qDebug() << "\n3. 演示属性访问";
    demonstratePropertyAccess(m_person1);
    
    qDebug() << "\n4. 演示方法调用";
    demonstrateMethodInvocation(m_person1);
    
    emit demoFinished("反射和内省演示");
}

void DemoRunner::runDynamicInvocationDemo()
{
    /**
     * 动态方法调用演示
     */
    printSeparator("动态方法调用演示");
    emit demoStarted("动态方法调用演示");
    
    qDebug() << "1. 通过字符串名称调用方法";
    
    // 调用无参数方法
    bool result1 = QMetaObject::invokeMethod(m_person1, "introduce");
    qDebug() << "调用introduce()方法:" << (result1 ? "成功" : "失败");
    
    // 调用带参数的方法
    bool result2 = QMetaObject::invokeMethod(m_person1, "setName", 
                                           Q_ARG(QString, "动态调用设置的名字"));
    qDebug() << "调用setName()方法:" << (result2 ? "成功" : "失败");
    
    // 调用带返回值的方法
    QString info;
    bool result3 = QMetaObject::invokeMethod(m_person1, "getInfo", 
                                           Q_RETURN_ARG(QString, info));
    qDebug() << "调用getInfo()方法:" << (result3 ? "成功" : "失败");
    qDebug() << "返回值:" << info;
    
    qDebug() << "\n2. 队列调用演示";
    bool result4 = QMetaObject::invokeMethod(m_person1, "celebrateBirthday", 
                                           Qt::QueuedConnection);
    qDebug() << "队列调用celebrateBirthday():" << (result4 ? "成功" : "失败");
    
    // 处理队列中的事件
    QCoreApplication::processEvents();
    
    emit demoFinished("动态方法调用演示");
}

void DemoRunner::runCompanyDemo()
{
    /**
     * 公司对象演示 - 复杂对象交互
     */
    printSeparator("公司对象演示");
    emit demoStarted("公司对象演示");
    
    qDebug() << "1. 向公司添加员工";
    m_company->addEmployee(m_person1);
    m_company->addEmployee(m_person2);
    
    // 创建更多员工
    Person *person3 = new Person("赵六", 28, m_company);
    person3->setEmail("zhaoliu@example.com");
    person3->setSalary(9500.0);
    m_company->addEmployee(person3);
    
    Person *person4 = new Person("孙七", 32, m_company);
    person4->setEmail("sunqi@example.com");
    person4->setSalary(11000.0);
    m_company->addEmployee(person4);
    
    qDebug() << "\n2. 显示公司信息";
    qDebug() << m_company->getCompanyInfo();
    
    qDebug() << "\n3. 全员加薪演示";
    m_company->giveRaiseToAll(0.1);  // 全员加薪10%
    
    qDebug() << "\n4. 集体生日庆祝";
    m_company->celebrateAllBirthdays();
    
    qDebug() << "\n5. 生成公司报告";
    m_company->generateReport();
    
    qDebug() << "\n6. 员工查找演示";
    Person *found = m_company->findEmployee("张三丰");
    if (found) {
        qDebug() << "找到员工:" << found->getInfo();
    }
    
    emit demoFinished("公司对象演示");
}

void DemoRunner::runMetaObjectAnalysis()
{
    /**
     * 元对象深度分析
     */
    printSeparator("元对象深度分析");
    emit demoStarted("元对象深度分析");
    
    const QMetaObject *personMeta = m_person1->metaObject();
    const QMetaObject *companyMeta = m_company->metaObject();
    
    qDebug() << "1. 类继承关系分析";
    qDebug() << "Person类继承链:";
    const QMetaObject *current = personMeta;
    int level = 0;
    while (current) {
        qDebug() << QString("  %1%2").arg(QString(level * 2, ' ')).arg(current->className());
        current = current->superClass();
        level++;
    }
    
    qDebug() << "\n2. 枚举类型分析";
    for (int i = 0; i < companyMeta->enumeratorCount(); ++i) {
        QMetaEnum metaEnum = companyMeta->enumerator(i);
        qDebug() << "枚举:" << metaEnum.name();
        for (int j = 0; j < metaEnum.keyCount(); ++j) {
            qDebug() << "  " << metaEnum.key(j) << "=" << metaEnum.value(j);
        }
    }
    
    qDebug() << "\n3. 类信息分析";
    for (int i = 0; i < companyMeta->classInfoCount(); ++i) {
        QMetaClassInfo classInfo = companyMeta->classInfo(i);
        qDebug() << "类信息:" << classInfo.name() << "=" << classInfo.value();
    }
    
    emit demoFinished("元对象深度分析");
}

void DemoRunner::runPerformanceTest()
{
    /**
     * 性能测试演示
     */
    printSeparator("性能测试演示");
    emit demoStarted("性能测试演示");
    
    measurePerformance();
    
    emit demoFinished("性能测试演示");
}

// 槽函数实现
void DemoRunner::startDemo()
{
    qDebug() << "开始演示...";
    m_currentDemo = 0;
    m_demoTimer->start(1000);  // 1秒后开始
}

void DemoRunner::stopDemo()
{
    qDebug() << "停止演示";
    m_demoTimer->stop();
}

void DemoRunner::nextDemo()
{
    m_currentDemo++;
    m_demoTimer->start(2000);  // 2秒后执行下一个演示
}

void DemoRunner::onDemoTimer()
{
    /**
     * 定时器驱动的演示执行
     */
    switch (m_currentDemo) {
    case 0:
        runBasicSignalSlotDemo();
        break;
    case 1:
        runPropertySystemDemo();
        break;
    case 2:
        runReflectionDemo();
        break;
    case 3:
        runDynamicInvocationDemo();
        break;
    case 4:
        runCompanyDemo();
        break;
    case 5:
        runMetaObjectAnalysis();
        break;
    case 6:
        runPerformanceTest();
        break;
    default:
        emit allDemosCompleted();
        return;
    }
    
    if (m_currentDemo < 6) {
        nextDemo();
    }
}

void DemoRunner::onPersonStatusChanged(const QString &status)
{
    qDebug() << "演示控制器收到人员状态变化:" << status;
}

void DemoRunner::onCompanyReportGenerated(const QString &report)
{
    qDebug() << "演示控制器收到公司报告:\n" << report;
}

// 私有辅助方法
void DemoRunner::printSeparator(const QString &title)
{
    qDebug() << "\n" << QString(60, '=');
    qDebug() << title;
    qDebug() << QString(60, '=');
}

void DemoRunner::analyzeMetaObject(const QMetaObject *metaObj)
{
    /**
     * 分析元对象的详细信息
     */
    qDebug() << "类名:" << metaObj->className();
    qDebug() << "方法数量:" << metaObj->methodCount();
    qDebug() << "属性数量:" << metaObj->propertyCount();
    qDebug() << "枚举数量:" << metaObj->enumeratorCount();
    
    qDebug() << "\n方法列表:";
    for (int i = 0; i < metaObj->methodCount(); ++i) {
        QMetaMethod method = metaObj->method(i);
        QString typeStr;
        switch (method.methodType()) {
        case QMetaMethod::Method:
            typeStr = "方法";
            break;
        case QMetaMethod::Signal:
            typeStr = "信号";
            break;
        case QMetaMethod::Slot:
            typeStr = "槽";
            break;
        case QMetaMethod::Constructor:
            typeStr = "构造函数";
            break;
        }
        
        qDebug() << QString("  [%1] %2: %3")
                   .arg(typeStr)
                   .arg(method.name().constData())
                   .arg(method.methodSignature().constData());
    }
    
    qDebug() << "\n属性列表:";
    for (int i = 0; i < metaObj->propertyCount(); ++i) {
        QMetaProperty prop = metaObj->property(i);
        qDebug() << QString("  %1 (%2) - 可读:%3 可写:%4 有通知:%5")
                   .arg(prop.name())
                   .arg(prop.typeName())
                   .arg(prop.isReadable() ? "是" : "否")
                   .arg(prop.isWritable() ? "是" : "否")
                   .arg(prop.hasNotifySignal() ? "是" : "否");
    }
}

void DemoRunner::demonstratePropertyAccess(QObject *obj)
{
    /**
     * 演示属性访问
     */
    const QMetaObject *metaObj = obj->metaObject();
    
    qDebug() << "遍历所有属性并读取值:";
    for (int i = 0; i < metaObj->propertyCount(); ++i) {
        QMetaProperty prop = metaObj->property(i);
        if (prop.isReadable()) {
            QVariant value = prop.read(obj);
            qDebug() << QString("  %1 = %2").arg(prop.name()).arg(value.toString());
        }
    }
}

void DemoRunner::demonstrateMethodInvocation(QObject *obj)
{
    /**
     * 演示方法调用
     */
    const QMetaObject *metaObj = obj->metaObject();
    
    qDebug() << "查找并调用可调用方法:";
    for (int i = 0; i < metaObj->methodCount(); ++i) {
        QMetaMethod method = metaObj->method(i);
        
        // 只调用标记为Q_INVOKABLE的方法，且无参数
        if (method.methodType() == QMetaMethod::Method && 
            method.parameterCount() == 0 &&
            QString(method.name()) == "introduce") {
            
            qDebug() << "调用方法:" << method.name().constData();
            method.invoke(obj);
        }
    }
}

void DemoRunner::measurePerformance()
{
    /**
     * 性能测试
     */
    const int iterations = 100000;
    QElapsedTimer timer;
    
    qDebug() << "性能测试 - 执行" << iterations << "次操作";
    
    // 1. 直接方法调用
    timer.start();
    for (int i = 0; i < iterations; ++i) {
        m_person1->name();
    }
    qint64 directTime = timer.elapsed();
    qDebug() << "直接方法调用:" << directTime << "毫秒";
    
    // 2. 属性系统调用
    timer.restart();
    for (int i = 0; i < iterations; ++i) {
        m_person1->property("name");
    }
    qint64 propertyTime = timer.elapsed();
    qDebug() << "属性系统调用:" << propertyTime << "毫秒";
    
    // 3. 反射方法调用
    timer.restart();
    for (int i = 0; i < iterations; ++i) {
        QString result;
        QMetaObject::invokeMethod(m_person1, "getInfo", Q_RETURN_ARG(QString, result));
    }
    qint64 reflectionTime = timer.elapsed();
    qDebug() << "反射方法调用:" << reflectionTime << "毫秒";
    
    // 性能比较
    qDebug() << "\n性能比较 (以直接调用为基准):";
    qDebug() << QString("属性系统调用慢 %1 倍").arg(double(propertyTime) / directTime, 0, 'f', 1);
    qDebug() << QString("反射调用慢 %1 倍").arg(double(reflectionTime) / directTime, 0, 'f', 1);
}