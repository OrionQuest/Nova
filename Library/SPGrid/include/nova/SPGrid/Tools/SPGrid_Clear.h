//!#####################################################################
//! \file SPGrid_Clear.h
//!#####################################################################
// Class Clear
//######################################################################
#ifndef __Clear__
#define __Clear__

#include <nova/SPGrid/Core/SPGrid_Allocator.h>
#include <nova/SPGrid/Tools/SPGrid_Threading_Helper.h>

namespace SPGrid{
template<class Struct_type,class T,int d>
class Clear
{
    using Flags_type            = typename Struct_type::Flags_type;
    using Allocator_type        = SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask       = typename Allocator_type::template Array_mask<unsigned>;

  public:
    Clear(Allocator_type& allocator,const std::pair<const uint64_t*,unsigned>& blocks,T Struct_type::* channel)
    {Run(allocator,blocks,channel);}

    void Run(Allocator_type& allocator,const std::pair<const uint64_t*,unsigned>& blocks,T Struct_type::* channel) const
    {
        auto data=allocator.template Get_Array<Struct_type,T>(channel);

        auto channel_cleaner=[&](uint64_t offset)
        {
            for(int e=0;e<Flag_array_mask::elements_per_block;++e,offset+=sizeof(Flags_type))
                data(offset)=(T)0.;
        };

        SPGrid_Computations::Run_Parallel_Blocks(blocks,channel_cleaner);
    }
};

template<class Struct_type,class T,int d>
class Masked_Clear
{
    using Flags_type            = typename Struct_type::Flags_type;
    using Allocator_type        = SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask       = typename Allocator_type::template Array_mask<unsigned>;

  public:
    Masked_Clear(Allocator_type& allocator,const std::pair<const uint64_t*,unsigned>& blocks,T Struct_type::* channel,const unsigned mask)
    {Run(allocator,blocks,channel,mask);}

    void Run(Allocator_type& allocator,const std::pair<const uint64_t*,unsigned>& blocks,T Struct_type::* channel,const unsigned mask) const
    {
        auto flags=allocator.template Get_Const_Array<Struct_type,unsigned>(&Struct_type::flags);
        auto data=allocator.template Get_Array<Struct_type,T>(channel);

        auto masked_clear=[&](uint64_t offset)
        {
            for(int e=0;e<Flag_array_mask::elements_per_block;++e,offset+=sizeof(Flags_type)) if(flags(offset)&mask) data(offset)=(T)0.;
        };

        SPGrid_Computations::Run_Parallel_Blocks(blocks,masked_clear);
    }
};
}
#endif
