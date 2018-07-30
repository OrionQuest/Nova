//!#####################################################################
//! \file Range.h
//!#####################################################################
// Class Range
//##################################################################### 
#ifndef __Range__
#define __Range__

#include <nova/Tools/Vectors/Vector.h>
#include <limits>
#include <type_traits>

namespace Nova{
template<class T,int d>
class Range
{
    typedef Vector<T,d> TV;

  public:
    TV min_corner,max_corner;

    Range()
        :min_corner(TV(std::numeric_limits<T>::max())),max_corner(TV(-std::numeric_limits<T>::max()))
    {}

    Range(const T xmin,const T xmax)
        :min_corner(xmin),max_corner(xmax)
    {
        static_assert(d==1,"Dimension should be 1.");
    }

    Range(const T xmin,const T xmax,const T ymin,const T ymax)
        :min_corner(xmin,ymin),max_corner(xmax,ymax)
    {
        static_assert(d==2,"Dimension should be 2.");
    }

    Range(const T xmin,const T xmax,const T ymin,const T ymax,const T zmin,const T zmax)
        :min_corner(xmin,ymin,zmin),max_corner(xmax,ymax,zmax)
    {
        static_assert(d==3,"Dimension should be 3.");
    }

    Range(const TV& minimum_corner,const TV& maximum_corner)
        :min_corner(minimum_corner),max_corner(maximum_corner)
    {}

    Range(const TV& point)
        :min_corner(point),max_corner(point)
    {}

    Range(const Range<T,d>& box)
        :min_corner(box.min_corner),max_corner(box.max_corner)
    {}

    static Range<T,d> Unit_Box()
    {
        return Range<T,d>(TV(),TV(1));
    }

    TV Edge_Lengths() const
    {
        return max_corner-min_corner;
    }

    T Size() const
    {return Edge_Lengths().Product();}

    Range<T,d>& operator=(const Range<T,d>& rhs)
    {
        if(this==&rhs) return *this;
        min_corner=rhs.min_corner;
        max_corner=rhs.max_corner;
        return *this;
    }

    Range<T,d> Thickened(const T epsilon) const
    {return Range<T,d>(min_corner-epsilon,max_corner+epsilon);}

    TV Clamp(const TV& location) const
    {
        TV value=location;
        for(int v=0;v<d;++v){
            value(v)=std::max(value(v),min_corner(v));
            value(v)=std::min(value(v),max_corner(v));}
        return value;
    }

    TV Center() const
    {return (min_corner+max_corner)/2;}

    bool Inside(const TV& location) const
    {return (location>=min_corner && location<=max_corner);}

    void Enlarge_To_Include_Point(const TV& point)
    {
        min_corner=Componentwise_Min(min_corner,point);
        max_corner=Componentwise_Max(max_corner,point);
    }

    void Enlarge_To_Include_Box(const Range<T,d>& box)
    {
        min_corner=Componentwise_Min(min_corner,box.min_corner);
        max_corner=Componentwise_Max(max_corner,box.max_corner);
    }

    bool Intersection(const Range<T,d>& box) const
    {return min_corner.All_Less_Equal(box.max_corner) && max_corner.All_Greater_Equal(box.min_corner);}

    bool Intersection(const Range<T,d>& box,const T thickness_over_two) const
    {return Thickened(thickness_over_two).Intersection(box);}
};
}
#include <nova/Tools/Read_Write/Utilities/Read_Write_Range.h>
#endif
