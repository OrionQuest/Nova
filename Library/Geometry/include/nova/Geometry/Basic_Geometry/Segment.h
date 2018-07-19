//!#####################################################################
//! \file Segment.h
//!#####################################################################
// Class Segment
//######################################################################
#ifndef __Segment__
#define __Segment__

#include <nova/Tools/Vectors/Vector.h>

namespace Nova{
template<class T,int d>
class Segment
{
    using TV            = Vector<T,d>;

  public:
    TV x1,x2;

    Segment()
        :x1(0),x2(TV::Axis_Vector(0))
    {}

    Segment(const TV& x1_input,const TV& x2_input)
        :x1(x1_input),x2(x2_input)
    {}

//######################################################################
    TV Closest_Point_On_Segment(const TV& point) const;
    T Distance_From_Point_To_Segment(const TV& point) const;
//######################################################################
};
}
#endif
