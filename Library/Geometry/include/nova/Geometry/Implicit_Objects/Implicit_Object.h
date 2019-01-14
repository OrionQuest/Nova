//!#####################################################################
//! \file Implicit_Object.h
//!#####################################################################
// Class Implicit_Object
//######################################################################
#ifndef __Implicit_Object__
#define __Implicit_Object__

#include <nova/Tools/Vectors/Vector.h>
#include <limits>

namespace Nova{
template<class T,int d>
class Implicit_Object
{
    using TV            = Vector<T,d>;

  public:
    Implicit_Object() {}
    virtual ~Implicit_Object() {}

    bool Inside(const TV& X) const
    {return Signed_Distance(X)<=(T)0.;}

//######################################################################
    virtual T Signed_Distance(const TV& X) const
    {return std::numeric_limits<T>::max();}
//######################################################################
};
}
#endif
