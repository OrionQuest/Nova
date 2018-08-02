//!#####################################################################
//! \file Diagonal_Matrix.h
//!#####################################################################
// Class Diagonal_Matrix
//######################################################################
#ifndef __Diagonal_Matrix__
#define __Diagonal_Matrix__

#include <nova/Tools/Vectors/Vector.h>

namespace Nova{

template<class T,int d1,int d2> class Matrix;

template<class T,int d>
class Diagonal_Matrix
{
    using T_Storage             = Vector<T,d>;

  public:
    T_Storage _data;

    Diagonal_Matrix() {}

    Diagonal_Matrix(std::initializer_list<T> l)
        :_data(l)
    {}

    Diagonal_Matrix(const Diagonal_Matrix& other)
        :_data(other._data)
    {}

    Diagonal_Matrix(const T_Storage& other)
        :_data(other)
    {}

    Diagonal_Matrix& operator=(const Diagonal_Matrix& other)
    {
        _data=other._data;
        return *this;
    }

    int Rows() const
    {return d;}

    int Columns() const
    {return d;}

    Diagonal_Matrix Clamp_Min(const T a) const
    {return Diagonal_Matrix(_data.Clamp_Min(a));}

    T Determinant() const
    {return _data.Product();}

    T operator()(const int i,const int j) const
    {
        assert(i>=0 && i<d);
        assert(j>=0 && j<d);
        if(i==j) return _data[i];
        else return (T)0.;
    }

    T& operator()(const int i)
    {
        assert(i>=0 && i<d);
        return _data[i];
    }

    const T& operator()(const int i) const
    {
        assert(i>=0 && i<d);
        return _data[i];
    }

    Diagonal_Matrix operator*(const T& a) const
    {return Diagonal_Matrix(_data*a);}

    Diagonal_Matrix& operator*=(const T& a)
    {return *this=*this*a;}

    Diagonal_Matrix operator+(const T& a) const
    {return Diagonal_Matrix(_data+a);}

    Diagonal_Matrix& operator+=(const T& a)
    {return *this=*this+a;}

    Diagonal_Matrix operator+(const Diagonal_Matrix& other) const
    {return Diagonal_Matrix(_data+other._data);}

    Diagonal_Matrix& operator+=(const Diagonal_Matrix& other)
    {return *this=*this+other;}

    template<int d1>
    Matrix<T,d,d1> operator*(const Matrix<T,d,d1>& other) const
    {
        Matrix<T,d,d1> result;
        for(int i=0;i<d;++i) for(int j=0;j<d1;++j) result(i,j)=_data(i)*other(i,j);
        return result;
    }

    Diagonal_Matrix Inverse() const
    {return Diagonal_Matrix((T)1./_data);}

    T Trace() const
    {return _data.Sum();}

    Diagonal_Matrix Logarithm() const
    {
        Diagonal_Matrix ret;
        for(int i=0;i<d;++i) ret(i)=log(_data(i));
        return ret;
    }

    T Frobenius_Norm_Squared() const
    {
        T value;
        for(int i=0;i<d;++i) value += (_data[i])*(_data[i]);
        return value;
    }
};

template<class T,int d>
Diagonal_Matrix<T,d> operator*(const T& a,const Diagonal_Matrix<T,d>& other)
{return Diagonal_Matrix<T,d>(other._data*a);}
}
#endif
