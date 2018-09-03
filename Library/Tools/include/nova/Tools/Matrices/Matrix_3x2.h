//!#####################################################################
//! \file Matrix_3x2.h
//!#####################################################################
// Class Matrix_3x2
//######################################################################
#ifndef __Matrix_3x2__
#define __Matrix_3x2__

#include <nova/Tools/Matrices/Matrix_Forward.h>
#include <nova/Tools/Vectors/Vector.h>

namespace Nova{
template<class T>
class Matrix<T,3,2>
{
    using TV            = Vector<T,3>;

  public:
    T x[6];

    Matrix()
    {
        for(int i=0;i<6;i++) x[i]=(T)0.;
    }

    explicit Matrix(const int mm=3,const int nn=2)
    {
        assert(mm==3 && nn==2);
        for(int i=0;i<6;i++) x[i]=(T)0.;
    }

    Matrix(const Matrix& matrix_input)
    {
        for(int i=0;i<6;i++) x[i]=matrix_input.x[i];
    }

    template<class T2> explicit
    Matrix(const Matrix<T2,3,2>& matrix_input)
    {
        for(int i=0;i<6;i++) x[i]=(T)matrix_input.x[i];
    }

    Matrix(const T x11,const T x21,const T x31,const T x12,const T x22,const T x32)
    {
        x[0]=x11;x[1]=x21;x[2]=x31;x[3]=x12;x[4]=x22;x[5]=x32;
    }

    Matrix(const TV& column1,const TV& column2)
    {
        x[0]=column1[0];x[1]=column1[1];x[2]=column1[2];
        x[3]=column2[0];x[4]=column2[1];x[5]=column2[2];
    }

    Matrix(const Vector<TV,2>& columns)
    {
        x[0]=columns[0][0];x[1]=columns[0][1];x[2]=columns[0][2];
        x[3]=columns[1][0];x[4]=columns[1][1];x[5]=columns[1][2];
    }

    Matrix& operator=(const Matrix& matrix_input)
    {
        for(int i=0;i<6;i++) x[i]=matrix_input.x[i];
        return *this;
    }

    int Rows() const
    {return 3;}

    int Columns() const
    {return 2;}

    T& operator()(const int i,const int j)
    {assert(i>=0 && i<=2);assert(j>=0 && j<=1);return x[i+3*j];}

    const T& operator()(const int i,const int j) const
    {assert(i>=0 && i<=2);assert(j>=0 && j<=1);return x[i+3*j];}

    bool Valid_Index(const int i,const int j) const
    {return 0<=i && i<=2 && 0<=j && j<=1;}

    TV Column(const int j) const
    {
        assert(j>=0 && j<=1);
        if(j==0) return TV({x[0],x[1],x[2]});
        return TV({x[3],x[4],x[5]});
    }

    bool operator==(const Matrix& A) const
    {
        for(int i=0;i<6;i++) if(x[i]!=A.x[i]) return false;
        return true;
    }

    bool operator!=(const Matrix& A) const
    {return !(*this==A);}

    Matrix operator-() const
    {return Matrix(-x[0],-x[1],-x[2],-x[3],-x[4],-x[5]);}

    Matrix& operator+=(const Matrix& A)
    {
        for(int i=0;i<6;i++) x[i]+=A.x[i];
        return *this;
    }

    Matrix& operator-=(const Matrix& A)
    {
        for(int i=0;i<6;i++) x[i]-=A.x[i];
        return *this;
    }

    Matrix& operator*=(const Matrix<T,2>& A)
    {return *this=*this*A;}

    Matrix& operator*=(const T a)
    {
        for(int i=0;i<6;i++) x[i]*=a;
        return *this;
    }

    Matrix& operator/=(const T a)
    {
        assert(a!=0);
        T s=(T)1./a;
        for(int i=0;i<6;i++) x[i]*=s;
        return *this;
    }

    Matrix operator+(const Matrix& A) const
    {return Matrix(x[0]+A.x[0],x[1]+A.x[1],x[2]+A.x[2],x[3]+A.x[3],x[4]+A.x[4],x[5]+A.x[5]);}

    Matrix operator-(const Matrix& A) const
    {return Matrix(x[0]-A.x[0],x[1]-A.x[1],x[2]-A.x[2],x[3]-A.x[3],x[4]-A.x[4],x[5]-A.x[5]);}

    Matrix operator*(const Matrix<T,2>& A) const
    {return Matrix(x[0]*A.x[0]+x[3]*A.x[1],x[1]*A.x[0]+x[4]*A.x[1],x[2]*A.x[0]+x[5]*A.x[1],x[0]*A.x[2]+x[3]*A.x[3],x[1]*A.x[2]+x[4]*A.x[3],x[2]*A.x[2]+x[5]*A.x[3]);}

    Matrix Times_Transpose(const Matrix<T,2>& A) const
    {return Matrix(x[0]*A.x[0]+x[3]*A.x[2],x[1]*A.x[0]+x[4]*A.x[2],x[2]*A.x[0]+x[5]*A.x[2],x[0]*A.x[1]+x[3]*A.x[3],x[1]*A.x[1]+x[4]*A.x[3],x[2]*A.x[1]+x[5]*A.x[3]);}

    Matrix<T,3> Times_Transpose(const Matrix& A) const
    {
        return Matrix<T,3>(x[0]*A.x[0]+x[3]*A.x[3],x[1]*A.x[0]+x[4]*A.x[3],x[2]*A.x[0]+x[5]*A.x[3],
                           x[0]*A.x[1]+x[3]*A.x[4],x[1]*A.x[1]+x[4]*A.x[4],x[2]*A.x[1]+x[5]*A.x[4],
                           x[0]*A.x[2]+x[3]*A.x[5],x[1]*A.x[2]+x[4]*A.x[5],x[2]*A.x[2]+x[5]*A.x[5]);
    }

    Matrix operator*(const T a) const
    {return Matrix(a*x[0],a*x[1],a*x[2],a*x[3],a*x[4],a*x[5]);}

    Matrix operator/(const T a) const
    {
        assert(a!=0);
        T s=(T)1./a;
        return Matrix(s*x[0],s*x[1],s*x[2],s*x[3],s*x[4],s*x[5]);
    }

    TV operator*(const Vector<T,2>& v) const
    {return TV({x[0]*v[0]+x[3]*v[1],x[1]*v[0]+x[4]*v[1],x[2]*v[0]+x[5]*v[1]});}

    Symmetric_Matrix<T,2> Normal_Equations_Matrix() const
    {return Symmetric_Matrix<T,2>(x[0]*x[0]+x[1]*x[1]+x[2]*x[2],x[3]*x[0]+x[4]*x[1]+x[5]*x[2],x[3]*x[3]+x[4]*x[4]+x[5]*x[5]);}

    Matrix<T,2> Transpose_Times(const Matrix<T,3>& A) const
    {return Matrix<T,2>(x[0]*A.x[0]+x[1]*A.x[1]+x[2]*A.x[2],x[3]*A.x[0]+x[4]*A.x[1]+x[5]*A.x[2],x[0]*A.x[3]+x[1]*A.x[4]+x[2]*A.x[5],x[3]*A.x[3]+x[4]*A.x[4]+x[5]*A.x[5]);}

    Vector<T,2> Transpose_Times(const TV& v) const
    {return Vector<T,2>({x[0]*v[0]+x[1]*v[1]+x[2]*v[2],x[3]*v[0]+x[4]*v[1]+x[5]*v[2]});}

    Matrix operator*(const Symmetric_Matrix<T,2>& A) const
    {return Matrix(x[0]*A.x11+x[3]*A.x21,x[1]*A.x11+x[4]*A.x21,x[2]*A.x11+x[5]*A.x21,x[0]*A.x21+x[3]*A.x22,x[1]*A.x21+x[4]*A.x22,x[2]*A.x21+x[5]*A.x22);}

    Matrix operator*(const Diagonal_Matrix<T,2>& A) const
    {return Matrix(x[0]*A.x11,x[1]*A.x11,x[2]*A.x11,x[3]*A.x22,x[4]*A.x22,x[5]*A.x22);}

    static T Inner_Product(const Matrix& A,const Matrix& B)
    {return A.x[0]*B.x[0]+A.x[1]*B.x[1]+A.x[2]*B.x[2]+A.x[3]*B.x[3]+A.x[4]*B.x[4]+A.x[5]*B.x[5];}

    TV Weighted_Normal() const
    {return Cross_Product(Column(1),Column(2));}

    Matrix Cofactor_Matrix() const
    {
        TV normal=Weighted_Normal().Normalized();
        return Matrix(Cross_Product(Column(2),normal),Cross_Product(normal,Column(1)));
    }

    static Matrix Outer_Product(const TV& u,const Vector<T,2>& v)
    {
        Matrix result;
        for(int i=0;i<3;i++) for(int j=0;j<2;j++) result(i,j)=u(i)*v(j);
        return result;
    }
};

template<class T>
std::ostream& operator<<(std::ostream& output,const Matrix<T,3,2>& m)
{
    output<<m.x[0];
    for(int i=1;i<6;++i) output<<" "<<m.x[i];
    return output;
}

template<class T>
inline Matrix<T,3,2> operator*(const Matrix<T,3>& B,const Matrix<T,3,2>& A)
{
    return Matrix<T,3,2>(B.x[0]*A.x[0]+B.x[3]*A.x[1]+B.x[6]*A.x[2],B.x[1]*A.x[0]+B.x[4]*A.x[1]+B.x[7]*A.x[2],B.x[2]*A.x[0]+B.x[5]*A.x[1]+B.x[8]*A.x[2],
                         B.x[0]*A.x[3]+B.x[3]*A.x[4]+B.x[6]*A.x[5],B.x[1]*A.x[3]+B.x[4]*A.x[4]+B.x[7]*A.x[5],B.x[2]*A.x[3]+B.x[5]*A.x[4]+B.x[8]*A.x[5]);
}

template<class T>
inline Matrix<T,3,2> operator*(const T a,const Matrix<T,3,2>& A)
{return Matrix<T,3,2>(a*A.x[0],a*A.x[1],a*A.x[2],a*A.x[3],a*A.x[4],a*A.x[5]);}

template<class T>
inline Vector<T,3> operator*(const Vector<T,2>& v,const Matrix<T,3,2>& A)
{return Vector<T,3>({v[0]*A.x[0]+v[1]*A.x[3],v[0]*A.x[1]+v[1]*A.x[4],v[0]*A.x[2]+v[1]*A.x[5]});}

template<class T>
inline Matrix<T,3,2> operator*(const Symmetric_Matrix<T,3>& A,const Matrix<T,3,2>& B)
{
    return Matrix<T,3,2>(A.x11*B.x[0]+A.x21*B.x[1]+A.x31*B.x[2],A.x21*B.x[0]+A.x22*B.x[1]+A.x32*B.x[2],A.x31*B.x[0]+A.x32*B.x[1]+A.x33*B.x[2],
                         A.x11*B.x[3]+A.x21*B.x[4]+A.x31*B.x[5],A.x21*B.x[3]+A.x22*B.x[4]+A.x32*B.x[5],A.x31*B.x[3]+A.x32*B.x[4]+A.x33*B.x[5]);
}
}
#include <nova/Tools/Read_Write/Matrices/Read_Write_Matrix_3x2.h>
#endif
