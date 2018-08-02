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
namespace Nova_Utilities
{
    /*! Swapping Function
     *
     * Swaps the two arguments in ascending order.
     * \param a First argument of type T.
     * \param b Second argument of type T.
     */
    template<class T>
    inline void Exchange_Sort(T& a,T& b)
    {if(a>b) std::swap(a,b);}

    /*! Swapping Function
     *
     * Swaps the three arguments in ascending order.
     * \param a First argument of type T.
     * \param b Second argument of type T.
     * \param c Third argument of type T.
     */
    template<class T>
    inline void Exchange_Sort(T& a,T& b,T& c)
    {
        if(a>b) std::swap(a,b);
        if(b>c) std::swap(b,c);
        if(a>b) std::swap(a,b);
    }

    /*! Squaring function
     *
     * Returns the square a given value.
     * \param a Argument of type T.
     */
    template<class T>
    inline T Sqr(const T a)
    {return a*a;}
}
}
#endif
