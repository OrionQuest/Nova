//!#####################################################################
//! \file Sparse_Matrix_Flat.h
//!#####################################################################
// Class Sparse_Matrix_Flat
//######################################################################
#ifndef __Sparse_Matrix_Flat__
#define __Sparse_Matrix_Flat__

#include <nova/Tools/Matrices/Diagonal_Matrix.h>
#include <nova/Tools/Matrices/Symmetric_Matrix_3x3.h>
#include <nova/Tools/Utilities/Utilities.h>
#include <nova/Tools/Vectors/Vector.h>
#include <nova/Tools/Arrays/Array.h>
#include <nova/Tools/Utilities/Pthread_Queue.h>
#include <nova/Tools/Utilities/Interval.h>

namespace Nova{

template<class T>
struct Sparse_Matrix_Flat_Entry
{
    size_t j;T a;
    Sparse_Matrix_Flat_Entry():j(0),a(0){}
    Sparse_Matrix_Flat_Entry(size_t index,T value):j(index),a(value){}
    bool operator<(const Sparse_Matrix_Flat_Entry& s) const {return j<s.j;}
};


template<class T,size_t d>
class Sparse_Matrix_Flat
{
    using Sparse_Matrix         = Sparse_Matrix_Flat<T,d>;
    using TV                    = Vector<T,d>;
  public:
    // d x d matrix
    Array<size_t> offsets;
    Array<Sparse_Matrix_Flat_Entry<T> > A;
    Array<size_t> diagonal_index;
    Sparse_Matrix* C;
    Pthread_Queue* thread_queue;

    Sparse_Matrix_Flat():C(0),thread_queue(0) {}

    Sparse_Matrix_Flat(const Sparse_Matrix_Flat& matrix):offsets(matrix.offsets),A(matrix.A),C(0),thread_queue(0) {}

    ~Sparse_Matrix_Flat() {delete C;}

    void Set_Element(const size_t i,const size_t j,const T element) 
    {(*this)(i,j)=element;}

    void Add_Element(const size_t i,const size_t j,const T element)
    {(*this)(i,j)+=element;}

    void Set_Symmetric_Elements(const size_t i,const size_t j,const T element)
    {assert(i!=j);Set_Element(i,j,element);Set_Element(j,i,element);}

    void Add_Symmetric_Elements(const size_t i,const size_t j,const T element)
    {assert(i!=j);Add_Element(i,j,element);Add_Element(j,i,element);}

    const T operator()(const size_t i,const size_t j) const
    {size_t index=Find_Index(i,j);assert(A(index).j==j);return A(index).a;}
    
    Sparse_Matrix& operator=(const Sparse_Matrix& matrix)
    {offsets=matrix.offsets;A=matrix.A;diagonal_index=matrix.diagonal_index;delete C;C=0; return *this;}
    
    void Set_Row_Lengths(const Array<size_t>& lengths)
    {
        diagonal_index.Clear();delete C;C=0;
        assert(d==lengths.size());
        offsets.resize(d+1);offsets(1)=1;
        for(size_t i=0;i<d;++i){assert(lengths(i));offsets(i+1)=offsets(i)+lengths(i);}
        A.resize(offsets(d+1)-1);
    }
    size_t Find_Index(const size_t i,const size_t j) const
    {
        assert(A.size());assert(0<=i && i<d);assert(0<=j && j<d);
        size_t index=offsets(i); while(A(index).j && A(index).j<j)index++;
        assert(index<offsets(i+1)); return index;
    }
    T& operator()(const size_t i,const size_t j)
    {
        size_t index=Find_Index(i,j);
        if(A(index).j!=j){ // need to add entry
            if(A(index).j){ // shift entries over to make room
                assert(!A(offsets(i+1)-1).j);
                for(size_t jj=offsets(i+1)-1;jj>index;jj--)A(jj)=A(jj-1);}
            A(index).j=j;A(index).a=0;}
        return A(index).a;
    }
    bool Element_Present(const size_t i,const size_t j) const
    {
        assert(1<=i && i<=d);assert(1<=j && j<=d);
        for(size_t index=offsets(i);index<offsets(i+1);index++)if(A(index).j==j) return true;
        return false;
    }
    void Initialize_Diagonal_Index()
    {
        diagonal_index.resize(d);
        for(size_t i=0;i<d;++i){diagonal_index(i)=Find_Index(i,i);assert(A(diagonal_index(i)).j==i);}
    }

    void Times(const size_t row_start,const size_t row_end,const TV& x,TV& result) const
    {
        size_t index=offsets(row_start);
        for(size_t i=row_start;i<=row_end;i++){
            size_t end=offsets(i+1);T sum=0;
            for(;index<end;index++)sum+=A(index).a*x(A(index).j);
            result(i)=sum;}
    }

    void Negate()
    {
        for(size_t index=0;index<A.size();++index) A(index).a=-A(index).a;
    }
    Sparse_Matrix& operator*=(const T a)
    {
        for(size_t index=0;index<A.size();++index) A(index).a*=a;
        return *this;
    }
    Sparse_Matrix& operator+=(const T a)
    {
        for(size_t i=0;i<d;++i) (*this)(i,i)+=a;
        return *this;
    }
    Sparse_Matrix& operator-=(const T a)
    {
        for(size_t i=0;i<d;++i) (*this)(i,i)-=a;
        return *this;
    }
    bool Symmetric(const T tolerance) const
    {
        for(size_t i=0;i<d;++i)for(size_t index=offsets(i);index<offsets(i+1);index++)
            if(abs(A(index).a-(*this)(A(index).j,i))>tolerance) return false;
        return true;
    }

    void Transpose(Sparse_Matrix& A_transpose) const
    {
        Array<size_t> row_lengths(d);for(size_t index=0;index<A.size();++index) row_lengths(A(index).j)++;
        A_transpose.Set_Row_Lengths(row_lengths);
        for(size_t i=0;i<d;++i) for(size_t index=offsets(i);index<offsets(i+1);index++) A_transpose(A(index).j,i)=A(index).a;
    }
    
    void Reset()
    {
        delete C; C=0; offsets.Clear(); A.Clear();
        offsets.Append(1); diagonal_index.Clear();
    }
    void Append_Entry_To_Current_Row(const size_t c,const T a)
    {
        A.Append(Sparse_Matrix_Flat_Entry<T>(c,a));
    }
};

template<class T,size_t d> inline std::ostream& operator<<(std::ostream& output_stream,const Sparse_Matrix_Flat<T,d>& A)
{for(size_t i=0;i<d;++i){
    for(size_t j=0;j<d;++j)output_stream<<(A.Element_Present(i,j)?A(i,j):0)<<" ";
    output_stream<<std::endl;}
return output_stream;}
template<class T,size_t d>
inline Sparse_Matrix_Flat<T,d> operator+(const T a,const Sparse_Matrix_Flat<T,d>& A)
{return A+a;}
template<class T,size_t d>
inline Sparse_Matrix_Flat<T,d> operator*(const T a,const Sparse_Matrix_Flat<T,d>& A)
{return A*a;}
}
#endif
