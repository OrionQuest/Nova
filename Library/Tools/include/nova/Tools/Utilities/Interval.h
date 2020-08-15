//!#####################################################################
//! \file Interval.h
//!#####################################################################
// Class Interval
//######################################################################
#ifndef __Interval__
#define __Interval__

#include <cfloat>
namespace Nova{
template<class T>
class Interval
{
public:
    T min_corner,max_corner;

    Interval():min_corner(),max_corner(){}

    Interval(const T value) :min_corner(value),max_corner(value){}

    Interval(const T min_corner,const T max_corner):min_corner(min_corner),max_corner(max_corner){}

    static Interval Unit_Box()
    {return Interval((T)0.,(T)1.);}

    static Interval Zero_Box() {return Interval();}

    static Interval Empty_Box() {return Interval((T)FLT_MAX,-(T)FLT_MAX);}

    static Interval Full_Box()
    {return Interval(-(T)FLT_MAX,(T)FLT_MAX);}

    bool Empty() const {return min_corner>max_corner;}

    bool operator==(const Interval& r) const {return min_corner==r.min_corner && max_corner==r.max_corner;}

    bool operator!=(const Interval& r) const {return !(*this==r);}

    Interval operator-() const {return Interval(-max_corner,-min_corner);}

    Interval& operator+=(const Interval& r) {min_corner+=r.min_corner;max_corner+=r.max_corner;return *this;}

    Interval& operator-=(const Interval& r) {min_corner-=r.max_corner;max_corner-=r.min_corner;return *this;}

    Interval operator+(const Interval& r) const {return Interval(min_corner+r.min_corner,max_corner+r.max_corner);}

    Interval operator-(const Interval& r) const {return Interval(min_corner-r.max_corner,max_corner-r.min_corner);}

    Interval operator*(const T a) const {return a>=0?Interval(min_corner*a,max_corner*a):Interval(max_corner*a,min_corner*a);}

    Interval& operator*=(const T a) {return *this=*this*a;}

    Interval operator/(const T a) const {assert(a!=0);return *this*(1/a);}

    Interval& operator/=(const T a) {return *this=*this/a;}

    T Center() const {return (T).5*(min_corner+max_corner);}

    T Size() const {return max_corner-min_corner;}

    void Enlarge_To_Include_Point(const T& point) {min_corner=min(min_corner,point);max_corner=max(max_corner,point);}

    void Enlarge_Nonempty_Box_To_Include_Point(const T& point) {assert(!Empty());if(point<min_corner) min_corner=point;else if(point>max_corner) max_corner=point;}

    void Enlarge_Nonempty_Box_To_Include_Points(const T& p1,const T& p2) {Enlarge_Nonempty_Box_To_Include_Point(p1);Enlarge_Nonempty_Box_To_Include_Point(p2);}

    void Enlarge_Nonempty_Box_To_Include_Points(const T& p1,const T& p2,const T& p3) {Enlarge_Nonempty_Box_To_Include_Point(p1);Enlarge_Nonempty_Box_To_Include_Point(p2);Enlarge_Nonempty_Box_To_Include_Point(p3);}

    void Enlarge_To_Include_Box(const Interval& interval) {min_corner=min(min_corner,interval.min_corner);max_corner=max(max_corner,interval.max_corner);}

    void Change_Size(const T delta) {min_corner-=delta;max_corner+=delta;}

    Interval Thickened(const T thickness_over_two) const {return Interval(min_corner-thickness_over_two,max_corner+thickness_over_two);}

    static Interval Combine(const Interval& box1,const Interval& box2) {return Interval(min(box1.min_corner,box2.min_corner),max(box1.max_corner,box2.max_corner));}

    static Interval Intersect(const Interval& box1,const Interval& box2) {return Interval(max(box1.min_corner,box2.min_corner),min(box1.max_corner,box2.max_corner));}

    void Scale_About_Center(const T factor) {T center=(T).5*(min_corner+max_corner),length_over_two=factor*(T).5*(max_corner-min_corner);min_corner=center-length_over_two;max_corner=center+length_over_two;}

    bool Lazy_Inside(const T& location) const {return min_corner<=location && location<=max_corner;}

    bool Lazy_Inside_Half_Open(const T& location) const {return min_corner<=location && location<max_corner;}

    bool Inside(const T& location,const T thickness_over_two) const {return Thickened(-thickness_over_two).Lazy_Inside(location);}

    bool Lazy_Outside(const T& location) const {return !Lazy_Inside(location);}

    bool Outside(const T& location,const T thickness_over_two) const {return Thickened(thickness_over_two).Lazy_Outside(location);}

    bool Boundary(const T& location,const T thickness_over_two) const {bool strict_inside=min_corner+thickness_over_two<location && location<max_corner-thickness_over_two;
    return !strict_inside && !Outside(location,thickness_over_two);}

    T Clamp(const T& location) const
    {return clamp(location,min_corner,max_corner);}

    bool Contains(const Interval& interval) const
    {return min_corner<=interval.min_corner && interval.max_corner<=max_corner;}

    bool Lazy_Intersection(const Interval& interval) const
    {return min_corner<=interval.max_corner && interval.min_corner<=max_corner;}

    bool Intersection(const Interval& interval,const T thickness_over_two) const
    {return Thickened(thickness_over_two).Lazy_Intersection(interval);}

    bool Intersection(const Interval& interval) const
    {return Lazy_Intersection(interval);}

    T Signed_Distance(const T& X) const
    {return abs(X-Center())-Size();}

//#####################################################################
};

template<class T>
inline Interval<T> operator+(const T& a,const Interval<T>& b)
{return Interval<T>(a+b.min_corner,a+b.max_corner);}

template<class T>
inline Interval<T> operator-(const T& a,const Interval<T>& b)
{return Interval<T>(a-b.max_corner,a-b.min_corner);}

template<class T> inline Interval<T> operator*(const typename T::SCALAR a,const Interval<T>& interval)
{return interval*a;}
}
#endif
