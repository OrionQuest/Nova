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

const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000


template<class T,int d>
class Range
{
    typedef Vector<T,d> TV;
    using OutCode = int;

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

    static Range<T,d> Centered_Box(T half_width)
    {
        return Range<T,d>(TV(-half_width),TV(half_width));
    }

    TV Edge_Lengths() const
    {
        return max_corner-min_corner;
    }

    T Size() const
    {return (Edge_Lengths()+1).Product();}

    T Area() const
    {
        return Edge_Lengths().Product();
    }

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

    OutCode ComputeOutCode(const TV point) const 
    {
        OutCode code=INSIDE;
        if(point(0)<min_corner(0)) code|=LEFT; else if(point(0)>max_corner(0)) code|=RIGHT;
        if(point(1)<min_corner(1)) code|=BOTTOM; else if(point(1)>max_corner(1)) code|=TOP;
	    return code;
    }

    bool Intersection(const TV start_point,const TV end_point) const
    {
        OutCode code_sp=ComputeOutCode(start_point); OutCode code_ep=ComputeOutCode(end_point);
        T x0=start_point(0),y0=start_point(1),x1=end_point(0),y1=end_point(1);
        const T xmin=min_corner(0);
        const T ymin=min_corner(1);
        const T xmax=max_corner(0);
        const T ymax=max_corner(1);
	    while (true) {
		    if (!(code_sp | code_ep)) {
			// bitwise OR is 0: both points inside window; trivially accept and exit loop
			return true; break;} 
        else if (code_sp & code_ep) {
			// bitwise AND is not 0: both points share an outside zone (LEFT, RIGHT, TOP,
			// or BOTTOM), so both must be outside window; exit loop (accept is false)
			return false;
		} else {
			// failed both tests, so calculate the line segment to clip
			// from an outside point to an intersection with clip edge
			T x, y;

			// At least one endpoint is outside the clip rectangle; pick it.
			OutCode outcodeOut = code_ep > code_sp ? code_ep : code_sp;

			// Now find the intersection point;
			// use formulas:
			//   slope = (y1 - y0) / (x1 - x0)
			//   x = x0 + (1 / slope) * (ym - y0), where ym is ymin or ymax
			//   y = y0 + slope * (xm - x0), where xm is xmin or xmax
			// No need to worry about divide-by-zero because, in each case, the
			// outcode bit being tested guarantees the denominator is non-zero
			if (outcodeOut & TOP) {           // point is above the clip window
				x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
				y = ymax;
			} else if (outcodeOut & BOTTOM) { // point is below the clip window
				x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
				y = ymin;
			} else if (outcodeOut & RIGHT) {  // point is to the right of clip window
				y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
				x = xmax;
			} else if (outcodeOut & LEFT) {   // point is to the left of clip window
				y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
				x = xmin;
			}

			// Now we move outside point to intersection point to clip
			// and get ready for next pass.
			if (outcodeOut == code_sp) {
				x0 = x;
				y0 = y;
				code_sp = ComputeOutCode(TV({x0,y0}));
			} else {
				x1 = x;
				y1 = y;
				code_ep = ComputeOutCode(TV({x1,y1}));
			}
		}
	}
    }
};
}
#include <nova/Tools/Read_Write/Utilities/Read_Write_Range.h>
#endif
