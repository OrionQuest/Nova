//!#####################################################################
//! \file Matrix.h
//!#####################################################################
// Class Matrix
//######################################################################
#ifndef __Matrix__
#define __Matrix__

#include <nova/Tools/Log/Debug_Utilities.h>
#include <nova/Tools/Matrices/Diagonal_Matrix.h>
#include <nova/Tools/Vectors/Vector.h>

namespace Nova{
template<class T,int d1,int d2>
class Matrix
{
    using T_Storage             = Eigen::Matrix<T,d1,d2>;
    using SVD_Solver            = Eigen::JacobiSVD<Eigen::MatrixXf>;
    using Eigen_Solver          = Eigen::SelfAdjointEigenSolver<T_Storage>;

  public:
    T_Storage _data;

    Matrix()
    {_data<<T_Storage::Zero(d1,d2);}

    Matrix(std::initializer_list<Vector<T,d1>> l)
    {
        if(l.size()==d2) for(int i=0;i<d2;++i) _data.col(i)=*reinterpret_cast<const Eigen::Matrix<T,d1,1>*>(&l.begin()[i]);
        else throw std::out_of_range("Cannot construct Matrix from an initializer_list this size.");
    }

    Matrix(const Matrix& other)
        :_data(other._data)
    {}

    Matrix(const T_Storage& other)
        :_data(other)
    {}

    Matrix(const Vector<Vector<T,d1>,d2>& columns)
    {
        for(int i=0;i<d2;++i) _data.col(i)=*reinterpret_cast<const Eigen::Matrix<T,d1,1>*>(&columns(i));
    }

    Matrix& operator=(const Matrix& other)
    {
        _data=other._data;
        return *this;
    }

    int Rows() const
    {return d1;}

    int Columns() const
    {return d2;}

    Vector<T,d1> Column(const int j) const
    {
        assert(j>=0 && j<d2);
        return Vector<T,d1>(_data.col(j));
    }

    T& operator()(const int i,const int j)
    {
        assert(i>=0 && i<d1);
        assert(j>=0 && j<d2);
        return _data(i,j);
    }

    const T& operator()(const int i,const int j) const
    {
        assert(i>=0 && i<d1);
        assert(j>=0 && j<d2);
        return _data(i,j);
    }

    Matrix operator*(const T a) const
    {return Matrix(_data*a);}

    Matrix& operator*=(const T a)
    {return *this=*this*a;}

    template<int d3>
    Matrix<T,d1,d3> operator*(const Matrix<T,d2,d3>& m) const
    {return Matrix<T,d1,d3>(_data*m._data);}

    static Matrix<T,d1,d2> Outer_Product(const Vector<T,d1>& u,const Vector<T,d2>& v)
    {
        Matrix<T,d1,d2> ret;
        for(int i=0;i<d1;++i) for(int j=0;j<d2;++j) ret(i,j)=u(i)*v(j);
        return ret;
    }

    template<int d3>
    Matrix<T,d2,d3> Transpose_Times(const Matrix<T,d1,d3>& m) const
    {return Matrix<T,d2,d3>(_data.transpose()*m._data);}

    template<int d3>
    Matrix<T,d1,d3> Times_Transpose(const Matrix<T,d3,d2>& m) const
    {return Matrix<T,d1,d3>(_data*m._data.transpose());}

    Matrix& operator*=(const Matrix<T,d2,d2>& m)
    {return *this=*this*m;}

    Vector<T,d1> operator*(const Vector<T,d2>& v) const
    {
        Vector<T,d1> result;
        for(int i=0;i<d1;++i) for(int j=0;j<d2;++j) result(i)+=(*this)(i,j)*v(j);
        return result;
    }

    Matrix operator+(const Matrix& m) const
    {return Matrix(_data+m._data);}

    Matrix& operator+=(const Matrix& m)
    {return *this=*this+m;}

    Matrix operator-(const Matrix& m) const
    {return Matrix(_data-m._data);}

    Matrix& operator-=(const Matrix& m)
    {return *this=*this-m;}

    Matrix operator-(const T& a) const
    {
        static_assert(d1==d2,"Dimension mismatch");
        return Matrix(_data-T_Storage::Identity()*a);
    }

    Matrix& operator-=(const T& a)
    {
        static_assert(d1==d2,"Dimension mismatch");
        return *this=*this-a;
    }

    Matrix operator+(const T& a) const
    {
        static_assert(d1==d2,"Dimension mismatch");
        return Matrix(_data+T_Storage::Identity()*a);
    }

    Matrix& operator+=(const T& a)
    {
        static_assert(d1==d2,"Dimension mismatch");
        return *this=*this+a;
    }

    Matrix operator-() const
    {return Matrix(-_data);}

    Matrix operator*(const Diagonal_Matrix<T,d2>& other) const
    {
        Matrix result;
        for(int i=0;i<d1;++i) for(int j=0;j<d2;++j) result(i,j)=(*this)(i,j)*other._data(j);
        return result;
    }

    Matrix& operator*=(const Diagonal_Matrix<T,d2>& other)
    {return *this=*this*other;}

    Matrix Transposed() const
    {return Matrix(_data.transpose());}

    T Trace() const
    {
        static_assert(d1==d2,"Dimension mismatch");
        return _data.trace();
    }

    Matrix Inverse() const
    {
        static_assert(d1==d2,"Dimension mismatch");
        return Matrix(_data.inverse());
    }

    T Determinant() const
    {
        static_assert(d1==d2,"Dimension mismatch");
        return _data.determinant();
    }

    Matrix Positive_Definite_Part() const
    {
        static_assert(d1==d2,"Dimension mismatch");
        Eigen_Solver solver(_data);
        if(solver.info()!=Eigen::Success) FATAL_ERROR("Eigen decomposition failed!");
        Eigen::Matrix<T,d1,1> eigenvalues=solver.eigenvalues();
        for(int i=0;i<d1;++i) eigenvalues(i)=std::max(eigenvalues(i),(T)0.);
        auto D=eigenvalues.asDiagonal();T_Storage V=solver.eigenvectors();
        return Matrix(V*D*V.transpose());
    }

    void Singular_Value_Decomposition(Matrix& U,Diagonal_Matrix<T,d1>& Sigma,Matrix& V)
    {
        static_assert(d1==d2,"Dimension mismatch");
        SVD_Solver solver(_data,Eigen::ComputeThinU|Eigen::ComputeThinV);
        Sigma=Diagonal_Matrix<T,d1>(Vector<T,d1>(solver.singularValues()));
        U=Matrix(solver.matrixU());V=Matrix(solver.matrixV());
    }
};

template<class T,int d1,int d2>
Matrix<T,d1,d2> operator*(const T& a,const Matrix<T,d1,d2>& m)
{return Matrix<T,d1,d2>(m._data*a);}

template<class T,int d1,int d2>
std::ostream& operator<<(std::ostream& output,const Matrix<T,d1,d2>& m)
{output<<m._data;return output;}

template<class T>
Matrix<T,2,2> Cofactor_Matrix(const Matrix<T,2,2>& m)
{
    Matrix<T,2,2> ret;
    ret(0,0)= m(1,1);
    ret(1,0)=-m(0,1);
    ret(0,1)=-m(1,0);
    ret(1,1)= m(0,0);
    return ret;
}

template<class T>
Matrix<T,3,3> Cofactor_Matrix(const Matrix<T,3,3>& m)
{
    Matrix<T,3,3> ret;
    ret(0,0)=m(1,1)*m(2,2) - m(2,1)*m(1,2);
    ret(1,0)=m(2,1)*m(0,2) - m(0,1)*m(2,2);
    ret(2,0)=m(0,1)*m(1,2) - m(1,1)*m(0,2);
    ret(0,1)=m(2,0)*m(1,2) - m(1,0)*m(2,2);
    ret(1,1)=m(0,0)*m(2,2) - m(2,0)*m(0,2);
    ret(2,1)=m(1,0)*m(0,2) - m(0,0)*m(1,2);
    ret(0,2)=m(1,0)*m(2,1) - m(2,0)*m(1,1);
    ret(1,2)=m(2,0)*m(0,1) - m(0,0)*m(2,1);
    ret(2,2)=m(0,0)*m(1,1) - m(1,0)*m(0,1);
    return ret;
}
}
#include <nova/Tools/Read_Write/Matrices/Read_Write_Matrix.h>
#endif
