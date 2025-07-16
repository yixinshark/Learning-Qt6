#ifndef DEMO_RUNNER_H
#define DEMO_RUNNER_H

#include <QObject>
#include <QTimer>
#include <QMetaObject>
#include <QMetaMethod>
#include <QMetaProperty>
#include "person.h"
#include "company.h"

/**
 * @brief DemoRunner类 - 元对象系统演示控制器
 * 
 * 这个类负责运行各种元对象系统的演示，包括：
 * 1. 基本的信号槽演示
 * 2. 属性系统演示
 * 3. 反射和内省演示
 * 4. 动态方法调用演示
 * 5. 复杂对象交互演示
 */
class DemoRunner : public QObject
{
    Q_OBJECT
    
public:
    explicit DemoRunner(QObject *parent = nullptr);
    ~DemoRunner();
    
    // 演示方法
    Q_INVOKABLE void runAllDemos();
    Q_INVOKABLE void runBasicSignalSlotDemo();
    Q_INVOKABLE void runPropertySystemDemo();
    Q_INVOKABLE void runReflectionDemo();
    Q_INVOKABLE void runDynamicInvocationDemo();
    Q_INVOKABLE void runCompanyDemo();
    Q_INVOKABLE void runMetaObjectAnalysis();
    Q_INVOKABLE void runPerformanceTest();

public slots:
    void startDemo();
    void stopDemo();
    void nextDemo();

signals:
    void demoStarted(const QString &demoName);
    void demoFinished(const QString &demoName);
    void allDemosCompleted();

private slots:
    void onDemoTimer();
    void onPersonStatusChanged(const QString &status);
    void onCompanyReportGenerated(const QString &report);

private:
    Person *m_person1;
    Person *m_person2;
    Company *m_company;
    QTimer *m_demoTimer;
    int m_currentDemo;
    
    // 辅助方法
    void setupObjects();
    void cleanupObjects();
    void printSeparator(const QString &title);
    void analyzeMetaObject(const QMetaObject *metaObj);
    void demonstratePropertyAccess(QObject *obj);
    void demonstrateMethodInvocation(QObject *obj);
    void measurePerformance();
};

#endif // DEMO_RUNNER_H