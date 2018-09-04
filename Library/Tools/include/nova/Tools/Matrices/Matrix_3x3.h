//!#####################################################################
//! \file Matrix_3x3.h
//!#####################################################################
// Class Matrix_3x3
//######################################################################
#ifndef __Matrix_3x3__
#define __Matrix_3x3__

#include <nova/Tools/Matrices/Diagonal_Matrix.h>
#include <nova/Tools/Matrices/Symmetric_Matrix_3x3.h>
#include <nova/Tools/Utilities/Utilities.h>
#include <nova/Tools/Vectors/Vector.h>

namespace Nova{
template<class T>
class Matrix<T,3>
{
    using TV                = Vector<T,3>;

  public:
    T x[9];

    explicit Matrix(const int mm=3,const int nn=3)
    {
        assert(mm==3 && nn==3);
        for(int i=0;i<9;i++) x[i]=T();
    }

    Matrix(const Matrix& matrix_input)
    {
        for(int i=0;i<9;i++) x[i]=matrix_input.x[i];
    }

    template<class T2> explicit
    Matrix(const Matrix<T2,3>& matrix_input)
    {
        for(int i=0;i<9;i++) x[i]=(T)matrix_input.x[i];
    }

    Matrix(const Diagonal_Matrix<T,3>& matrix_input)
    {
        x[0]=matrix_input(0);x[4]=matrix_input(1);x[8]=matrix_input(2);
        x[1]=x[2]=x[3]=x[5]=x[6]=x[7]=0;
    }

    Matrix(const Symmetric_Matrix<T,3>& matrix_input)
    {
        x[0]=matrix_input.x11;x[1]=x[3]=matrix_input.x21;x[2]=x[6]=matrix_input.x31;x[4]=matrix_input.x22;x[5]=x[7]=matrix_input.x32;x[8]=matrix_input.x33;
    }

    Matrix(const T x11,const T x21,const T x31,const T x12,const T x22,const T x32,const T x13,const T x23,const T x33)
    {
        x[0]=x11;x[1]=x21;x[2]=x31;x[3]=x12;x[4]=x22;x[5]=x32;x[6]=x13;x[7]=x23;x[8]=x33;
    }

    Matrix(const TV& column1,const TV& column2,const TV& column3)
    {
        x[0]=column1[0];x[1]=column1[1];x[2]=column1[2];
        x[3]=column2[0];x[4]=column2[1];x[5]=column2[2];
        x[6]=column3[0];x[7]=column3[1];x[8]=column3[2];
    }

    Matrix(const Vector<TV,3>& columns)
    {
        x[0]=columns[0][0];x[1]=columns[0][1];x[2]=columns[0][2];
        x[3]=columns[1][0];x[4]=columns[1][1];x[5]=columns[1][2];
        x[6]=columns[2][0];x[7]=columns[2][1];x[8]=columns[2][2];
    }

    Matrix& operator=(const Matrix& matrix_input)
    {
        for(int i=0;i<9;i++) x[i]=matrix_input.x[i];
        return *this;
    }

    int Rows() const
    {return 3;}

    int Columns() const
    {return 3;}

    T& operator()(const int i,const int j)
    {assert(i>=0 && i<=2);assert(j>=0 && j<=2);return x[i+3*j];}

    const T& operator()(const int i,const int j) const
    {assert(i>=0 && i<=2);assert(j>=0 && j<=2);return x[i+3*j];}

    bool Valid_Index(const int i,const int j) const
    {return 0<=i && i<=2 && 0<=j && j<=2;}

    TV Column(const int j) const
    {
        assert(j>=0 && j<=2);
        if(j==0) return TV({x[0],x[1],x[2]});
        else if(j==1) return TV({x[3],x[4],x[5]});
        return TV({x[6],x[7],x[8]});
    }

    bool operator==(const Matrix& A) const
    {
        for(int i=0;i<9;i++) if(x[i]!=A.x[i]) return false;
        return true;
    }

    bool operator!=(const Matrix& A) const
    {return !(*this==A);}

    Matrix operator-() const
    {return Matrix(-x[0],-x[1],-x[2],-x[3],-x[4],-x[5],-x[6],-x[7],-x[8]);}

    Matrix& operator+=(const Matrix& A)
    {
        for(int i=0;i<9;i++) x[i]+=A.x[i];
        return *this;
    }

    Matrix& operator+=(const T& a)
    {
        x[0]+=a;x[4]+=a;x[8]+=a;
        return *this;
    }

    Matrix& operator-=(const Matrix& A)
    {
        for(int i=0;i<9;i++) x[i]-=A.x[i];
        return *this;
    }

    Matrix& operator-=(const T& a)
    {
        x[0]-=a;x[4]-=a;x[8]-=a;
        return *this;
    }

    Matrix& operator*=(const Matrix& A)
    {return *this=*this*A;}

    Matrix& operator*=(const T& a)
    {
        for(int i=0;i<9;i++) x[i]*=a;
        return *this;
    }

    Matrix& operator/=(const T& a)
    {
        assert(a!=0);
        T s=(T)1./a;
        for(int i=0;i<9;i++) x[i]*=s;
        return *this;
    }

    Matrix operator+(const Matrix& A) const
    {return Matrix(x[0]+A.x[0],x[1]+A.x[1],x[2]+A.x[2],x[3]+A.x[3],x[4]+A.x[4],x[5]+A.x[5],x[6]+A.x[6],x[7]+A.x[7],x[8]+A.x[8]);}

    Matrix operator+(const T& a) const
    {return Matrix(x[0]+a,x[1],x[2],x[3],x[4]+a,x[5],x[6],x[7],x[8]+a);}

    Matrix operator-(const Matrix& A) const
    {return Matrix(x[0]-A.x[0],x[1]-A.x[1],x[2]-A.x[2],x[3]-A.x[3],x[4]-A.x[4],x[5]-A.x[5],x[6]-A.x[6],x[7]-A.x[7],x[8]-A.x[8]);}

    Matrix operator-(const T& a) const
    {return Matrix(x[0]-a,x[1],x[2],x[3],x[4]-a,x[5],x[6],x[7],x[8]-a);}

    Matrix operator*(const Matrix& A) const
    {return Matrix(x[0]*A.x[0]+x[3]*A.x[1]+x[6]*A.x[2],x[1]*A.x[0]+x[4]*A.x[1]+x[7]*A.x[2],x[2]*A.x[0]+x[5]*A.x[1]+x[8]*A.x[2],
                   x[0]*A.x[3]+x[3]*A.x[4]+x[6]*A.x[5],x[1]*A.x[3]+x[4]*A.x[4]+x[7]*A.x[5],x[2]*A.x[3]+x[5]*A.x[4]+x[8]*A.x[5],
                   x[0]*A.x[6]+x[3]*A.x[7]+x[6]*A.x[8],x[1]*A.x[6]+x[4]*A.x[7]+x[7]*A.x[8],x[2]*A.x[6]+x[5]*A.x[7]+x[8]*A.x[8]);}

    Matrix operator*(const T& a) const
    {return Matrix(a*x[0],a*x[1],a*x[2],a*x[3],a*x[4],a*x[5],a*x[6],a*x[7],a*x[8]);}

    Matrix operator/(const T& a) const
    {
        assert(a!=0);
        T s=(T)1./a;
        return Matrix(s*x[0],s*x[1],s*x[2],s*x[3],s*x[4],s*x[5],s*x[6],s*x[7],s*x[8]);
    }

    TV operator*(const TV& v) const
    {return TV({x[0]*v[0]+x[3]*v[1]+x[6]*v[2],x[1]*v[0]+x[4]*v[1]+x[7]*v[2],x[2]*v[0]+x[5]*v[1]+x[8]*v[2]});}

    T Determinant() const
    {return x[0]*(x[4]*x[8]-x[7]*x[5])+x[3]*(x[7]*x[2]-x[1]*x[8])+x[6]*(x[1]*x[5]-x[4]*x[2]);}

    void Invert()
    {*this=Inverse();}

    Matrix Inverse_Transposed() const
    {return Inverse().Transposed();}

    void Transpose()
    {
        Nova_Utilities::Exchange_Sort(x[1],x[3]);
        Nova_Utilities::Exchange_Sort(x[2],x[6]);
        Nova_Utilities::Exchange_Sort(x[5],x[7]);
    }

    Matrix Transposed() const
    {return Matrix(x[0],x[3],x[6],x[1],x[4],x[7],x[2],x[5],x[8]);}

    T Trace() const
    {return x[0]+x[4]+x[8];}

    Matrix Cofactor_Matrix() const
    {
        return Matrix(x[4]*x[8]-x[5]*x[7],x[5]*x[6]-x[3]*x[8],x[3]*x[7]-x[4]*x[6],
                      x[2]*x[7]-x[1]*x[8],x[0]*x[8]-x[2]*x[6],x[1]*x[6]-x[0]*x[7],
                      x[1]*x[5]-x[2]*x[4],x[2]*x[3]-x[0]*x[5],x[0]*x[4]-x[1]*x[3]);
    }

    Symmetric_Matrix<T,3> Outer_Product_Matrix() const
    {
        return Symmetric_Matrix<T,3>(x[0]*x[0]+x[3]*x[3]+x[6]*x[6],x[1]*x[0]+x[4]*x[3]+x[7]*x[6],x[2]*x[0]+x[5]*x[3]+x[8]*x[6],
                                     x[1]*x[1]+x[4]*x[4]+x[7]*x[7],x[2]*x[1]+x[5]*x[4]+x[8]*x[7],x[2]*x[2]+x[5]*x[5]+x[8]*x[8]);
    }

    Symmetric_Matrix<T,3> Normal_Equations_Matrix() const
    {
        return Symmetric_Matrix<T,3>(x[0]*x[0]+x[1]*x[1]+x[2]*x[2],x[3]*x[0]+x[4]*x[1]+x[5]*x[2],x[6]*x[0]+x[7]*x[1]+x[8]*x[2],
                                     x[3]*x[3]+x[4]*x[4]+x[5]*x[5],x[6]*x[3]+x[7]*x[4]+x[8]*x[5],x[6]*x[6]+x[7]*x[7]+x[8]*x[8]);
    }

    Symmetric_Matrix<T,3> Symmetric_Part() const
    {
        return Symmetric_Matrix<T,3>(x[0],(T).5*(x[1]+x[3]),(T).5*(x[2]+x[6]),x[4],(T).5*(x[5]+x[7]),x[8]);
    }

    Symmetric_Matrix<T,3> Twice_Symmetric_Part() const
    {
        return Symmetric_Matrix<T,3>(2*x[0],x[1]+x[3],x[2]+x[6],2*x[4],x[5]+x[7],2*x[8]);
    }

    Diagonal_Matrix<T,3> Diagonal_Part() const
    {
        return Diagonal_Matrix<T,3>(x[0],x[4],x[8]);
    }

    TV Largest_Normalized_Column() const
    {
        T scale1=Nova_Utilities::Sqr(x[0])+Nova_Utilities::Sqr(x[1])+Nova_Utilities::Sqr(x[2]);
        T scale2=Nova_Utilities::Sqr(x[3])+Nova_Utilities::Sqr(x[4])+Nova_Utilities::Sqr(x[5]);
        T scale3=Nova_Utilities::Sqr(x[6])+Nova_Utilities::Sqr(x[7])+Nova_Utilities::Sqr(x[8]);
        if(scale1>scale2){if(scale1>scale3) return TV({x[0],x[1],x[2]})/std::sqrt(scale1);}
        else if(scale2>scale3) return TV({x[3],x[4],x[5]})/std::sqrt(scale2);
        return TV({x[6],x[7],x[8]})/std::sqrt(scale3);
    }

    static Matrix Transpose(const Matrix& A)
    {return Matrix(A.x[0],A.x[3],A.x[6],A.x[1],A.x[4],A.x[7],A.x[2],A.x[5],A.x[8]);}

    static Matrix Identity_Matrix()
    {return Matrix(1,0,0,0,1,0,0,0,1);}

    static Matrix Outer_Product(const TV& u,const TV& v)
    {return Matrix(u[0]*v[0],u[1]*v[0],u[2]*v[0],u[0]*v[1],u[1]*v[1],u[2]*v[1],u[0]*v[2],u[1]*v[2],u[2]*v[2]);}

    Matrix operator*(const Diagonal_Matrix<T,3>& A) const
    {return Matrix(x[0]*A(0),x[1]*A(0),x[2]*A(0),x[3]*A(1),x[4]*A(1),x[5]*A(1),x[6]*A(2),x[7]*A(2),x[8]*A(2));}

    Matrix operator*(const Symmetric_Matrix<T,3>& A) const
    {
        return Matrix(x[0]*A.x11+x[3]*A.x21+x[6]*A.x31,x[1]*A.x11+x[4]*A.x21+x[7]*A.x31,x[2]*A.x11+x[5]*A.x21+x[8]*A.x31,
                      x[0]*A.x21+x[3]*A.x22+x[6]*A.x32,x[1]*A.x21+x[4]*A.x22+x[7]*A.x32,x[2]*A.x21+x[5]*A.x22+x[8]*A.x32,
                      x[0]*A.x31+x[3]*A.x32+x[6]*A.x33,x[1]*A.x31+x[4]*A.x32+x[7]*A.x33,x[2]*A.x31+x[5]*A.x32+x[8]*A.x33);
    }

    Matrix Transpose_Times(const Matrix& A) const
    {
        return Matrix(x[0]*A.x[0]+x[1]*A.x[1]+x[2]*A.x[2],x[3]*A.x[0]+x[4]*A.x[1]+x[5]*A.x[2],x[6]*A.x[0]+x[7]*A.x[1]+x[8]*A.x[2],
                      x[0]*A.x[3]+x[1]*A.x[4]+x[2]*A.x[5],x[3]*A.x[3]+x[4]*A.x[4]+x[5]*A.x[5],x[6]*A.x[3]+x[7]*A.x[4]+x[8]*A.x[5],
                      x[0]*A.x[6]+x[1]*A.x[7]+x[2]*A.x[8],x[3]*A.x[6]+x[4]*A.x[7]+x[5]*A.x[8],x[6]*A.x[6]+x[7]*A.x[7]+x[8]*A.x[8]);
    }

    TV Transpose_Times(const TV& v) const
    {
        return TV({x[0]*v[0]+x[1]*v[1]+x[2]*v[2],x[3]*v[0]+x[4]*v[1]+x[5]*v[2],x[6]*v[0]+x[7]*v[1]+x[8]*v[2]});
    }

    Matrix Times_Transpose(const Matrix& A) const
    {
        return Matrix(x[0]*A.x[0]+x[3]*A.x[3]+x[6]*A.x[6],x[1]*A.x[0]+x[4]*A.x[3]+x[7]*A.x[6],x[2]*A.x[0]+x[5]*A.x[3]+x[8]*A.x[6],
                      x[0]*A.x[1]+x[3]*A.x[4]+x[6]*A.x[7],x[1]*A.x[1]+x[4]*A.x[4]+x[7]*A.x[7],x[2]*A.x[1]+x[5]*A.x[4]+x[8]*A.x[7],
                      x[0]*A.x[2]+x[3]*A.x[5]+x[6]*A.x[8],x[1]*A.x[2]+x[4]*A.x[5]+x[7]*A.x[8],x[2]*A.x[2]+x[5]*A.x[5]+x[8]*A.x[8]);
    }

    void Fast_Singular_Value_Decomposition(Matrix<T,3>& U,Diagonal_Matrix<T,3>& singular_values,Matrix<T,3>& V) const
    {
        if(!std::is_same<T,double>::value){Matrix<double,3> U_double,V_double;
            Diagonal_Matrix<double,3> singular_values_double;
            Matrix<double,3>(*this).Fast_Singular_Value_Decomposition(U_double,singular_values_double,V_double);
            U=Matrix<T,3>(U_double);singular_values=Diagonal_Matrix<T,3>(singular_values_double);V=Matrix<T,3>(V_double);
            return;}
        // now T is double

        // decompose normal equations
        Diagonal_Matrix<T,3> lambda;
        Normal_Equations_Matrix().Fast_Solve_Eigenproblem(lambda,V);

        // compute singular values
        if(lambda(2)<(T)0.) lambda=lambda.Clamp_Min(0);
        singular_values=lambda.Sqrt();
        if(Determinant()<(T)0.) singular_values(2)=-singular_values(2);

        // compute singular vectors
        TV column1=(*this*V.Column(0)).Normalized();
        TV v1_orthogonal=column1.Unit_Orthogonal_Vector();
        Matrix<T,3,2> other_v(v1_orthogonal,Cross_Product(column1,v1_orthogonal));
        TV column2=other_v*(other_v.Transpose_Times(*this*V.Column(1))).Normalized();
        TV column3=Cross_Product(column1,column2);
        U=Matrix(column1,column2,column3);
    }

    void Fast_Indefinite_Polar_Decomposition(Matrix<T,3>& Q,Symmetric_Matrix<T,3>& S) const
    {
        Matrix<T,3> U,V;Diagonal_Matrix<T,3> D;
        Fast_Singular_Value_Decomposition(U,D,V);
        Q=U.Times_Transpose(V);
        S=Symmetric_Matrix<T,3>::Conjugate(V,D);
    }

    Matrix<T,3> Inverse() const
    {
        T cofactor11=x[4]*x[8]-x[7]*x[5],cofactor12=x[7]*x[2]-x[1]*x[8],cofactor13=x[1]*x[5]-x[4]*x[2];
        T determinant=x[0]*cofactor11+x[3]*cofactor12+x[6]*cofactor13;
        assert(determinant!=0);
        T s=(T)1./determinant;
        return s*Matrix(cofactor11,cofactor12,cofactor13,x[6]*x[5]-x[3]*x[8],x[0]*x[8]-x[6]*x[2],x[3]*x[2]-x[0]*x[5],x[3]*x[7]-x[6]*x[4],x[6]*x[1]-x[0]*x[7],x[0]*x[4]-x[3]*x[1]);
    }

    TV Solve_Linear_System(const TV& b) const
    {
        T cofactor11=x[4]*x[8]-x[7]*x[5],cofactor12=x[7]*x[2]-x[1]*x[8],cofactor13=x[1]*x[5]-x[4]*x[2];
        T determinant=x[0]*cofactor11+x[3]*cofactor12+x[6]*cofactor13;
        assert(determinant!=0);
        return Matrix(cofactor11,cofactor12,cofactor13,x[6]*x[5]-x[3]*x[8],x[0]*x[8]-x[6]*x[2],x[3]*x[2]-x[0]*x[5],x[3]*x[7]-x[6]*x[4],x[6]*x[1]-x[0]*x[7],x[0]*x[4]-x[3]*x[1])*b/determinant;
    }
};

template<class T>
std::ostream& operator<<(std::ostream& output,const Matrix<T,3>& m)
{
    output<<"["<<m.x[0]<<" "<<m.x[3]<<" "<<m.x[6]<<"; "<<m.x[1]<<" "<<m.x[4]<<" "<<m.x[7]<<"; "<<m.x[2]<<" "<<m.x[5]<<" "<<m.x[8]<<"]";
    return output;
}

template<class T>
inline Matrix<T,3> operator+(const T a,const Matrix<T,3>& A)
{return A+a;}

template<class T>
inline Matrix<T,3> operator*(const T a,const Matrix<T,3>& A)
{return A*a;}

template<class T>
inline Matrix<T,3> operator-(const T a,const Matrix<T,3>& A)
{return Matrix<T,3>(a-A.x[0],-A.x[1],-A.x[2],-A.x[3],a-A.x[4],-A.x[5],-A.x[6],-A.x[7],a-A.x[8]);}

template<class T>
inline Vector<T,3> operator*(const Vector<T,3>& v,const Matrix<T,3>& A)
{return Vector<T,3>(v[0]*A.x[0]+v[1]*A.x[1]+v[2]*A.x[2],v[0]*A.x[3]+v[1]*A.x[4]+v[2]*A.x[5],v[0]*A.x[6]+v[1]*A.x[7]+v[2]*A.x[8]);}

template<class T>
inline Matrix<T,3> operator*(const Diagonal_Matrix<T,3>& A,const Matrix<T,3>& B)
{return Matrix<T,3>(A(0)*B.x[0],A(1)*B.x[1],A(2)*B.x[2],A(0)*B.x[3],A(1)*B.x[4],A(2)*B.x[5],A(0)*B.x[6],A(1)*B.x[7],A(2)*B.x[8]);}

template<class T>
inline Matrix<T,3> operator*(const Symmetric_Matrix<T,3>& A,const Matrix<T,3>& B)
{
    return Matrix<T,3>(A.x11*B.x[0]+A.x21*B.x[1]+A.x31*B.x[2],A.x21*B.x[0]+A.x22*B.x[1]+A.x32*B.x[2],A.x31*B.x[0]+A.x32*B.x[1]+A.x33*B.x[2],
                       A.x11*B.x[3]+A.x21*B.x[4]+A.x31*B.x[5],A.x21*B.x[3]+A.x22*B.x[4]+A.x32*B.x[5],A.x31*B.x[3]+A.x32*B.x[4]+A.x33*B.x[5],
                       A.x11*B.x[6]+A.x21*B.x[7]+A.x31*B.x[8],A.x21*B.x[6]+A.x22*B.x[7]+A.x32*B.x[8],A.x31*B.x[6]+A.x32*B.x[7]+A.x33*B.x[8]);
}

template<class T>
inline Matrix<T,3> operator+(const Symmetric_Matrix<T,3>& A,const Matrix<T,3>& B)
{return Matrix<T,3>(A.x11+B.x[0],A.x21+B.x[1],A.x31+B.x[2],A.x21+B.x[3],A.x22+B.x[4],A.x32+B.x[5],A.x31+B.x[6],A.x32+B.x[7],A.x33+B.x[8]);}

template<class T>
inline Matrix<T,3> operator-(const Symmetric_Matrix<T,3>& A,const Matrix<T,3>& B)
{return Matrix<T,3>(A.x11-B.x[0],A.x21-B.x[1],A.x31-B.x[2],A.x21-B.x[3],A.x22-B.x[4],A.x32-B.x[5],A.x31-B.x[6],A.x32-B.x[7],A.x33-B.x[8]);}
}
#include <nova/Tools/Read_Write/Matrices/Read_Write_Matrix_3x3.h>
#endif
