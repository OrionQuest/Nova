//!#####################################################################
//! \file Identity_Array.h
//!#####################################################################
// Class Identity_Array
//######################################################################
#ifndef __Identity_Array__
#define __Identity_Array__

#include <cassert>
#include <cstddef>
#include <vector>

namespace Nova{
class Identity_Array
{
    std::vector<int> data;

  public:
    explicit Identity_Array(const size_t m_input)
        :data(m_input)
    {
        for(size_t i=0;i<data.size();++i) data[i]=i;
    }

    size_t size() const
    {return data.size();}

    int operator()(const int i) const
    {
        assert(i>=0 && i<data.size());
        return data[i];
    }

    int Pop()
    {
        int last_element=data.back();
        data.pop_back();
        return last_element;
    }

    void Append(const int& x)
    {data.push_back(x);}
};
}
#endif
