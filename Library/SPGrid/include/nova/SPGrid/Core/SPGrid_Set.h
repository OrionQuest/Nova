//!#####################################################################
//! \file SPGrid_Set.h
//!#####################################################################
// Class SPGrid_Set
//######################################################################
#ifndef __SPGrid_Set__
#define __SPGrid_Set__

#include <nova/SPGrid/Core/SPGrid_Page_Map.h>

namespace SPGrid{
template<class T_ARRAY>
class SPGrid_Set
{
  public:
    enum {d=T_ARRAY::dim};
    typedef typename T_ARRAY::DATA T;

    SPGrid_Page_Map& pagemap;
    T_ARRAY array;
    uint64_t max_linear_offset;

    SPGrid_Set(SPGrid_Page_Map& pagemap_input,T_ARRAY array_input)
        :pagemap(pagemap_input),array(array_input)
    {max_linear_offset=array.geometry.Padded_Volume()*T_ARRAY::MASK::Bytes_Per_Element();}

    inline bool Check_Bounds(unsigned long linear_offset)
    {return (linear_offset<max_linear_offset);}

    void Mask(const uint64_t linear_offset,const T mask)
    {
        static_assert(std::is_integral<T>::value,"Masking allowed only with integral types in SPGrid_Set");
        pagemap.Set_Page(linear_offset);
        array(linear_offset) |= mask;
    }

    // Note: the following are thread-safe only at coordinate granularity.
    // Different threads can safely process entries of the same page, but cannot safely mask different bits of the same entry
    void Mask(const std::array<ucoord_t,d>& coord,const T mask)
    {
        static_assert(std::is_integral<T>::value,"Masking allowed only with integral types in SPGrid_Set");
        auto linear_offset=T_ARRAY::MASK::Linear_Offset(coord);
        Mask(linear_offset,mask);
    }

    bool Is_Set(const uint64_t linear_offset,const T mask) const
    {
        if(linear_offset<max_linear_offset && pagemap.Test_Page(linear_offset))
        {
            T* data=reinterpret_cast<T*>(reinterpret_cast<uint64_t>(array.Get_Data_Ptr())+linear_offset);
            return (*data & mask);
        }
        return false;
    }

    bool Is_Set(const std::array<ucoord_t,d>& coord,const T mask) const
    {
        auto linear_offset=T_ARRAY::MASK::Linear_Offset(coord);
        return Is_Set(linear_offset,mask);
    }
//######################################################################
};
}
#endif
