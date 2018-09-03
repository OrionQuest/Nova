//!#####################################################################
//! \file Vector_3D.cpp
//!#####################################################################
#include <nova/Tools/Vectors/Vector.h>
using namespace Nova;
//######################################################################
// Orthogonal_Vector
//######################################################################
template<class T,int d,bool order_ascending> Vector<T,d,order_ascending> Vector<T,d,order_ascending>::
Orthogonal_Vector() const
{
    T abs_x=fabs(_data[0]),abs_y=fabs(_data[1]),abs_z=fabs(_data[2]);
    if(abs_x<abs_y) return abs_x<abs_z?Vector({0,_data[2],-_data[1]}):Vector({_data[1],-_data[0],0});
    return abs_y<abs_z?Vector({-_data[2],0,_data[0]}):Vector({_data[1],-_data[0],0});
}
//######################################################################
template class Nova::Vector<float,3>;
template class Nova::Vector<double,3>;
