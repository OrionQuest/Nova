//!#####################################################################
//! \file Grid_Hierarchy_Iterator.h
//!#####################################################################
// Class Grid_Hierarchy_Iterator
//######################################################################
#ifndef __Grid_Hierarchy_Iterator__
#define __Grid_Hierarchy_Iterator__

#include <nova/Tools/Utilities/Adaptive_Progress_Indicator.h>
#include <nova/Tools/Utilities/Range.h>
#include <nova/Tools/Utilities/Range_Iterator.h>
#include <stack>
#include <utility>

namespace Nova{
template<int d,class CELL_FUNCTOR>
class Grid_Hierarchy_Iterator
{
    using T_INDEX                   = Vector<int,d>;
    using T_RANGE                   = Range<int,d>;
    using T_CELL                    = std::pair<unsigned,T_INDEX>;

    std::stack<T_CELL> stack;
    Adaptive_Progress_Indicator indicator;
    CELL_FUNCTOR& functor;

  public:
    Grid_Hierarchy_Iterator(const T_RANGE& range,const unsigned level,CELL_FUNCTOR& functor_input)
        :indicator(Vector<unsigned long,d>(range.Edge_Lengths()+1).Product()<<(d*level),0x100000),functor(functor_input)
    {
        for(Range_Iterator<d> iterator(range);iterator.Valid();iterator.Next())
            stack.push(T_CELL(level,iterator.Index()));
    }

    bool Valid() const
    {return !stack.empty();}

    void Next()
    {
        T_CELL cell=stack.top();
        stack.pop();
        const unsigned level=cell.first;
        const T_INDEX& index=cell.second;

        bool recurse=functor.Consume(cell);
        if(!recurse){indicator.Progress(1UL<<(d*level));return;}

        for(Range_Iterator<d> iterator(T_RANGE::Unit_Box());iterator.Valid();iterator.Next())
            stack.push(T_CELL(level-1,index*2-iterator.Index()));
    }
};
}
#endif
