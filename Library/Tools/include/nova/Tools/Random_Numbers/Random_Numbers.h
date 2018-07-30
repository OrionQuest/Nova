//!#####################################################################
//! \file Random_Numbers.h
//!#####################################################################
// Class Random_Numbers
//######################################################################
#ifndef __Random_Numbers__
#define __Random_Numbers__

#include <nova/Tools/Random_Numbers/MT19937.h>
#include <nova/Tools/Utilities/Non_Copyable.h>
#include <nova/Tools/Utilities/Range.h>
#include <nova/Tools/Vectors/Vector.h>
#include <algorithm>
#include <ctime>

namespace Nova{
template<class T,class Generator=MT19937<T> >
class Random_Numbers: public Non_Copyable
{
  public:
    Generator random_number_generator;

    explicit Random_Numbers(const unsigned int seed=time(0))
    {Set_Seed(seed);}

    virtual ~Random_Numbers() {}

    T Get_Number()
    {return random_number_generator();}

    void Set_Seed(const unsigned int seed_input)
    {random_number_generator.Set_Seed(seed_input);}

    T Get_Uniform_Number(const T a,const T b)
    {return a+(b-a)*Get_Number();}                                  // in [a,b)

    int Get_Uniform_Integer(const int a,const int b)
    {return std::min(b,(int)(a+(b+1-a)*Get_Number()));}             // in [a,b]

    template<int d> Vector<T,d> Get_Uniform_Vector(const Vector<T,d>& v0,const Vector<T,d>& v1)
    {
        Vector<T,d> r;
        for(int i=0;i<d;++i) r[i]=Get_Uniform_Number(v0[i],v1[i]);
        return r;
    }

    template<int d> Vector<T,d> Get_Uniform_Vector(const Range<T,d>& box)
    {return Get_Uniform_Vector(box.min_corner,box.max_corner);}
};
}
#endif
