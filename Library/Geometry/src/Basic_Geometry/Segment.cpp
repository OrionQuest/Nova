//!#####################################################################
//! \file Segment_2D.cpp
//!#####################################################################
#include <nova/Geometry/Basic_Geometry/Segment.h>
using namespace Nova;
//######################################################################
// Distance_From_Point_To_Segment
//######################################################################
template<class T,int d> Nova::Vector<T,d> Nova::Segment<T,d>::
Closest_Point_On_Segment(const TV& point) const
{
    TV v=x2-x1;
    T denominator=v.Dot_Product(v);
    if(denominator==0) return x1;       // x1 and x2 are a single point

    T t=v.Dot_Product(point-x1)/denominator;
    if(t<=0) return x1;
    else if(t>=1) return x2;
    return x1+(x2-x1)*t;
}
//######################################################################
// Distance_From_Point_To_Segment
//######################################################################
template<class T,int d> T Nova::Segment<T,d>::
Distance_From_Point_To_Segment(const TV& point) const
{
    TV v=Closest_Point_On_Segment(point);
    return (v-point).Norm();
}
//######################################################################
template class Nova::Segment<float,2>;
template class Nova::Segment<float,3>;
#ifdef COMPILE_WITH_DOUBLE_SUPPORT
template class Nova::Segment<double,2>;
template class Nova::Segment<double,3>;
#endif
