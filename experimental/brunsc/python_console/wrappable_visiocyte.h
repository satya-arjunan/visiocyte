#ifndef WRAPPABLE_VISIOCYTE_H_
#define WRAPPABLE_VISIOCYTE_H_

#include "ImageWindow.h"
#include "visiocyte_qt_environment.h"
#include "SimTKcommon/internal/Rotation.h"
#include "SimTKcommon/internal/Quaternion.h"
#include "ImageWindow.h"

std::string get_argitem_type(const VISIOCYTEPluginArgItem& item);
void set_argitem_type(VISIOCYTEPluginArgItem& item, const std::string& s);
std::string get_argitem_pointer(const VISIOCYTEPluginArgItem& item);
void set_argitem_pointer(VISIOCYTEPluginArgItem& item, void* ptr);

// Unfortunately, we might have to instantiate each possible subtype of VISIOCYTEPluginArg
// to expose it to python
template<class T>
class VISIOCYTEPluginArg : public VISIOCYTEPluginArgItem {
public:
    VISIOCYTEPluginArg(T& item, const std::string& typeName) {
        setItem(item);
        setType(typeName);
    }
    const T& getItem() const {
        const T* ptr = static_cast<const T*>(p);
        return *ptr;
    }
    void setItem(T& item) {p = (void*) &item;}
    std::string getType() const {return type.toStdString();}
    void setType(const std::string& s) {type = QString::fromStdString(s);}
};

// typedef c_array< double, 3 > double3;
// typedef c_array< double3, 3 > double3x3;

// "extern" to avoid multiply defined symbol error on Mac
#ifdef _MSC_VER
#define PYVISIOCYTE_HEADER_TEMPLATE template
unsigned int qHash(const LocationSimple& loc);
unsigned int qHash(const QPolygon&);
unsigned int qHash(const VISIOCYTEPluginArgItem& lhs);
bool operator==(const LocationSimple&, const LocationSimple&);
// operator== needed for wrapping QList<VISIOCYTEPluginArgItem>
bool operator==(const VISIOCYTEPluginArgItem& lhs, const VISIOCYTEPluginArgItem& rhs);
#else
#define PYVISIOCYTE_HEADER_TEMPLATE extern template
#endif

PYVISIOCYTE_HEADER_TEMPLATE class QList<LocationSimple>;
PYVISIOCYTE_HEADER_TEMPLATE class QVector<QPoint>;
PYVISIOCYTE_HEADER_TEMPLATE class QList<QPolygon>;
PYVISIOCYTE_HEADER_TEMPLATE class QHash<int, int>;
PYVISIOCYTE_HEADER_TEMPLATE class QList<VISIOCYTEPluginArgItem>;
PYVISIOCYTE_HEADER_TEMPLATE class c_array< double, 3 >;
PYVISIOCYTE_HEADER_TEMPLATE class c_array< c_array< double, 3 >, 3 >;
PYVISIOCYTE_HEADER_TEMPLATE class VISIOCYTEPluginArg< c_array< c_array< double, 3 >, 3 > >;
PYVISIOCYTE_HEADER_TEMPLATE class SimTK::Rotation_<double>;
PYVISIOCYTE_HEADER_TEMPLATE class SimTK::Vec<3, double, 1>;
PYVISIOCYTE_HEADER_TEMPLATE class SimTK::Quaternion_<double>;

/*! \brief Returns general parameters of the VISIOCYTE program.
 *
 */
VISIOCYTE_GlobalSetting getGlobalSetting();

/*! \brief Calls a function in a dynamically loaded VISIOCYTE plugin module.
 *
 */
bool callPluginFunc(const QString & plugin_name, const QString & func_name,
        const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);

/*! \brief Sets general parameters of the VISIOCYTE program.
 *
 */
bool setGlobalSetting(VISIOCYTE_GlobalSetting& gs);


namespace pyplusplus { namespace alias {
    // Need to wrap QVector<QPoint> to avoid runtime error at startup on Mac
    // This wrapper is hollow.
    typedef QVector<QPoint> QVector_QPoint;
    typedef c_array<unsigned char, 3> c_array_uint_3;
    typedef c_array<int, 3> c_array_int_3;
    typedef c_array<unsigned char, 4> c_array_uint_4;
    typedef c_array<float, 3> c_array_float_3;
    typedef c_array<short, 3> c_array_short_3;

    typedef c_array< double, 3 > double3;
    typedef c_array< double3, 3 > double3x3;
    typedef VISIOCYTEPluginArg<double3x3> VISIOCYTEPluginArg_double3x3;
    typedef SimTK::Rotation_<double> Rotation;
    typedef SimTK::Vec<3, double, 1> Vec3;
}}

#endif /* WRAPPABLE_VISIOCYTE_H_ */
