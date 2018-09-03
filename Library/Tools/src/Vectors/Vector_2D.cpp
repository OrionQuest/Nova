//!#####################################################################
//! \file Vector_2D.cpp
//!#####################################################################
#include <nova/Tools/Vectors/Vector.h>
using namespace Nova;
//######################################################################
// Orthogonal_Vector
//######################################################################
template<class T,int d,bool order_ascending> Vector<T,d,order_ascending> Vector<T,d,order_ascending>::
Orthogonal_Vector() const
{
    return Vector({-_data[1],_data[0]});
}
//######################################################################
template class Nova::Vector<float,2>;
template class Nova::Vector<double,2>;
