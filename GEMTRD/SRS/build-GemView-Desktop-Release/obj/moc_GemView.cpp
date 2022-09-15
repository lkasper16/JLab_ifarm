/****************************************************************************
** Meta object code from reading C++ file 'GemView.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../GemViewForJLab/src/GemView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GemView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GemView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,    8,    9,    8, 0x08,
      32,   30,    8,    8, 0x08,
      54,   30,    8,    8, 0x08,
      79,    8,    8,    8, 0x08,
      96,    8,    9,    8, 0x08,
     117,    8,    8,    8, 0x08,
     150,  135,    8,    8, 0x08,
     173,    8,    8,    8, 0x08,
     188,    8,    8,    8, 0x08,
     238,  209,    8,    8, 0x08,
     276,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_GemView[] = {
    "GemView\0\0int\0GemMonitorLoop()\0i\0"
    "SpinCurrentEvent(int)\0ChangeTimerInterval(int)\0"
    "ChangeAutoMode()\0AutoGemMonitorLoop()\0"
    "ParserInnerLoop()\0outputRootFile\0"
    "WriteRootFile(TString)\0SearchEvents()\0"
    "AnalyzeMultiEvents()\0inputDataFile,outputRootFile\0"
    "AnalyzeMultipleFiles(TString,TString)\0"
    "ProducePedestals()\0"
};

void GemView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GemView *_t = static_cast<GemView *>(_o);
        switch (_id) {
        case 0: { int _r = _t->GemMonitorLoop();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 1: _t->SpinCurrentEvent((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->ChangeTimerInterval((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->ChangeAutoMode(); break;
        case 4: { int _r = _t->AutoGemMonitorLoop();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 5: _t->ParserInnerLoop(); break;
        case 6: _t->WriteRootFile((*reinterpret_cast< TString(*)>(_a[1]))); break;
        case 7: _t->SearchEvents(); break;
        case 8: _t->AnalyzeMultiEvents(); break;
        case 9: _t->AnalyzeMultipleFiles((*reinterpret_cast< TString(*)>(_a[1])),(*reinterpret_cast< TString(*)>(_a[2]))); break;
        case 10: _t->ProducePedestals(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GemView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GemView::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_GemView,
      qt_meta_data_GemView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GemView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GemView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GemView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GemView))
        return static_cast<void*>(const_cast< GemView*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int GemView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
