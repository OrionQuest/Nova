//!#####################################################################
//! \file Matrix_2x2.h
//!#####################################################################
// Class Matrix_2x2
//######################################################################
#ifndef __Matrix_2x2__
#define __Matrix_2x2__

#include <nova/Tools/Matrices/Diagonal_Matrix.h>
#include <nova/Tools/Matrices/Symmetric_Matrix_2x2.h>
#include <nova/Tools/Utilities/Utilities.h>
#include <nova/Tools/Vectors/Vector.h>

namespace Nova{
template<class T>
class Matrix<T,2>
{
    using TV                = Vector<T,2>;

  public:
    T x[4];

    explicit Matrix(const int mm=2,const int nn=2)
    {
        assert(mm==2 && nn==2);
        for(int i=0;i<4;++i) x[i]=(T)0.;
    }

    Matrix(const Matrix& matrix_input)
    {
        for(int i=0;i<4;++i) x[i]=matrix_input.x[i];
    }

    template<class T2> explicit
    Matrix(const Matrix<T2,2>& matrix_input)
    {
        for(int i=0;i<4;++i) x[i]=(T)matrix_input.x[i];
    }

    Matrix(const T x11,const T x21,const T x12,const T x22)
    {
        x[0]=x11;x[1]=x21;x[2]=x12;x[3]=x22;
    }

    Matrix(const TV& column1,const TV& column2)
    {
        x[0]=column1[0];x[1]=column1[1];
        x[2]=column2[0];x[3]=column2[1];
    }

    Matrix(const Vector<TV,2>& columns)
    {
        x[0]=columns[0][0];x[1]=columns[0][1];
        x[2]=columns[1][0];x[3]=columns[1][1];
    }

    Matrix& operator=(const Matrix& matrix_input)
    {
        for(int i=0;i<4;++i) x[i]=matrix_input.x[i];
        return *this;
    }

    int Rows() const
    {return 2;}

    int Columns() const
    {return 2;}

    T& operator()(const int i,const int j)
    {
        assert(i>=0 && i<2);
        assert(j>=0 && j<2);
        return x[i+2*j];
    }

    const T& operator()(const int i,const int j) const
    {
        assert(i>=0 && i<2);
        assert(j>=0 && j<2);
        return x[i+2*j];
    }

    bool Valid_Index(const int i,const int j) const
    {return 0<=i && i<2 && 0<=j && j<2;}

    TV Column(const int j) const
    {
        assert(j>=0 && j<2);
        if(j==0) return TV({x[0],x[1]});
        return TV({x[2],x[3]});
    }

    bool operator==(const Matrix& A) const
    {
        for(int i=0;i<4;++i) if(x[i]!=A.x[i]) return false;
        return true;
    }

    bool operator!=(const Matrix& A) const
    {return !(*this==A);}

    Matrix operator-() const
    {return Matrix(-x[0],-x[1],-x[2],-x[3]);}

    Matrix& operator+=(const Matrix& A)
    {
        for(int i=0;i<4;++i) x[i]+=A.x[i];
        return *this;
    }

    Matrix& operator+=(const T a)
    {
        x[0]+=a;x[3]+=a;
        return *this;
    }

    Matrix& operator-=(const Matrix& A)
    {
        for(int i=0;i<4;++i) x[i]-=A.x[i];
        return *this;
    }

    Matrix& operator-=(const T a)
    {
        x[0]-=a;x[3]-=a;
        return *this;
    }

    Matrix& operator*=(const Matrix& A)
    {return *this=*this*A;}

    Matrix& operator*=(const T a)
    {
        for(int i=0;i<4;++i) x[i]*=a;
        return *this;
    }

    Matrix& operator/=(const T a)
    {
        assert(a!=0);
        T s=(T)1./a;
        for(int i=0;i<4;++i) x[i]*=s;
        return *this;
    }

    Matrix operator+(const Matrix& A) const
    {return Matrix(x[0]+A.x[0],x[1]+A.x[1],x[2]+A.x[2],x[3]+A.x[3]);}

    Matrix operator+(const T a) const
    {return Matrix(x[0]+a,x[1],x[2],x[3]+a);}

    Matrix operator-(const Matrix& A) const
    {return Matrix(x[0]-A.x[0],x[1]-A.x[1],x[2]-A.x[2],x[3]-A.x[3]);}

    Matrix operator-(const T a) const
    {return Matrix(x[0]-a,x[1],x[2],x[3]-a);}

    Matrix operator*(const Matrix& A) const
    {return Matrix(x[0]*A.x[0]+x[2]*A.x[1],x[1]*A.x[0]+x[3]*A.x[1],x[0]*A.x[2]+x[2]*A.x[3],x[1]*A.x[2]+x[3]*A.x[3]);}

    Matrix operator*(const T a) const
    {return Matrix(a*x[0],a*x[1],a*x[2],a*x[3]);}

    Matrix operator/(const T a) const
    {
        assert(a!=0);
        T s=(T)1./a;
        return Matrix(s*x[0],s*x[1],s*x[2],s*x[3]);
    }

    TV operator*(const TV& v) const
    {return TV({x[0]*v[0]+x[2]*v[1],x[1]*v[0]+x[3]*v[1]});}

    T Determinant() const
    {return x[0]*x[3]-x[1]*x[2];}

    void Invert()
    {*this=Inverse();}

    Matrix Inverse() const
    {
        T one_over_determinant=(T)1./Determinant();
        return Matrix(one_over_determinant*x[3],-one_over_determinant*x[1],-one_over_determinant*x[2],one_over_determinant*x[0]);
    }

    Matrix Inverse_Transposed() const
    {return Inverse().Transposed();}

    Vector<T,2> Solve_Linear_System(const TV& b) const
    {
        T one_over_determinant=(T)1./Determinant();
        return one_over_determinant*TV({x[3]*b[0]-x[2]*b[1],x[0]*b[1]-x[1]*b[0]});
    }

    void Transpose()
    {Nova_Utilities::Exchange_Sort<T>(x[1],x[2]);}

    Matrix Transposed() const
    {return Matrix(x[0],x[2],x[1],x[3]);}

    T Trace() const
    {return x[0]+x[3];}

    Matrix Cofactor_Matrix() const
    {return Matrix(x[3],-x[2],-x[1],x[0]);}

    Diagonal_Matrix<T,2> Diagonal_Part() const
    {return Diagonal_Matrix<T,2>({x[0],x[3]});}

    static Matrix Transpose(const Matrix& A)
    {return Matrix(A.x[0],A.x[2],A.x[1],A.x[3]);}

    static Matrix Identity_Matrix()
    {return Matrix(1,0,0,1);}

    static Matrix Outer_Product(const TV& u,const TV& v)
    {return Matrix(u[0]*v[0],u[1]*v[0],u[0]*v[1],u[1]*v[1]);}

    Matrix operator*(const Diagonal_Matrix<T,2>& A) const
    {return Matrix(x[0]*A(0,0),x[1]*A(0,0),x[2]*A(1,1),x[3]*A(1,1));}

    Matrix& operator*=(const Diagonal_Matrix<T,2>& A)
    {return *this=*this*A;}

    Matrix Times_Transpose(const Matrix& A) const
    {return Matrix(x[0]*A.x[0]+x[2]*A.x[2],x[1]*A.x[0]+x[3]*A.x[2],x[0]*A.x[1]+x[2]*A.x[3],x[1]*A.x[1]+x[3]*A.x[3]);}

    Matrix Transpose_Times(const Matrix& A) const
    {return Matrix(x[0]*A.x[0]+x[1]*A.x[1],x[2]*A.x[0]+x[3]*A.x[1],x[0]*A.x[2]+x[1]*A.x[3],x[2]*A.x[2]+x[3]*A.x[3]);}

    TV Transpose_Times(const TV& v) const
    {return TV({x[0]*v[0]+x[1]*v[1],x[2]*v[0]+x[3]*v[1]});}

    void Indefinite_Polar_Decomposition(Matrix& Q,Symmetric_Matrix<T,2>& S) const
    {
        T x03=x[0]+x[3],cosine,sine;
        if(x03==0){cosine=0;sine=1;}
        else{T t=(x[1]-x[2])/x03;cosine=1/std::sqrt(1+t*t);sine=t*cosine;}
        Q=Matrix(cosine,sine,-sine,cosine);
        S=Symmetric_Matrix<T,2>(Q.x[0]*x[0]+Q.x[1]*x[1],Q.x[0]*x[2]+Q.x[1]*x[3],Q.x[2]*x[2]+Q.x[3]*x[3]);
    }

    void Fast_Singular_Value_Decomposition(Matrix& U,Diagonal_Matrix<T,2>& singular_values,Matrix& V) const
    {
        Matrix Q;Symmetric_Matrix<T,2> S;
        Indefinite_Polar_Decomposition(Q,S);
        S.Solve_Eigenproblem(singular_values,V);
        if(singular_values(1)<0 && fabs(singular_values(1))>=fabs(singular_values(0))){
            singular_values=Diagonal_Matrix<T,2>({-singular_values(1),-singular_values(0)});
            Q=-Q;V=Matrix(V.x[2],V.x[3],-V.x[0],-V.x[1]);}
        U=Q*V;
    }
};

template<class T>
std::ostream& operator<<(std::ostream& output,const Matrix<T,2>& m)
{output<<"["<<m.x[0]<<" "<<m.x[2]<<"; "<<m.x[1]<<" "<<m.x[3]<<"]";return output;}

template<class T>
inline Matrix<T,2> operator+(const T a,const Matrix<T,2>& A)
{return A+a;}

template<class T>
inline Matrix<T,2> operator-(const T a,const Matrix<T,2>& A)
{return -A+a;}

template<class T>
inline Matrix<T,2> operator+(const Symmetric_Matrix<T,2>& A,const Matrix<T,2>& B)
{return Matrix<T,2>(A.x11+B.x[0],A.x21+B.x[1],A.x21+B.x[2],A.x22+B.x[3]);}

template<class T>
inline Matrix<T,2> operator-(const Symmetric_Matrix<T,2>& A,const Matrix<T,2>& B)
{return Matrix<T,2>(A.x11-B.x[0],A.x21-B.x[1],A.x21-B.x[2],A.x22-B.x[3]);}

template<class T>
inline Matrix<T,2> operator*(const T a,const Matrix<T,2>& A)
{return Matrix<T,2>(a*A.x[0],a*A.x[1],a*A.x[2],a*A.x[3]);}

template<class T>
inline Vector<T,2> operator*(const Vector<T,2>& v,const Matrix<T,2>& A)
{return Vector<T,2>(v[0]*A.x[0]+v[1]*A.x[2],v[0]*A.x[1]+v[1]*A.x[3]);}

template<class T>
inline Matrix<T,2> operator*(const Diagonal_Matrix<T,2>& A,const Matrix<T,2>& B)
{return Matrix<T,2>(A(0)*B.x[0],A(1)*B.x[1],A(0)*B.x[2],A(1)*B.x[3]);}
}
#include <nova/Tools/Read_Write/Matrices/Read_Write_Matrix_2x2.h>
#endif
