//!#####################################################################
//! \file MT19937.h
//!#####################################################################
// Class MT19937
//######################################################################
#ifndef __MT19937__
#define __MT19937__

namespace Nova{
template<class T>
class MT19937
{
  public:
    unsigned int mt[624];
    int index;
    static const int n=624,m=397;
    static const unsigned int a=0x9908b0df,UPPER_MASK=0x80000000,LOWER_MASK=0x7fffffff;

    MT19937()
    {Set_Seed();}

    explicit MT19937(const unsigned int value)
    {Set_Seed(value);}

    virtual ~MT19937() {}

//######################################################################
    void Set_Seed(const unsigned int value=5489);
    T operator()();         // in [0,1)
//######################################################################
};
}
#endif
