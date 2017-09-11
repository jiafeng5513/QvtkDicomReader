/****************************************************************************
** Meta object code from reading C++ file 'SlicePlayer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../SlicePlayer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SlicePlayer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SlicePlayer_t {
    QByteArrayData data[4];
    char stringdata0[49];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SlicePlayer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SlicePlayer_t qt_meta_stringdata_SlicePlayer = {
    {
QT_MOC_LITERAL(0, 0, 11), // "SlicePlayer"
QT_MOC_LITERAL(1, 12, 21), // "isTimeToTurnNextSlice"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 13) // "isTimeToReset"

    },
    "SlicePlayer\0isTimeToTurnNextSlice\0\0"
    "isTimeToReset"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SlicePlayer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,
       3,    0,   25,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SlicePlayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SlicePlayer *_t = static_cast<SlicePlayer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->isTimeToTurnNextSlice(); break;
        case 1: _t->isTimeToReset(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SlicePlayer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SlicePlayer::isTimeToTurnNextSlice)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (SlicePlayer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SlicePlayer::isTimeToReset)) {
                *result = 1;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject SlicePlayer::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_SlicePlayer.data,
      qt_meta_data_SlicePlayer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SlicePlayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SlicePlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SlicePlayer.stringdata0))
        return static_cast<void*>(const_cast< SlicePlayer*>(this));
    return QThread::qt_metacast(_clname);
}

int SlicePlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void SlicePlayer::isTimeToTurnNextSlice()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SlicePlayer::isTimeToReset()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
