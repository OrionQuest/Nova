//!#####################################################################
//! \file SPGrid_Upsample_Inject_Shared.h
//!#####################################################################
// Class Upsample_Inject_Shared
//######################################################################
#ifndef __Upsample_Inject_Shared__
#define __Upsample_Inject_Shared__

#include <nova/SPGrid/Core/SPGrid_Allocator.h>
#include <nova/SPGrid/Tools/SPGrid_Threading_Helper.h>

namespace SPGrid{
template<class Struct_type,class T,int d>
class Upsample_Inject_Shared
{
    using Flags_type                = typename Struct_type::Flags_type;
    using Allocator_type            = SPGrid_Allocator<NextLogTwo<sizeof(Struct_type)>::value,d>;
    using Flag_array_mask           = typename Allocator_type::template Array_mask<unsigned>;

  public:
    Upsample_Inject_Shared(Allocator_type& allocator,const std::pair<const uint64_t*,unsigned>& blocks,
                           Allocator_type& coarse_allocator,T Struct_type::* data_channel,const unsigned mask)
    {Run(allocator,blocks,coarse_allocator,data_channel,mask);}

    void Run(Allocator_type& allocator,const std::pair<const uint64_t*,unsigned>& blocks,
             Allocator_type& coarse_allocator,T Struct_type::* data_channel,const unsigned mask) const
    {
        auto flags=allocator.template Get_Const_Array<Struct_type,unsigned>(&Struct_type::flags);
        auto data=allocator.template Get_Array<Struct_type,T>(data_channel);
        auto coarse_data=coarse_allocator.template Get_Const_Array<Struct_type,T>(data_channel);

        auto upsample_inject_shared=[&](uint64_t offset)
        {
            for(int e=0;e<Flag_array_mask::elements_per_block;++e,offset+=sizeof(Flags_type)) if(flags(offset)&mask){
                uint64_t coarse_offset=Flag_array_mask::DownsampleOffset(offset);
                data(offset)=coarse_data(coarse_offset);}
        };

        SPGrid_Computations::Run_Parallel_Blocks(blocks,upsample_inject_shared);
    }
};
}
#endif
