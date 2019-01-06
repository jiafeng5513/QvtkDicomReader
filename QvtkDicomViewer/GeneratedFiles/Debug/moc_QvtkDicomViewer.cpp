/****************************************************************************
** Meta object code from reading C++ file 'QvtkDicomViewer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QvtkDicomViewer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QvtkDicomViewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QvtkDicomViewer_t {
    QByteArrayData data[53];
    char stringdata0[874];
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
QT_MOC_LITERAL(3, 36, 15), // "WindowWLChanged"
QT_MOC_LITERAL(4, 52, 15), // "AppStateChanged"
QT_MOC_LITERAL(5, 68, 16), // "OnPlayerTimerOut"
QT_MOC_LITERAL(6, 85, 16), // "OnChangeAppState"
QT_MOC_LITERAL(7, 102, 19), // "OnChangeCursorValue"
QT_MOC_LITERAL(8, 122, 17), // "OnChangeWindowsWL"
QT_MOC_LITERAL(9, 140, 18), // "OnOpenSeriesFolder"
QT_MOC_LITERAL(10, 159, 15), // "OnOpenDicomFile"
QT_MOC_LITERAL(11, 175, 18), // "OnOpenDicomDirFile"
QT_MOC_LITERAL(12, 194, 9), // "OnForward"
QT_MOC_LITERAL(13, 204, 10), // "OnBackward"
QT_MOC_LITERAL(14, 215, 14), // "OnResetToFirst"
QT_MOC_LITERAL(15, 230, 17), // "OnSelectedPointer"
QT_MOC_LITERAL(16, 248, 20), // "OnSelectedProtractor"
QT_MOC_LITERAL(17, 269, 15), // "OnSelectedRuler"
QT_MOC_LITERAL(18, 285, 17), // "OnSelectedContour"
QT_MOC_LITERAL(19, 303, 23), // "OnSelectedBiDimensional"
QT_MOC_LITERAL(20, 327, 19), // "OnSelectedGrayLevel"
QT_MOC_LITERAL(21, 347, 14), // "OnSelectedZoom"
QT_MOC_LITERAL(22, 362, 14), // "OnSelectedMove"
QT_MOC_LITERAL(23, 377, 10), // "OnNegative"
QT_MOC_LITERAL(24, 388, 7), // "OnReset"
QT_MOC_LITERAL(25, 396, 6), // "OnPlay"
QT_MOC_LITERAL(26, 403, 6), // "OnStop"
QT_MOC_LITERAL(27, 410, 16), // "OnSwitchProperty"
QT_MOC_LITERAL(28, 427, 38), // "on_treeView_customContextMenu..."
QT_MOC_LITERAL(29, 466, 3), // "pos"
QT_MOC_LITERAL(30, 470, 22), // "OnShowDicomCurrentTags"
QT_MOC_LITERAL(31, 493, 20), // "OnShowSelectedSeries"
QT_MOC_LITERAL(32, 514, 19), // "OnShowSelectedImage"
QT_MOC_LITERAL(33, 534, 27), // "OnSliceScrollBarValueChange"
QT_MOC_LITERAL(34, 562, 1), // "a"
QT_MOC_LITERAL(35, 564, 11), // "receiveData"
QT_MOC_LITERAL(36, 576, 4), // "data"
QT_MOC_LITERAL(37, 581, 15), // "OnLatterPatient"
QT_MOC_LITERAL(38, 597, 17), // "OnPreviousPatient"
QT_MOC_LITERAL(39, 615, 17), // "OnWindowWL_Defaut"
QT_MOC_LITERAL(40, 633, 14), // "OnWindowWL_All"
QT_MOC_LITERAL(41, 648, 21), // "OnWindowWL_CT_Abdomen"
QT_MOC_LITERAL(42, 670, 25), // "OnWindowWL_CT_BloodVessel"
QT_MOC_LITERAL(43, 696, 19), // "OnWindowWL_CT_Bones"
QT_MOC_LITERAL(44, 716, 19), // "OnWindowWL_CT_Brain"
QT_MOC_LITERAL(45, 736, 20), // "OnWindowWL_CT_Medias"
QT_MOC_LITERAL(46, 757, 19), // "OnWindowWL_CT_Lungs"
QT_MOC_LITERAL(47, 777, 17), // "OnTestEntrance_01"
QT_MOC_LITERAL(48, 795, 17), // "OnTestEntrance_02"
QT_MOC_LITERAL(49, 813, 19), // "On3D_Reconstruction"
QT_MOC_LITERAL(50, 833, 14), // "OnSegmentImage"
QT_MOC_LITERAL(51, 848, 14), // "OnRegistration"
QT_MOC_LITERAL(52, 863, 10) // "OnOpenLIDC"

    },
    "QvtkDicomViewer\0CursorValueChanged\0\0"
    "WindowWLChanged\0AppStateChanged\0"
    "OnPlayerTimerOut\0OnChangeAppState\0"
    "OnChangeCursorValue\0OnChangeWindowsWL\0"
    "OnOpenSeriesFolder\0OnOpenDicomFile\0"
    "OnOpenDicomDirFile\0OnForward\0OnBackward\0"
    "OnResetToFirst\0OnSelectedPointer\0"
    "OnSelectedProtractor\0OnSelectedRuler\0"
    "OnSelectedContour\0OnSelectedBiDimensional\0"
    "OnSelectedGrayLevel\0OnSelectedZoom\0"
    "OnSelectedMove\0OnNegative\0OnReset\0"
    "OnPlay\0OnStop\0OnSwitchProperty\0"
    "on_treeView_customContextMenuRequested\0"
    "pos\0OnShowDicomCurrentTags\0"
    "OnShowSelectedSeries\0OnShowSelectedImage\0"
    "OnSliceScrollBarValueChange\0a\0receiveData\0"
    "data\0OnLatterPatient\0OnPreviousPatient\0"
    "OnWindowWL_Defaut\0OnWindowWL_All\0"
    "OnWindowWL_CT_Abdomen\0OnWindowWL_CT_BloodVessel\0"
    "OnWindowWL_CT_Bones\0OnWindowWL_CT_Brain\0"
    "OnWindowWL_CT_Medias\0OnWindowWL_CT_Lungs\0"
    "OnTestEntrance_01\0OnTestEntrance_02\0"
    "On3D_Reconstruction\0OnSegmentImage\0"
    "OnRegistration\0OnOpenLIDC"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QvtkDicomViewer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      48,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  254,    2, 0x06 /* Public */,
       3,    0,  255,    2, 0x06 /* Public */,
       4,    0,  256,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,  257,    2, 0x0a /* Public */,
       6,    0,  258,    2, 0x0a /* Public */,
       7,    0,  259,    2, 0x0a /* Public */,
       8,    0,  260,    2, 0x0a /* Public */,
       9,    0,  261,    2, 0x0a /* Public */,
      10,    0,  262,    2, 0x0a /* Public */,
      11,    0,  263,    2, 0x0a /* Public */,
      12,    0,  264,    2, 0x0a /* Public */,
      13,    0,  265,    2, 0x0a /* Public */,
      14,    0,  266,    2, 0x0a /* Public */,
      15,    0,  267,    2, 0x0a /* Public */,
      16,    0,  268,    2, 0x0a /* Public */,
      17,    0,  269,    2, 0x0a /* Public */,
      18,    0,  270,    2, 0x0a /* Public */,
      19,    0,  271,    2, 0x0a /* Public */,
      20,    0,  272,    2, 0x0a /* Public */,
      21,    0,  273,    2, 0x0a /* Public */,
      22,    0,  274,    2, 0x0a /* Public */,
      23,    0,  275,    2, 0x0a /* Public */,
      24,    0,  276,    2, 0x0a /* Public */,
      25,    0,  277,    2, 0x0a /* Public */,
      26,    0,  278,    2, 0x0a /* Public */,
      27,    0,  279,    2, 0x0a /* Public */,
      28,    1,  280,    2, 0x0a /* Public */,
      30,    0,  283,    2, 0x0a /* Public */,
      31,    0,  284,    2, 0x0a /* Public */,
      32,    0,  285,    2, 0x0a /* Public */,
      33,    1,  286,    2, 0x0a /* Public */,
      35,    1,  289,    2, 0x0a /* Public */,
      37,    0,  292,    2, 0x0a /* Public */,
      38,    0,  293,    2, 0x0a /* Public */,
      39,    0,  294,    2, 0x0a /* Public */,
      40,    0,  295,    2, 0x0a /* Public */,
      41,    0,  296,    2, 0x0a /* Public */,
      42,    0,  297,    2, 0x0a /* Public */,
      43,    0,  298,    2, 0x0a /* Public */,
      44,    0,  299,    2, 0x0a /* Public */,
      45,    0,  300,    2, 0x0a /* Public */,
      46,    0,  301,    2, 0x0a /* Public */,
      47,    0,  302,    2, 0x0a /* Public */,
      48,    0,  303,    2, 0x0a /* Public */,
      49,    0,  304,    2, 0x0a /* Public */,
      50,    0,  305,    2, 0x0a /* Public */,
      51,    0,  306,    2, 0x0a /* Public */,
      52,    0,  307,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,   29,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   34,
    QMetaType::Void, QMetaType::QString,   36,
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
        case 1: _t->WindowWLChanged(); break;
        case 2: _t->AppStateChanged(); break;
        case 3: _t->OnPlayerTimerOut(); break;
        case 4: _t->OnChangeAppState(); break;
        case 5: _t->OnChangeCursorValue(); break;
        case 6: _t->OnChangeWindowsWL(); break;
        case 7: _t->OnOpenSeriesFolder(); break;
        case 8: _t->OnOpenDicomFile(); break;
        case 9: _t->OnOpenDicomDirFile(); break;
        case 10: _t->OnForward(); break;
        case 11: _t->OnBackward(); break;
        case 12: _t->OnResetToFirst(); break;
        case 13: _t->OnSelectedPointer(); break;
        case 14: _t->OnSelectedProtractor(); break;
        case 15: _t->OnSelectedRuler(); break;
        case 16: _t->OnSelectedContour(); break;
        case 17: _t->OnSelectedBiDimensional(); break;
        case 18: _t->OnSelectedGrayLevel(); break;
        case 19: _t->OnSelectedZoom(); break;
        case 20: _t->OnSelectedMove(); break;
        case 21: _t->OnNegative(); break;
        case 22: _t->OnReset(); break;
        case 23: _t->OnPlay(); break;
        case 24: _t->OnStop(); break;
        case 25: _t->OnSwitchProperty(); break;
        case 26: _t->on_treeView_customContextMenuRequested((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 27: _t->OnShowDicomCurrentTags(); break;
        case 28: _t->OnShowSelectedSeries(); break;
        case 29: _t->OnShowSelectedImage(); break;
        case 30: _t->OnSliceScrollBarValueChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 31: _t->receiveData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 32: _t->OnLatterPatient(); break;
        case 33: _t->OnPreviousPatient(); break;
        case 34: _t->OnWindowWL_Defaut(); break;
        case 35: _t->OnWindowWL_All(); break;
        case 36: _t->OnWindowWL_CT_Abdomen(); break;
        case 37: _t->OnWindowWL_CT_BloodVessel(); break;
        case 38: _t->OnWindowWL_CT_Bones(); break;
        case 39: _t->OnWindowWL_CT_Brain(); break;
        case 40: _t->OnWindowWL_CT_Medias(); break;
        case 41: _t->OnWindowWL_CT_Lungs(); break;
        case 42: _t->OnTestEntrance_01(); break;
        case 43: _t->OnTestEntrance_02(); break;
        case 44: _t->On3D_Reconstruction(); break;
        case 45: _t->OnSegmentImage(); break;
        case 46: _t->OnRegistration(); break;
        case 47: _t->OnOpenLIDC(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (QvtkDicomViewer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QvtkDicomViewer::CursorValueChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (QvtkDicomViewer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QvtkDicomViewer::WindowWLChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (QvtkDicomViewer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QvtkDicomViewer::AppStateChanged)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QvtkDicomViewer::staticMetaObject = {
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
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int QvtkDicomViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 48)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 48;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 48)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 48;
    }
    return _id;
}

// SIGNAL 0
void QvtkDicomViewer::CursorValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void QvtkDicomViewer::WindowWLChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void QvtkDicomViewer::AppStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
