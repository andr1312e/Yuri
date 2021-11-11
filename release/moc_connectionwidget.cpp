/****************************************************************************
** Meta object code from reading C++ file 'connectionwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../ui/connectionwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'connectionwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ConnectionWidget_t {
    QByteArrayData data[14];
    char stringdata0[223];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConnectionWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConnectionWidget_t qt_meta_stringdata_ConnectionWidget = {
    {
QT_MOC_LITERAL(0, 0, 16), // "ConnectionWidget"
QT_MOC_LITERAL(1, 17, 12), // "ToConsoleLog"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 7), // "message"
QT_MOC_LITERAL(4, 39, 21), // "ToConnectEthernetMoxa"
QT_MOC_LITERAL(5, 61, 6), // "adress"
QT_MOC_LITERAL(6, 68, 4), // "port"
QT_MOC_LITERAL(7, 73, 16), // "ToConnectUsbMoxa"
QT_MOC_LITERAL(8, 90, 11), // "comPortName"
QT_MOC_LITERAL(9, 102, 20), // "ToDisconnectFromMoxa"
QT_MOC_LITERAL(10, 123, 26), // "comPortNameComboBoxChanged"
QT_MOC_LITERAL(11, 150, 43), // "OnCurrentIndexConnectionTypeC..."
QT_MOC_LITERAL(12, 194, 5), // "index"
QT_MOC_LITERAL(13, 200, 22) // "OnConnectButtonClicked"

    },
    "ConnectionWidget\0ToConsoleLog\0\0message\0"
    "ToConnectEthernetMoxa\0adress\0port\0"
    "ToConnectUsbMoxa\0comPortName\0"
    "ToDisconnectFromMoxa\0comPortNameComboBoxChanged\0"
    "OnCurrentIndexConnectionTypeComboBoxChanged\0"
    "index\0OnConnectButtonClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConnectionWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       4,    2,   52,    2, 0x06 /* Public */,
       7,    1,   57,    2, 0x06 /* Public */,
       9,    0,   60,    2, 0x06 /* Public */,
      10,    0,   61,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    1,   62,    2, 0x08 /* Private */,
      13,    0,   65,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    5,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void,

       0        // eod
};

void ConnectionWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ConnectionWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ToConsoleLog((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->ToConnectEthernetMoxa((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->ToConnectUsbMoxa((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->ToDisconnectFromMoxa(); break;
        case 4: _t->comPortNameComboBoxChanged(); break;
        case 5: _t->OnCurrentIndexConnectionTypeComboBoxChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->OnConnectButtonClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ConnectionWidget::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConnectionWidget::ToConsoleLog)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ConnectionWidget::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConnectionWidget::ToConnectEthernetMoxa)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ConnectionWidget::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConnectionWidget::ToConnectUsbMoxa)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ConnectionWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConnectionWidget::ToDisconnectFromMoxa)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ConnectionWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConnectionWidget::comPortNameComboBoxChanged)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ConnectionWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ConnectionWidget.data,
    qt_meta_data_ConnectionWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ConnectionWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConnectionWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ConnectionWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ConnectionWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void ConnectionWidget::ToConsoleLog(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ConnectionWidget::ToConnectEthernetMoxa(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ConnectionWidget::ToConnectUsbMoxa(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ConnectionWidget::ToDisconnectFromMoxa()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void ConnectionWidget::comPortNameComboBoxChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
