// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrappable_visiocyte.h"
#include "vec3_container_traits.hpp"
#include "Quaternion.pypp.hpp"

namespace bp = boost::python;

void register_Quaternion_class(){

    { //::SimTK::Quaternion_< double >
        typedef bp::class_< SimTK::Quaternion_< double > > Quaternion_exposer_t;
        Quaternion_exposer_t Quaternion_exposer = Quaternion_exposer_t( "Quaternion", bp::init< >("\n Default constructor produces the ZeroRotation quaternion [1 0 0 0]\n (not NaN - even in debug mode).\n") );
        bp::scope Quaternion_scope( Quaternion_exposer );
        Quaternion_exposer.def( bp::init< SimTK::Quaternion_< double > const & >(( bp::arg("q") ), "\n Zero-cost copy constructor just copies the source without conversion to\n canonical form or normalization.\n") );
        Quaternion_exposer.def( bp::init< double, double, double, double >(( bp::arg("e0"), bp::arg("e1"), bp::arg("e2"), bp::arg("e3") ), "\n Construct a quaternion from four scalars and normalize the result,\n which costs about 40 flops.\n") );
        Quaternion_exposer.def( bp::init< SimTK::Vec< 4, double, 1 > const & >(( bp::arg("q") ), "\n Construct a quaternion from a 4-vector and normalize the result,\n which costs about 40 flops.\n") );
        bp::implicitly_convertible< SimTK::Vec< 4, double, 1 > const &, SimTK::Quaternion_< double > >();
        Quaternion_exposer.def( bp::init< SimTK::Rotation_< double > const & >(( bp::arg("arg0") ), "\n Constructs a canonical quaternion from a rotation matrix (cost is\n about 60 flops).\n") );
        bp::implicitly_convertible< SimTK::Rotation_< double > const &, SimTK::Quaternion_< double > >();
        Quaternion_exposer.def( bp::init< SimTK::Vec< 4, double, 1 > const &, bool >(( bp::arg("v"), bp::arg("arg1") ), "\n Use this constructor only if you are *sure* v is normalized to 1.0.\n This zero cost method is faster than the Quaternion(Vec4) constructor\n which normalizes the Vec4. The second argument forces the compiler to\n call the fast constructor; it is otherwise ignored. By convention, set\n the second argument to true.\n") );
        { //::SimTK::Quaternion_< double >::asVec4
        
            typedef SimTK::Quaternion_< double > exported_class_t;
            typedef ::SimTK::Vec< 4, double, 1 > const & ( exported_class_t::*asVec4_function_type )(  ) const;
            
            Quaternion_exposer.def( 
                "asVec4"
                , asVec4_function_type( &::SimTK::Quaternion_< double >::asVec4 )
                , bp::return_value_policy< bp::copy_const_reference >()
                , "\n Zero-cost cast of a Quaternion_ to its underlying Vec4; this is E: not\n converted to axis-angle form.\n" );
        
        }
        { //::SimTK::Quaternion_< double >::convertQuaternionToAngleAxis
        
            typedef SimTK::Quaternion_< double > exported_class_t;
            typedef ::SimTK::Vec< 4, double, 1 > ( exported_class_t::*convertQuaternionToAngleAxis_function_type )(  ) const;
            
            Quaternion_exposer.def( 
                "convertQuaternionToAngleAxis"
                , convertQuaternionToAngleAxis_function_type( &::SimTK::Quaternion_< double >::convertQuaternionToAngleAxis )
                , "\n Returns [ a vx vy vz ] with (a,v) in canonical form, i.e.,\n -180 < a <= 180 and |v|=1.\n" );
        
        }
        { //::SimTK::Quaternion_< double >::normalize
        
            typedef SimTK::Quaternion_< double > exported_class_t;
            typedef ::SimTK::Quaternion_< double > ( exported_class_t::*normalize_function_type )(  ) const;
            
            Quaternion_exposer.def( 
                "normalize"
                , normalize_function_type( &::SimTK::Quaternion_< double >::normalize )
                , "\n Return a normalized copy of this quaternion; but do you really need to\n do this? Quaternions should be kept normalized at all times. One of\n the advantages of using them is that you dont have to check if they\n are normalized or renormalize them. However, under some situations they\n do need renormalization, but it is costly if you dont actually need it.\n aee normalizeThis() for details.\n" );
        
        }
        { //::SimTK::Quaternion_< double >::normalizeThis
        
            typedef SimTK::Quaternion_< double > exported_class_t;
            typedef ::SimTK::Quaternion_< double > & ( exported_class_t::*normalizeThis_function_type )(  ) ;
            
            Quaternion_exposer.def( 
                "normalizeThis"
                , normalizeThis_function_type( &::SimTK::Quaternion_< double >::normalizeThis )
                , bp::return_internal_reference< >()
                , "\n Normalize an already constructed quaternion in place; but do you really\n need to do this? Quaternions should be kept normalized at all times.\n One of the advantages of using them is that you dont have to check if\n they are normalized or renormalize them. However, under some situations\n they do need renormalization, but it is costly if you dont actually\n need it. If the quaternion is E: exactly zero, set it to [1 0 0 0]. If\n its magnitude is 0 < magnitude < epsilon  (epsilon is machine\n tolerance), set it to NaN (treated as an error). Otherwise, normalize\n the quaternion which costs about 40 flops. The quaternion is NOT put\n in canonical form.\n" );
        
        }
        { //::SimTK::Quaternion_< double >::operator=
        
            typedef SimTK::Quaternion_< double > exported_class_t;
            typedef ::SimTK::Quaternion_< double > & ( exported_class_t::*assign_function_type )( ::SimTK::Quaternion_< double > const & ) ;
            
            Quaternion_exposer.def( 
                "assign"
                , assign_function_type( &::SimTK::Quaternion_< double >::operator= )
                , ( bp::arg("q") )
                , bp::return_self< >()
                , "\n Zero-cost copy assignment just copies the source without conversion to\n canonical form or normalization.\n" );
        
        }
        { //::SimTK::Quaternion_< double >::setQuaternionFromAngleAxis
        
            typedef SimTK::Quaternion_< double > exported_class_t;
            typedef void ( exported_class_t::*setQuaternionFromAngleAxis_function_type )( ::SimTK::Vec< 4, double, 1 > const & ) ;
            
            Quaternion_exposer.def( 
                "setQuaternionFromAngleAxis"
                , setQuaternionFromAngleAxis_function_type( &::SimTK::Quaternion_< double >::setQuaternionFromAngleAxis )
                , ( bp::arg("av") )
                , "\n Set this quaternion from an angle-axis rotation packed into a 4-vector\n as [a vx vy vz]. The result will be put in canonical form, i.e.,\n it will have a non-negative first element. If the axis portion of av\n is a zero vector on input, the quaternion is set to all-NaN.\n" );
        
        }
        { //::SimTK::Quaternion_< double >::setQuaternionFromAngleAxis
        
            typedef SimTK::Quaternion_< double > exported_class_t;
            typedef void ( exported_class_t::*setQuaternionFromAngleAxis_function_type )( double const &,::SimTK::UnitVec< double, 1 > const & ) ;
            
            Quaternion_exposer.def( 
                "setQuaternionFromAngleAxis"
                , setQuaternionFromAngleAxis_function_type( &::SimTK::Quaternion_< double >::setQuaternionFromAngleAxis )
                , ( bp::arg("a"), bp::arg("v") )
                , "\n Set this quaternion from an angle-axis rotation provided as an angle a\n and a separate unit vector [vx vy vz]. The result will be put in\n canonical form, i.e., it will have a non-negative first element.\n" );
        
        }
        { //::SimTK::Quaternion_< double >::setQuaternionToNaN
        
            typedef SimTK::Quaternion_< double > exported_class_t;
            typedef void ( exported_class_t::*setQuaternionToNaN_function_type )(  ) ;
            
            Quaternion_exposer.def( 
                "setQuaternionToNaN"
                , setQuaternionToNaN_function_type( &::SimTK::Quaternion_< double >::setQuaternionToNaN )
                , "\n Set quaternion to all-NaN. Note that this is not the same as produced\n by default construction, even in Debug mode -- default construction\n always produces an identity rotation of [1 0 0 0].\n" );
        
        }
        { //::SimTK::Quaternion_< double >::setQuaternionToZeroRotation
        
            typedef SimTK::Quaternion_< double > exported_class_t;
            typedef void ( exported_class_t::*setQuaternionToZeroRotation_function_type )(  ) ;
            
            Quaternion_exposer.def( 
                "setQuaternionToZeroRotation"
                , setQuaternionToZeroRotation_function_type( &::SimTK::Quaternion_< double >::setQuaternionToZeroRotation )
                , "\n The ZeroRotation quaternion is [1 0 0 0]. Note: Default constructor\n is ZeroRotation (unlike Vec4P which start as NaN in Debug mode).\n" );
        
        }
        Quaternion_exposer.def(bp::indexing::container_suite<
                            SimTK::Quaternion_<double>,
                            bp::indexing::all_methods,
                            list_algorithms< vec_container_traits< SimTK::Quaternion_<double> > > >());
    }

}