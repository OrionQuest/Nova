//!#####################################################################
//! \file Sphere.h
//!#####################################################################
// Class Sphere
//######################################################################
#ifndef __Sphere__
#define __Sphere__

#include <nova/Tools/Vectors/Vector.h>
#include <nova/Tools/Utilities/Constants.h>

namespace Nova{
template<class T,int d>
class Sphere
{
    using TV                    = Vector<T,d>;

  public:
    TV center;
    T radius;

    Sphere(const TV& center_input,const T& radius_input)
        :center(center_input),radius(radius_input)
    {}

    T Signed_Distance(const TV& X) const
    {return (X-center).Norm()-radius;}

    T Size() const 
    {
      if(d==2) return pi*radius*radius;
      if(d==3) return four_thirds*pi*radius*radius*radius;
    }
};
}
#endif
