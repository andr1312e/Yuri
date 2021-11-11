/****************************************************************************
** Meta object code from reading C++ file 'firmwarepresenter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../presenter/firmwarepresenter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'firmwarepresenter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FirmwarePresenter_t {
    QByteArrayData data[20];
    char stringdata0[302];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FirmwarePresenter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FirmwarePresenter_t qt_meta_stringdata_FirmwarePresenter = {
    {
QT_MOC_LITERAL(0, 0, 17), // "FirmwarePresenter"
QT_MOC_LITERAL(1, 18, 19), // "OnSetButtonsEnabled"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 5), // "state"
QT_MOC_LITERAL(4, 45, 12), // "OnConsoleLog"
QT_MOC_LITERAL(5, 58, 12), // "errorMessage"
QT_MOC_LITERAL(6, 71, 22), // "SetMaximumCountOfPages"
QT_MOC_LITERAL(7, 94, 7), // "pageNum"
QT_MOC_LITERAL(8, 102, 11), // "PageUpdated"
QT_MOC_LITERAL(9, 114, 16), // "SetWidgetsEnable"
QT_MOC_LITERAL(10, 131, 5), // "Flash"
QT_MOC_LITERAL(11, 137, 30), // "StartReadingFirmWareFromDevice"
QT_MOC_LITERAL(12, 168, 12), // "TimerTimeout"
QT_MOC_LITERAL(13, 181, 19), // "OnReadFirmwareAgain"
QT_MOC_LITERAL(14, 201, 19), // "WritingTimerTimeOut"
QT_MOC_LITERAL(15, 221, 21), // "OnErasingTimerTimeOut"
QT_MOC_LITERAL(16, 243, 15), // "PageUpdatedSLOT"
QT_MOC_LITERAL(17, 259, 11), // "QByteArray&"
QT_MOC_LITERAL(18, 271, 8), // "firmware"
QT_MOC_LITERAL(19, 280, 21) // "OnUpdateHardwareState"

    },
    "FirmwarePresenter\0OnSetButtonsEnabled\0"
    "\0state\0OnConsoleLog\0errorMessage\0"
    "SetMaximumCountOfPages\0pageNum\0"
    "PageUpdated\0SetWidgetsEnable\0Flash\0"
    "StartReadingFirmWareFromDevice\0"
    "TimerTimeout\0OnReadFirmwareAgain\0"
    "WritingTimerTimeOut\0OnErasingTimerTimeOut\0"
    "PageUpdatedSLOT\0QByteArray&\0firmware\0"
    "OnUpdateHardwareState"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FirmwarePresenter[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x06 /* Public */,
       4,    1,   82,    2, 0x06 /* Public */,
       6,    1,   85,    2, 0x06 /* Public */,
       8,    1,   88,    2, 0x06 /* Public */,
       9,    1,   91,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   94,    2, 0x0a /* Public */,
      11,    0,   95,    2, 0x0a /* Public */,
      12,    0,   96,    2, 0x08 /* Private */,
      13,    0,   97,    2, 0x08 /* Private */,
      14,    0,   98,    2, 0x08 /* Private */,
      15,    0,   99,    2, 0x08 /* Private */,
      16,    1,  100,    2, 0x08 /* Private */,
      19,    1,  103,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Bool,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void, QMetaType::UChar,    3,

       0        // eod
};

void FirmwarePresenter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FirmwarePresenter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnSetButtonsEnabled((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->OnConsoleLog((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->SetMaximumCountOfPages((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->PageUpdated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->SetWidgetsEnable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->Flash(); break;
        case 6: _t->StartReadingFirmWareFromDevice(); break;
        case 7: _t->TimerTimeout(); break;
        case 8: _t->OnReadFirmwareAgain(); break;
        case 9: _t->WritingTimerTimeOut(); break;
        case 10: _t->OnErasingTimerTimeOut(); break;
        case 11: _t->PageUpdatedSLOT((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 12: _t->OnUpdateHardwareState((*reinterpret_cast< quint8(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (FirmwarePresenter::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FirmwarePresenter::OnSetButtonsEnabled)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (FirmwarePresenter::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FirmwarePresenter::OnConsoleLog)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (FirmwarePresenter::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FirmwarePresenter::SetMaximumCountOfPages)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (FirmwarePresenter::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FirmwarePresenter::PageUpdated)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (FirmwarePresenter::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FirmwarePresenter::SetWidgetsEnable)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FirmwarePresenter::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_FirmwarePresenter.data,
    qt_meta_data_FirmwarePresenter,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FirmwarePresenter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FirmwarePresenter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FirmwarePresenter.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int FirmwarePresenter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void FirmwarePresenter::OnSetButtonsEnabled(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FirmwarePresenter::OnConsoleLog(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FirmwarePresenter::SetMaximumCountOfPages(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void FirmwarePresenter::PageUpdated(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void FirmwarePresenter::SetWidgetsEnable(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
