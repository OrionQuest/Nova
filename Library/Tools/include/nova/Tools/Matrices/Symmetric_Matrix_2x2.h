//!#####################################################################
//! \file Symmetric_Matrix_2x2.h
//!#####################################################################
// Class Symmetric_Matrix_2x2
//######################################################################
#ifndef __Symmetric_Matrix_2x2__
#define __Symmetric_Matrix_2x2__

#include <nova/Tools/Matrices/Matrix_Forward.h>
#include <nova/Tools/Vectors/Vector.h>
#include <nova/Tools/Utilities/Utilities.h>
#include <cassert>

namespace Nova{
template<class T>
class Symmetric_Matrix<T,2>
{
    using TV                = Vector<T,2>;

  public:
    T x11,x21,x22;

    Symmetric_Matrix(const int mm=2,const int nn=2)
        :x11(0),x21(0),x22(0)
    {
        assert(mm==2 && nn==2);
    }

    template<class T2> explicit
    Symmetric_Matrix(const Symmetric_Matrix<T2,2>& matrix_input)
        :x11((T)matrix_input.x11),x21((T)matrix_input.x21),x22((T)matrix_input.x22)
    {}

    Symmetric_Matrix(const Diagonal_Matrix<T,2>& matrix_input)
        :x11(matrix_input.x11),x21(0),x22(matrix_input.x22)
    {}

    Symmetric_Matrix(const T y11,const T y21,const T y22)
        :x11(y11),x21(y21),x22(y22)
    {}

    int Rows() const
    {return 2;}

    int Columns() const
    {return 2;}

    TV Column(const int axis) const
    {
        assert(0<=axis && axis<=1);
        return axis==0?TV({x11,x21}):TV({x21,x22});
    }

    T& operator()(int i,int j)
    {return i<j?Element_Upper(i,j):Element_Lower(i,j);}

    const T& operator()(int i,int j) const
    {return i<j?Element_Upper(i,j):Element_Lower(i,j);}

    bool Valid_Index(const int i,const int j) const
    {return 0<=i && i<=1 && 0<=j && j<=1;}

    T& Element_Upper(int i,int j)
    {return Element_Lower(j,i);}

    const T& Element_Upper(int i,int j) const
    {return Element_Lower(j,i);}

    T& Element_Lower(int i,int j)
    {assert(i<=1 && j>=0 && j<=i);return ((T*)this)[((3-j)*j>>1)+i];}

    const T& Element_Lower(int i,int j) const
    {assert(i<=1 && j>=0 && j<=i);return ((T*)this)[((3-j)*j>>1)+i];}

    bool operator==(const Symmetric_Matrix& A) const
    {return x11==A.x11 && x21==A.x21 && x22==A.x22;}

    bool operator!=(const Symmetric_Matrix& A) const
    {return !(*this==A);}

    Symmetric_Matrix operator-() const
    {return Symmetric_Matrix(-x11,-x21,-x22);}

    Symmetric_Matrix& operator+=(const Symmetric_Matrix& A)
    {x11+=A.x11;x21+=A.x21;x22+=A.x22;return *this;}

    Symmetric_Matrix& operator+=(const T& a)
    {x11+=a;x22+=a;return *this;}

    Symmetric_Matrix& operator-=(const Symmetric_Matrix& A)
    {x11-=A.x11;x21-=A.x21;x22-=A.x22;return *this;}

    Symmetric_Matrix& operator-=(const T& a)
    {x11-=a;x22-=a;return *this;}

    Symmetric_Matrix& operator*=(const T a)
    {x11*=a;x21*=a;x22*=a;return *this;}

    Symmetric_Matrix& operator/=(const T a)
    {
        assert(a!=0);
        T s=(T)1./a;
        x11*=s;x21*=s;x22*=s;
        return *this;
    }

    Symmetric_Matrix operator+(const Symmetric_Matrix& A) const
    {return Symmetric_Matrix(x11+A.x11,x21+A.x21,x22+A.x22);}

    Symmetric_Matrix operator+(const T a) const
    {return Symmetric_Matrix(x11+a,x21,x22+a);}

    Symmetric_Matrix operator-(const Symmetric_Matrix& A) const
    {return Symmetric_Matrix(x11-A.x11,x21-A.x21,x22-A.x22);}

    Symmetric_Matrix operator-(const T a) const
    {return Symmetric_Matrix(x11-a,x21,x22-a);}

    Symmetric_Matrix operator*(const T a) const
    {return Symmetric_Matrix(a*x11,a*x21,a*x22);}

    Symmetric_Matrix operator/(const T a) const
    {assert(a!=0);return *this*((T)1./a);}

    TV operator*(const TV& v) const
    {return TV({x11*v[0]+x21*v[1],x21*v[0]+x22*v[1]});}

    T Determinant() const
    {return x11*x22-x21*x21;}

    Symmetric_Matrix Inverse() const
    {return Symmetric_Matrix(x22,-x21,x11)/Determinant();}

    Symmetric_Matrix Transposed() const
    {return *this;}

    void Transpose() {}

    Symmetric_Matrix Cofactor_Matrix()
    {return Symmetric_Matrix(x22,-x21,x11);}

    TV Largest_Column() const
    {return fabs(x11)>fabs(x22)?TV({x11,x21}):TV({x21,x22});}

    TV Largest_Column_Normalized() const
    {
        T sqr11=Nova_Utilities::Sqr(x11),sqr12=Nova_Utilities::Sqr(x21),sqr22=Nova_Utilities::Sqr(x22);
        T scale1=sqr11+sqr12,scale2=sqr12+sqr22;
        if(scale1>scale2) return TV({x11,x21})/std::sqrt(scale1);
        else if(scale2>0) return TV({x21,x22})/std::sqrt(scale2);
        return TV({1,0});
    }

    Symmetric_Matrix Positive_Definite_Part() const
    {
        Diagonal_Matrix<T,2> D;
        Matrix<T,2> V;
        Solve_Eigenproblem(D,V);
        D=D.Clamp_Min(0);
        return Conjugate(V,D);
    }

    void Solve_Eigenproblem(Diagonal_Matrix<T,2>& eigenvalues,Matrix<T,2>& eigenvectors) const
    {
        T a=(T).5*(x11+x22),b=(T).5*(x11-x22),c=x21;
        T c_squared=Nova_Utilities::Sqr(c),m=std::sqrt(Nova_Utilities::Sqr(b)+c_squared),k=x11*x22-c_squared;
        if(a>=0){eigenvalues(0)=a+m;eigenvalues(1)=eigenvalues(0)?k/eigenvalues(0):0;}
        else{eigenvalues(1)=a-m;eigenvalues(0)=eigenvalues(1)?k/eigenvalues(1):0;}
        Nova_Utilities::Exchange_Sort(eigenvalues(1),eigenvalues(0));
        TV column1=(b>=0?TV({m+b,c}):TV({-c,b-m})).Normalized();
        TV column2=column1.Orthogonal_Vector();
        eigenvectors=Matrix<T,2>(column1,column2);
    }

    Symmetric_Matrix Conjugate(const Matrix<T,2>& A,const Diagonal_Matrix<T,2>& B) const
    {
        Matrix<T,2> BA=B*A.Transposed();
        return Symmetric_Matrix<T,2>(A.x[0]*BA.x[0]+A.x[2]*BA.x[1],A.x[1]*BA.x[0]+A.x[3]*BA.x[1],A.x[1]*BA.x[2]+A.x[3]*BA.x[3]);
    }

    Symmetric_Matrix Conjugate(const Matrix<T,2>& A,const Symmetric_Matrix<T,2>& B) const
    {
        Matrix<T,2> BA=(A*B).Transposed();
        return Symmetric_Matrix<T,2>(A.x[0]*BA.x[0]+A.x[2]*BA.x[1],A.x[1]*BA.x[0]+A.x[3]*BA.x[1],A.x[1]*BA.x[2]+A.x[3]*BA.x[3]);
    }

    static Symmetric_Matrix Transpose_Times_With_Symmetric_Result(const Matrix<T,2>& A,const Matrix<T,2>& B)
    {return Symmetric_Matrix(A.x[0]*B.x[0]+A.x[1]*B.x[1],A.x[2]*B.x[0]+A.x[3]*B.x[1],A.x[2]*B.x[2]+A.x[3]*B.x[3]);}

    static Symmetric_Matrix Transpose_Times_With_Symmetric_Result(const Matrix<T,3,2>& A,const Matrix<T,3,2>& B)
    {return Symmetric_Matrix(A.x[0]*B.x[0]+A.x[1]*B.x[1]+A.x[2]*B.x[2],A.x[3]*B.x[0]+A.x[4]*B.x[1]+A.x[5]*B.x[2],A.x[3]*B.x[3]+A.x[4]*B.x[4]+A.x[5]*B.x[5]);}

    static Symmetric_Matrix Conjugate_With_Transpose(const Matrix<T,2>& A,const Diagonal_Matrix<T,2>& B)
    {return Transpose_Times_With_Symmetric_Result(B*A,A);}

    static Symmetric_Matrix Conjugate_With_Transpose(const Matrix<T,2>& A,const Symmetric_Matrix<T,2>& B)
    {return Transpose_Times_With_Symmetric_Result(B*A,A);}

    static Symmetric_Matrix Conjugate_With_Transpose(const Matrix<T,3,2>& A,const Symmetric_Matrix<T,3>& B)
    {return Transpose_Times_With_Symmetric_Result(B*A,A);}
};

template<class T>
std::ostream& operator<<(std::ostream& output,const Symmetric_Matrix<T,2>& A)
{
    output<<"["<<A.x11<<" "<<A.x21<<"; "<<A.x21<<" "<<A.x22<<"]";
    return output;
}
}
#include <nova/Tools/Read_Write/Matrices/Read_Write_Symmetric_Matrix_2x2.h>
#endif
