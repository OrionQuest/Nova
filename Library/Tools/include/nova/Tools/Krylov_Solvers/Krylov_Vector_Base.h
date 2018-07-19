//!#####################################################################
//! \file Krylov_Vector_Base.h
//!#####################################################################
// Class Krylov_Vector_Base
//######################################################################
#ifndef __Krylov_Vector_Base__
#define __Krylov_Vector_Base__

#include <iostream>

namespace Nova{
template<class T>
class Krylov_Vector_Base
{
  public:
    Krylov_Vector_Base() {}

    virtual ~Krylov_Vector_Base() {}

    const Krylov_Vector_Base& operator=(const Krylov_Vector_Base& bv)
    {Copy((T)1,bv);return *this;}

    const T& Raw_Get(int i) const
    {return const_cast<Krylov_Vector_Base<T>*>(this)->Raw_Get(i);}

//###################################################################### 
    virtual Krylov_Vector_Base& operator+=(const Krylov_Vector_Base& bv)=0;
    virtual Krylov_Vector_Base& operator-=(const Krylov_Vector_Base& bv)=0;
    virtual Krylov_Vector_Base& operator*=(const T a)=0;
    virtual void Copy(const T c,const Krylov_Vector_Base& bv)=0;
    virtual void Copy(const T c1,const Krylov_Vector_Base& bv1,const Krylov_Vector_Base& bv2)=0;
    virtual size_t Raw_Size() const=0;
    virtual T& Raw_Get(int i)=0;
//###################################################################### 
};

template<class T> std::ostream&
operator<<(std::ostream& output,const Krylov_Vector_Base<T>& bv)
{
    for(int j=0;j<bv.Raw_Size();++j) output<<bv.Raw_Get(j)<<" ";
    return output;
}
}
#endif
