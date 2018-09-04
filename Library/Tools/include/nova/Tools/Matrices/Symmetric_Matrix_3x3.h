//!#####################################################################
//! \file Symmetric_Matrix_3x3.h
//!#####################################################################
// Class Symmetric_Matrix_3x3
//######################################################################
#ifndef __Symmetric_Matrix_3x3__
#define __Symmetric_Matrix_3x3__

#include <nova/Tools/Log/Debug_Utilities.h>
#include <nova/Tools/Matrices/Matrix_Forward.h>
#include <nova/Tools/Matrices/Matrix_3x2.h>
#include <nova/Tools/Matrices/Symmetric_Matrix_2x2.h>
#include <nova/Tools/Vectors/Vector.h>
#include <nova/Tools/Utilities/Constants.h>
#include <nova/Tools/Utilities/Utilities.h>
#include <cassert>
#include <type_traits>

namespace Nova{
template<class T>
class Symmetric_Matrix<T,3>
{
    using TV                = Vector<T,3>;

  public:
    T x11,x21,x31,x22,x32,x33;

    Symmetric_Matrix(const int mm=3,const int nn=3)
        :x11(0),x21(0),x31(0),x22(0),x32(0),x33(0)
    {
        assert(mm==3 && nn==3);
    }

    template<class T2> explicit
    Symmetric_Matrix(const Symmetric_Matrix<T2,3>& matrix_input)
        :x11((T)matrix_input.x11),x21((T)matrix_input.x21),x31((T)matrix_input.x31),x22((T)matrix_input.x22),x32((T)matrix_input.x32),x33((T)matrix_input.x33)
    {}

    Symmetric_Matrix(const Diagonal_Matrix<T,3>& matrix_input)
        :x11(matrix_input.x11),x21(0),x31(0),x22(matrix_input.x22),x32(0),x33(matrix_input.x33)
    {}

    Symmetric_Matrix(const T y11,const T y21,const T y31,const T y22,const T y32,const T y33)
        :x11(y11),x21(y21),x31(y31),x22(y22),x32(y32),x33(y33)
    {}

    int Rows() const
    {return 3;}

    int Columns() const
    {return 3;}

    TV Column(const int axis) const
    {
        assert(0<=axis && axis<=2);
        return axis==0?TV(x11,x21,x31):axis==1?TV(x21,x22,x32):TV(x31,x32,x33);
    }

    T& operator()(int i,int j)
    {return i<j?Element_Upper(i,j):Element_Lower(i,j);}

    const T& operator()(int i,int j) const
    {return i<j?Element_Upper(i,j):Element_Lower(i,j);}

    bool Valid_Index(const int i,const int j) const
    {return 0<=i && i<=2 && 0<=j && j<=2;}

    T& Element_Upper(int i,int j)
    {return Element_Lower(j,i);}

    const T& Element_Upper(int i,int j) const
    {return Element_Lower(j,i);}

    T& Element_Lower(int i,int j)
    {assert(i<=2 && j>=0 && j<=i);return ((T*)this)[((5-j)*j>>1)+i];}

    const T& Element_Lower(int i,int j) const
    {assert(i<=2 && j>=0 && j<=i);return ((T*)this)[((5-j)*j>>1)+i];}

    bool operator==(const Symmetric_Matrix& A) const
    {return x11==A.x11 && x21==A.x21 && x31==A.x31 && x22==A.x22 && x32==A.x32 && x33==A.x33;}

    bool operator!=(const Symmetric_Matrix& A) const
    {return !(*this==A);}

    Symmetric_Matrix operator-() const
    {return Symmetric_Matrix(-x11,-x21,-x31,-x22,-x32,-x33);}

    Symmetric_Matrix& operator+=(const Symmetric_Matrix& A)
    {   
        x11+=A.x11;x21+=A.x21;x31+=A.x31;x22+=A.x22;x32+=A.x32;x33+=A.x33;
        return *this;
    }

    Symmetric_Matrix& operator+=(const T& a)
    {
        x11+=a;x22+=a;x33+=a;
        return *this;
    }

    Symmetric_Matrix& operator-=(const Symmetric_Matrix& A)
    {
        x11-=A.x11;x21-=A.x21;x31-=A.x31;x22-=A.x22;x32-=A.x32;x33-=A.x33;
        return *this;
    }

    Symmetric_Matrix& operator-=(const T& a)
    {
        x11-=a;x22-=a;x33-=a;
        return *this;
    }

    Symmetric_Matrix& operator*=(const T a)
    {
        x11*=a;x21*=a;x31*=a;x22*=a;x32*=a;x33*=a;
        return *this;
    }

    Symmetric_Matrix& operator/=(const T a)
    {
        assert(a!=0);
        T s=(T)1./a;
        x11*=s;x21*=s;x31*=s;x22*=s;x32*=s;x33*=s;
        return *this;
    }

    Symmetric_Matrix operator+(const Symmetric_Matrix& A) const
    {return Symmetric_Matrix(x11+A.x11,x21+A.x21,x31+A.x31,x22+A.x22,x32+A.x32,x33+A.x33);}

    Symmetric_Matrix operator+(const T a) const
    {return Symmetric_Matrix(x11+a,x21,x31,x22+a,x32,x33+a);}

    Symmetric_Matrix operator-(const Symmetric_Matrix& A) const
    {return Symmetric_Matrix(x11-A.x11,x21-A.x21,x31-A.x31,x22-A.x22,x32-A.x32,x33-A.x33);}

    Symmetric_Matrix operator-(const T a) const
    {return Symmetric_Matrix(x11-a,x21,x31,x22-a,x32,x33-a);}

    Symmetric_Matrix operator*(const T a) const
    {return Symmetric_Matrix(a*x11,a*x21,a*x31,a*x22,a*x32,a*x33);}

    Matrix<T,3> operator*(const Symmetric_Matrix& A) const
    {
        return Matrix<T,3>(x11*A.x11+x21*A.x21+x31*A.x31,x21*A.x11+x22*A.x21+x32*A.x31,x31*A.x11+x32*A.x21+x33*A.x31,
                           x11*A.x21+x21*A.x22+x31*A.x32,x21*A.x21+x22*A.x22+x32*A.x32,x31*A.x21+x32*A.x22+x33*A.x32,
                           x11*A.x31+x21*A.x32+x31*A.x33,x21*A.x31+x22*A.x32+x32*A.x33,x31*A.x31+x32*A.x32+x33*A.x33);
    }

    Matrix<T,3> Times_Transpose(const Symmetric_Matrix& M) const
    {return *this*M;}

    Symmetric_Matrix operator/(const T a) const
    {
        assert(a!=0);
        T s=(T)1./a;
        return Symmetric_Matrix(s*x11,s*x21,s*x31,s*x22,s*x32,s*x33);
    }

    TV operator*(const TV& v) const
    {return TV({x11*v[0]+x21*v[1]+x31*v[2],x21*v[0]+x22*v[1]+x32*v[2],x31*v[0]+x32*v[1]+x33*v[2]});}

    T Determinant() const
    {return x11*(x22*x33-x32*x32)+x21*(2*x32*x31-x21*x33)-x31*x22*x31;}

    Symmetric_Matrix Inverse() const
    {
        T cofactor11=x22*x33-x32*x32,cofactor12=x32*x31-x21*x33,cofactor13=x21*x32-x22*x31;
        return Symmetric_Matrix(cofactor11,cofactor12,cofactor13,x11*x33-x31*x31,x21*x31-x11*x32,x11*x22-x21*x21)/(x11*cofactor11+x21*cofactor12+x31*cofactor13);
    }

    Symmetric_Matrix Transposed() const
    {return *this;}

    void Transpose() {}

    T Trace() const
    {return x11+x22+x33;}

    Symmetric_Matrix Cofactor_Matrix() const
    {return Symmetric_Matrix(x22*x33-x32*x32,x32*x31-x21*x33,x21*x32-x22*x31,x11*x33-x31*x31,x21*x31-x11*x32,x11*x22-x21*x21);}

    TV Largest_Column() const
    {
        T sqr11=Nova_Utilities::Sqr(x11),sqr12=Nova_Utilities::Sqr(x21),sqr13=Nova_Utilities::Sqr(x31);
        T sqr22=Nova_Utilities::Sqr(x22),sqr23=Nova_Utilities::Sqr(x32),sqr33=Nova_Utilities::Sqr(x33);
        T scale1=sqr11+sqr12+sqr13,scale2=sqr12+sqr22+sqr23,scale3=sqr13+sqr23+sqr33;
        return scale1>scale2?(scale1>scale3?TV({x11,x21,x31}):TV({x31,x32,x33})):(scale2>scale3?TV({x21,x22,x32}):TV({x31,x32,x33}));
    }

    TV Largest_Column_Normalized() const
    {
        T sqr11=Nova_Utilities::Sqr(x11),sqr12=Nova_Utilities::Sqr(x21),sqr13=Nova_Utilities::Sqr(x31);
        T sqr22=Nova_Utilities::Sqr(x22),sqr23=Nova_Utilities::Sqr(x32),sqr33=Nova_Utilities::Sqr(x33);
        T scale1=sqr11+sqr12+sqr13,scale2=sqr12+sqr22+sqr23,scale3=sqr13+sqr23+sqr33;
        if(scale1>scale2){if(scale1>scale3) return TV({x11,x21,x31})/std::sqrt(scale1);}
        else if(scale2>scale3) return TV({x21,x22,x32})/std::sqrt(scale2);
        if(scale3>0) return TV({x31,x32,x33})/std::sqrt(scale3);else return TV({1,0,0});
    }

    Symmetric_Matrix Positive_Definite_Part() const
    {
        Diagonal_Matrix<T,3> D;
        Matrix<T,3> V;
        Fast_Solve_Eigenproblem(D,V);
        D=D.Clamp_Min(0);
        return Conjugate(V,D);
    }

    Diagonal_Matrix<T,3> Fast_Eigenvalues() const
    {
        if(!std::is_same<T,double>::value) return Diagonal_Matrix<T,3>(Symmetric_Matrix<double,3>(*this).Fast_Eigenvalues());
        // now T is double
        T m=(T)one_third*(x11+x22+x33);
        T a11=x11-m,a22=x22-m,a33=x33-m,a12_sqr=x21*x21,a13_sqr=x31*x31,a23_sqr=x32*x32;
        T p=(T)one_sixth*(a11*a11+a22*a22+a33*a33+2*(a12_sqr+a13_sqr+a23_sqr));
        T q=(T).5*(a11*(a22*a33-a23_sqr)-a22*a13_sqr-a33*a12_sqr)+x21*x31*x32;
        T sqrt_p=std::sqrt(p),disc=p*p*p-q*q;
        T phi=(T)one_third*atan2(std::sqrt(std::max((T)0,disc)),q),c=cos(phi),s=sin(phi);
        T sqrt_p_cos=sqrt_p*c,root_three_sqrt_p_sin=(T)root_three*sqrt_p*s;
        Diagonal_Matrix<T,3> lambda({m+2*sqrt_p_cos,m-sqrt_p_cos-root_three_sqrt_p_sin,m-sqrt_p_cos+root_three_sqrt_p_sin});
        Nova_Utilities::Exchange_Sort(lambda(2),lambda(1),lambda(0));
        return lambda;
    }

    Matrix<T,3> Fast_Eigenvectors(const Symmetric_Matrix<T,3>& A,const Diagonal_Matrix<T,3>& lambda) const
    {
        if(!std::is_same<T,double>::value) FATAL_ERROR("Only doubles allowed!");
        // T is now always double

        // flip if necessary so that first eigenvalue is the most different
        bool flipped=false;
        Diagonal_Matrix<T,3> lambda_flip(lambda);
        if(lambda(0)-lambda(1)<lambda(1)-lambda(2)){
            Nova_Utilities::Exchange_Sort(lambda_flip(0),lambda_flip(2));
            flipped=true;}

        // get first eigenvector
        TV v1=(A-lambda_flip(0)).Cofactor_Matrix().Largest_Column_Normalized();

        // form basis for orthogonal complement to v1, and reduce A to this space
        TV v1_orthogonal=v1.Unit_Orthogonal_Vector();
        Matrix<T,3,2> other_v(v1_orthogonal,Cross_Product(v1,v1_orthogonal));
        Symmetric_Matrix<T,2> A_reduced=Symmetric_Matrix<T,2>::Conjugate_With_Transpose(other_v,A);

        // find third eigenvector from A_reduced, and fill in second via cross product
        TV v3=other_v*(A_reduced-lambda_flip(2)).Cofactor_Matrix().Largest_Column_Normalized();
        TV v2=Cross_Product(v3,v1);

        // finish
        return flipped?Matrix<T,3>(v3,v2,-v1):Matrix<T,3>(v1,v2,v3);
    }

    void Fast_Solve_Eigenproblem(Diagonal_Matrix<T,3>& eigenvalues,Matrix<T,3>& eigenvectors) const
    {
        if(!std::is_same<T,double>::value){
            Diagonal_Matrix<double,3> eigenvalues_double;Matrix<double,3> eigenvectors_double;
            Symmetric_Matrix<double,3>(*this).Fast_Solve_Eigenproblem(eigenvalues_double,eigenvectors_double);
            eigenvalues=Diagonal_Matrix<T,3>(eigenvalues_double);eigenvectors=Matrix<T,3>(eigenvectors_double);return;}
        // now T is double
        eigenvalues=Fast_Eigenvalues();
        eigenvectors=Fast_Eigenvectors(*this,eigenvalues);
    }

    static Symmetric_Matrix Times_Transpose_With_Symmetric_Result(const Matrix<T,3>& A,const Matrix<T,3>& B)
    {
        return Symmetric_Matrix(A.x[0]*B.x[0]+A.x[3]*B.x[3]+A.x[6]*B.x[6],A.x[1]*B.x[0]+A.x[4]*B.x[3]+A.x[7]*B.x[6],
                                A.x[2]*B.x[0]+A.x[5]*B.x[3]+A.x[8]*B.x[6],A.x[1]*B.x[1]+A.x[4]*B.x[4]+A.x[7]*B.x[7],
                                A.x[2]*B.x[1]+A.x[5]*B.x[4]+A.x[8]*B.x[7],A.x[2]*B.x[2]+A.x[5]*B.x[5]+A.x[8]*B.x[8]);
    }

    static Symmetric_Matrix Times_Transpose_With_Symmetric_Result(const Matrix<T,3,2>& A,const Matrix<T,3,2>& B)
    {
        return Symmetric_Matrix(A.x[0]*B.x[0]+A.x[3]*B.x[3],A.x[1]*B.x[0]+A.x[4]*B.x[3],A.x[2]*B.x[0]+A.x[5]*B.x[3],
                                A.x[1]*B.x[1]+A.x[4]*B.x[4],A.x[2]*B.x[1]+A.x[5]*B.x[4],A.x[2]*B.x[2]+A.x[5]*B.x[5]);
    }

    static Symmetric_Matrix Transpose_Times_With_Symmetric_Result(const Matrix<T,3>& A,const Matrix<T,3>& B)
    {
        return Symmetric_Matrix(A.x[0]*B.x[0]+A.x[1]*B.x[1]+A.x[2]*B.x[2],A.x[3]*B.x[0]+A.x[4]*B.x[1]+A.x[5]*B.x[2],A.x[6]*B.x[0]+A.x[7]*B.x[1]+A.x[8]*B.x[2],
                                A.x[3]*B.x[3]+A.x[4]*B.x[4]+A.x[5]*B.x[5],A.x[6]*B.x[3]+A.x[7]*B.x[4]+A.x[8]*B.x[5],A.x[6]*B.x[6]+A.x[7]*B.x[7]+A.x[8]*B.x[8]);
    }

    static Symmetric_Matrix Conjugate(const Matrix<T,3>& A,const Diagonal_Matrix<T,3>& B)
    {return Times_Transpose_With_Symmetric_Result(A*B,A);}

    static Symmetric_Matrix Conjugate(const Matrix<T,3>& A,const Symmetric_Matrix<T,3>& B)
    {return Times_Transpose_With_Symmetric_Result(A*B,A);}

    static Symmetric_Matrix Conjugate(const Symmetric_Matrix& A,const Symmetric_Matrix& B)
    {return Times_Transpose_With_Symmetric_Result(A*B,A);}

    static Symmetric_Matrix Conjugate(const Matrix<T,3,2>& A,const Diagonal_Matrix<T,2>& B)
    {return Times_Transpose_With_Symmetric_Result(A*B,A);}

    static Symmetric_Matrix Conjugate(const Matrix<T,3,2>& A,const Symmetric_Matrix<T,2>& B)
    {return Times_Transpose_With_Symmetric_Result(A*B,A);}
};

template<class T>
std::ostream& operator<<(std::ostream& output,const Symmetric_Matrix<T,3>& A)
{
    output<<"["<<A.x11<<" "<<A.x21<<" "<<A.x31<<"; "<<A.x21<<" "<<A.x22<<" "<<A.x32<<"; "<<A.x31<<" "<<A.x32<<" "<<A.x33<<"]";
    return output;
}
}
#endif
