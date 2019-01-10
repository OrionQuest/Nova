//!#####################################################################
//! \file SPGrid_Downsample_Accumulate_Shared.h
//!#####################################################################
// Class Downsample_Accumulate_Shared
//######################################################################
#ifndef __Downsample_Accumulate_Shared__
#define __Downsample_Accumulate_Shared__

#include <nova/SPGrid/Core/SPGrid_Allocator.h>

namespace SPGrid{
template<class Struct_type,class T,int d>
class Downsample_Accumulate_Shared
{
    using Flags_type                = typename Struct_type::Flags_type;
    using Allocator_type            = SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask           = typename Allocator_type::template Array_mask<unsigned>;

  public:
    Downsample_Accumulate_Shared(Allocator_type& allocator,const std::pair<const uint64_t*,unsigned>& blocks,Allocator_type& coarse_allocator,
                                 T Struct_type::* data_channel,T Struct_type::* weight_channel,const unsigned mask)
    {Run(allocator,blocks,coarse_allocator,data_channel,weight_channel,mask);}

    void Run(Allocator_type& allocator,const std::pair<const uint64_t*,unsigned>& blocks,Allocator_type& coarse_allocator,
             T Struct_type::* data_channel,T Struct_type::* weight_channel,const unsigned mask) const
    {
        auto flags=allocator.template Get_Const_Array<Struct_type,unsigned>(&Struct_type::flags);
        auto data=allocator.template Get_Const_Array<Struct_type,T>(data_channel);
        auto weights=allocator.template Get_Const_Array<Struct_type,T>(weight_channel);
        auto coarse_data=coarse_allocator.template Get_Array<Struct_type,T>(data_channel);
        auto coarse_weights=coarse_allocator.template Get_Array<Struct_type,T>(weight_channel);

        for(unsigned block=0;block<blocks.second;++block){uint64_t offset=blocks.first[block];
            for(int e=0;e<Flag_array_mask::elements_per_block;++e,offset+=sizeof(Flags_type)) if(flags(offset)&mask){
                uint64_t coarse_offset=Flag_array_mask::DownsampleOffset(offset);
                coarse_data(coarse_offset)+=data(offset);
                coarse_weights(coarse_offset)+=weights(offset);}}
    }
};
}
#endif
