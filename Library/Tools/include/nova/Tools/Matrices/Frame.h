//!#####################################################################
//! \file Frame.h
//!#####################################################################
// Class Frame
//######################################################################
#ifndef __Frame__
#define __Frame__

#include <nova/Tools/Matrices/Rotation.h>

namespace Nova{
template<class T,int d>
class Frame
{
    using TV                = Vector<T,d>;

  public:
    TV t;
    Rotation<T,d> r;

    Frame() {}

    explicit Frame(const TV& t_input)
        :t(t_input)
    {}

    explicit Frame(const Rotation<T,d>& r_input)
        :r(r_input)
    {}

    Frame(const TV& t_input,const Rotation<T,d>& r_input)
        :t(t_input),r(r_input)
    {}

    template<class T2>
    explicit Frame(const Frame<T2,d>& f)
        :t(f.t),r(f.r)
    {}

    Frame& operator=(const Frame<T,d>& rhs)
    {
        if(*this==&rhs) return *this;
        t=rhs.t;r=rhs.r;
        return *this;
    }

    bool operator==(const Frame& f) const
    {return t==f.t && r==f.r;}

    bool operator!=(const Frame& f) const
    {return t!=f.t || r!=f.r;}

    Frame& operator*=(const Frame& f)
    {t+=r.Rotate(f.t);r*=f.r;return *this;}

    Frame operator*(const Frame& f) const
    {return Frame(t+r.Rotate(f.t),r*f.r);}

    TV operator*(const TV& v) const
    {return t+r.Rotate(v);}

    void Invert()
    {*this=Inverse();}

    Frame Inverse() const
    {
        Rotation<T,d> r_inverse=r.Inverse();
        return Frame(-r_inverse.Rotate(t),r_inverse);
    }

    TV Inverse_Times(const TV& v) const
    {return r.Inverse_Rotate(v-t);}

    Frame Inverse_Times(const Frame& f) const
    {return Frame(r.Inverse_Rotate(f.t-t),r.Inverse()*f.r);}

    static Frame Interpolation(const Frame& f1,const Frame& f2,const T s)
    {return Frame((1-s)*f1.t+s*f2.t,Rotation<T,d>::Spherical_Linear_Interpolation(f1.r,f2.r,s));}
};
}
#include <nova/Tools/Read_Write/Matrices/Read_Write_Frame.h>
#endif
