//!#####################################################################
//! \file Cylinder_Implicit_Object.h
//!#####################################################################
// Class Cylinder_Implicit_Object
//######################################################################
#ifndef __Cylinder_Implicit_Object__
#define __Cylinder_Implicit_Object__

#include <nova/Geometry/Basic_Geometry/Cylinder.h>
#include <nova/Geometry/Implicit_Objects/Analytic_Implicit_Object.h>

namespace Nova{
template<class T>
class Cylinder_Implicit_Object: public Analytic_Implicit_Object<T,3>, public Cylinder<T>
{
    enum {d=3};
    using TV                        = Vector<T,d>;
    using Geometry_Base             = Cylinder<T>;

  public:
    Cylinder_Implicit_Object(const TV& point1,const TV& point2,const T radius)
        :Geometry_Base(point1,point2,radius)
    {}

    ~Cylinder_Implicit_Object() {}

//######################################################################
    T Signed_Distance(const TV& X) const override
    {return Geometry_Base::Signed_Distance(X);}
//######################################################################
};
}
#endif
