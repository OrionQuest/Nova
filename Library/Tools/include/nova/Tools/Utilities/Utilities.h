//!#####################################################################
//! \file Utilities.h
//!#####################################################################
// Class Utilities
//###################################################################### 
#ifndef __Utilities__
#define __Utilities__

#include "nova/Tools/Utilities/infix_iterator.h"
#include <utility>

namespace Nova{
namespace Nova_Utilities{
template<class T>
inline void Exchange_Sort(T& a,T& b)
{if(a>b) std::swap(a,b);}

template<class T>
inline void Exchange_Sort(T& a,T& b,T& c)
{
    if(a>b) std::swap(a,b);
    if(b>c) std::swap(b,c);
    if(a>b) std::swap(a,b);
}
}
}
#endif
