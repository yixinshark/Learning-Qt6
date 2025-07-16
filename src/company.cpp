#include "company.h"
#include <QDebug>
#include <QMetaEnum>
#include <algorithm>
#include <numeric>

/**
 * @brief Company类的实现
 * 
 * 这个类展示了更复杂的元对象系统使用场景，
 * 包括对象组合、信号槽网络、枚举类型等。
 */

Company::Company(QObject *parent)
    : QObject(parent)
    , m_name("未命名公司")
    , m_address("未知地址")
    , m_type(Other)
{
    qDebug() << "公司对象创建:" << this;
    updateDynamicProperties();
}

Company::Company(const QString &name, const QString &address, QObject *parent)
    : QObject(parent)
    , m_name(name)
    , m_address(address)
    , m_type(Technology)  // 默认为科技公司
{
    qDebug() << "公司创建:" << m_name << "地址:" << m_address;
    updateDynamicProperties();
}

void Company::setName(const QString &name)
{
    if (m_name != name) {
        QString oldName = m_name;
        m_name = name;
        
        qDebug() << "公司名称从" << oldName << "变更为" << m_name;
        emit nameChanged(name);
        updateDynamicProperties();
    }
}

void Company::setAddress(const QString &address)
{
    if (m_address != address) {
        m_address = address;
        qDebug() << m_name << "的地址变更为:" << m_address;
        emit addressChanged(address);
        updateDynamicProperties();
    }
}

void Company::setType(CompanyType type)
{
    if (m_type != type) {
        CompanyType oldType = m_type;
        m_type = type;
        
        qDebug() << m_name << "的类型从" << typeToString(oldType) 
                 << "变更为" << typeToString(type);
        emit typeChanged(type);
        updateDynamicProperties();
    }
}

double Company::totalSalary() const
{
    /**
     * 计算所有员工的总薪水
     * 使用STL算法进行函数式编程
     */
    return std::accumulate(m_employees.begin(), m_employees.end(), 0.0,
                          [](double sum, const Person *person) {
                              return sum + person->salary();
                          });
}

double Company::averageSalary() const
{
    /**
     * 计算平均薪水
     * 这是一个计算属性，不存储值
     */
    if (m_employees.isEmpty()) {
        return 0.0;
    }
    return totalSalary() / m_employees.size();
}

void Company::addEmployee(Person *person)
{
    /**
     * 添加员工到公司
     * 建立信号槽连接，实现员工状态监控
     */
    if (!person || m_employees.contains(person)) {
        return;
    }
    
    // 设置父对象关系（对象树管理）
    person->setParent(this);
    
    m_employees.append(person);
    
    // 连接员工的信号到公司的槽
    connectEmployeeSignals(person);
    
    qDebug() << "员工" << person->name() << "加入了" << m_name;
    
    // 发射相关信号
    emit employeeAdded(person);
    emit employeeCountChanged(employeeCount());
    emit totalSalaryChanged(totalSalary());
    
    updateDynamicProperties();
}

void Company::removeEmployee(Person *person)
{
    /**
     * 从公司移除员工
     */
    if (!person || !m_employees.contains(person)) {
        return;
    }
    
    // 断开信号槽连接
    disconnectEmployeeSignals(person);
    
    m_employees.removeOne(person);
    
    // 清除父对象关系
    person->setParent(nullptr);
    
    qDebug() << "员工" << person->name() << "离开了" << m_name;
    
    emit employeeRemoved(person);
    emit employeeCountChanged(employeeCount());
    emit totalSalaryChanged(totalSalary());
    
    updateDynamicProperties();
}

Person* Company::findEmployee(const QString &name) const
{
    /**
     * 按姓名查找员工
     * 使用STL算法进行查找
     */
    auto it = std::find_if(m_employees.begin(), m_employees.end(),
                          [&name](const Person *person) {
                              return person->name() == name;
                          });
    
    return (it != m_employees.end()) ? *it : nullptr;
}

QStringList Company::getEmployeeNames() const
{
    /**
     * 获取所有员工姓名列表
     */
    QStringList names;
    for (const Person *person : m_employees) {
        names << person->name();
    }
    return names;
}

QString Company::getCompanyInfo() const
{
    /**
     * 获取公司详细信息
     * 演示复杂的字符串格式化
     */
    QString info = QString("=== %1 公司信息 ===\n").arg(m_name);
    info += QString("地址: %1\n").arg(m_address);
    info += QString("类型: %1\n").arg(typeToString(m_type));
    info += QString("员工数量: %1\n").arg(employeeCount());
    info += QString("总薪水: %.2f\n").arg(totalSalary());
    info += QString("平均薪水: %.2f\n").arg(averageSalary());
    
    info += "\n员工列表:\n";
    for (int i = 0; i < m_employees.size(); ++i) {
        const Person *person = m_employees[i];
        info += QString("%1. %2\n").arg(i + 1).arg(person->getInfo());
    }
    
    return info;
}

void Company::giveRaiseToAll(double percentage)
{
    /**
     * 给所有员工加薪
     * 演示批量操作和信号的级联效应
     */
    if (percentage <= 0 || m_employees.isEmpty()) {
        return;
    }
    
    qDebug() << "🎉" << m_name << "给所有员工加薪" << (percentage * 100) << "%！";
    
    for (Person *person : m_employees) {
        person->giveRaise(percentage);
    }
    
    emit massRaiseGiven(percentage);
    qDebug() << "全员加薪完成！新的总薪水:" << totalSalary();
}

void Company::celebrateAllBirthdays()
{
    /**
     * 为所有员工庆祝生日
     */
    qDebug() << "🎂" << m_name << "为所有员工庆祝生日！";
    
    for (Person *person : m_employees) {
        person->celebrateBirthday();
    }
    
    qDebug() << "集体生日庆祝完成！";
}

void Company::resetAllEmployees()
{
    /**
     * 重置所有员工信息
     */
    qDebug() << "重置" << m_name << "所有员工信息";
    
    for (Person *person : m_employees) {
        person->reset();
    }
    
    qDebug() << "所有员工信息重置完成";
}

// 槽函数实现
void Company::onEmployeeStatusChanged(const QString &status)
{
    /**
     * 响应员工状态变化
     * 这个槽连接到每个员工的statusChanged信号
     */
    Person *employee = qobject_cast<Person*>(sender());
    if (employee) {
        qDebug() << "公司收到员工状态更新:" << status;
        
        // 可以在这里添加更多的业务逻辑
        // 比如更新公司统计信息、发送通知等
        updateStatistics();
    }
}

void Company::onEmployeeSalaryRaised(double oldSalary, double newSalary)
{
    /**
     * 响应员工加薪事件
     */
    Person *employee = qobject_cast<Person*>(sender());
    if (employee) {
        qDebug() << "公司记录:" << employee->name() 
                 << "薪水从" << oldSalary << "涨到" << newSalary;
        
        // 检查是否需要升职
        if (newSalary > 10000) {
            qDebug() << employee->name() << "薪水超过10000，考虑升职！";
            emit employeePromoted(employee);
        }
        
        emit totalSalaryChanged(totalSalary());
    }
}

void Company::onEmployeeBecameAdult()
{
    /**
     * 响应员工成年事件
     */
    Person *employee = qobject_cast<Person*>(sender());
    if (employee) {
        qDebug() << "恭喜" << employee->name() << "成年了！公司将提供更多机会。";
    }
}

void Company::generateReport()
{
    /**
     * 生成公司报告
     */
    QString report = getCompanyInfo();
    
    // 添加统计分析
    report += "\n=== 统计分析 ===\n";
    
    int adultCount = std::count_if(m_employees.begin(), m_employees.end(),
                                  [](const Person *person) {
                                      return person->isAdult();
                                  });
    
    report += QString("成年员工: %1/%2\n").arg(adultCount).arg(employeeCount());
    
    if (!m_employees.isEmpty()) {
        auto minMaxSalary = std::minmax_element(m_employees.begin(), m_employees.end(),
                                               [](const Person *a, const Person *b) {
                                                   return a->salary() < b->salary();
                                               });
        
        report += QString("最低薪水: %.2f (%1)\n")
                 .arg((*minMaxSalary.first)->name())
                 .arg((*minMaxSalary.first)->salary());
        
        report += QString("最高薪水: %.2f (%1)\n")
                 .arg((*minMaxSalary.second)->name())
                 .arg((*minMaxSalary.second)->salary());
    }
    
    qDebug() << "生成公司报告:\n" << report;
    emit reportGenerated(report);
}

void Company::updateStatistics()
{
    /**
     * 更新公司统计信息
     */
    // 这里可以更新各种统计数据
    // 比如员工年龄分布、薪水分布等
    
    emit statisticsUpdated();
}

void Company::onEmployeeDestroyed(QObject *obj)
{
    /**
     * 处理员工对象被销毁的情况
     */
    Person *person = static_cast<Person*>(obj);
    if (m_employees.contains(person)) {
        qDebug() << "员工对象被销毁，从公司列表中移除";
        m_employees.removeOne(person);
        emit employeeCountChanged(employeeCount());
        emit totalSalaryChanged(totalSalary());
    }
}

// 私有辅助方法
void Company::connectEmployeeSignals(Person *person)
{
    /**
     * 连接员工的信号到公司的槽
     * 建立完整的信号槽网络
     */
    
    // 连接状态变化信号
    connect(person, &Person::statusChanged, 
            this, &Company::onEmployeeStatusChanged);
    
    // 连接薪水变化信号
    connect(person, &Person::salaryRaised, 
            this, &Company::onEmployeeSalaryRaised);
    
    // 连接成年信号
    connect(person, &Person::becameAdult, 
            this, &Company::onEmployeeBecameAdult);
    
    // 连接对象销毁信号
    connect(person, &QObject::destroyed, 
            this, &Company::onEmployeeDestroyed);
    
    // 连接其他属性变化信号到统计更新
    connect(person, &Person::ageChanged, 
            this, &Company::updateStatistics);
    connect(person, &Person::salaryChanged, 
            this, &Company::updateStatistics);
}

void Company::disconnectEmployeeSignals(Person *person)
{
    /**
     * 断开员工信号连接
     */
    disconnect(person, nullptr, this, nullptr);
}

QString Company::typeToString(CompanyType type) const
{
    /**
     * 将枚举类型转换为字符串
     * 使用Qt的元对象系统获取枚举名称
     */
    const QMetaObject *metaObj = this->metaObject();
    QMetaEnum metaEnum = metaObj->enumerator(metaObj->indexOfEnumerator("CompanyType"));
    return QString::fromLatin1(metaEnum.valueToKey(type));
}

void Company::updateDynamicProperties()
{
    /**
     * 更新动态属性
     * 演示动态属性的使用
     */
    
    // 设置一些动态属性
    setProperty("foundedYear", 2020);
    setProperty("isPublicCompany", false);
    setProperty("stockSymbol", "");
    
    // 根据公司类型设置不同的属性
    switch (m_type) {
    case Technology:
        setProperty("industry", "科技");
        setProperty("innovationLevel", "高");
        break;
    case Finance:
        setProperty("industry", "金融");
        setProperty("riskLevel", "中等");
        break;
    case Manufacturing:
        setProperty("industry", "制造业");
        setProperty("productionCapacity", "大");
        break;
    case Service:
        setProperty("industry", "服务业");
        setProperty("serviceQuality", "优秀");
        break;
    case Other:
        setProperty("industry", "其他");
        break;
    }
    
    // 根据员工数量设置公司规模
    QString scale;
    int count = employeeCount();
    if (count < 10) {
        scale = "小型";
    } else if (count < 100) {
        scale = "中型";
    } else {
        scale = "大型";
    }
    setProperty("companyScale", scale);
}

/**
 * 重要说明：
 * 
 * Company类展示了元对象系统的高级用法：
 * 
 * 1. 枚举类型的元对象支持 (Q_ENUM)
 * 2. 复杂的信号槽网络
 * 3. 对象组合和生命周期管理
 * 4. 动态属性的使用
 * 5. 计算属性的实现
 * 6. 批量操作和事件传播
 * 
 * MOC会为这个类生成相应的元对象代码，
 * 包括枚举的元信息、信号槽的调用代码等。
 */