/****************************************************************************
** Meta object code from reading C++ file 'demo_runner.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/demo_runner.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'demo_runner.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSDemoRunnerENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSDemoRunnerENDCLASS = QtMocHelpers::stringData(
    "DemoRunner",
    "demoStarted",
    "",
    "demoName",
    "demoFinished",
    "allDemosCompleted",
    "startDemo",
    "stopDemo",
    "nextDemo",
    "onDemoTimer",
    "onPersonStatusChanged",
    "status",
    "onCompanyReportGenerated",
    "report",
    "runAllDemos",
    "runBasicSignalSlotDemo",
    "runPropertySystemDemo",
    "runReflectionDemo",
    "runDynamicInvocationDemo",
    "runCompanyDemo",
    "runMetaObjectAnalysis",
    "runPerformanceTest"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSDemoRunnerENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  116,    2, 0x06,    1 /* Public */,
       4,    1,  119,    2, 0x06,    3 /* Public */,
       5,    0,  122,    2, 0x06,    5 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       6,    0,  123,    2, 0x0a,    6 /* Public */,
       7,    0,  124,    2, 0x0a,    7 /* Public */,
       8,    0,  125,    2, 0x0a,    8 /* Public */,
       9,    0,  126,    2, 0x08,    9 /* Private */,
      10,    1,  127,    2, 0x08,   10 /* Private */,
      12,    1,  130,    2, 0x08,   12 /* Private */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
      14,    0,  133,    2, 0x02,   14 /* Public */,
      15,    0,  134,    2, 0x02,   15 /* Public */,
      16,    0,  135,    2, 0x02,   16 /* Public */,
      17,    0,  136,    2, 0x02,   17 /* Public */,
      18,    0,  137,    2, 0x02,   18 /* Public */,
      19,    0,  138,    2, 0x02,   19 /* Public */,
      20,    0,  139,    2, 0x02,   20 /* Public */,
      21,    0,  140,    2, 0x02,   21 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, QMetaType::QString,   13,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject DemoRunner::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSDemoRunnerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSDemoRunnerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSDemoRunnerENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<DemoRunner, std::true_type>,
        // method 'demoStarted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'demoFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'allDemosCompleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'startDemo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'stopDemo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'nextDemo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDemoTimer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onPersonStatusChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onCompanyReportGenerated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'runAllDemos'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'runBasicSignalSlotDemo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'runPropertySystemDemo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'runReflectionDemo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'runDynamicInvocationDemo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'runCompanyDemo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'runMetaObjectAnalysis'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'runPerformanceTest'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void DemoRunner::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DemoRunner *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->demoStarted((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->demoFinished((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->allDemosCompleted(); break;
        case 3: _t->startDemo(); break;
        case 4: _t->stopDemo(); break;
        case 5: _t->nextDemo(); break;
        case 6: _t->onDemoTimer(); break;
        case 7: _t->onPersonStatusChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->onCompanyReportGenerated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->runAllDemos(); break;
        case 10: _t->runBasicSignalSlotDemo(); break;
        case 11: _t->runPropertySystemDemo(); break;
        case 12: _t->runReflectionDemo(); break;
        case 13: _t->runDynamicInvocationDemo(); break;
        case 14: _t->runCompanyDemo(); break;
        case 15: _t->runMetaObjectAnalysis(); break;
        case 16: _t->runPerformanceTest(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DemoRunner::*)(const QString & );
            if (_t _q_method = &DemoRunner::demoStarted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DemoRunner::*)(const QString & );
            if (_t _q_method = &DemoRunner::demoFinished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DemoRunner::*)();
            if (_t _q_method = &DemoRunner::allDemosCompleted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *DemoRunner::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DemoRunner::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSDemoRunnerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DemoRunner::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void DemoRunner::demoStarted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DemoRunner::demoFinished(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DemoRunner::allDemosCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
