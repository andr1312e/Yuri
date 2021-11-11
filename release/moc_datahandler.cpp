/****************************************************************************
** Meta object code from reading C++ file 'datahandler.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../connections/datahandler.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'datahandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DataHandler_t {
    QByteArrayData data[12];
    char stringdata0[198];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DataHandler_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DataHandler_t qt_meta_stringdata_DataHandler = {
    {
QT_MOC_LITERAL(0, 0, 11), // "DataHandler"
QT_MOC_LITERAL(1, 12, 24), // "ToButtonsEnabledChanging"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 5), // "state"
QT_MOC_LITERAL(4, 44, 12), // "ToConsoleLog"
QT_MOC_LITERAL(5, 57, 7), // "message"
QT_MOC_LITERAL(6, 65, 23), // "ToStateWidgetConsoleLog"
QT_MOC_LITERAL(7, 89, 26), // "ToFirmwareWidgetConsoleLog"
QT_MOC_LITERAL(8, 116, 25), // "ToStateGettingFromMessage"
QT_MOC_LITERAL(9, 142, 11), // "QByteArray*"
QT_MOC_LITERAL(10, 154, 23), // "ToHardwareStateChanging"
QT_MOC_LITERAL(11, 178, 19) // "ToReadFirmwareAgain"

    },
    "DataHandler\0ToButtonsEnabledChanging\0"
    "\0state\0ToConsoleLog\0message\0"
    "ToStateWidgetConsoleLog\0"
    "ToFirmwareWidgetConsoleLog\0"
    "ToStateGettingFromMessage\0QByteArray*\0"
    "ToHardwareStateChanging\0ToReadFirmwareAgain"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DataHandler[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       4,    1,   52,    2, 0x06 /* Public */,
       6,    1,   55,    2, 0x06 /* Public */,
       7,    1,   58,    2, 0x06 /* Public */,
       8,    1,   61,    2, 0x06 /* Public */,
      10,    1,   64,    2, 0x06 /* Public */,
      11,    0,   67,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, 0x80000000 | 9,    5,
    QMetaType::Void, QMetaType::UChar,    3,
    QMetaType::Void,

       0        // eod
};

void DataHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DataHandler *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ToButtonsEnabledChanging((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->ToConsoleLog((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->ToStateWidgetConsoleLog((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->ToFirmwareWidgetConsoleLog((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->ToStateGettingFromMessage((*reinterpret_cast< QByteArray*(*)>(_a[1]))); break;
        case 5: _t->ToHardwareStateChanging((*reinterpret_cast< quint8(*)>(_a[1]))); break;
        case 6: _t->ToReadFirmwareAgain(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DataHandler::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataHandler::ToButtonsEnabledChanging)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DataHandler::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataHandler::ToConsoleLog)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DataHandler::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataHandler::ToStateWidgetConsoleLog)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (DataHandler::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataHandler::ToFirmwareWidgetConsoleLog)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (DataHandler::*)(QByteArray * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataHandler::ToStateGettingFromMessage)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (DataHandler::*)(quint8 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataHandler::ToHardwareStateChanging)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (DataHandler::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataHandler::ToReadFirmwareAgain)) {
                *result = 6;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DataHandler::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_DataHandler.data,
    qt_meta_data_DataHandler,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DataHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DataHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DataHandler.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DataHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void DataHandler::ToButtonsEnabledChanging(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DataHandler::ToConsoleLog(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DataHandler::ToStateWidgetConsoleLog(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DataHandler::ToFirmwareWidgetConsoleLog(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DataHandler::ToStateGettingFromMessage(QByteArray * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void DataHandler::ToHardwareStateChanging(quint8 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void DataHandler::ToReadFirmwareAgain()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
