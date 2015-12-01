/****************************************************************************
** Meta object code from reading C++ file 'hciwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../hciwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hciwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_HCIWindow_t {
    QByteArrayData data[18];
    char stringdata[153];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HCIWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HCIWindow_t qt_meta_stringdata_HCIWindow = {
    {
QT_MOC_LITERAL(0, 0, 9), // "HCIWindow"
QT_MOC_LITERAL(1, 10, 11), // "fileloadsig"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 11), // "savefilesig"
QT_MOC_LITERAL(4, 35, 13), // "setfileloaded"
QT_MOC_LITERAL(5, 49, 4), // "open"
QT_MOC_LITERAL(6, 54, 4), // "save"
QT_MOC_LITERAL(7, 59, 4), // "icut"
QT_MOC_LITERAL(8, 64, 5), // "ishow"
QT_MOC_LITERAL(9, 70, 5), // "vshow"
QT_MOC_LITERAL(10, 76, 4), // "vcut"
QT_MOC_LITERAL(11, 81, 5), // "about"
QT_MOC_LITERAL(12, 87, 9), // "imageload"
QT_MOC_LITERAL(13, 97, 9), // "videoload"
QT_MOC_LITERAL(14, 107, 8), // "imagecut"
QT_MOC_LITERAL(15, 116, 9), // "nextFrame"
QT_MOC_LITERAL(16, 126, 11), // "showStatues"
QT_MOC_LITERAL(17, 138, 14) // "finishsavefile"

    },
    "HCIWindow\0fileloadsig\0\0savefilesig\0"
    "setfileloaded\0open\0save\0icut\0ishow\0"
    "vshow\0vcut\0about\0imageload\0videoload\0"
    "imagecut\0nextFrame\0showStatues\0"
    "finishsavefile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HCIWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x06 /* Public */,
       3,    0,   95,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   96,    2, 0x08 /* Private */,
       5,    0,   97,    2, 0x08 /* Private */,
       6,    0,   98,    2, 0x08 /* Private */,
       7,    0,   99,    2, 0x08 /* Private */,
       8,    0,  100,    2, 0x08 /* Private */,
       9,    0,  101,    2, 0x08 /* Private */,
      10,    0,  102,    2, 0x08 /* Private */,
      11,    0,  103,    2, 0x08 /* Private */,
      12,    0,  104,    2, 0x08 /* Private */,
      13,    0,  105,    2, 0x08 /* Private */,
      14,    0,  106,    2, 0x08 /* Private */,
      15,    0,  107,    2, 0x08 /* Private */,
      16,    0,  108,    2, 0x08 /* Private */,
      17,    0,  109,    2, 0x08 /* Private */,

 // signals: parameters
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

       0        // eod
};

void HCIWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HCIWindow *_t = static_cast<HCIWindow *>(_o);
        switch (_id) {
        case 0: _t->fileloadsig(); break;
        case 1: _t->savefilesig(); break;
        case 2: _t->setfileloaded(); break;
        case 3: _t->open(); break;
        case 4: _t->save(); break;
        case 5: _t->icut(); break;
        case 6: _t->ishow(); break;
        case 7: _t->vshow(); break;
        case 8: _t->vcut(); break;
        case 9: _t->about(); break;
        case 10: _t->imageload(); break;
        case 11: _t->videoload(); break;
        case 12: _t->imagecut(); break;
        case 13: _t->nextFrame(); break;
        case 14: _t->showStatues(); break;
        case 15: _t->finishsavefile(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (HCIWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HCIWindow::fileloadsig)) {
                *result = 0;
            }
        }
        {
            typedef void (HCIWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HCIWindow::savefilesig)) {
                *result = 1;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject HCIWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_HCIWindow.data,
      qt_meta_data_HCIWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *HCIWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HCIWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_HCIWindow.stringdata))
        return static_cast<void*>(const_cast< HCIWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int HCIWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void HCIWindow::fileloadsig()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void HCIWindow::savefilesig()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
