//!#####################################################################
//! \file Fast_Marching.h
//!#####################################################################
// Class Fast_Marching
//######################################################################
#ifndef __Fast_Marching__
#define __Fast_Marching__

#include <nova/Geometry/Level_Sets/Levelset_Utilities.h>
#include <nova/Tools/Utilities/Utilities.h>
#include <cassert>

namespace Nova{
template<class T>
class Fast_Marching
{
  public:
    static T Solve_Quadratic(const T phi,const T value_x,const T value_y,const T dx,const T dy)
    {
        assert(Levelset_Utilities<T>::Sign(value_x)==Levelset_Utilities<T>::Sign(value_y));
        if(fabs(value_x) >= fabs(value_y)+dy) return value_y+Levelset_Utilities<T>::Sign(phi)*dy;
        else if(fabs(value_y) >= fabs(value_x)+dx) return value_x+Levelset_Utilities<T>::Sign(phi)*dx;
        else{T dx2=dx*dx,dy2=dy*dy,value=Nova_Utilities::Sqr(value_x-value_y);
            return (dy2*value_x+dx2*value_y+Levelset_Utilities<T>::Sign(phi)*dx*dy*std::sqrt(dx2+dy2-value))/(dx2+dy2);}
    }

    template<int dimension>
    static T Solve_Close_Point(const T phi,const int number_of_axis,const T value[dimension],const T dx[dimension])
    {
        assert(number_of_axis);
        if(dimension==1 || number_of_axis==1) return value[0]+Levelset_Utilities<T>::Sign(phi)*dx[0];
        if(dimension==2 || number_of_axis==2) return Solve_Quadratic(phi,value[0],value[1],dx[0],dx[1]);
        assert(dimension==3); // candidates exist in all three directions (must be in 3D)
        T value_yz=Solve_Quadratic(phi,value[1],value[2],dx[1],dx[2]);
        if(fabs(value[0]) >= fabs(value_yz)) return value_yz;
        T value_xz=Solve_Quadratic(phi,value[0],value[2],dx[0],dx[2]);
        if(fabs(value[1]) >= fabs(value_xz)) return value_xz;
        T value_xy=Solve_Quadratic(phi,value[0],value[1],dx[0],dx[1]);
        if(fabs(value[2]) >= fabs(value_xy)) return value_xy;
        // use the candidates in all three directions
        T dx2=dx[0]*dx[0],dy2=dx[1]*dx[1],dz2=dx[2]*dx[2],dx2dy2=dx2*dy2,dx2dz2=dx2*dz2,dy2dz2=dy2*dz2;
        return (dy2dz2*value[0]+dx2dz2*value[1]+dx2dy2*value[2]+Levelset_Utilities<T>::Sign(phi)*dx[0]*dx[1]*dx[2]*
                std::sqrt(dx2dy2+dx2dz2+dy2dz2-dx2*Nova_Utilities::Sqr(value[1]-value[2])-dy2*Nova_Utilities::Sqr(value[0]-value[2])-dz2*Nova_Utilities::Sqr(value[0]-value[1])))/(dx2dy2+dx2dz2+dy2dz2);
    }
};
}
#endif
