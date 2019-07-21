//!#####################################################################
//! \file Utilities.h
//!#####################################################################
// Class Utilities
//###################################################################### 
#ifndef __Utilities__
#define __Utilities__

#include "nova/Tools/Utilities/infix_iterator.h"
#include <cmath>
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

    /*! Minmag function
     *
     * Returns the value with minimum magnitude.
     * \param a Argument of type T.
     * \param b Argument of type T.
     */
    template<class T>
    inline T Minmag(const T a,const T b)
    {return fabs(a)<fabs(b)?a:b;}
    
    /*! Minmag function
     *
     * Returns the value with minimum magnitude.
     * \param a Argument of type T.
     * \param b Argument of type T.
     * \param c Argument of type T.
     */
    template<class T>
    inline T Minmag(const T a,const T b,const T c)
    {return Minmag(a,Minmag(b,c));}

    /*! Clamp function
     *
     * Returns the clamped value within specified bounds.
     * \param x Argument of type T.
     * \param xmin Argument of type T.
     * \param xmax Argument of type T.
     */
    template<class T> 
    inline T Clamp(const T x,const T xmin,const T xmax)
    {
        if(x<=xmin) return xmin;
        else if(x>=xmax) return xmax;
        return x;
    }

    /*! Squaring function
     *
     * Returns the square of a given value.
     * \param a Argument of type T.
     */
    template<class T>
    inline T Sqr(const T a)
    {return a*a;}

    /*! Cubing function
     *
     * Returns the cube of a given value.
     * \param a Argument of type T.
     */
    template<class T>
    inline T Cube(const T a)
    {return a*a*a;}

    /*! Sign function
     *
     * Returns the sign of a given value.
     * \param a Argument of type T.
     */
    template<class T>
    inline T Sign(const T a)
    {
        if(a>0) return (T)1;
        else if(a<0) return -(T)1;
        else return (T)0;
    }

    /*! Sinc function
     *
     * Returns sin(x)/x of a given value.
     * \param x Argument of type T.
     */
    template<class T>
    inline T Sinc(const T x)
    {
        if(fabs(x)<1e-8) return 1;
        return (T)std::sin(x)/x;
    }
}
}
#endif
