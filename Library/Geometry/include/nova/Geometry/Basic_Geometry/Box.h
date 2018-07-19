//!#####################################################################
//! \file Box.h
//!#####################################################################
// Class Box
//######################################################################
#ifndef __Box__
#define __Box__

#include <nova/Tools/Utilities/Range.h>

namespace Nova{
template<class T,int d>
class Box: public Range<T,d>
{
    using TV            = Vector<T,d>;
    using Base          = Range<T,d>;

  public:
    using Base::min_corner;using Base::max_corner;

    Box() {}

    Box(const T xmin,const T xmax)
        :Base(xmin,xmax)
    {}

    Box(const T xmin,const T xmax,const T ymin,const T ymax)
        :Base(xmin,xmax,ymin,ymax)
    {}

    Box(const T xmin,const T xmax,const T ymin,const T ymax,const T zmin,const T zmax)
        :Base(xmin,xmax,ymin,ymax,zmin,zmax)
    {}

    Box(const TV& point)
        :Base(point)
    {}

    Box(const TV& minimum_corner,const TV& maximum_corner)
        :Base(minimum_corner,maximum_corner)
    {}

    Box(const Range<T,d>& box)
        :Base(box)
    {}

    ~Box() {}

    Box& operator=(const Box& box)
    {
        if(this==&box) return *this;
        min_corner=box.min_corner;
        max_corner=box.max_corner;
        return *this;
    }

    void Build_From_Points(const TV& x0,const TV& x1,const TV& x2)
    {
        for(int i=0;i<d;++i){
            min_corner(i)=std::min(x0[i],std::min(x1[i],x2[i]));
            max_corner(i)=std::max(x0[i],std::max(x1[i],x2[i]));}
    }

    void Build_From_Points(const TV& x0,const TV& x1,const TV& x2,const TV& x3)
    {
        for(int i=0;i<d;++i){
            min_corner(i)=std::min(x0[i],std::min(x1[i],std::min(x2[i],x3[i])));
            max_corner(i)=std::max(x0[i],std::max(x1[i],std::max(x2[i],x3[i])));}
    }

    TV World_Space(const TV& fraction) const
    {return ((T)1.-fraction)*min_corner+fraction*max_corner;}
};
}
#endif
