/****************************************************************************
** Meta object code from reading C++ file 'person.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/person.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'person.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSPersonENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSPersonENDCLASS = QtMocHelpers::stringData(
    "Person",
    "nameChanged",
    "",
    "newName",
    "ageChanged",
    "newAge",
    "emailChanged",
    "newEmail",
    "salaryChanged",
    "newSalary",
    "birthdayCelebrated",
    "salaryRaised",
    "oldSalary",
    "becameAdult",
    "statusChanged",
    "status",
    "onNameChanged",
    "onAgeChanged",
    "reset",
    "startAging",
    "stopAging",
    "onTimerTimeout",
    "introduce",
    "getInfo",
    "celebrateBirthday",
    "giveRaise",
    "percentage",
    "name",
    "age",
    "isAdult",
    "email",
    "salary"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSPersonENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       5,  162, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  122,    2, 0x06,    6 /* Public */,
       4,    1,  125,    2, 0x06,    8 /* Public */,
       6,    1,  128,    2, 0x06,   10 /* Public */,
       8,    1,  131,    2, 0x06,   12 /* Public */,
      10,    1,  134,    2, 0x06,   14 /* Public */,
      11,    2,  137,    2, 0x06,   16 /* Public */,
      13,    0,  142,    2, 0x06,   19 /* Public */,
      14,    1,  143,    2, 0x06,   20 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      16,    1,  146,    2, 0x0a,   22 /* Public */,
      17,    1,  149,    2, 0x0a,   24 /* Public */,
      18,    0,  152,    2, 0x0a,   26 /* Public */,
      19,    0,  153,    2, 0x0a,   27 /* Public */,
      20,    0,  154,    2, 0x0a,   28 /* Public */,
      21,    0,  155,    2, 0x08,   29 /* Private */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
      22,    0,  156,    2, 0x102,   30 /* Public | MethodIsConst  */,
      23,    0,  157,    2, 0x102,   31 /* Public | MethodIsConst  */,
      24,    0,  158,    2, 0x02,   32 /* Public */,
      25,    1,  159,    2, 0x02,   33 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::Double,    9,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   12,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   15,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void,
    QMetaType::QString,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   26,

 // properties: name, type, flags, notifyId, revision
      27, QMetaType::QString, 0x00015103, uint(0), 0,
      28, QMetaType::Int, 0x00015103, uint(1), 0,
      29, QMetaType::Bool, 0x00005001, uint(-1), 0,
      30, QMetaType::QString, 0x00015103, uint(2), 0,
      31, QMetaType::Double, 0x00015103, uint(3), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject Person::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSPersonENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSPersonENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSPersonENDCLASS_t,
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

void Person::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Person *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->nameChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->ageChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->emailChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->salaryChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 4: _t->birthdayCelebrated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->salaryRaised((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2]))); break;
        case 6: _t->becameAdult(); break;
        case 7: _t->statusChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->onNameChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->onAgeChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 10: _t->reset(); break;
        case 11: _t->startAging(); break;
        case 12: _t->stopAging(); break;
        case 13: _t->onTimerTimeout(); break;
        case 14: _t->introduce(); break;
        case 15: { QString _r = _t->getInfo();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 16: _t->celebrateBirthday(); break;
        case 17: _t->giveRaise((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Person::*)(const QString & );
            if (_t _q_method = &Person::nameChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Person::*)(int );
            if (_t _q_method = &Person::ageChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Person::*)(const QString & );
            if (_t _q_method = &Person::emailChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Person::*)(double );
            if (_t _q_method = &Person::salaryChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Person::*)(int );
            if (_t _q_method = &Person::birthdayCelebrated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (Person::*)(double , double );
            if (_t _q_method = &Person::salaryRaised; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (Person::*)();
            if (_t _q_method = &Person::becameAdult; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (Person::*)(const QString & );
            if (_t _q_method = &Person::statusChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
    } else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<Person *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->name(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->age(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->isAdult(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->email(); break;
        case 4: *reinterpret_cast< double*>(_v) = _t->salary(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<Person *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setName(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setAge(*reinterpret_cast< int*>(_v)); break;
        case 3: _t->setEmail(*reinterpret_cast< QString*>(_v)); break;
        case 4: _t->setSalary(*reinterpret_cast< double*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

const QMetaObject *Person::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Person::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSPersonENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Person::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 18;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Person::nameChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Person::ageChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Person::emailChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Person::salaryChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Person::birthdayCelebrated(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Person::salaryRaised(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Person::becameAdult()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void Person::statusChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
