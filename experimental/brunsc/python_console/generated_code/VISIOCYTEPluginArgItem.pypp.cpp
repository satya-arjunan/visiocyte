// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrappable_visiocyte.h"
#include "VISIOCYTEPluginArgItem.pypp.hpp"

namespace bp = boost::python;

void set_pluginargitem_pointer(
                    VISIOCYTEPluginArgItem& arg, 
                    bp::object& bpo) 
            {
                arg.p = &bpo;
            }
            bp::object* get_pluginargitem_pointer(
                    VISIOCYTEPluginArgItem& arg) 
            {
                return static_cast<bp::object*>(arg.p);
            }

void register_VISIOCYTEPluginArgItem_class(){

    { //::VISIOCYTEPluginArgItem
        typedef bp::class_< VISIOCYTEPluginArgItem > VISIOCYTEPluginArgItem_exposer_t;
        VISIOCYTEPluginArgItem_exposer_t VISIOCYTEPluginArgItem_exposer = VISIOCYTEPluginArgItem_exposer_t( "VISIOCYTEPluginArgItem" );
        bp::scope VISIOCYTEPluginArgItem_scope( VISIOCYTEPluginArgItem_exposer );
        { //property "type"[fget=::get_argitem_type, fset=::set_argitem_type]
        
            typedef ::std::string ( *fget )( ::VISIOCYTEPluginArgItem const & );
            typedef void ( *fset )( ::VISIOCYTEPluginArgItem &,::std::string const & );
            
            VISIOCYTEPluginArgItem_exposer.add_property( 
                "type"
                , fget( &::get_argitem_type )
                , fset( &::set_argitem_type ) );
        
        }
        VISIOCYTEPluginArgItem_exposer.add_property("p",
                    bp::make_function(&get_pluginargitem_pointer,
                        bp::return_internal_reference<>()),
                    bp::make_function(&set_pluginargitem_pointer,
                        bp::with_custodian_and_ward<1,2>() ));
    }

}
