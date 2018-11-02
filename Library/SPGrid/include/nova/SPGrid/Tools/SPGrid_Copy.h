//!#####################################################################
//! \file SPGrid_Copy.h
//!#####################################################################
// Class Copy
//######################################################################
#ifndef __Copy__
#define __Copy__

#include <nova/SPGrid/Core/SPGrid_Allocator.h>
#include <nova/SPGrid/Tools/SPGrid_Threading_Helper.h>

namespace SPGrid{
template<class Struct_type,class T,int d>
class Masked_Copy
{
    using Flags_type                = typename Struct_type::Flags_type;
    using Allocator_type            = SPGrid::SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask           = typename Allocator_type::template Array_mask<unsigned>;

  public:
    Masked_Copy(Allocator_type& allocator,const std::pair<const uint64_t*,unsigned>& blocks,T Struct_type::* source_channel,
                T Struct_type::* destination_channel,const unsigned mask)
    {Run(allocator,blocks,source_channel,destination_channel,mask);}

    void Run(Allocator_type& allocator,const std::pair<const uint64_t*,unsigned>& blocks,T Struct_type::* source_channel,
             T Struct_type::* destination_channel,const unsigned mask) const
    {
        auto flags=allocator.template Get_Const_Array<Struct_type,unsigned>(&Struct_type::flags);
        auto source=allocator.template Get_Const_Array<Struct_type,T>(source_channel);
        auto destination=allocator.template Get_Array<Struct_type,T>(destination_channel);

        auto masked_copy=[&](uint64_t offset)
        {
            for(int e=0;e<Flag_array_mask::elements_per_block;++e,offset+=sizeof(Flags_type))
                if(flags(offset)&mask) destination(offset)=source(offset);
        };

        SPGrid_Computations::Run_Parallel_Blocks(blocks,masked_copy);
    }
};
}
#endif
