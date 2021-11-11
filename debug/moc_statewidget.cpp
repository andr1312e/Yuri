/****************************************************************************
** Meta object code from reading C++ file 'statewidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../ui/statewidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'statewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_StateWidget_t {
    QByteArrayData data[18];
    char stringdata0[233];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_StateWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_StateWidget_t qt_meta_stringdata_StateWidget = {
    {
QT_MOC_LITERAL(0, 0, 11), // "StateWidget"
QT_MOC_LITERAL(1, 12, 15), // "SetStateMessage"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 9), // "messageId"
QT_MOC_LITERAL(4, 39, 9), // "fistParam"
QT_MOC_LITERAL(5, 49, 11), // "SecondParam"
QT_MOC_LITERAL(6, 61, 23), // "SetStateButtonIdClicked"
QT_MOC_LITERAL(7, 85, 2), // "id"
QT_MOC_LITERAL(8, 88, 23), // "GetStateButtonIdClicked"
QT_MOC_LITERAL(9, 112, 16), // "messageWantToGet"
QT_MOC_LITERAL(10, 129, 16), // "SetButtonEnabled"
QT_MOC_LITERAL(11, 146, 5), // "state"
QT_MOC_LITERAL(12, 152, 10), // "ConsoleLog"
QT_MOC_LITERAL(13, 163, 7), // "message"
QT_MOC_LITERAL(14, 171, 19), // "ChangeSpeedLineEdit"
QT_MOC_LITERAL(15, 191, 10), // "doplerText"
QT_MOC_LITERAL(16, 202, 20), // "ChangeDoplerLineEdit"
QT_MOC_LITERAL(17, 223, 9) // "speedText"

    },
    "StateWidget\0SetStateMessage\0\0messageId\0"
    "fistParam\0SecondParam\0SetStateButtonIdClicked\0"
    "id\0GetStateButtonIdClicked\0messageWantToGet\0"
    "SetButtonEnabled\0state\0ConsoleLog\0"
    "message\0ChangeSpeedLineEdit\0doplerText\0"
    "ChangeDoplerLineEdit\0speedText"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_StateWidget[] = {

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
       1,    3,   59,    2, 0x06 /* Public */,
       1,    2,   66,    2, 0x26 /* Public | MethodCloned */,
       1,    1,   71,    2, 0x26 /* Public | MethodCloned */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   74,    2, 0x0a /* Public */,
       8,    1,   77,    2, 0x0a /* Public */,
      10,    1,   80,    2, 0x0a /* Public */,
      12,    1,   83,    2, 0x0a /* Public */,
      14,    1,   86,    2, 0x08 /* Private */,
      16,    1,   89,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::UChar, QMetaType::Double, QMetaType::Double,    3,    4,    5,
    QMetaType::Void, QMetaType::UChar, QMetaType::Double,    3,    4,
    QMetaType::Void, QMetaType::UChar,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void, QMetaType::QString,   17,

       0        // eod
};

void StateWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<StateWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SetStateMessage((*reinterpret_cast< quint8(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 1: _t->SetStateMessage((*reinterpret_cast< quint8(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 2: _t->SetStateMessage((*reinterpret_cast< quint8(*)>(_a[1]))); break;
        case 3: _t->SetStateButtonIdClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->GetStateButtonIdClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->SetButtonEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->ConsoleLog((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->ChangeSpeedLineEdit((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->ChangeDoplerLineEdit((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (StateWidget::*)(quint8 , double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&StateWidget::SetStateMessage)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject StateWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_StateWidget.data,
    qt_meta_data_StateWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *StateWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *StateWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_StateWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int StateWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void StateWidget::SetStateMessage(quint8 _t1, double _t2, double _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
