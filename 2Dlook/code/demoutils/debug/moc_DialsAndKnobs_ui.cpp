/****************************************************************************
** Meta object code from reading C++ file 'DialsAndKnobs_ui.h'
**
** Created: Mon Oct 22 10:34:56 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/DialsAndKnobs_ui.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DialsAndKnobs_ui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FileNameLineEdit[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   18,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      47,   17,   17,   17, 0x0a,
      64,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FileNameLineEdit[] = {
    "FileNameLineEdit\0\0name\0updateFilename(QString)\0"
    "setFromBrowser()\0callUpdateFilename()\0"
};

void FileNameLineEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FileNameLineEdit *_t = static_cast<FileNameLineEdit *>(_o);
        switch (_id) {
        case 0: _t->updateFilename((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->setFromBrowser(); break;
        case 2: _t->callUpdateFilename(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FileNameLineEdit::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FileNameLineEdit::staticMetaObject = {
    { &QLineEdit::staticMetaObject, qt_meta_stringdata_FileNameLineEdit,
      qt_meta_data_FileNameLineEdit, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FileNameLineEdit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FileNameLineEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FileNameLineEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FileNameLineEdit))
        return static_cast<void*>(const_cast< FileNameLineEdit*>(this));
    return QLineEdit::qt_metacast(_clname);
}

int FileNameLineEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void FileNameLineEdit::updateFilename(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_UpdatingTextEdit[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   18,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      41,   17,   17,   17, 0x0a,
      62,   56,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_UpdatingTextEdit[] = {
    "UpdatingTextEdit\0\0name\0sendText(QString)\0"
    "callSendText()\0value\0updateText(QString)\0"
};

void UpdatingTextEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        UpdatingTextEdit *_t = static_cast<UpdatingTextEdit *>(_o);
        switch (_id) {
        case 0: _t->sendText((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->callSendText(); break;
        case 2: _t->updateText((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData UpdatingTextEdit::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject UpdatingTextEdit::staticMetaObject = {
    { &QTextEdit::staticMetaObject, qt_meta_stringdata_UpdatingTextEdit,
      qt_meta_data_UpdatingTextEdit, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &UpdatingTextEdit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *UpdatingTextEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *UpdatingTextEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UpdatingTextEdit))
        return static_cast<void*>(const_cast< UpdatingTextEdit*>(this));
    return QTextEdit::qt_metacast(_clname);
}

int UpdatingTextEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTextEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void UpdatingTextEdit::sendText(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
