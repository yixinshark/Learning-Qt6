/****************************************************************************
** Meta object code from reading C++ file 'company.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/company.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>

#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'company.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSCompanyENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSCompanyENDCLASS =
    QtMocHelpers::stringData(
        "Company", "nameChanged", "", "newName", "addressChanged", "newAddress",
        "typeChanged", "CompanyType", "newType", "employeeCountChanged",
        "count", "totalSalaryChanged", "total", "employeeAdded", "Person*",
        "person", "employeeRemoved", "employeePromoted", "massRaiseGiven",
        "percentage", "reportGenerated", "report", "statisticsUpdated",
        "onEmployeeStatusChanged", "status", "onEmployeeSalaryRaised",
        "oldSalary", "newSalary", "onEmployeeBecameAdult", "generateReport",
        "updateStatistics", "onEmployeeDestroyed", "obj", "addEmployee",
        "removeEmployee", "findEmployee", "name", "getEmployeeNames",
        "getCompanyInfo", "giveRaiseToAll", "celebrateAllBirthdays",
        "resetAllEmployees", "address", "employeeCount", "totalSalary",
        "averageSalary", "type", "Technology", "Finance", "Manufacturing",
        "Service", "Other");
#else // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSCompanyENDCLASS[] = {

    // content:
    12,     // revision
    0,      // classname
    0, 0,   // classinfo
    25, 14, // methods
    6, 225, // properties
    1, 255, // enums/sets
    0, 0,   // constructors
    0,      // flags
    11,     // signalCount

    // signals: name, argc, parameters, tag, flags, initial metatype offsets
    1, 1, 164, 2, 0x06, 8 /* Public */, 4, 1, 167, 2, 0x06, 10 /* Public */, 6,
    1, 170, 2, 0x06, 12 /* Public */, 9, 1, 173, 2, 0x06, 14 /* Public */, 11,
    1, 176, 2, 0x06, 16 /* Public */, 13, 1, 179, 2, 0x06, 18 /* Public */, 16,
    1, 182, 2, 0x06, 20 /* Public */, 17, 1, 185, 2, 0x06, 22 /* Public */, 18,
    1, 188, 2, 0x06, 24 /* Public */, 20, 1, 191, 2, 0x06, 26 /* Public */, 22,
    0, 194, 2, 0x06, 28 /* Public */,

    // slots: name, argc, parameters, tag, flags, initial metatype offsets
    23, 1, 195, 2, 0x0a, 29 /* Public */, 25, 2, 198, 2, 0x0a, 31 /* Public */,
    28, 0, 203, 2, 0x0a, 34 /* Public */, 29, 0, 204, 2, 0x0a, 35 /* Public */,
    30, 0, 205, 2, 0x0a, 36 /* Public */, 31, 1, 206, 2, 0x08, 37 /* Private */,

    // methods: name, argc, parameters, tag, flags, initial metatype offsets
    33, 1, 209, 2, 0x02, 39 /* Public */, 34, 1, 212, 2, 0x02, 41 /* Public */,
    35, 1, 215, 2, 0x102, 43 /* Public | MethodIsConst  */, 37, 0, 218, 2,
    0x102, 45 /* Public | MethodIsConst  */, 38, 0, 219, 2, 0x102,
    46 /* Public | MethodIsConst  */, 39, 1, 220, 2, 0x02, 47 /* Public */, 40,
    0, 223, 2, 0x02, 49 /* Public */, 41, 0, 224, 2, 0x02, 50 /* Public */,

    // signals: parameters
    QMetaType::Void, QMetaType::QString, 3, QMetaType::Void, QMetaType::QString,
    5, QMetaType::Void, 0x80000000 | 7, 8, QMetaType::Void, QMetaType::Int, 10,
    QMetaType::Void, QMetaType::Double, 12, QMetaType::Void, 0x80000000 | 14,
    15, QMetaType::Void, 0x80000000 | 14, 15, QMetaType::Void, 0x80000000 | 14,
    15, QMetaType::Void, QMetaType::Double, 19, QMetaType::Void,
    QMetaType::QString, 21, QMetaType::Void,

    // slots: parameters
    QMetaType::Void, QMetaType::QString, 24, QMetaType::Void, QMetaType::Double,
    QMetaType::Double, 26, 27, QMetaType::Void, QMetaType::Void,
    QMetaType::Void, QMetaType::Void, QMetaType::QObjectStar, 32,

    // methods: parameters
    QMetaType::Void, 0x80000000 | 14, 15, QMetaType::Void, 0x80000000 | 14, 15,
    0x80000000 | 14, QMetaType::QString, 36, QMetaType::QStringList,
    QMetaType::QString, QMetaType::Void, QMetaType::Double, 19, QMetaType::Void,
    QMetaType::Void,

    // properties: name, type, flags, notifyId, revision
    36, QMetaType::QString, 0x00015103, uint(0), 0, 42, QMetaType::QString,
    0x00015103, uint(1), 0, 43, QMetaType::Int, 0x00015001, uint(3), 0, 44,
    QMetaType::Double, 0x00015001, uint(4), 0, 45, QMetaType::Double,
    0x00005001, uint(-1), 0, 46, 0x80000000 | 7, 0x0001510b, uint(2), 0,

    // enums: name, alias, flags, count, data
    7, 7, 0x0, 5, 260,

    // enum data: key, value
    47, uint(Company::Technology), 48, uint(Company::Finance), 49,
    uint(Company::Manufacturing), 50, uint(Company::Service), 51,
    uint(Company::Other),

    0 // eod
};

Q_CONSTINIT const QMetaObject Company::staticMetaObject = {
    {QMetaObject::SuperData::link<QObject::staticMetaObject>(),
     qt_meta_stringdata_CLASSCompanyENDCLASS.offsetsAndSizes,
     qt_meta_data_CLASSCompanyENDCLASS, qt_static_metacall, nullptr,
     qt_incomplete_metaTypeArray<
         qt_meta_stringdata_CLASSCompanyENDCLASS_t,
         // property 'name'
         QtPrivate::TypeAndForceComplete<QString, std::true_type>,
         // property 'address'
         QtPrivate::TypeAndForceComplete<QString, std::true_type>,
         // property 'employeeCount'
         QtPrivate::TypeAndForceComplete<int, std::true_type>,
         // property 'totalSalary'
         QtPrivate::TypeAndForceComplete<double, std::true_type>,
         // property 'averageSalary'
         QtPrivate::TypeAndForceComplete<double, std::true_type>,
         // property 'type'
         QtPrivate::TypeAndForceComplete<CompanyType, std::true_type>,
         // enum 'CompanyType'
         QtPrivate::TypeAndForceComplete<Company::CompanyType, std::true_type>,
         // Q_OBJECT / Q_GADGET
         QtPrivate::TypeAndForceComplete<Company, std::true_type>,
         // method 'nameChanged'
         QtPrivate::TypeAndForceComplete<void, std::false_type>,
         QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
         // method 'addressChanged'
         QtPrivate::TypeAndForceComplete<void, std::false_type>,
         QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
         // method 'typeChanged'
         QtPrivate::TypeAndForceComplete<void, std::false_type>,
         QtPrivate::TypeAndForceComplete<CompanyType, std::false_type>,
         // method 'employeeCountChanged'
         QtPrivate::TypeAndForceComplete<void, std::false_type>,
         QtPrivate::TypeAndForceComplete<int, std::false_type>,
         // method 'totalSalaryChanged'
         QtPrivate::TypeAndForceComplete<void, std::false_type>,
         QtPrivate::TypeAndForceComplete<double, std::false_type>,
         // method 'employeeAdded'
         QtPrivate::TypeAndForceComplete<void, std::false_type>,
         QtPrivate::TypeAndForceComplete<Person *, std::false_type>,
         // method 'employeeRemoved'
         QtPrivate::TypeAndForceComplete<void, std::false_type>,
         QtPrivate::TypeAndForceComplete<Person *, std::false_type>,
         // method 'employeePromoted'
         QtPrivate::TypeAndForceComplete<void, std::false_type>,
         QtPrivate::TypeAndForceComplete<Person *, std::false_type>,
         // method 'massRaiseGiven'
         QtPrivate::TypeAndForceComplete<void, std::false_type>,
         QtPrivate::TypeAndForceComplete<double, std::false_type>,
         // method 'reportGenerated'
         QtPrivate::TypeAndForceComplete<void, std::false_type>,
         QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
         // method 'statisticsUpdated'
         QtPrivate::TypeAndForceComplete<void, std::false_type>,
         // method 'onEmployeeStatusChanged'
         QtPrivate::TypeAndForceComplete<void, std::false_type>,
         QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
         // method 'onEmployeeSalaryRaised'
         QtPrivate::TypeAndForceComplete<void, std::false_type>,
         QtPrivate::TypeAndForceComplete<double, std::false_type>,
         QtPrivate::TypeAndForceComplete<double, std::false_type>,
         // method 'onEmployeeBecameAdult'
         QtPrivate::TypeAndForceComplete<void, std::false_type>,
         // method 'generateReport'
         QtPrivate::TypeAndForceComplete<void, std::false_type>,
         // method 'updateStatistics'
         QtPrivate::TypeAndForceComplete<void, std::false_type>,
         // method 'onEmployeeDestroyed'
         QtPrivate::TypeAndForceComplete<void, std::false_type>,
         QtPrivate::TypeAndForceComplete<QObject *, std::false_type>,
         // method 'addEmployee'
         QtPrivate::TypeAndForceComplete<void, std::false_type>,
         QtPrivate::TypeAndForceComplete<Person *, std::false_type>,
         // method 'removeEmployee'
         QtPrivate::TypeAndForceComplete<void, std::false_type>,
         QtPrivate::TypeAndForceComplete<Person *, std::false_type>,
         // method 'findEmployee'
         QtPrivate::TypeAndForceComplete<Person *, std::false_type>,
         QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
         // method 'getEmployeeNames'
         QtPrivate::TypeAndForceComplete<QStringList, std::false_type>,
         // method 'getCompanyInfo'
         QtPrivate::TypeAndForceComplete<QString, std::false_type>,
         // method 'giveRaiseToAll'
         QtPrivate::TypeAndForceComplete<void, std::false_type>,
         QtPrivate::TypeAndForceComplete<double, std::false_type>,
         // method 'celebrateAllBirthdays'
         QtPrivate::TypeAndForceComplete<void, std::false_type>,
         // method 'resetAllEmployees'
         QtPrivate::TypeAndForceComplete<void, std::false_type>>,
     nullptr}};

void Company::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id,
                                 void **_a) {
  if (_c == QMetaObject::InvokeMetaMethod) {
    auto *_t = static_cast<Company *>(_o);
    (void)_t;
    switch (_id) {
    case 0:
      _t->nameChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
      break;
    case 1:
      _t->addressChanged(
          (*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
      break;
    case 2:
      _t->typeChanged(
          (*reinterpret_cast<std::add_pointer_t<CompanyType>>(_a[1])));
      break;
    case 3:
      _t->employeeCountChanged(
          (*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
      break;
    case 4:
      _t->totalSalaryChanged(
          (*reinterpret_cast<std::add_pointer_t<double>>(_a[1])));
      break;
    case 5:
      _t->employeeAdded(
          (*reinterpret_cast<std::add_pointer_t<Person *>>(_a[1])));
      break;
    case 6:
      _t->employeeRemoved(
          (*reinterpret_cast<std::add_pointer_t<Person *>>(_a[1])));
      break;
    case 7:
      _t->employeePromoted(
          (*reinterpret_cast<std::add_pointer_t<Person *>>(_a[1])));
      break;
    case 8:
      _t->massRaiseGiven(
          (*reinterpret_cast<std::add_pointer_t<double>>(_a[1])));
      break;
    case 9:
      _t->reportGenerated(
          (*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
      break;
    case 10:
      _t->statisticsUpdated();
      break;
    case 11:
      _t->onEmployeeStatusChanged(
          (*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
      break;
    case 12:
      _t->onEmployeeSalaryRaised(
          (*reinterpret_cast<std::add_pointer_t<double>>(_a[1])),
          (*reinterpret_cast<std::add_pointer_t<double>>(_a[2])));
      break;
    case 13:
      _t->onEmployeeBecameAdult();
      break;
    case 14:
      _t->generateReport();
      break;
    case 15:
      _t->updateStatistics();
      break;
    case 16:
      _t->onEmployeeDestroyed(
          (*reinterpret_cast<std::add_pointer_t<QObject *>>(_a[1])));
      break;
    case 17:
      _t->addEmployee((*reinterpret_cast<std::add_pointer_t<Person *>>(_a[1])));
      break;
    case 18:
      _t->removeEmployee(
          (*reinterpret_cast<std::add_pointer_t<Person *>>(_a[1])));
      break;
    case 19: {
      Person *_r = _t->findEmployee(
          (*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
      if (_a[0])
        *reinterpret_cast<Person **>(_a[0]) = std::move(_r);
    } break;
    case 20: {
      QStringList _r = _t->getEmployeeNames();
      if (_a[0])
        *reinterpret_cast<QStringList *>(_a[0]) = std::move(_r);
    } break;
    case 21: {
      QString _r = _t->getCompanyInfo();
      if (_a[0])
        *reinterpret_cast<QString *>(_a[0]) = std::move(_r);
    } break;
    case 22:
      _t->giveRaiseToAll(
          (*reinterpret_cast<std::add_pointer_t<double>>(_a[1])));
      break;
    case 23:
      _t->celebrateAllBirthdays();
      break;
    case 24:
      _t->resetAllEmployees();
      break;
    default:;
    }
  } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
    switch (_id) {
    default:
      *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
      break;
    case 5:
      switch (*reinterpret_cast<int *>(_a[1])) {
      default:
        *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        break;
      case 0:
        *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType<Person *>();
        break;
      }
      break;
    case 6:
      switch (*reinterpret_cast<int *>(_a[1])) {
      default:
        *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        break;
      case 0:
        *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType<Person *>();
        break;
      }
      break;
    case 7:
      switch (*reinterpret_cast<int *>(_a[1])) {
      default:
        *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        break;
      case 0:
        *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType<Person *>();
        break;
      }
      break;
    case 17:
      switch (*reinterpret_cast<int *>(_a[1])) {
      default:
        *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        break;
      case 0:
        *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType<Person *>();
        break;
      }
      break;
    case 18:
      switch (*reinterpret_cast<int *>(_a[1])) {
      default:
        *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        break;
      case 0:
        *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType<Person *>();
        break;
      }
      break;
    }
  } else if (_c == QMetaObject::IndexOfMethod) {
    int *result = reinterpret_cast<int *>(_a[0]);
    {
      using _t = void (Company::*)(const QString &);
      if (_t _q_method = &Company::nameChanged;
          *reinterpret_cast<_t *>(_a[1]) == _q_method) {
        *result = 0;
        return;
      }
    }
    {
      using _t = void (Company::*)(const QString &);
      if (_t _q_method = &Company::addressChanged;
          *reinterpret_cast<_t *>(_a[1]) == _q_method) {
        *result = 1;
        return;
      }
    }
    {
      using _t = void (Company::*)(CompanyType);
      if (_t _q_method = &Company::typeChanged;
          *reinterpret_cast<_t *>(_a[1]) == _q_method) {
        *result = 2;
        return;
      }
    }
    {
      using _t = void (Company::*)(int);
      if (_t _q_method = &Company::employeeCountChanged;
          *reinterpret_cast<_t *>(_a[1]) == _q_method) {
        *result = 3;
        return;
      }
    }
    {
      using _t = void (Company::*)(double);
      if (_t _q_method = &Company::totalSalaryChanged;
          *reinterpret_cast<_t *>(_a[1]) == _q_method) {
        *result = 4;
        return;
      }
    }
    {
      using _t = void (Company::*)(Person *);
      if (_t _q_method = &Company::employeeAdded;
          *reinterpret_cast<_t *>(_a[1]) == _q_method) {
        *result = 5;
        return;
      }
    }
    {
      using _t = void (Company::*)(Person *);
      if (_t _q_method = &Company::employeeRemoved;
          *reinterpret_cast<_t *>(_a[1]) == _q_method) {
        *result = 6;
        return;
      }
    }
    {
      using _t = void (Company::*)(Person *);
      if (_t _q_method = &Company::employeePromoted;
          *reinterpret_cast<_t *>(_a[1]) == _q_method) {
        *result = 7;
        return;
      }
    }
    {
      using _t = void (Company::*)(double);
      if (_t _q_method = &Company::massRaiseGiven;
          *reinterpret_cast<_t *>(_a[1]) == _q_method) {
        *result = 8;
        return;
      }
    }
    {
      using _t = void (Company::*)(const QString &);
      if (_t _q_method = &Company::reportGenerated;
          *reinterpret_cast<_t *>(_a[1]) == _q_method) {
        *result = 9;
        return;
      }
    }
    {
      using _t = void (Company::*)();
      if (_t _q_method = &Company::statisticsUpdated;
          *reinterpret_cast<_t *>(_a[1]) == _q_method) {
        *result = 10;
        return;
      }
    }
  } else if (_c == QMetaObject::ReadProperty) {
    auto *_t = static_cast<Company *>(_o);
    (void)_t;
    void *_v = _a[0];
    switch (_id) {
    case 0:
      *reinterpret_cast<QString *>(_v) = _t->name();
      break;
    case 1:
      *reinterpret_cast<QString *>(_v) = _t->address();
      break;
    case 2:
      *reinterpret_cast<int *>(_v) = _t->employeeCount();
      break;
    case 3:
      *reinterpret_cast<double *>(_v) = _t->totalSalary();
      break;
    case 4:
      *reinterpret_cast<double *>(_v) = _t->averageSalary();
      break;
    case 5:
      *reinterpret_cast<CompanyType *>(_v) = _t->type();
      break;
    default:
      break;
    }
  } else if (_c == QMetaObject::WriteProperty) {
    auto *_t = static_cast<Company *>(_o);
    (void)_t;
    void *_v = _a[0];
    switch (_id) {
    case 0:
      _t->setName(*reinterpret_cast<QString *>(_v));
      break;
    case 1:
      _t->setAddress(*reinterpret_cast<QString *>(_v));
      break;
    case 5:
      _t->setType(*reinterpret_cast<CompanyType *>(_v));
      break;
    default:
      break;
    }
  } else if (_c == QMetaObject::ResetProperty) {
  } else if (_c == QMetaObject::BindableProperty) {
  }
}

const QMetaObject *Company::metaObject() const {
  return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject()
                                    : &staticMetaObject;
}

void *Company::qt_metacast(const char *_clname) {
  if (!_clname)
    return nullptr;
  if (!strcmp(_clname, qt_meta_stringdata_CLASSCompanyENDCLASS.stringdata0))
    return static_cast<void *>(this);
  return QObject::qt_metacast(_clname);
}

int Company::qt_metacall(QMetaObject::Call _c, int _id, void **_a) {
  _id = QObject::qt_metacall(_c, _id, _a);
  if (_id < 0)
    return _id;
  if (_c == QMetaObject::InvokeMetaMethod) {
    if (_id < 25)
      qt_static_metacall(this, _c, _id, _a);
    _id -= 25;
  } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
    if (_id < 25)
      qt_static_metacall(this, _c, _id, _a);
    _id -= 25;
  } else if (_c == QMetaObject::ReadProperty ||
             _c == QMetaObject::WriteProperty ||
             _c == QMetaObject::ResetProperty ||
             _c == QMetaObject::BindableProperty ||
             _c == QMetaObject::RegisterPropertyMetaType) {
    qt_static_metacall(this, _c, _id, _a);
    _id -= 6;
  }
  return _id;
}

// SIGNAL 0
void Company::nameChanged(const QString &_t1) {
  void *_a[] = {nullptr, const_cast<void *>(reinterpret_cast<const void *>(
                             std::addressof(_t1)))};
  QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Company::addressChanged(const QString &_t1) {
  void *_a[] = {nullptr, const_cast<void *>(reinterpret_cast<const void *>(
                             std::addressof(_t1)))};
  QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Company::typeChanged(CompanyType _t1) {
  void *_a[] = {nullptr, const_cast<void *>(reinterpret_cast<const void *>(
                             std::addressof(_t1)))};
  QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Company::employeeCountChanged(int _t1) {
  void *_a[] = {nullptr, const_cast<void *>(reinterpret_cast<const void *>(
                             std::addressof(_t1)))};
  QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Company::totalSalaryChanged(double _t1) {
  void *_a[] = {nullptr, const_cast<void *>(reinterpret_cast<const void *>(
                             std::addressof(_t1)))};
  QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Company::employeeAdded(Person *_t1) {
  void *_a[] = {nullptr, const_cast<void *>(reinterpret_cast<const void *>(
                             std::addressof(_t1)))};
  QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Company::employeeRemoved(Person *_t1) {
  void *_a[] = {nullptr, const_cast<void *>(reinterpret_cast<const void *>(
                             std::addressof(_t1)))};
  QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void Company::employeePromoted(Person *_t1) {
  void *_a[] = {nullptr, const_cast<void *>(reinterpret_cast<const void *>(
                             std::addressof(_t1)))};
  QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void Company::massRaiseGiven(double _t1) {
  void *_a[] = {nullptr, const_cast<void *>(reinterpret_cast<const void *>(
                             std::addressof(_t1)))};
  QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void Company::reportGenerated(const QString &_t1) {
  void *_a[] = {nullptr, const_cast<void *>(reinterpret_cast<const void *>(
                             std::addressof(_t1)))};
  QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void Company::statisticsUpdated() {
  QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}
QT_WARNING_POP
