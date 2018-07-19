//!#####################################################################
//! \file Cylinder.h
//!#####################################################################
// Class Cylinder
//######################################################################
#ifndef __Cylinder__
#define __Cylinder__

#include <nova/Geometry/Basic_Geometry/Plane.h>

namespace Nova{
template<class T>
class Cylinder
{
    using TV                    = Vector<T,3>;

  public:
    T height,radius;
    Plane<T> plane1,plane2;             // plane2 is height units behind circle

    Cylinder()
        :height(1),radius(1),plane2(plane1.x1-height*plane1.normal,-plane1.normal)
    {}

    Cylinder(const TV& point1,const TV& point2,const T radius_input)
        :radius(radius_input)
    {Set_Endpoints(point1,point2);}

    void Set_Height(const T height_input)
    {height=height_input;plane2.x1=plane1.x1-height*plane1.normal;}

    void Set_Endpoints(const TV& point1,const TV& point2)
    {
        TV height_vector=point1-point2;height=height_vector.Normalize();
        plane1.x1=point1;plane1.normal=height_vector;
        plane2.x1=point2;plane2.normal=-plane1.normal;
    }
};
}
#endif
