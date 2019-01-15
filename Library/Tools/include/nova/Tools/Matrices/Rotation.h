//!#####################################################################
//! \file Rotation.h
//!#####################################################################
// Class Rotation
//######################################################################
#ifndef __Rotation__
#define __Rotation__

#include <nova/Tools/Arrays/Array.h>
#include <nova/Tools/Matrices/Matrix_2x2.h>
#include <nova/Tools/Matrices/Matrix_3x3.h>
#include <nova/Tools/Matrices/Quaternion.h>
#include <nova/Tools/Vectors/Complex.h>

namespace Nova{
template<class T,int d> class Rotation;

template<class T>
class Rotation<T,2>
{
    enum {d=2};
    using TV            = Vector<T,d>;
    using T_Spin        = Vector<T,d-1>;

  public:
    Complex<T> c;

  private:
    Rotation(const Complex<T>& c2)
        :c(c2)
    {}

  public:
    Rotation()
        :c(1,0)
    {}

    explicit Rotation(const Matrix<T,2>& A)
        :c(A.Column(1).Normalized())
    {}

    static Rotation<T,d> From_Complex(const Complex<T>& c2)
    {return Rotation<T,d>(c2).Normalized();}

    bool operator==(const Rotation<T,d>& r) const
    {return c==r.c;}

    bool operator!=(const Rotation<T,d>& r) const
    {return c!=r.c;}

    Rotation<T,d>& operator*=(const Rotation<T,d>& r)
    {c*=r.c;return *this;}

    Rotation<T,d> operator*(const Rotation<T,d>& r) const
    {return Rotation<T,d>(c*r.c);}

    Rotation<T,d> Inverse() const
    {return Rotation<T,d>(c.Conjugated());}

    TV Rotate(const TV& v) const
    {return TV({c.re*v[0]-c.im*v[1],c.im*v[0]+c.re*v[1]});}

    TV Inverse_Rotate(const TV& v) const
    {return TV({c.re*v[0]+c.im*v[1],c.re*v[1]-c.im*v[0]});}

    const T_Spin& Rotate_Spin(const T_Spin& spin) const
    {return spin;}

    T Normalize()
    {return c.Normalize();}

    Rotation<T,d> Normalized() const
    {
        Rotation<T,d> r(*this);
        r.Normalize();
        return r;
    }

    bool Is_Normalized(const T tolerance=(T)1e-3) const
    {return fabs(c.Norm_Squared()-(T)1.)<=tolerance;}

    void Get_Rotated_Frame(TV& x_axis,TV& y_axis) const
    {
        assert(Is_Normalized());
        x_axis=TV({c.re,c.im});
        y_axis=TV({-c.im,c.re});
    }

    T Angle() const
    {return atan2(c.im,c.re);}

    Vector<T,1> Euler_Angles() const
    {return Rotation_Vector();}

    Vector<T,1> Rotation_Vector() const
    {return Vector<T,1>(Angle());}

    Matrix<T,2> Rotation_Matrix() const
    {return Matrix<T,2>(c.re,c.im,-c.im,c.re);}

    TV Rotated_X_Axis() const // Q*(1,0)
    {return TV({c.re,c.im});}

    TV Rotated_Y_Axis() const // Q*(0,1)
    {return TV({-c.im,c.re});}

    TV Rotated_Axis(const int axis) const
    {
        assert(axis>=0 && axis<=1);
        if(axis==0) return Rotated_X_Axis();
        return Rotated_Y_Axis();
    }

    static Rotation<T,d> From_Angle(const T& a)
    {return Rotation<T,d>(Complex<T>::Unit_Polar(a));}

    static Rotation<T,d> From_Rotation_Vector(const Vector<T,1>& v)
    {return From_Angle(v[0]);}

    static Rotation<T,d> From_Euler_Angles(const Vector<T,1>& angle)
    {return From_Rotation_Vector(angle);}

    static Rotation<T,d> From_Rotated_Vector(const TV& v1,const TV& v2)
    {return Rotation<T,d>(Complex<T>(v1[0],-v1[1])*Complex<T>(v2[0],v2[1])).Normalized();}

    Rotation<T,d> Scale_Angle(const T a) const
    {return Rotation<T,d>::From_Rotation_Vector(a*Rotation_Vector());}

    static Rotation<T,d> Spherical_Linear_Interpolation(const Rotation<T,d>& r1,const Rotation<T,d>& r2,const T t)
    {return r1*(r1.Inverse()*r2).Scale_Angle(t);}

    static Rotation<T,d> Average_Rotation(const Array<Rotation<T,d> >& rotations)
    {
        Complex<T> sum;
        for(int i=0;i<(int)rotations.size();++i) sum+=rotations(i).c;
        return Rotation<T,d>(sum.Normalized());
    }
};

template<class T>
class Rotation<T,3>
{
    enum {d=3};
    using TV            = Vector<T,d>;
};
}
#endif
