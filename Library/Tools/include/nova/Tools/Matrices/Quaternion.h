//!#####################################################################
//! \file Quaternion.h
//!#####################################################################
// Class Quaternion
//######################################################################
#ifndef __Quaternion__
#define __Quaternion__

#include <nova/Tools/Vectors/Vector.h>

namespace Nova{
template<class T>
class Quaternion
{
    enum {d=3};
    using TV                                    = Vector<T,d>;

  public:
    T s;
    TV v;

    Quaternion()
        :s(0)
    {}

    Quaternion(const T s_input,const T x,const T y,const T z)
        :s(s_input),v({x,y,z})
    {}

    Quaternion(const T s_input,const TV& v_input)
        :s(s_input),v(v_input)
    {}

    explicit Quaternion(const Vector<T,4>& q)
        :s(q[0]),v(q[1],q[2],q[3])
    {}

    Quaternion& operator=(const Quaternion<T>& rhs)
    {
        if(*this==&rhs) return *this;
        s=rhs.s;
        v=rhs.v;
        return *this;
    }

    static Quaternion One()
    {return Quaternion(1,0,0,0);}

    bool operator==(const Quaternion& q) const
    {return s==q.s && v==q.v;}

    bool operator!=(const Quaternion& q) const
    {return s!=q.s || v!=q.v;}

    Quaternion operator-() const
    {return Quaternion(-s,-v);}

    Quaternion& operator+=(const Quaternion& q)
    {s+=q.s;v+=q.v;return *this;}

    Quaternion& operator-=(const Quaternion& q)
    {s-=q.s;v-=q.v;return *this;}

    Quaternion& operator*=(const Quaternion& q)
    {return *this=*this*q;}

    Quaternion& operator*=(const T a)
    {s*=a;v*=a;return *this;}

    Quaternion& operator/=(const T a)
    {
        assert(a!=0);
        T r=(T)1./s;
        s*=r;v*=r;
        return *this;
    }

    Quaternion operator+(const Quaternion& q) const
    {return Quaternion(s+q.s,v+q.v);}

    Quaternion operator-(const Quaternion& q) const
    {return Quaternion(s-q.s,v-q.v);}

    Quaternion operator*(const Quaternion& q) const
    {return Quaternion(s*q.s-v.Dot_Product(q.v),s*q.v+q.s*v+Cross_Product(v,q.v));}

    Quaternion operator*(const T a) const
    {return Quaternion(s*a,v*a);}

    Quaternion operator/(const T a) const
    {
        assert(a!=0);
        T r=(T)1./a;
        return Quaternion(s*r,v*r);
    }

    T Norm() const
    {return std::sqrt(Norm_Squared());}

    T Norm_Squared() const
    {return s*s+v.Norm_Squared();}

    T Normalize()
    {
        T norm=Norm();
        if(norm) *this/=norm;
        else *this=One();
        return norm;
    }

    Quaternion Normalized() const
    {
        Quaternion q(*this);
        q.Normalize();
        return q;
    }

    bool Is_Normalized(const T tolerance=(T)1e-3) const
    {return fabs(Norm_Squared()-(T)1.)<=tolerance;}

    Quaternion Conjugate() const
    {return Quaternion(s,-v);}

    Quaternion Inverse() const
    {return Conjugate()/Norm_Squared();}

    static T Dot_Product(const Quaternion& q1,const Quaternion& q2)
    {return q1.s*q2.s+q1.v.Dot_Product(q2.v);}
};

template<class T>
inline Quaternion<T> operator*(const T a,const Quaternion<T>& q)
{return Quaternion<T>(q.s*a,q.v*a);}

template<class T>
inline std::ostream& operator<<(std::ostream& output,const Quaternion<T>& q)
{output<<"("<<q.s<<" "<<q.v<<")";return output;}
}
#include <nova/Tools/Read_Write/Matrices/Read_Write_Quaternion.h>
#endif
