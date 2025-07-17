/****************************************************************************
** MOC生成的Person类元对象代码 - 详细中文注释版本
** 
** 这个文件是Qt的元对象编译器(MOC)自动生成的，包含了Person类的完整元信息
** 
** 重要说明：
** 1. 这个文件会在每次编译时重新生成，不要手动修改
** 2. 这里的注释是为了学习目的添加的，实际生成的文件没有这些注释
** 3. 这个文件展示了Qt元对象系统的内部实现机制
*****************************************************************************/

#include "../../src/person.h"
#include <QtCore/qmetatype.h>
#include <QtCore/qtmochelpers.h>
#include <memory>

// MOC版本检查 - 确保头文件和MOC版本兼容
#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'person.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")

namespace {

/**
 * 字符串数据表 - 存储所有元信息中用到的字符串
 * 
 * Qt使用字符串表来优化内存使用，所有的方法名、属性名、参数名等
 * 都存储在这个统一的字符串表中，通过索引来引用。
 */
#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSPersonENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSPersonENDCLASS = QtMocHelpers::stringData(
    "Person",              // 0: 类名
    "nameChanged",         // 1: 信号名
    "",                    // 2: 空字符串（用作标签）
    "newName",            // 3: 参数名
    "ageChanged",         // 4: 信号名
    "newAge",             // 5: 参数名
    "emailChanged",       // 6: 信号名
    "newEmail",           // 7: 参数名
    "salaryChanged",      // 8: 信号名
    "newSalary",          // 9: 参数名
    "birthdayCelebrated", // 10: 信号名
    "salaryRaised",       // 11: 信号名
    "oldSalary",          // 12: 参数名
    "becameAdult",        // 13: 信号名
    "statusChanged",      // 14: 信号名
    "status",             // 15: 参数名
    "onNameChanged",      // 16: 槽函数名
    "onAgeChanged",       // 17: 槽函数名
    "reset",              // 18: 槽函数名
    "startAging",         // 19: 槽函数名
    "stopAging",          // 20: 槽函数名
    "onTimerTimeout",     // 21: 私有槽函数名
    "introduce",          // 22: 可调用方法名
    "getInfo",            // 23: 可调用方法名
    "celebrateBirthday",  // 24: 可调用方法名
    "giveRaise",          // 25: 可调用方法名
    "percentage",         // 26: 参数名
    "name",               // 27: 属性名
    "age",                // 28: 属性名
    "isAdult",            // 29: 属性名
    "email",              // 30: 属性名
    "salary"              // 31: 属性名
);
#else
#error "qtmochelpers.h not found or too old."
#endif

} // unnamed namespace

/**
 * 静态元对象数据表 - Person类的核心元信息
 * 
 * 这个数组包含了Person类的所有元信息，包括：
 * - 类的基本信息（版本、类名等）
 * - 方法信息（信号、槽、普通方法）
 * - 属性信息
 * - 枚举信息等
 * 
 * 数据按照特定的格式组织，Qt的元对象系统通过解析这个数组
 * 来获取类的运行时信息。
 */
Q_CONSTINIT static const uint qt_meta_data_CLASSPersonENDCLASS[] = {

 // 内容头部信息
      12,       // revision - MOC数据格式版本
       0,       // classname - 类名在字符串表中的索引（Person）
       0,    0, // classinfo - 类信息数量和起始索引
      18,   14, // methods - 方法数量(18个)和起始索引(14)
       5,  162, // properties - 属性数量(5个)和起始索引(162)
       0,    0, // enums/sets - 枚举数量和起始索引
       0,    0, // constructors - 构造函数数量和起始索引
       0,       // flags - 类标志
       8,       // signalCount - 信号数量(8个)

 /**
  * 方法表 - 描述所有方法（信号、槽、普通方法）
  * 
  * 每个方法占用6个uint值：
  * - name: 方法名在字符串表中的索引
  * - argc: 参数数量
  * - parameters: 参数信息在参数表中的起始索引
  * - tag: 标签（通常为空）
  * - flags: 方法标志（信号、槽、访问级别等）
  * - initial metatype offsets: 元类型偏移
  */
 
 // 信号列表 (signals: name, argc, parameters, tag, flags, initial metatype offsets)
       1,    1,  122,    2, 0x06,    6, // nameChanged(QString) - 索引1，1个参数
       4,    1,  125,    2, 0x06,    8, // ageChanged(int) - 索引4，1个参数
       6,    1,  128,    2, 0x06,   10, // emailChanged(QString) - 索引6，1个参数
       8,    1,  131,    2, 0x06,   12, // salaryChanged(double) - 索引8，1个参数
      10,    1,  134,    2, 0x06,   14, // birthdayCelebrated(int) - 索引10，1个参数
      11,    2,  137,    2, 0x06,   16, // salaryRaised(double,double) - 索引11，2个参数
      13,    0,  142,    2, 0x06,   19, // becameAdult() - 索引13，0个参数
      14,    1,  143,    2, 0x06,   20, // statusChanged(QString) - 索引14，1个参数

 // 公共槽列表 (slots: name, argc, parameters, tag, flags, initial metatype offsets)
      16,    1,  146,    2, 0x0a,   22, // onNameChanged(QString) - 索引16，1个参数
      17,    1,  149,    2, 0x0a,   24, // onAgeChanged(int) - 索引17，1个参数
      18,    0,  152,    2, 0x0a,   26, // reset() - 索引18，0个参数
      19,    0,  153,    2, 0x0a,   27, // startAging() - 索引19，0个参数
      20,    0,  154,    2, 0x0a,   28, // stopAging() - 索引20，0个参数

 // 私有槽列表
      21,    0,  155,    2, 0x08,   29, // onTimerTimeout() - 索引21，0个参数，私有(0x08)

 // 可调用方法列表 (methods: name, argc, parameters, tag, flags, initial metatype offsets)
      22,    0,  156,    2, 0x02,   30, // introduce() - 索引22，0个参数，Q_INVOKABLE
      23,    0,  157,    2, 0x02,   31, // getInfo() - 索引23，0个参数，Q_INVOKABLE
      24,    0,  158,    2, 0x02,   32, // celebrateBirthday() - 索引24，0个参数
      25,    1,  159,    2, 0x02,   33, // giveRaise(double) - 索引25，1个参数

 /**
  * 参数表 - 描述每个方法的参数类型
  * 
  * 对于每个方法，这里存储了：
  * - 返回类型的元类型ID
  * - 每个参数的元类型ID
  * - 参数名称在字符串表中的索引
  */
 
 // 参数类型信息 (parameters: return type, parameter types, parameter names)
    QMetaType::Void, QMetaType::QString,    3,  // nameChanged(QString newName)
    QMetaType::Void, QMetaType::Int,        5,  // ageChanged(int newAge)
    QMetaType::Void, QMetaType::QString,    7,  // emailChanged(QString newEmail)
    QMetaType::Void, QMetaType::Double,     9,  // salaryChanged(double newSalary)
    QMetaType::Void, QMetaType::Int,        5,  // birthdayCelebrated(int newAge)
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   12,    9,  // salaryRaised(double oldSalary, double newSalary)
    QMetaType::Void,                               // becameAdult()
    QMetaType::Void, QMetaType::QString,   15,  // statusChanged(QString status)
    QMetaType::Void, QMetaType::QString,    3,  // onNameChanged(QString newName)
    QMetaType::Void, QMetaType::Int,        5,  // onAgeChanged(int newAge)
    QMetaType::Void,                               // reset()
    QMetaType::Void,                               // startAging()
    QMetaType::Void,                               // stopAging()
    QMetaType::Void,                               // onTimerTimeout()
    QMetaType::Void,                               // introduce()
    QMetaType::QString,                            // getInfo()
    QMetaType::Void,                               // celebrateBirthday()
    QMetaType::Void, QMetaType::Double,    26,  // giveRaise(double percentage)

 /**
  * 属性表 - 描述类的所有属性
  * 
  * 每个属性占用3个uint值：
  * - name: 属性名在字符串表中的索引
  * - type: 属性类型信息
  * - flags: 属性标志（可读、可写、有通知信号等）
  */
 
 // 属性信息 (properties: name, type, flags)
      27, QMetaType::QString, 0x00015103, uint(0), 0,  // name - 可读写，有通知信号
      28, QMetaType::Int,     0x00015103, uint(1), 0,  // age - 可读写，有通知信号
      29, QMetaType::Bool,    0x00015001, uint(-1), 0, // isAdult - 只读，计算属性
      30, QMetaType::QString, 0x00015103, uint(2), 0,  // email - 可读写，有通知信号
      31, QMetaType::Double,  0x00015103, uint(3), 0,  // salary - 可读写，有通知信号

       0        // eod - 数据结束标记
};

/**
 * 元对象调用函数 - 统一的方法调用入口
 * 
 * 这个函数是Qt元对象系统的核心，它根据传入的调用类型和方法ID，
 * 将调用分发到具体的方法实现。支持以下调用类型：
 * - InvokeMetaMethod: 调用方法（信号、槽、普通方法）
 * - ReadProperty: 读取属性值
 * - WriteProperty: 写入属性值
 * - ResetProperty: 重置属性值
 * - BindableProperty: 获取可绑定属性
 */
void Person::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    // 将QObject指针转换为Person指针
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Person *>(_o);
        Q_UNUSED(_t)
        
        /**
         * 方法调用分发 - 根据方法ID调用对应的方法
         * 
         * 这里使用switch语句将方法ID映射到具体的方法调用。
         * _a数组包含了方法的参数，_a[0]是返回值，_a[1]开始是参数。
         */
        switch (_id) {
        // 信号发射 (0-7)
        case 0: _t->nameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->ageChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->emailChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->salaryChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->birthdayCelebrated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->salaryRaised((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 6: _t->becameAdult(); break;
        case 7: _t->statusChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        
        // 公共槽调用 (8-12)
        case 8: _t->onNameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->onAgeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->reset(); break;
        case 11: _t->startAging(); break;
        case 12: _t->stopAging(); break;
        
        // 私有槽调用 (13)
        case 13: _t->onTimerTimeout(); break;
        
        // 可调用方法 (14-17)
        case 14: _t->introduce(); break;
        case 15: { QString _r = _t->getInfo();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 16: _t->celebrateBirthday(); break;
        case 17: _t->giveRaise((*reinterpret_cast< double(*)>(_a[1]))); break;
        
        default: ;
        }
    } else if (_c == QMetaObject::ReadProperty) {
        /**
         * 属性读取 - 根据属性ID读取属性值
         */
        auto *_t = static_cast<Person *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];  // _v指向存储返回值的位置
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->name(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->age(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->isAdult(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->email(); break;
        case 4: *reinterpret_cast< double*>(_v) = _t->salary(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        /**
         * 属性写入 - 根据属性ID写入属性值
         */
        auto *_t = static_cast<Person *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];  // _v指向要写入的值
        switch (_id) {
        case 0: _t->setName(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setAge(*reinterpret_cast< int*>(_v)); break;
        case 3: _t->setEmail(*reinterpret_cast< QString*>(_v)); break;
        case 4: _t->setSalary(*reinterpret_cast< double*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
        // 属性重置（这个类没有定义重置方法）
        Q_UNUSED(_o);
        Q_UNUSED(_id);
        Q_UNUSED(_a);
    } else if (_c == QMetaObject::BindableProperty) {
        // 可绑定属性（Qt6新特性，这个类没有使用）
        Q_UNUSED(_o);
        Q_UNUSED(_id);
        Q_UNUSED(_a);
    }
}

/**
 * 静态元对象实例 - Person类的元对象描述符
 * 
 * 这个静态对象包含了Person类的完整元信息，
 * 是Qt元对象系统访问类信息的入口点。
 */
Q_CONSTINIT const QMetaObject Person::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(), // 父类的元对象指针
    qt_meta_stringdata_CLASSPersonENDCLASS.offsetsAndSizes, // 字符串数据
    qt_meta_data_CLASSPersonENDCLASS, // 元数据
    qt_static_metacall, // 静态元调用函数
    nullptr, // 额外数据（通常为nullptr）
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSPersonENDCLASS_t, // Qt6新增的类型特征
        // property 'name'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'age'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'isAdult'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // property 'email'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'salary'
        QtPrivate::TypeAndForceComplete<double, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Person, std::true_type>,
        // method 'nameChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'ageChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'emailChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'salaryChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'birthdayCelebrated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'salaryRaised'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'becameAdult'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'statusChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onNameChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onAgeChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'reset'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'startAging'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'stopAging'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onTimerTimeout'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'introduce'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'getInfo'
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'celebrateBirthday'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'giveRaise'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>
    >,
    nullptr
} };

/**
 * 虚函数实现 - 返回对象的元对象
 * 
 * 这个虚函数被QObject::metaObject()调用，
 * 返回当前对象的元对象指针。
 */
const QMetaObject *Person::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

/**
 * 类型转换函数 - 安全的向下转型
 * 
 * 这个函数实现了qobject_cast的功能，
 * 提供类型安全的对象转换。
 */
void *Person::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSPersonENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

/**
 * 元对象调用接口 - 对外的统一调用入口
 * 
 * 这个函数是QMetaObject::invokeMethod等API的底层实现，
 * 它将调用转发给qt_static_metacall函数。
 */
int Person::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)  // 18个方法
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
               || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
               || _c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 5)  // 5个属性
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

/**
 * 信号发射函数 - 每个信号的具体实现
 * 
 * MOC为每个信号生成一个发射函数，这些函数调用QMetaObject::activate
 * 来触发信号槽机制。
 */

// nameChanged信号的发射函数
void Person::nameChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// ageChanged信号的发射函数
void Person::ageChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// emailChanged信号的发射函数
void Person::emailChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// salaryChanged信号的发射函数
void Person::salaryChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// birthdayCelebrated信号的发射函数
void Person::birthdayCelebrated(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// salaryRaised信号的发射函数（带两个参数）
void Person::salaryRaised(double _t1, double _t2)
{
    void *_a[] = { nullptr, 
                   const_cast<void*>(reinterpret_cast<const void*>(&_t1)), 
                   const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// becameAdult信号的发射函数（无参数）
void Person::becameAdult()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// statusChanged信号的发射函数
void Person::statusChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

QT_WARNING_POP

/**
 * 总结：
 * 
 * 这个MOC生成的文件展示了Qt元对象系统的完整实现：
 * 
 * 1. 字符串表：优化内存使用，统一存储所有字符串
 * 2. 元数据表：包含类的完整结构信息
 * 3. 调用分发：统一的方法调用入口，支持反射调用
 * 4. 属性访问：统一的属性读写接口
 * 5. 信号发射：每个信号的具体实现函数
 * 6. 类型安全：提供安全的类型转换和检查
 * 
 * 通过这些机制，Qt实现了：
 * - 运行时类型信息（RTTI）
 * - 反射和内省
 * - 信号槽机制
 * - 属性系统
 * - 动态方法调用
 * 
 * 这就是Qt元对象系统的核心实现原理！
 */