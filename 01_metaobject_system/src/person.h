#ifndef PERSON_H
#define PERSON_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QTimer>

/**
 * @brief Person类 - Qt元对象系统演示类
 * 
 * 这个类展示了Qt元对象系统的核心特性：
 * 1. Q_OBJECT宏 - 启用元对象功能
 * 2. Q_PROPERTY - 属性系统
 * 3. signals - 信号机制
 * 4. slots - 槽机制
 * 5. Q_INVOKABLE - 可调用方法标记
 * 
 * MOC(Meta-Object Compiler)会为这个类生成：
 * - moc_person.cpp文件
 * - 静态元对象数据表
 * - 信号发射函数
 * - 元调用分发函数
 */
class Person : public QObject
{
    Q_OBJECT  // 元对象宏 - 告诉MOC这个类需要元对象支持
    
    // 属性声明 - 这些宏会被MOC解析并生成相应的元数据
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int age READ age WRITE setAge NOTIFY ageChanged)
    Q_PROPERTY(bool isAdult READ isAdult STORED false)  // 计算属性，不存储
    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY emailChanged)
    Q_PROPERTY(double salary READ salary WRITE setSalary NOTIFY salaryChanged)

public:
    /**
     * @brief 构造函数
     * @param parent 父对象指针，用于Qt的对象树管理
     */
    explicit Person(QObject *parent = nullptr);
    
    /**
     * @brief 带参数的构造函数
     * @param name 姓名
     * @param age 年龄
     * @param parent 父对象
     */
    Person(const QString &name, int age, QObject *parent = nullptr);
    
    // 属性访问器 - 这些方法会被MOC记录在元对象数据中
    QString name() const { return m_name; }
    void setName(const QString &name);
    
    int age() const { return m_age; }
    void setAge(int age);
    
    // 计算属性 - 基于其他属性计算得出，不直接存储
    bool isAdult() const { return m_age >= 18; }
    
    QString email() const { return m_email; }
    void setEmail(const QString &email);
    
    double salary() const { return m_salary; }
    void setSalary(double salary);
    
    /**
     * @brief Q_INVOKABLE标记的方法可以通过元对象系统调用
     * 这意味着可以通过QMetaObject::invokeMethod()来调用这些方法
     */
    Q_INVOKABLE void introduce() const;
    Q_INVOKABLE QString getInfo() const;
    Q_INVOKABLE void celebrateBirthday();
    Q_INVOKABLE void giveRaise(double percentage);

public slots:
    /**
     * @brief 槽函数 - 可以连接到信号
     * 槽函数会被MOC记录，可以通过字符串名称连接
     */
    void onNameChanged(const QString &newName);
    void onAgeChanged(int newAge);
    void reset();
    void startAging(); // 演示定时器和信号槽
    void stopAging();

signals:
    /**
     * @brief 信号声明 - MOC会为每个信号生成发射函数
     * 信号不需要实现，MOC会自动生成实现代码
     */
    void nameChanged(const QString &newName);
    void ageChanged(int newAge);
    void emailChanged(const QString &newEmail);
    void salaryChanged(double newSalary);
    void birthdayCelebrated(int newAge);
    void salaryRaised(double oldSalary, double newSalary);
    
    // 状态变化信号
    void becameAdult();  // 当从未成年变为成年时发射
    void statusChanged(const QString &status);

private slots:
    /**
     * @brief 私有槽 - 内部使用的槽函数
     */
    void onTimerTimeout();

private:
    // 成员变量 - 存储对象状态
    QString m_name;
    int m_age;
    QString m_email;
    double m_salary;
    
    // 用于演示定时器功能
    QTimer *m_agingTimer;
    
    // 辅助方法
    void setupConnections();  // 设置内部信号槽连接
    void emitStatusChange();  // 发射状态变化信号
};

#endif // PERSON_H