/****************************************************************************
** Meta object code from reading C++ file 'Opencv_example_plugin.h'
**
** Created: Thu Jun 11 14:16:02 2015
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Opencv_example_plugin.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Opencv_example_plugin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LoadImageAndSWCPlugin[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_LoadImageAndSWCPlugin[] = {
    "LoadImageAndSWCPlugin\0"
};

const QMetaObject LoadImageAndSWCPlugin::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_LoadImageAndSWCPlugin,
      qt_meta_data_LoadImageAndSWCPlugin, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LoadImageAndSWCPlugin::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LoadImageAndSWCPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LoadImageAndSWCPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LoadImageAndSWCPlugin))
        return static_cast<void*>(const_cast< LoadImageAndSWCPlugin*>(this));
    if (!strcmp(_clname, "VISIOCYTEPluginInterface2_1"))
        return static_cast< VISIOCYTEPluginInterface2_1*>(const_cast< LoadImageAndSWCPlugin*>(this));
    if (!strcmp(_clname, "com.janelia.visiocyte.VISIOCYTEPluginInterface/2.1"))
        return static_cast< VISIOCYTEPluginInterface2_1*>(const_cast< LoadImageAndSWCPlugin*>(this));
    return QObject::qt_metacast(_clname);
}

int LoadImageAndSWCPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
