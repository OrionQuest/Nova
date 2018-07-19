//!#####################################################################
//! \file Triangle.h
//!#####################################################################
// Class Triangle
//######################################################################
#ifndef __Triangle__
#define __Triangle__

#include <nova/Tools/Vectors/Vector.h>

namespace Nova{
template<class T,int d>
class Triangle
{
    using TV            = Vector<T,d>;

  public:
    Vector<TV,3> X;

    Triangle()
    {
        TV x1,x2=TV::Axis_Vector(0),x3=TV::Axis_Vector(1);
        Specify_Three_Points(x1,x2,x3);
    }

    Triangle(const TV& x1,const TV& x2,const TV& x3)
    {Specify_Three_Points(x1,x2,x3);}

    static T Signed_Area(const TV& x1,const TV& x2,const TV& x3)
    {return (T).5*Cross_Product(x2-x1,x3-x1).Norm();}

    T Signed_Area() const
    {return Signed_Area(X(0),X(1),X(2));}

    static bool Check_Orientation(const TV& x1,const TV& x2,const TV& x3)
    {return Signed_Area(x1,x2,x3)>=0;}

    bool Check_Orientation() const
    {return Signed_Area()>=0;}

    void Specify_Three_Points(const TV& x1,const TV& x2,const TV& x3)
    {X(0)=x1;X(1)=x2;X(2)=x3;}

    bool Outside(const TV& location,const T thickness_over_two=0)
    {return Outside(location,X(0),X(1),X(2),thickness_over_two);}

    static bool Outside(const TV& location,const TV& x1,const TV& x2,const TV& x3,const T thickness_over_two=0)
    {
        assert(Check_Orientation(x1,x2,x3));
        Vector<T,2> b_coords=Triangle(x1,x2,x3).First_Two_Barycentric_Coordinates(location);
        if(b_coords(0)>=0 && b_coords(0)<=1 && b_coords(1)>=0 && b_coords(1)<=1 && (b_coords(0)+b_coords(1))<=1) return false;
        return true;
    }

    bool Inside(const TV& location,const T thickness_over_two=0) const
    {return !Outside(location,thickness_over_two);}

    Vector<T,2> First_Two_Barycentric_Coordinates(const TV& location) const
    {
        TV v0=X(1)-X(0),v1=X(2)-X(0),v2=location-X(0);
        T d00=v0.Dot_Product(v0),d01=v0.Dot_Product(v1),d11=v1.Dot_Product(v1);
        T d20=v2.Dot_Product(v0),d21=v2.Dot_Product(v1);
        T inverse_denom=(T)1./(d00*d11-d01*d01);
        T v=(d11*d20-d01*d21)*inverse_denom;
        T w=(d00*d21-d01*d20)*inverse_denom;
        T u=(T)1.-v-w;
        return Vector<T,2>({u,v});
    }
};
}
#endif
