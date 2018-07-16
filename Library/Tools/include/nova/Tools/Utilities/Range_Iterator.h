//!#####################################################################
//! \file Range_Iterator.h
//!#####################################################################
// Class Range_Iterator
//######################################################################
#ifndef __Range_Iterator__
#define __Range_Iterator__

#include <nova/Tools/Vectors/Vector.h>
#include <type_traits>
#include <ostream>
#include <iterator>

namespace Nova{
template<int d,class TV_INT=Vector<int,d> > 
class Range_Iterator
{
    const TV_INT min_corner;
    const TV_INT max_corner;
    const TV_INT direction;
    TV_INT index;
    
  public:
    Range_Iterator()
        :min_corner(TV_INT()),max_corner(TV_INT()),direction(TV_INT(1))
    {Reset();}

    template< class RANGE_TYPE>
    Range_Iterator(const RANGE_TYPE& range)
        :min_corner(range.min_corner),max_corner(range.max_corner),direction(initializeDirection(range.min_corner,range.max_corner))
    {Reset();}
    
    Range_Iterator(const TV_INT& min_corner_input,const TV_INT& max_corner_input)
        :min_corner(min_corner_input),max_corner(max_corner_input),direction(initializeDirection(min_corner,max_corner))
    {Reset();}
    
    ~Range_Iterator() {}
    
    static TV_INT initializeDirection(const TV_INT& min_corner,const TV_INT& max_corner)
    {
        TV_INT direction;
        for(int i=0;i<d;i++) direction[i]=min_corner[i]<max_corner[i]?1:-1;
        return direction;
    }

    Range_Iterator operator++()
    {Next();if(!Valid()) SetEnd();return *this;}

    const TV_INT& operator*()
    {return Index();}

    bool operator!=(const Range_Iterator& other)
    {return !(min_corner==other.min_corner && max_corner==other.max_corner && index==other.index);}

    void SetEnd()
    {index=max_corner;index[0]+=direction[0];}

    void Reset()
    {index=min_corner;}

    bool Valid() const
    {return (direction[0]*index[0])<=(direction[0]*max_corner[0]);}

    void Next()
    {index=Upcoming();}

    TV_INT Upcoming() const
    {
        TV_INT index_tmp = index;
        for(int i=d-1;i>=0;i--) if((direction[i]*index_tmp[i])<(direction[i]*max_corner[i]) || i==0){index_tmp[i]+=direction[i];return index_tmp;} else index_tmp[i]=min_corner[i];
        return index_tmp;
    }

    const TV_INT& Index() const
    {return index;}
};

template<> 
class Range_Iterator<1,int>
{
    const int min_corner;
    const int max_corner;
    const int direction;
    int index;
    
  public:

    Range_Iterator()
        :min_corner(0),max_corner(0),direction(1)
    {Reset();}

    template<class RANGE_TYPE>
    Range_Iterator(const RANGE_TYPE& range)
        :min_corner(range.min_corner),max_corner(range.max_corner),direction(initializeDirection(range.min_corner,range.max_corner))
    {Reset();}
    
    Range_Iterator(const int& min_corner_input,const int& max_corner_input)
        :min_corner(min_corner_input),max_corner(max_corner_input),direction(initializeDirection(min_corner,max_corner))
    {Reset();}
    
    ~Range_Iterator() {}
    
    static int initializeDirection(const int& min_corner,const int& max_corner)
    {
        int direction=min_corner<max_corner?1:-1;
        return direction;
    }

    Range_Iterator operator++()
    {Next();if(!Valid()) SetEnd();return *this;}

    const int& operator*()
    {return Index();}

    bool operator!=(const Range_Iterator& other)
    {return !(min_corner==other.min_corner && max_corner==other.max_corner && index==other.index);}

    void SetEnd()
    {index=max_corner;index+=direction;}

    void Reset()
    {index=min_corner;}

    bool Valid() const
    {return (direction*index)<=(direction*max_corner);}

    void Next()
    {index=Upcoming();}

    int Upcoming() const
    {
        int index_tmp=index;
        index_tmp+=direction;
        return index_tmp;
    }

    const int& Index() const
    {return index;}
};

template<int d, class TV_INT>
std::ostream& operator<<(std::ostream& out,const Range_Iterator<d,TV_INT>& iter)
{
    out<<"Index: "<<iter.Index()<<", Next: "<<iter.Upcoming()<<", Valid: "<<iter.Valid();
    return out;
}
}
#endif
