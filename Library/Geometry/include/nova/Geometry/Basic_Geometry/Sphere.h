//!#####################################################################
//! \file Sphere.h
//!#####################################################################
// Class Sphere
//######################################################################
#ifndef __Sphere__
#define __Sphere__

#include <nova/Tools/Vectors/Vector.h>

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
};
}
#endif
