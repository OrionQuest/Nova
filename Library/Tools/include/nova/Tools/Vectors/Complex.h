//!#####################################################################
//! \file Complex.h
//!#####################################################################
// Class Complex
//######################################################################
#ifndef __Complex__
#define __Complex__

#include <nova/Tools/Vectors/Vector.h>
#include <nova/Tools/Utilities/Utilities.h>

namespace Nova{
template<class T>
class Complex
{
    enum {d=2};

  public:
    T re,im;

    Complex()
        :re(0),im(0)
    {}

    Complex(const T re_input,T im_input)
        :re(re_input),im(im_input)
    {}

    explicit Complex(const Vector<T,d>& input)
        :re(input(0)),im(input(1))
    {}

    static Complex<T> One()
    {return Complex(1,0);}

    Complex& operator=(const Complex<T>& rhs)
    {
        if(this==&rhs) return *this;
        re=rhs.re;
        im=rhs.im;
        return *this;
    }

    bool operator==(const Complex<T>& c) const
    {return re==c.re && im==c.im;}

    bool operator!=(const Complex<T>& c) const
    {return re!=c.re || im!=c.im;}

    Complex<T>& operator*=(const Complex<T>& c)
    {
        T old_re=re;
        re=re*c.re-im*c.im;
        im=old_re*c.im+im*c.re;
        return *this;
    }

    Complex<T>& operator*=(const T a)
    {re*=a;im*=a;return *this;}

    Complex<T> operator*(const Complex<T>& c) const
    {return Complex<T>(re*c.re-im*c.im,re*c.im+im*c.re);}

    Complex<T> operator*(const T a) const
    {return Complex<T>(a*re,a*im);}

    Complex<T>& operator+=(const Complex<T>& c)
    {re+=c.re;im+=c.im;return *this;}

    Complex<T> operator+(const Complex<T>& c) const
    {return Complex<T>(re+c.re,im+c.im);}

    Complex<T> operator+(const T& a) const
    {return Complex<T>(re+a,im);}

    Complex<T> operator-=(const Complex<T>& c)
    {re-=c.re;im-=c.im;return *this;}

    Complex<T> operator-(const Complex<T>& c) const
    {return Complex<T>(re-c.re,im-c.im);}

    Complex<T> operator-(const T a) const
    {return Complex<T>(re-a,im);}

    T Norm_Squared() const
    {return re*re+im*im;}

    T Norm() const
    {return std::sqrt(Norm_Squared());}

    void Conjugate()
    {im*=-1;}

    Complex<T> Conjugated() const
    {return Complex<T>(re,-im);}

    Complex<T> Inverse() const
    {
        assert(re!=0 || im!=0);
        T denominator=(T)1./(re*re+im*im);
        return Complex<T>(re*denominator,-im*denominator);
    }

    Complex<T> Sqrt() const
    {
        T norm=Norm();
        return Complex<T>(std::sqrt((T).5*(norm+re)),Nova_Utilities::Sign<T>(im)*std::sqrt((T).5*(norm-re)));
    }

    static T Dot_Product(const Complex<T>& c1,const Complex<T>& c2)
    {return c1.re*c2.re+c1.im*c2.im;}

    T Normalize()
    {
        T norm=Norm();
        if(norm) *this*=(T)1./norm;
        else *this=One();
        return norm;
    }

    Complex<T> Normalized() const
    {
        Complex<T> c(*this);
        c.Normalize();
        return c;
    }

    bool Is_Normalized(const T tolerance=(T)1e-3) const
    {return fabs(Norm_Squared()-(T)1.)<=tolerance;}

    Complex<T> Rotated_Counter_Clockwise_90() const
    {return Complex<T>(-im,re);}

    Complex<T> Rotated_Clockwise_90() const
    {return Complex<T>(im,-re);}

    static Complex<T> Polar(const T r,const T theta)
    {return Complex<T>(r*(T)std::cos(theta),r*(T)std::sin(theta));}

    static Complex<T> Unit_Polar(const T theta)
    {return Complex<T>((T)std::cos(theta),(T)std::sin(theta));}
};

template<class T>
inline Complex<T> operator*(const T a,const Complex<T>& c)
{return Complex<T>(a*c.re,a*c.im);}
}
#include <nova/Tools/Read_Write/Vectors/Read_Write_Complex.h>
#endif
