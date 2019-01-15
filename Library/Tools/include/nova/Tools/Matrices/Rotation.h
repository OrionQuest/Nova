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
#include <nova/Tools/Utilities/Constants.h>
#include <nova/Tools/Vectors/Complex.h>

namespace Nova{
template<class T,int d> class Rotation;

template<class T>
class Rotation<T,2>
{
    enum {d=2};
    using TV            = Vector<T,d>;
  public:
    using T_Spin        = Vector<T,d-1>;

  private:
    Rotation(const Complex<T>& c2)
        :c(c2)
    {}

  public:
    Complex<T> c;

    Rotation()
        :c(1,0)
    {}

    explicit Rotation(const Matrix<T,2>& A)
        :c(A.Column(1).Normalized())
    {}

    static Rotation<T,d> From_Complex(const Complex<T>& c2)
    {return Rotation<T,d>(c2).Normalized();}

    Rotation& operator=(const Rotation<T,d>& rhs)
    {
        if(*this==&rhs) return *this;
        c=rhs.c;
        return *this;
    }

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
  public:
    using T_Spin        = Vector<T,d>;

  private:
    Rotation(const Quaternion<T>& q_input)
        :q(q_input)
    {}

    Rotation(const T s,const T x,const T y,const T z)
        :q(s,x,y,z)
    {}

    template<class T2>
    static Rotation<T,d> From_Components_Helper(const T2 s,const T x,const T y,const T z)
    {return Rotation<T,d>(s,x,y,z).Normalized();}

    static Rotation<T,d> From_Components_Helper(const int s,const int x,const int y,const int z)
    {return Rotation<T,d>((T)s,(T)x,(T)y,(T)z);}

  public:
    Quaternion<T> q;

    Rotation()
        :q(1,0,0,0)
    {}

    Rotation(const T angle,const TV& direction)
        :q((T)std::cos((T).5*angle),direction)
    {
        q.v.Normalize();
        q.v*=(T)std::sin((T).5*angle);
    }

    explicit Rotation(const Matrix<T,3>& A)
    {
        T trace=1+A(0,0)+A(1,1)+A(2,2);                             // trace=4*cos^2(theta/2)
        if(trace>1){q.s=(T).5*sqrt(trace);q.v.x=A(2,1)-A(1,2);q.v.y=A(0,2)-A(2,0);q.v.z=A(1,0)-A(0,1);q.v*=(T).25/q.s;}
        else{int i=(A(0,0)>A(1,1)) ? 0:1;i=(A(i,i)>A(2,2)) ? i:2;   // set i to be the index of the dominating diagonal term
            switch(i){
                case 0:q.v.x=T(.5)*sqrt(1+A(0,0)-A(1,1)-A(2,2));q.v.y=T(.25)*(A(1,0)+A(0,1))/q.v.x;q.v.z=T(.25)*(A(0,2)+A(2,0))/q.v.x;q.s=T(.25)*(A(2,1)-A(1,2))/q.v.x;break;
                case 1:q.v.y=T(.5)*sqrt(1-A(0,0)+A(1,1)-A(2,2));q.v.x=T(.25)*(A(1,0)+A(0,1))/q.v.y;q.v.z=T(.25)*(A(2,1)+A(1,2))/q.v.y;q.s=T(.25)*(A(0,2)-A(2,0))/q.v.y;break;
                case 2:default:q.v.z=T(.5)*sqrt(1-A(0,0)-A(1,1)+A(2,2));q.v.x=T(.25)*(A(0,2)+A(2,0))/q.v.z;q.v.y=T(.25)*(A(2,1)+A(1,2))/q.v.z;q.s=T(.25)*(A(1,0)-A(0,1))/q.v.z;break;}}
        Normalize();
    }

    static Rotation<T,d> From_Components(const T s,const T x,const T y,const T z)
    {return From_Components_Helper(s,x,y,z);}

    static Rotation<T,d> From_Quaternion(const Quaternion<T>& q)
    {return Rotation<T,d>(q).Normalized();}

    Rotation& operator=(const Rotation<T,d>& rhs)
    {
        if(*this==&rhs) return *this;
        q=rhs.q;
        return *this;
    }

    bool operator==(const Rotation<T,d>& r) const
    {return q==r.q || q==-r.q;}

    bool operator!=(const Rotation<T,d>& r) const
    {return !(*this==r);}

    Rotation<T,d>& operator*=(const Rotation<T,d>& r)
    {q*=r.q;return *this;}

    Quaternion<T> operator*(const Quaternion<T>& w) const
    {return q*w;}

    Rotation<T,d> operator*(const Rotation<T,d>& r) const
    {return Rotation<T,d>(q*r.q);}

    T Normalize()
    {return q.Normalize();}

    Rotation<T,d> Normalized() const
    {
        Rotation<T,d> r(*this);
        r.Normalize();
        return r;
    }

    bool Is_Normalized(const T tolerance=(T)1e-4) const
    {return q.Is_Normalized(tolerance);}

    Rotation<T,d> Inverse() const
    {return Rotation<T,d>(q.Conjugate());}

    static Rotation<T,d> From_Euler_Angles(const TV& euler_angles)
    {return From_Euler_Angles(euler_angles[0],euler_angles[1],euler_angles[2]);}

    TV Rotate(const TV& v) const                        // 20 mult and 13 add/sub
    {
        assert(Is_Normalized());
        T two_s=q.s+q.s;
        return two_s*Cross_Product(q.v,v)+(two_s*q.s-(T)1)*v+(T)2*v.Dot_Product(q.v)*q.v;
    }

    TV Inverse_Rotate(const TV& v) const                // 20 mult and 13 add/sub
    {
        assert(Is_Normalized());
        T two_s=q.s+q.s;
        return two_s*Cross_Product(v,q.v)+(two_s*q.s-(T)1)*v+(T)2*v.Dot_Product(q.v)*q.v;
    }

    T_Spin Rotate_Spin(const T_Spin& spin) const
    {return Rotate(spin);}

    TV Euler_Angles() const
    {
        TV euler_angles;
        Euler_Angles(euler_angles[0],euler_angles[1],euler_angles[2]);
        return euler_angles;
    }

    inline T atan2_y_x_over_y(const T y,const T x)      // atan2(y,x)/y
    {
        if(fabs(y)<1e-8) return 1;
        return atan2(y,x)/y;
    }

    TV Rotation_Vector() const
    {
        return 2*atan2_y_x_over_y(q.v.Norm(),fabs(q.s))*(q.s<0?-q.v:q.v);
    }

    inline T sinc(const T x)                            // sin(x)/x
    {
        if(fabs(x)<1e-8) return 1;
        return sin(x)/x;
    }

    static Rotation<T,d> From_Rotation_Vector(const TV& v)
    {
        T norm=v.Norm();
        Rotation<T,d> r;
        r.q.s=(T)std::cos((T).5*norm);
        r.q.v=(T).5*sinc((T).5*norm)*v;
        return r;
    }

    static Rotation<T,d> From_Euler_Angles(const T euler_angle_x,const T euler_angle_y,const T euler_angle_z)
    {
        T half_x_angle=(T).5*euler_angle_x,half_y_angle=(T).5*euler_angle_y,half_z_angle=(T).5*euler_angle_z;
        T cx=(T)std::cos(half_x_angle),sx=(T)std::sin(half_x_angle),cy=(T)std::cos(half_y_angle),sy=(T)std::sin(half_y_angle),cz=(T)std::cos(half_z_angle),sz=(T)std::sin(half_z_angle);
        return Rotation<T,d>(cx*cy*cz+sx*sy*sz,sx*cy*cz-cx*sy*sz,cx*sy*cz+sx*cy*sz,cx*cy*sz-sx*sy*cz);
    }

    inline T clamp_min(const T x,const T xmin)
    {if(x<=xmin) return xmin;else return x;}

    static Rotation<T,d> From_Rotation_Matrix(const Matrix<T,3>& R)
    {
        T q0=std::sqrt(clamp_min((R(0,0)+R(1,1)+R(2,2)+(T)1.0)/(T)4.0,(T)0.0)),q1=std::sqrt(clamp_min((R(0,0)-R(1,1)-R(2,2)+(T)1.0)/(T)4.0,(T)0.0));
        T q2=std::sqrt(clamp_min((-R(0,0)+R(1,1)-R(2,2)+(T)1.0)/(T)4.0,(T)0.0)),q3=sqrt(clamp_min((-R(0,0)-R(1,1)+R(2,2)+(T)1.0)/(T)4.0,(T)0.0));
        if(q0>=q1&&q0>=q2&&q0>=q3){q1*=Nova_Utilities::Sign(R(2,1)-R(1,2));q2*=Nova_Utilities::Sign(R(0,2)-R(2,0));q3*=Nova_Utilities::Sign(R(1,0)-R(0,1));}
        else if(q1>=q0&&q1>=q2&&q1>=q3){q0*=Nova_Utilities::Sign(R(2,1)-R(1,2));q2*=Nova_Utilities::Sign(R(1,0)+R(0,1));q3*=Nova_Utilities::Sign(R(0,2)+R(2,0));}
        else if(q2>=q0&&q2>=q1&&q2>=q3){q0*=Nova_Utilities::Sign(R(0,2)-R(2,0));q1*=Nova_Utilities::Sign(R(1,0)+R(0,1));q3*=Nova_Utilities::Sign(R(2,1)+R(1,2));}
        else if(q3>=q0&&q3>=q1&&q3>=q2){q0*=Nova_Utilities::Sign(R(1,0)-R(0,1));q1*=Nova_Utilities::Sign(R(2,0)+R(0,2));q2*=Nova_Utilities::Sign(R(2,1)+R(1,2));}
        else{FATAL_ERROR("Could not convert from rotation matrix!");}
        Vector<T,4> quat(q0,q1,q2,q3);quat.Normalize();
        return Rotation<T,d>(Quaternion<T>(quat));
    }

    void Euler_Angles(T& euler_angle_x,T& euler_angle_y,T& euler_angle_z) const
    {
        Matrix<T,3> R(Rotation_Matrix());
        T cos_beta_squared=Nova_Utilities::Sqr<T>(R.x[0])+Nova_Utilities::Sqr<T>(R.x[1]);
        if(cos_beta_squared<1e-30){
            euler_angle_x=0;
            euler_angle_z=-atan2(R.x[3],R.x[4]);
            euler_angle_y=R.x[2]<0?(T).5*(T)pi:-(T).5*(T)pi;}
        else{
            euler_angle_x=atan2(R.x[5],R.x[8]); // between -pi and pi
            euler_angle_y=atan2(-R.x[2],sqrt(cos_beta_squared)); // between -pi/2 and pi/2
            euler_angle_z=atan2(R.x[1],R.x[0]);}
    }

    TV Rotated_X_Axis() const
    {
        T vy2=Nova_Utilities::Sqr(q.v[1]),vz2=Nova_Utilities::Sqr(q.v[2]),vxvy=q.v[0]*q.v[1],vxvz=q.v[0]*q.v[2],svy=q.s*q.v[1],svz=q.s*q.v[2];
        return TV({1-2*(vy2+vz2),2*(vxvy+svz),2*(vxvz-svy)});
    }

    TV Rotated_Y_Axis() const
    {
        T vx2=Nova_Utilities::Sqr(q.v[0]),vz2=Nova_Utilities::Sqr(q.v[2]),vxvy=q.v[0]*q.v[1],vyvz=q.v[1]*q.v[2],svx=q.s*q.v[0],svz=q.s*q.v[2];
        return TV({2*(vxvy-svz),1-2*(vx2+vz2),2*(vyvz+svx)});
    }

    TV Rotated_Z_Axis() const
    {
        T vx2=Nova_Utilities::Sqr(q.v[0]),vy2=Nova_Utilities::Sqr(q.v[1]),vxvz=q.v[0]*q.v[2],vyvz=q.v[1]*q.v[2],svx=q.s*q.v[0],svy=q.s*q.v[1];
        return TV({2*(vxvz+svy),2*(vyvz-svx),1-2*(vx2+vy2)});
    }

    Matrix<T,3> Rotation_Matrix() const
    {
        T vx2=Nova_Utilities::Sqr<T>(q.v[0]),vy2=Nova_Utilities::Sqr<T>(q.v[1]),vz2=Nova_Utilities::Sqr<T>(q.v[2]);
        T vxvy=q.v[0]*q.v[1],vxvz=q.v[0]*q.v[2],vyvz=q.v[1]*q.v[2],svx=q.s*q.v[0],svy=q.s*q.v[1],svz=q.s*q.v[2];
        return Matrix<T,3>(1-2*(vy2+vz2),2*(vxvy+svz),2*(vxvz-svy),2*(vxvy-svz),1-2*(vx2+vz2),2*(vyvz+svx),2*(vxvz+svy),2*(vyvz-svx),1-2*(vx2+vy2));
    }

    void Get_Rotated_Frame(TV& x_axis,TV& y_axis,TV& z_axis) const
    {
        Matrix<T,3> M=Rotation_Matrix();
        x_axis=M.Column(0);
        y_axis=M.Column(1);
        z_axis=M.Column(2);
    }

    TV Rotated_Axis(const int axis) const
    {
        assert(axis>=0 && axis<=2);
        if(axis==0) return Rotated_X_Axis();
        if(axis==1) return Rotated_Y_Axis();
        return Rotated_Z_Axis();
    }

    void Get_Angle_Axis(T& angle,TV& axis) const
    {
        axis=q.s<0?-q.v:q.v;
        angle=2*atan2(axis.Normalize(),fabs(q.s));
    }

    T Angle() const
    {return 2*atan2(q.v.Norm(),fabs(q.s));}

    TV Get_Axis() const
    {return (q.s<0?-q.v:q.v).Normalized();}

    Rotation<T,d> Scale_Angle(const T a) const
    {
        T angle;TV axis;
        Get_Angle_Axis(angle,axis);
        return Rotation<T,d>(a*angle,axis);
    }

    static Rotation<T,d> Spherical_Linear_Interpolation(const Rotation<T,d>& r1,const Rotation<T,d>& r2,const T t)
    {return r1*(r1.Inverse()*r2).Scale_Angle(t);}

    static Rotation<T,d> Average_Rotation(const Array<Rotation<T,d>>& rotations)
    {
        if(rotations.size()==0) return Rotation<T,d>();
        Array<Rotation<T,d>> r(rotations);
        for(int i=0;i<(int)r.size()-1;i+=2) r.Append(Spherical_Linear_Interpolation(r(i),r(i+1),(T).5));
        return r(r.size()-1);
    }

    inline T clamp(const T x,const T xmin,const T xmax)
    {
        if(x<=xmin) return xmin;
        else if(x>=xmax) return xmax;
        else return x;
    }

    static Rotation<T,d> From_Rotated_Vector(const TV& initial_vector,const TV& final_vector)
    {
        TV initial_unit=initial_vector.Normalized(),final_unit=final_vector.Normalized();
        T cos_theta=clamp(initial_unit.Dot_Product(final_unit),(T)-1,(T)1);
        TV v=Cross_Product(initial_unit,final_unit);
        T v_norm=v.Norm();if(v_norm==0) return Rotation<T,d>();     // initial and final vectors are collinear
        T s_squared=(T).5*(1+cos_theta);                            // uses the half angle formula
        T v_norm_desired=(T)std::sqrt(1-s_squared);v*=(v_norm_desired/v_norm);
        return Rotation<T,d>((T)std::sqrt(s_squared),v[0],v[1],v[2]);
    }
};
}
#include <nova/Tools/Read_Write/Matrices/Read_Write_Rotation.h>
#endif
