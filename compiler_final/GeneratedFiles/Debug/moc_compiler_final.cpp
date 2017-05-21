/****************************************************************************
** Meta object code from reading C++ file 'compiler_final.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../compiler_final.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'compiler_final.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_compiler_final_t {
    QByteArrayData data[6];
    char stringdata0[111];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_compiler_final_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_compiler_final_t qt_meta_stringdata_compiler_final = {
    {
QT_MOC_LITERAL(0, 0, 14), // "compiler_final"
QT_MOC_LITERAL(1, 15, 23), // "on_actionOpen_triggered"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 23), // "on_actionSave_triggered"
QT_MOC_LITERAL(4, 64, 22), // "on_actionRun_triggered"
QT_MOC_LITERAL(5, 87, 23) // "on_actionExit_triggered"

    },
    "compiler_final\0on_actionOpen_triggered\0"
    "\0on_actionSave_triggered\0"
    "on_actionRun_triggered\0on_actionExit_triggered"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_compiler_final[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    0,   35,    2, 0x08 /* Private */,
       4,    0,   36,    2, 0x08 /* Private */,
       5,    0,   37,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void compiler_final::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        compiler_final *_t = static_cast<compiler_final *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_actionOpen_triggered(); break;
        case 1: _t->on_actionSave_triggered(); break;
        case 2: _t->on_actionRun_triggered(); break;
        case 3: _t->on_actionExit_triggered(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject compiler_final::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_compiler_final.data,
      qt_meta_data_compiler_final,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *compiler_final::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *compiler_final::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_compiler_final.stringdata0))
        return static_cast<void*>(const_cast< compiler_final*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int compiler_final::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
