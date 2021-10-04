/****************************************************************************
** Meta object code from reading C++ file 'tcpsocket.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../tcpsocket.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcpsocket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TcpSocket_t {
    QByteArrayData data[17];
    char stringdata0[200];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TcpSocket_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TcpSocket_t qt_meta_stringdata_TcpSocket = {
    {
QT_MOC_LITERAL(0, 0, 9), // "TcpSocket"
QT_MOC_LITERAL(1, 10, 13), // "needToConnect"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 8), // "setState"
QT_MOC_LITERAL(4, 34, 5), // "state"
QT_MOC_LITERAL(5, 40, 16), // "sendMessageToLog"
QT_MOC_LITERAL(6, 57, 12), // "errorMessage"
QT_MOC_LITERAL(7, 70, 9), // "connected"
QT_MOC_LITERAL(8, 80, 9), // "readyRead"
QT_MOC_LITERAL(9, 90, 12), // "disconnected"
QT_MOC_LITERAL(10, 103, 16), // "getErrorMessqage"
QT_MOC_LITERAL(11, 120, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(12, 149, 11), // "socketError"
QT_MOC_LITERAL(13, 161, 4), // "send"
QT_MOC_LITERAL(14, 166, 11), // "QByteArray&"
QT_MOC_LITERAL(15, 178, 5), // "array"
QT_MOC_LITERAL(16, 184, 15) // "checkConnection"

    },
    "TcpSocket\0needToConnect\0\0setState\0"
    "state\0sendMessageToLog\0errorMessage\0"
    "connected\0readyRead\0disconnected\0"
    "getErrorMessqage\0QAbstractSocket::SocketError\0"
    "socketError\0send\0QByteArray&\0array\0"
    "checkConnection"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TcpSocket[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    1,   60,    2, 0x06 /* Public */,
       5,    1,   63,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   66,    2, 0x0a /* Public */,
       8,    0,   67,    2, 0x0a /* Public */,
       9,    0,   68,    2, 0x0a /* Public */,
      10,    1,   69,    2, 0x0a /* Public */,
      13,    1,   72,    2, 0x0a /* Public */,
      16,    0,   75,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void,

       0        // eod
};

void TcpSocket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TcpSocket *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->needToConnect(); break;
        case 1: _t->setState((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->sendMessageToLog((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->connected(); break;
        case 4: _t->readyRead(); break;
        case 5: _t->disconnected(); break;
        case 6: _t->getErrorMessqage((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 7: _t->send((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 8: _t->checkConnection(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TcpSocket::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpSocket::needToConnect)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TcpSocket::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpSocket::setState)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TcpSocket::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpSocket::sendMessageToLog)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TcpSocket::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_TcpSocket.data,
    qt_meta_data_TcpSocket,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TcpSocket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TcpSocket::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TcpSocket.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TcpSocket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void TcpSocket::needToConnect()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TcpSocket::setState(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TcpSocket::sendMessageToLog(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
