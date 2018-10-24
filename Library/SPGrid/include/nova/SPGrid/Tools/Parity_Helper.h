//!#####################################################################
//! \file Parity_Helper.h
//!#####################################################################
// Class Parity_Helper
//######################################################################
#ifndef __Parity_Helper__
#define __Parity_Helper__

namespace SPGrid{
template<class MASK,int d> class Parity_Helper;

template<class MASK>
class Parity_Helper<MASK,2>
{
  public:
    enum{
        GHOST_000 = MASK::template LinearOffset<0,0>::value,
        GHOST_010 = MASK::template LinearOffset<0,1>::value,
        GHOST_100 = MASK::template LinearOffset<1,0>::value,
        GHOST_110 = MASK::template LinearOffset<1,1>::value,
        GHOST_001 = MASK::template LinearOffset<1,1>::value,
        GHOST_011 = MASK::template LinearOffset<1,1>::value,
        GHOST_101 = MASK::template LinearOffset<1,1>::value,
        GHOST_111 = MASK::template LinearOffset<1,1>::value
    };
};

template<class MASK>
class Parity_Helper<MASK,3>
{
  public:
    enum{
        GHOST_000 = MASK::template LinearOffset<0,0,0>::value,
        GHOST_001 = MASK::template LinearOffset<0,0,1>::value,
        GHOST_010 = MASK::template LinearOffset<0,1,0>::value,
        GHOST_011 = MASK::template LinearOffset<0,1,1>::value,
        GHOST_100 = MASK::template LinearOffset<1,0,0>::value,
        GHOST_101 = MASK::template LinearOffset<1,0,1>::value,
        GHOST_110 = MASK::template LinearOffset<1,1,0>::value,
        GHOST_111 = MASK::template LinearOffset<1,1,1>::value
    };
};
}
#endif
