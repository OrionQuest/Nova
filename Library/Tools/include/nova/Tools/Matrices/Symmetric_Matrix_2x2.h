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
        eigenvectors.Column(0)=(b>=0?TV({m+b,c}):TV({-c,b-m})).Normalized();
        eigenvectors.Column(1)=eigenvectors.Column(0).Orthogonal_Vector();
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
};
}
#include <nova/Tools/Read_Write/Matrices/Read_Write_Symmetric_Matrix_2x2.h>
#endif
