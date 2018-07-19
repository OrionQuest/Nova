//!#####################################################################
//! \file Tetrahedron.h
//!#####################################################################
// Class Tetrahedron
//######################################################################
#ifndef __Tetrahedron__
#define __Tetrahedron__

#include <nova/Geometry/Basic_Geometry/Plane.h>
#include <nova/Geometry/Basic_Geometry/Triangle.h>

namespace Nova{
template<class T>
class Tetrahedron
{
    using TV            = Vector<T,3>;

  public:
    Vector<TV,4> X;
    Triangle<T,3> triangle1,triangle2,triangle3,triangle4;

    Tetrahedron()
        :X({TV(),TV({0,1,0}),TV({1,0,0}),TV({0,0,-1})})
    {Create_Triangles();}

    Tetrahedron(const TV& x1,const TV& x2,const TV& x3,const TV& x4)
        :X({x1,x2,x3,x4})
    {Create_Triangles();}

    void Create_Triangles()
    {
        if(Cross_Product(X(1)-X(0),X(2)-X(0)).Dot_Product(X(3)-X(0)) <= 0){
            triangle1.Specify_Three_Points(X(0),X(1),X(2));triangle2.Specify_Three_Points(X(0),X(3),X(1));
            triangle3.Specify_Three_Points(X(0),X(2),X(3));triangle4.Specify_Three_Points(X(1),X(3),X(2));}
        else{triangle1.Specify_Three_Points(X(0),X(2),X(1));triangle2.Specify_Three_Points(X(0),X(1),X(3));
            triangle3.Specify_Three_Points(X(0),X(3),X(2));triangle4.Specify_Three_Points(X(1),X(2),X(3));}
    }

    bool Inside(const TV& location,const T thickness_over_two=0) const
    {
        if(Plane<T>(triangle1.X).Inside(location,thickness_over_two) && Plane<T>(triangle2.X).Inside(location,thickness_over_two) &&
           Plane<T>(triangle3.X).Inside(location,thickness_over_two) && Plane<T>(triangle4.X).Inside(location,thickness_over_two)) return true;
        return false;
    }

    bool Outside(const TV& location,const T thickness_over_two=0) const
    {
        return Plane<T>(triangle1.X).Outside(location,thickness_over_two) || Plane<T>(triangle2.X).Outside(location,thickness_over_two) ||
               Plane<T>(triangle3.X).Outside(location,thickness_over_two) || Plane<T>(triangle4.X).Outside(location,thickness_over_two);
    }

    TV First_Three_Barycentric_Coordinates(const TV& location) const
    {
        TV vap=location-X(0),vbp=location-X(1);
        TV vab=X(1)-X(0),vac=X(2)-X(0),vad=X(3)-X(0);
        TV vbc=X(2)-X(1),vbd=X(3)-X(1);

        T va6=vbp.Dot_Product(Cross_Product(vbd,vbc));
        T vb6=vap.Dot_Product(Cross_Product(vac,vad));
        T vc6=vap.Dot_Product(Cross_Product(vad,vab));
        T vd6=vap.Dot_Product(Cross_Product(vab,vac));
        T v6=(T)1./vab.Dot_Product(Cross_Product(vac,vad));
        return TV({va6*v6,vb6*v6,vc6*v6});
    }
};
}
#endif
