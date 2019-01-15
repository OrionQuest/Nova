//!#####################################################################
//! \file Rigid_Geometry.h
//!#####################################################################
// Class Rigid_Geometry
//######################################################################
#ifndef __Rigid_Geometry__
#define __Rigid_Geometry__

#include <nova/Geometry/Implicit_Objects/Implicit_Object.h>
#include <nova/Geometry/Solids_Geometry/Rigid_Geometry_Policy.h>
#include <nova/Tools/Utilities/Range.h>

namespace Nova{
template<class T,int d>
class Rigid_Geometry
{
    using TV                                    = Vector<T,d>;
    using T_Implicit_Object                     = Implicit_Object<T,d>;
    using T_Simplicial_Object                   = Rigid_Geometry_Policy<T,d>::T_Simplicial_Object;

  public:
    T_Simplicial_Object *object;
    T_Implicit_Object *implicit_object;
    Range<T,d> axis_aligned_bounding_box;
    bool bounding_box_up_to_date;

    Rigid_Geometry()
        :object(nullptr),implicit_object(nullptr),bounding_box_up_to_date(false)
    {}

    Rigid_Geometry(T_Simplicial_Object *object_input,T_Implicit_Object *implicit_object_input)
        :object(object_input),implicit_object(implicit_object_input),bounding_box_up_to_date(false)
    {}

    ~Rigid_Geometry()
    {
        if(object!=nullptr) delete object;
        if(implicit_object!=nullptr) delete implicit_object;
    }
};
}
#endif
