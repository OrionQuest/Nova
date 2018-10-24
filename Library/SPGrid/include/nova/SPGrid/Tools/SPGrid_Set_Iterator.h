//!#####################################################################
//! \file SPGrid_Set_Iterator.h
//!#####################################################################
// Class SPGrid_Set_Iterator
//######################################################################
#ifndef __SPGrid_Set_Iterator__
#define __SPGrid_Set_Iterator__

#include <nova/SPGrid/Core/SPGrid_Set.h>

namespace SPGrid{
template<class T_ARRAY>
class SPGrid_Set_Iterator
{
    enum {d=T_ARRAY::dim};
    typedef typename T_ARRAY::DATA T;

    SPGrid_Set<T_ARRAY>& set;
  public:
    void* const data_ptr;
    const unsigned int elements_per_block;
    uint64_t page_map_index,bit,offset;
    unsigned element_index;

    SPGrid_Set_Iterator(SPGrid_Set<T_ARRAY>& set_input)
        :set(set_input),data_ptr(set.array.Get_Data_Ptr()),elements_per_block(set.array.geometry.Elements_Per_Block()),bit(0xffffffffffffffffUL),element_index(0)
    {
        for(offset=0,page_map_index=0;page_map_index<set.pagemap.map_size;page_map_index++,offset+=0x40000UL)
            if(set.pagemap.page_map[page_map_index]){
                for(bit=1UL;(set.pagemap.page_map[page_map_index]&bit)==0;bit<<=1,offset+=0x1000UL);
                break;}
    }

    inline bool Valid() const
    {return page_map_index<set.pagemap.map_size;}

    void Next()
    {
        element_index=(element_index+1)%elements_per_block;
        offset+=sizeof(T);

        if(!element_index){
            for(bit<<=1,offset+=0x1000UL-elements_per_block*sizeof(T);
                bit && (set.pagemap.page_map[page_map_index]&bit)==0;
                bit<<=1,offset+=0x1000UL);
            if(!bit)
                for(page_map_index++;page_map_index<set.pagemap.map_size;page_map_index++,offset+=0x40000UL)
                    if(set.pagemap.page_map[page_map_index]){
                        for(bit=1UL;(set.pagemap.page_map[page_map_index]&bit)==0;bit<<=1,offset+=0x1000UL);
                        break;}}
    }

    std::array<int,d> Index() const
    {return T_ARRAY::MASK::LinearToCoord(offset);}

    // Grab current data element
    T& Data()
    {return *reinterpret_cast<T*>(reinterpret_cast<unsigned long>(data_ptr) + offset);}

    const T& Data() const
    {return *reinterpret_cast<T*>(reinterpret_cast<unsigned long>(data_ptr) + offset);}

    // With offsets
    T& Data(const unsigned long offset_input)
    {return *reinterpret_cast<T*>(reinterpret_cast<unsigned long>(data_ptr) + T_ARRAY::MASK::Packed_Add(offset,offset_input));}

    const T& Data(const unsigned long offset_input) const
    {return *reinterpret_cast<T*>(reinterpret_cast<unsigned long>(data_ptr) + T_ARRAY::MASK::Packed_Add(offset,offset_input));}

    unsigned long Offset() const
    {return offset;}

    unsigned long Offset(const unsigned long offset_input) const
    {return T_ARRAY::MASK::Packed_Add(offset,offset_input);}
};
}
#endif
