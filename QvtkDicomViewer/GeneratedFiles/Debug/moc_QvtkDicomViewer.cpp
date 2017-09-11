/****************************************************************************
** Meta object code from reading C++ file 'QvtkDicomViewer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QvtkDicomViewer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QvtkDicomViewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QvtkDicomViewer_t {
    QByteArrayData data[18];
    char stringdata0[267];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QvtkDicomViewer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QvtkDicomViewer_t qt_meta_stringdata_QvtkDicomViewer = {
    {
QT_MOC_LITERAL(0, 0, 15), // "QvtkDicomViewer"
QT_MOC_LITERAL(1, 16, 18), // "CursorValueChanged"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 19), // "OnChangeCursorValue"
QT_MOC_LITERAL(4, 56, 10), // "OnOpenFile"
QT_MOC_LITERAL(5, 67, 12), // "OnRenderText"
QT_MOC_LITERAL(6, 80, 9), // "OnForward"
QT_MOC_LITERAL(7, 90, 10), // "OnBackward"
QT_MOC_LITERAL(8, 101, 17), // "OnSelectedPointer"
QT_MOC_LITERAL(9, 119, 20), // "OnSelectedProtractor"
QT_MOC_LITERAL(10, 140, 15), // "OnSelectedRuler"
QT_MOC_LITERAL(11, 156, 17), // "OnSelectedContour"
QT_MOC_LITERAL(12, 174, 23), // "OnSelectedBiDimensional"
QT_MOC_LITERAL(13, 198, 19), // "OnSelectedGrayLevel"
QT_MOC_LITERAL(14, 218, 14), // "OnSelectedZoom"
QT_MOC_LITERAL(15, 233, 14), // "OnSelectedMove"
QT_MOC_LITERAL(16, 248, 10), // "OnNegative"
QT_MOC_LITERAL(17, 259, 7) // "OnReset"

    },
    "QvtkDicomViewer\0CursorValueChanged\0\0"
    "OnChangeCursorValue\0OnOpenFile\0"
    "OnRenderText\0OnForward\0OnBackward\0"
    "OnSelectedPointer\0OnSelectedProtractor\0"
    "OnSelectedRuler\0OnSelectedContour\0"
    "OnSelectedBiDimensional\0OnSelectedGrayLevel\0"
    "OnSelectedZoom\0OnSelectedMove\0OnNegative\0"
    "OnReset"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QvtkDicomViewer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   95,    2, 0x0a /* Public */,
       4,    0,   96,    2, 0x0a /* Public */,
       5,    0,   97,    2, 0x0a /* Public */,
       6,    0,   98,    2, 0x0a /* Public */,
       7,    0,   99,    2, 0x0a /* Public */,
       8,    0,  100,    2, 0x0a /* Public */,
       9,    0,  101,    2, 0x0a /* Public */,
      10,    0,  102,    2, 0x0a /* Public */,
      11,    0,  103,    2, 0x0a /* Public */,
      12,    0,  104,    2, 0x0a /* Public */,
      13,    0,  105,    2, 0x0a /* Public */,
      14,    0,  106,    2, 0x0a /* Public */,
      15,    0,  107,    2, 0x0a /* Public */,
      16,    0,  108,    2, 0x0a /* Public */,
      17,    0,  109,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
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

void QvtkDicomViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QvtkDicomViewer *_t = static_cast<QvtkDicomViewer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->CursorValueChanged(); break;
        case 1: _t->OnChangeCursorValue(); break;
        case 2: _t->OnOpenFile(); break;
        case 3: _t->OnRenderText(); break;
        case 4: _t->OnForward(); break;
        case 5: _t->OnBackward(); break;
        case 6: _t->OnSelectedPointer(); break;
        case 7: _t->OnSelectedProtractor(); break;
        case 8: _t->OnSelectedRuler(); break;
        case 9: _t->OnSelectedContour(); break;
        case 10: _t->OnSelectedBiDimensional(); break;
        case 11: _t->OnSelectedGrayLevel(); break;
        case 12: _t->OnSelectedZoom(); break;
        case 13: _t->OnSelectedMove(); break;
        case 14: _t->OnNegative(); break;
        case 15: _t->OnReset(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QvtkDicomViewer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QvtkDicomViewer::CursorValueChanged)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject QvtkDicomViewer::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QvtkDicomViewer.data,
      qt_meta_data_QvtkDicomViewer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QvtkDicomViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QvtkDicomViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QvtkDicomViewer.stringdata0))
        return static_cast<void*>(const_cast< QvtkDicomViewer*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int QvtkDicomViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void QvtkDicomViewer::CursorValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
