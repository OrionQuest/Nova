//!#####################################################################
//! \file Linear_Interpolation.h
//!#####################################################################
// Class Linear_Interpolation
//######################################################################
#ifndef __Linear_Interpolation__
#define __Linear_Interpolation__

#include <nova/Tools/Vectors/Vector.h>

namespace Nova{
template<class T,class T2>
class Linear_Interpolation
{
  public:
    static T2 Linear(const T2& u_left,const T2& u_right,const T x)
    {return (1-x)*u_left+x*u_right;}

    static T2 Linear(const T2& u_left,const T2& u_right,const Vector<T,1>& X)
    {return Linear(u_left,u_right,X(0));}

    static T2 Linear(const T x_left,const T x_right,const T2& u_left,const T2& u_right,const T x)
    {return u_left+(x-x_left)*(u_right-u_left)/(x_right-x_left);}

    static T2 Linear(const T x_left,const T2& u_left,const T2& u_slope,const T x)
    {return u_left+(x-x_left)*u_slope;}

    static T2 Linear(const T x_left,const T2& u_left,const T2& u_slope,const Vector<T,1> X)
    {return u_left+(X(0)-x_left)*u_slope;}

    static T2 Linear_Predivided(const T x_left,const T one_over_x_right_minus_x_left,const T2& u_left,const T2& u_right,const T x)
    {return u_left+(x-x_left)*one_over_x_right_minus_x_left*(u_right-u_left);}

    static T2 Linear_Normalized(const T2& u_left,const T2& u_slope,const T x)
    {return u_left+x*u_slope;}

    static T2 Linear(const T2 nodes[2],const Vector<T,1>& X)
    {return Linear(nodes[0],nodes[1],X(0));}

    static T2 Linear(const T2 nodes[4],const Vector<T,2>& X)
    {return Bilinear(nodes[0],nodes[1],nodes[2],nodes[3],X);}

    static T2 Linear(const T2 nodes[8],const Vector<T,3>& X)
    {return Trilinear(nodes[0],nodes[1],nodes[2],nodes[3],nodes[4],nodes[5],nodes[6],nodes[7],X);}

    static T2 Bilinear(const T2& u1,const T2& u2,const T2& u3,const T2& u4,const Vector<T,2>& minimum_corner,const Vector<T,2>& maximum_corner,const Vector<T,2>& X)
    {
        T one_over_x_right_minus_x_left=(T)1./(maximum_corner(0)-minimum_corner(0));
        T2 u_bottom=Linear_Predivided(minimum_corner(0),one_over_x_right_minus_x_left,u1,u2,X(0)),
           u_top=Linear_Predivided(minimum_corner(0),one_over_x_right_minus_x_left,u3,u4,X(0));
        return Linear(minimum_corner(1),maximum_corner(1),u_bottom,u_top,X(1));
    }

    // X in [0,1]x[0,1]
    static T2 Bilinear(const T2& u1,const T2& u2,const T2& u3,const T2& u4,const Vector<T,2>& X)
    {
        T2 u_bottom=Linear_Normalized(u1,u2-u1,X(0)),u_top=Linear_Normalized(u3,u4-u3,X(0));
        return Linear_Normalized(u_bottom,u_top-u_bottom,X(1));
    }

    static T2 Bilinear(const T2& u1,const T2& u3,T one_over_y_top_minus_y_bottom,const T x_left,const T y_bottom,const T2& slope12,const T2& slope34,const Vector<T,2>& X)
    {
        T2 u_bottom=Linear(x_left,u1,slope12,X(0)),u_top=Linear(x_left,u3,slope34,X(0));
        return Linear_Predivided(y_bottom,one_over_y_top_minus_y_bottom,u_bottom,u_top,X(1));
    }

    static T2 Trilinear(const T2& u1,const T2& u2,const T2& u3,const T2& u4,const T2& u5,const T2& u6,const T2& u7,const T2& u8,
                        const Vector<T,3>& minimum_corner,const Vector<T,3>& maximum_corner,const Vector<T,3>& X)
    {
        T one_over_x_right_minus_x_left=(T)1./(maximum_corner(0)-minimum_corner(0)),one_over_y_right_minus_y_left=(T)1./(maximum_corner(1)-minimum_corner(1));
        T2 u_bottom=Linear_Predivided(minimum_corner(0),one_over_x_right_minus_x_left,u1,u2,X(0)),
           u_top=Linear_Predivided(minimum_corner(0),one_over_x_right_minus_x_left,u3,u4,X(0));
        T2 u_front=Linear_Predivided(minimum_corner(1),one_over_y_right_minus_y_left,u_bottom,u_top,X(1));
        u_bottom=Linear_Predivided(minimum_corner(0),one_over_x_right_minus_x_left,u5,u6,X(0));
        u_top=Linear_Predivided(minimum_corner(0),one_over_x_right_minus_x_left,u7,u8,X(0));    
        T2 u_back=Linear_Predivided(minimum_corner(1),one_over_y_right_minus_y_left,u_bottom,u_top,X(1));
        return Linear(minimum_corner(2),maximum_corner(2),u_front,u_back,X(2));
    }

    // X in [0,1]x[0,1]x[0,1]
    static T2 Trilinear(const T2& u1,const T2& u2,const T2& u3,const T2& u4,const T2& u5,const T2& u6,const T2& u7,const T2& u8,const Vector<T,3>& X)
    {
        T2 u_bottom=Linear_Normalized(u1,u2-u1,X(0)),u_top=Linear_Normalized(u3,u4-u3,X(0)),u_front=Linear_Normalized(u_bottom,u_top-u_bottom,X(1));
           u_bottom=Linear_Normalized(u5,u6-u5,X(0));u_top=Linear_Normalized(u7,u8-u7,X(0));T2 u_back=Linear_Normalized(u_bottom,u_top-u_bottom,X(1));
        return Linear_Normalized(u_front,u_back-u_front,X(2));
    }

    static T2 Trilinear(const T2& u1,const T2& u3,const T2& u5,const T2& u7,T one_over_y_top_minus_y_bottom,T one_over_z_back_minus_z_front,const T x_left,const T y_bottom,const T z_front,
                        const T2& slope12,const T2& slope34,const T2& slope56,const T2& slope78,const Vector<T,3>& X)
    {
        T2 u_bottom=Linear(x_left,u1,slope12,X(0)),u_top=Linear(x_left,u3,slope34,X(0));
        T2 u_front=Linear_Predivided(y_bottom,one_over_y_top_minus_y_bottom,u_bottom,u_top,X(1));
        u_bottom=Linear(x_left,u5,slope56,X(0));u_top=Linear(x_left,u7,slope78,X(0));    
        T2 u_back=Linear_Predivided(y_bottom,one_over_y_top_minus_y_bottom,u_bottom,u_top,X(1));
        return Linear_Predivided(z_front,one_over_z_back_minus_z_front,u_front,u_back,X(2));
    }
//######################################################################
};
}
#endif
