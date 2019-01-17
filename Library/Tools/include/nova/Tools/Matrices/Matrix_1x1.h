//!#####################################################################
//! \file Matrix_1x1.h
//!#####################################################################
// Class Matrix_1x1
//######################################################################
#ifndef __Matrix_1x1__
#define __Matrix_1x1__

#include <nova/Tools/Matrices/Matrix_Forward.h>
#include <nova/Tools/Vectors/Vector.h>

namespace Nova{
template<class T>
class Matrix<T,1>
{
  public:
    T x11;

    explicit Matrix(const int mm=1,const int nn=1)
    {
        assert(mm==1 && nn==1);
        x11=(T)0.;
    }

    Matrix(const Matrix& matrix_input)
        :x11(matrix_input.x11)
    {}

    template<class T2>
    explicit Matrix(const Matrix<T2,1>& matrix_input)
        :x11((T)matrix_input.x11)
    {}

    explicit Matrix(const T x11_input)
        :x11(x11_input)
    {}

    explicit Matrix(const Vector<T,1>& v)
        :x11(v[0])
    {}

    Matrix& operator=(const Matrix& matrix)
    {x11=matrix.x11;return *this;}

    int Rows() const
    {return 1;}

    int Columns() const
    {return 1;}

    T& operator()(const int i,const int j=0)
    {assert(i==0 && j==0);return x11;}

    const T& operator()(const int i,const int j=0) const
    {assert(i==0 && j==0);return x11;}

    bool Valid_Index(const int i,const int j) const
    {return i==0 && j==0;}

    bool operator==(const Matrix& A) const
    {return x11==A.x11;}

    bool operator!=(const Matrix& A) const
    {return !(*this==A);}

    Matrix Inverse() const
    {assert(x11);return Matrix((T)1./x11);}

    Matrix operator-() const
    {return Matrix(-x11);}

    Matrix operator+(const T a) const
    {return Matrix(x11+a);}

    Matrix operator+(const Matrix& A) const
    {return Matrix(x11+A.x11);}

    Matrix operator-(const T a) const
    {return Matrix(x11-a);}

    Matrix operator-(const Matrix& A) const
    {return Matrix(x11-A.x11);}

    Matrix operator*(const Matrix& A) const
    {return Matrix(x11*A.x11);}

    Matrix operator*(const T a) const
    {return Matrix(a*x11);}

    Matrix operator/(const T a) const
    {return Matrix(x11/a);}

    Vector<T,1> operator*(const Vector<T,1>& v) const
    {return Vector<T,1>(x11*v[0]);}

    Matrix& operator+=(const Matrix& A)
    {x11+=A.x11;return *this;}

    Matrix& operator-=(const Matrix& A)
    {x11-=A.x11;return *this;}

    Matrix& operator+=(const T& a)
    {x11+=a;return *this;}

    Matrix& operator-=(const T& a)
    {x11-=a;return *this;}

    Matrix& operator*=(const T a)
    {x11*=a;return *this;}

    Matrix& operator*=(const Matrix& A)
    {x11*=A.x11;return *this;}

    Matrix& operator/=(const T a)
    {x11/=a;return *this;}

    Vector<T,1> Transpose_Times(const Vector<T,1>& v) const
    {return Vector<T,1>(x11*v[0]);}

    void Transpose()
    {}

    Matrix Transposed() const
    {return *this;}

    T Trace() const
    {return x11;}

    T Determinant() const
    {return x11;}
};

template<class T>
inline Matrix<T,1> operator*(const T a,const Matrix<T,1>& A)
{return A*a;}

template<class T>
inline Matrix<T,1> operator+(const T a,const Matrix<T,1>& A)
{return A+a;}

template<class T>
inline Matrix<T,1> operator-(const T a,const Matrix<T,1>& A)
{return Matrix<T,1>(a-A.x11);}
}
#include <nova/Tools/Read_Write/Matrices/Read_Write_Matrix_1x1.h>
#endif
