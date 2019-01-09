//!#####################################################################
//! \file Analytic_Implicit_Object.h
//!#####################################################################
// Class Analytic_Implicit_Object
//######################################################################
#ifndef __Analytic_Implicit_Object__
#define __Analytic_Implicit_Object__

#include <nova/Tools/Vectors/Vector.h>
#include <limits>

namespace Nova{
template<class T,int d>
class Analytic_Implicit_Object
{
    using TV            = Vector<T,d>;

  public:
    Analytic_Implicit_Object() {}
    virtual ~Analytic_Implicit_Object() {}

    bool Inside(const TV& X) const
    {return Signed_Distance(X)<=(T)0.;}

//######################################################################
    virtual T Signed_Distance(const TV& X) const
    {return std::numeric_limits<T>::max();}
//######################################################################
};
}
#endif
