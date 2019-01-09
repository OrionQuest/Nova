//!#####################################################################
//! \file Box_Implicit_Object.h
//!#####################################################################
// Class Box_Implicit_Object
//######################################################################
#ifndef __Box_Implicit_Object__
#define __Box_Implicit_Object__

#include <nova/Geometry/Basic_Geometry/Box.h>
#include <nova/Geometry/Implicit_Objects/Analytic_Implicit_Object.h>
#include <nova/Tools/Log/Debug_Utilities.h>

namespace Nova{
template<class T,int d>
class Box_Implicit_Object: public Analytic_Implicit_Object<T,d>, public Box<T,d>
{
    using TV                        = Vector<T,d>;
    using Implicit_Base             = Analytic_Implicit_Object<T,d>;
    using Geometry_Base             = Box<T,d>;

  public:
    Box_Implicit_Object(const TV& min_corner,const TV& max_corner)
        :Geometry_Base(min_corner,max_corner)
    {}

    ~Box_Implicit_Object() {}

//######################################################################
    T Signed_Distance(const TV& X) const override
    {return Geometry_Base::Signed_Distance(X);}
//######################################################################
};
}
#endif
