#ifndef COMPANY_H
#define COMPANY_H

#include <QObject>
#include <QString>
#include <QList>
#include <QVariant>
#include "person.h"

/**
 * @brief Company类 - 演示复杂的元对象系统使用
 * 
 * 这个类展示了：
 * 1. 对象组合和信号槽连接
 * 2. 自定义类型的属性
 * 3. 对象列表管理
 * 4. 复杂的信号槽网络
 * 5. 动态属性的使用
 */
class Company : public QObject
{
    Q_OBJECT
    
    // 基本属性
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(int employeeCount READ employeeCount NOTIFY employeeCountChanged)
    Q_PROPERTY(double totalSalary READ totalSalary NOTIFY totalSalaryChanged)
    Q_PROPERTY(double averageSalary READ averageSalary STORED false)  // 计算属性
    
public:
    /**
     * @brief 公司类型枚举
     * Q_ENUM宏使得枚举值可以在元对象系统中使用
     */
    enum CompanyType {
        Technology,     // 科技公司
        Finance,        // 金融公司
        Manufacturing,  // 制造业
        Service,        // 服务业
        Other          // 其他
    };
    
    // 枚举类型演示 - 必须在枚举定义之后
    Q_ENUM(CompanyType)
    
private:
    // 属性声明
    Q_PROPERTY(CompanyType type READ type WRITE setType NOTIFY typeChanged)
    
public:

    explicit Company(QObject *parent = nullptr);
    Company(const QString &name, const QString &address, QObject *parent = nullptr);
    
    // 基本属性访问器
    QString name() const { return m_name; }
    void setName(const QString &name);
    
    QString address() const { return m_address; }
    void setAddress(const QString &address);
    
    CompanyType type() const { return m_type; }
    void setType(CompanyType type);
    
    // 计算属性
    int employeeCount() const { return m_employees.size(); }
    double totalSalary() const;
    double averageSalary() const;
    
    // 员工管理方法
    Q_INVOKABLE void addEmployee(Person *person);
    Q_INVOKABLE void removeEmployee(Person *person);
    Q_INVOKABLE Person* findEmployee(const QString &name) const;
    Q_INVOKABLE QStringList getEmployeeNames() const;
    Q_INVOKABLE QString getCompanyInfo() const;
    
    // 批量操作
    Q_INVOKABLE void giveRaiseToAll(double percentage);
    Q_INVOKABLE void celebrateAllBirthdays();
    Q_INVOKABLE void resetAllEmployees();

public slots:
    // 公司级别的槽函数
    void onEmployeeStatusChanged(const QString &status);
    void onEmployeeSalaryRaised(double oldSalary, double newSalary);
    void onEmployeeBecameAdult();
    void generateReport();
    void updateStatistics();

signals:
    // 公司属性变化信号
    void nameChanged(const QString &newName);
    void addressChanged(const QString &newAddress);
    void typeChanged(CompanyType newType);
    void employeeCountChanged(int count);
    void totalSalaryChanged(double total);
    
    // 员工相关信号
    void employeeAdded(Person *person);
    void employeeRemoved(Person *person);
    void employeePromoted(Person *person);
    
    // 公司事件信号
    void massRaiseGiven(double percentage);
    void reportGenerated(const QString &report);
    void statisticsUpdated();

private slots:
    // 内部槽函数
    void onEmployeeDestroyed(QObject *obj);

private:
    QString m_name;
    QString m_address;
    CompanyType m_type;
    QList<Person*> m_employees;
    
    // 辅助方法
    void connectEmployeeSignals(Person *person);
    void disconnectEmployeeSignals(Person *person);
    QString typeToString(CompanyType type) const;
    void updateDynamicProperties();
};

// 注册自定义类型到元对象系统
Q_DECLARE_METATYPE(Company::CompanyType)

#endif // COMPANY_H