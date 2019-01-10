//!#####################################################################
//! \file SPGrid_Masked_Normalize.h
//!#####################################################################
// Class Masked_Normalize
//######################################################################
#ifndef __Masked_Normalize__
#define __Masked_Normalize__

#include <nova/SPGrid/Core/SPGrid_Allocator.h>
#include <nova/SPGrid/Tools/SPGrid_Threading_Helper.h>

namespace SPGrid{
template<class Struct_type,class T,int d>
class Masked_Normalize
{
    using Flags_type                = typename Struct_type::Flags_type;
    using Allocator_type            = SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask           = typename Allocator_type::template Array_mask<unsigned>;

  public:
    Masked_Normalize(Allocator_type& allocator,const std::pair<const uint64_t*,unsigned>& blocks,T Struct_type::* data_channel,
                     T Struct_type::* weight_channel,const unsigned mask)
    {Run(allocator,blocks,data_channel,weight_channel,mask);}

    void Run(Allocator_type& allocator,const std::pair<const uint64_t*,unsigned>& blocks,T Struct_type::* data_channel,
             T Struct_type::* weight_channel,const unsigned mask) const
    {
        auto flags=allocator.template Get_Const_Array<Struct_type,unsigned>(&Struct_type::flags);
        auto weights=allocator.template Get_Const_Array<Struct_type,T>(weight_channel);
        auto data=allocator.template Get_Array<Struct_type,T>(data_channel);

        auto masked_normalize=[&](uint64_t offset)
        {
            for(int e=0;e<Flag_array_mask::elements_per_block;++e,offset+=sizeof(Flags_type)) if(flags(offset)&mask) data(offset)/=weights(offset);
        };

        SPGrid_Computations::Run_Parallel_Blocks(blocks,masked_normalize);
    }
};
}
#endif
