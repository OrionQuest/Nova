//!#####################################################################
//! \file Plane.h
//!#####################################################################
// Class Plane
//######################################################################
#ifndef __Plane__
#define __Plane__

#include <nova/Tools/Vectors/Vector.h>

namespace Nova{
template<class T>
class Plane
{
    using TV            = Vector<T,3>;

  public:
    TV normal;
    TV x1;          // point on the plane

    Plane()
        :normal({0,1,0}),x1(0)
    {}

    Plane(const TV& normal_input,const TV& x1_input)
        :normal(normal_input),x1(x1_input)
    {}

    Plane(const TV& x1_input,const TV& x2_input,const TV& x3_input)
    {Specify_Three_Points(x1_input,x2_input,x3_input);}

    Plane(const Vector<TV,3>& X)
    {Specify_Three_Points(X(0),X(1),X(2));}

    void Specify_Three_Points(const TV& x1_input,const TV& x2_input,const TV& x3_input)
    {
        normal=Normal(x1_input,x2_input,x3_input);
        x1=x1_input;
    }

    static TV Normal(const TV& x1,const TV& x2,const TV& x3)
    {return Cross_Product(x2-x1,x3-x1).Normalized();}

    TV Normal() const
    {return normal;}

    T Signed_Distance(const TV& location) const
    {return normal.Dot_Product(location-x1);}

    // inside is the half space behind the normal
    bool Inside(const TV& location,const T thickness_over_two=0) const
    {return Signed_Distance(location)<=-thickness_over_two;}

    bool Outside(const TV& location,const T thickness_over_two=0) const
    {return !Inside(location,-thickness_over_two);}
};
}
#endif
