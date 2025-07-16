#include "person.h"
#include <QTimer>

/**
 * @brief Person类的实现
 * 
 * 这个文件展示了Qt元对象系统中各种特性的具体实现。
 * 当MOC处理person.h时，会生成moc_person.cpp文件，
 * 其中包含了信号的实现、元对象数据等。
 */

Person::Person(QObject *parent)
    : QObject(parent)
    , m_name("Unknown")
    , m_age(0)
    , m_email("")
    , m_salary(0.0)
    , m_agingTimer(new QTimer(this))  // 使用对象树管理内存
{
    setupConnections();
    qDebug() << "Person对象创建:" << this;
}

Person::Person(const QString &name, int age, QObject *parent)
    : QObject(parent)
    , m_name(name)
    , m_age(age)
    , m_email("")
    , m_salary(0.0)
    , m_agingTimer(new QTimer(this))
{
    setupConnections();
    qDebug() << "Person对象创建:" << m_name << "年龄:" << m_age;
}

void Person::setupConnections()
{
    /**
     * 内部信号槽连接演示
     * 这些连接展示了Qt信号槽系统的灵活性
     */
    
    // 连接姓名变化信号到槽函数
    connect(this, &Person::nameChanged, this, &Person::onNameChanged);
    
    // 连接年龄变化信号到槽函数
    connect(this, &Person::ageChanged, this, &Person::onAgeChanged);
    
    // 设置定时器 - 每5秒触发一次
    m_agingTimer->setInterval(5000);
    connect(m_agingTimer, &QTimer::timeout, this, &Person::onTimerTimeout);
    
    // 使用lambda表达式连接信号槽（Qt5+特性）
    connect(this, &Person::salaryChanged, [this](double newSalary) {
        qDebug() << m_name << "的薪水变更为:" << newSalary;
        emitStatusChange();
    });
    
    qDebug() << "Person内部信号槽连接已建立";
}

void Person::setName(const QString &name)
{
    /**
     * 属性设置器的标准实现模式：
     * 1. 检查值是否真的改变了
     * 2. 更新内部状态
     * 3. 发射变化信号
     */
    if (m_name != name) {
        QString oldName = m_name;
        m_name = name;
        
        qDebug() << "姓名从" << oldName << "变更为" << m_name;
        
        // 发射信号 - MOC生成的信号发射函数会被调用
        emit nameChanged(name);
        
        emitStatusChange();
    }
}

void Person::setAge(int age)
{
    if (m_age != age && age >= 0) {
        bool wasAdult = isAdult();  // 记录之前的成年状态
        int oldAge = m_age;
        m_age = age;
        
        qDebug() << m_name << "年龄从" << oldAge << "变更为" << m_age;
        
        // 发射年龄变化信号
        emit ageChanged(age);
        
        // 检查是否刚刚成年
        if (!wasAdult && isAdult()) {
            qDebug() << m_name << "刚刚成年了！";
            emit becameAdult();
        }
        
        emitStatusChange();
    }
}

void Person::setEmail(const QString &email)
{
    if (m_email != email) {
        m_email = email;
        qDebug() << m_name << "的邮箱设置为:" << m_email;
        emit emailChanged(email);
        emitStatusChange();
    }
}

void Person::setSalary(double salary)
{
    if (qAbs(m_salary - salary) > 0.01) {  // 浮点数比较
        double oldSalary = m_salary;
        m_salary = salary;
        
        qDebug() << m_name << "的薪水从" << oldSalary << "变更为" << m_salary;
        
        emit salaryChanged(salary);
        
        // 如果是加薪，发射专门的加薪信号
        if (salary > oldSalary) {
            emit salaryRaised(oldSalary, salary);
        }
    }
}

void Person::introduce() const
{
    /**
     * Q_INVOKABLE方法 - 可以通过元对象系统调用
     * 这个方法可以通过QMetaObject::invokeMethod调用
     */
    QString intro = QString("大家好，我是%1，今年%2岁")
                   .arg(m_name)
                   .arg(m_age);
    
    if (!m_email.isEmpty()) {
        intro += QString("，我的邮箱是%1").arg(m_email);
    }
    
    if (m_salary > 0) {
        intro += QString("，我的薪水是%1").arg(m_salary, 0, 'f', 2);
    }
    
    qDebug() << intro;
}

QString Person::getInfo() const
{
    /**
     * 返回格式化的个人信息
     * 这个方法也可以通过反射调用
     */
    return QString("姓名: %1, 年龄: %2, 成年: %3, 邮箱: %4, 薪水: %5")
           .arg(m_name)
           .arg(m_age)
           .arg(isAdult() ? "是" : "否")
           .arg(m_email.isEmpty() ? "未设置" : m_email)
           .arg(m_salary, 0, 'f', 2);  // 保留2位小数
}

void Person::celebrateBirthday()
{
    /**
     * 庆祝生日 - 年龄+1
     * 这会触发一系列信号的发射
     */
    qDebug() << "🎉" << m_name << "正在庆祝生日！";
    
    setAge(m_age + 1);  // 这会触发ageChanged信号
    
    qDebug() << m_name << "现在" << m_age << "岁了！";
    emit birthdayCelebrated(m_age);
}

void Person::giveRaise(double percentage)
{
    /**
     * 加薪方法
     * @param percentage 加薪百分比（如0.1表示加薪10%）
     */
    if (percentage > 0 && m_salary > 0) {
        double newSalary = m_salary * (1.0 + percentage);
        qDebug() << m_name << "获得了" << (percentage * 100) << "%的加薪！";
        setSalary(newSalary);
    }
}

// 槽函数实现
void Person::onNameChanged(const QString &newName)
{
    /**
     * 姓名变化的响应槽
     * 这个槽连接到nameChanged信号
     */
    qDebug() << "槽函数响应: 姓名已变更为" << newName;
}

void Person::onAgeChanged(int newAge)
{
    /**
     * 年龄变化的响应槽
     */
    qDebug() << "槽函数响应: 年龄已变更为" << newAge;
    
    if (newAge >= 18 && newAge <= 22) {
        qDebug() << "正值青春年华！";
    } else if (newAge >= 60) {
        qDebug() << "进入了黄金年龄！";
    }
}

void Person::reset()
{
    /**
     * 重置所有属性到默认值
     * 这会触发多个信号的发射
     */
    qDebug() << "重置" << m_name << "的所有信息";
    
    setName("Unknown");
    setAge(0);
    setEmail("");
    setSalary(0.0);
    
    qDebug() << "重置完成";
}

void Person::startAging()
{
    /**
     * 开始自动老化过程（演示定时器）
     */
    qDebug() << m_name << "开始自动老化过程...";
    m_agingTimer->start();
}

void Person::stopAging()
{
    /**
     * 停止自动老化过程
     */
    qDebug() << m_name << "停止自动老化过程";
    m_agingTimer->stop();
}

void Person::onTimerTimeout()
{
    /**
     * 定时器超时槽 - 每5秒执行一次
     * 演示定时器与信号槽的结合使用
     */
    setAge(m_age + 1);
    qDebug() << "⏰ 时间流逝..." << m_name << "又老了一岁";
    
    // 如果年龄超过100岁，停止老化
    if (m_age >= 100) {
        qDebug() << m_name << "已经100岁了，停止老化过程";
        stopAging();
    }
}

void Person::emitStatusChange()
{
    /**
     * 发射状态变化信号
     * 汇总当前对象的状态信息
     */
    QString status = QString("%1 (%2岁, %3)")
                    .arg(m_name)
                    .arg(m_age)
                    .arg(isAdult() ? "成年" : "未成年");
    
    emit statusChanged(status);
}

/**
 * 重要说明：
 * 
 * 在这个文件的末尾，通常需要包含MOC生成的文件：
 * #include "moc_person.cpp"
 * 
 * 但是在使用CMake的AUTOMOC功能时，这个包含会自动处理，
 * 所以我们不需要手动添加。
 * 
 * MOC会为这个类生成以下内容：
 * 1. 静态元对象数据表
 * 2. 信号发射函数的实现
 * 3. qt_static_metacall函数
 * 4. 元对象的虚函数实现
 * 
 * 这些生成的代码将在编译后拷贝到src/generated目录中供查看。
 */