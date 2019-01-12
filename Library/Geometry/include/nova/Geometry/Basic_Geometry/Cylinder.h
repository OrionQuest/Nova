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

    T Signed_Distance(const TV& X) const
    {
        TV v=X-plane1.x1;
        T plane1_distance=v.Dot_Product(plane1.normal),plane_distance=std::max(plane1_distance,-height-plane1_distance);
        T cylinder_distance=(v-plane1_distance*plane1.normal).Norm()-radius;
        return cylinder_distance>0 && plane_distance>0?std::sqrt(std::pow(cylinder_distance,2)+std::pow(plane_distance,2)):std::max(cylinder_distance,plane_distance);
    }
};
}
#endif
