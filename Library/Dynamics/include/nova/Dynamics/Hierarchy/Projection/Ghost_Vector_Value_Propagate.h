//!#####################################################################
//! \file Ghost_Vector_Value_Propagate.h
//!#####################################################################
// Class Ghost_Vector_Value_Propagate
//######################################################################
#ifndef __Ghost_Vector_Value_Propagate__
#define __Ghost_Vector_Value_Propagate__

#include <nova/Dynamics/Hierarchy/Grid_Hierarchy.h>
#include <nova/Dynamics/Utilities/SPGrid_Flags.h>
#include <nova/SPGrid/Tools/SPGrid_Threading_Helper.h>
#include <cassert>

namespace Nova{
template<class Struct_type,class T,int d>
class Ghost_Vector_Value_Propagate
{
    using Flags_type                = typename Struct_type::Flags_type;
    using Hierarchy                 = Grid_Hierarchy<Struct_type,T,d>;
    using Channel_Vector            = Vector<T Struct_type::*,d>;
    using Allocator_type            = SPGrid::SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask           = typename Allocator_type::template Array_mask<unsigned>;

  public:
    Ghost_Vector_Value_Propagate(Hierarchy& hierarchy,const std::pair<const uint64_t*,unsigned>& blocks,Channel_Vector& fine_data_channel,
                                 Channel_Vector& coarse_data_channel,const int fine_level)
    {
        assert(fine_level>=0 && fine_level<hierarchy.Levels()-1);
        Run(hierarchy,blocks,fine_data_channel,coarse_data_channel,fine_level);
    }

    void Run(Hierarchy& hierarchy,const std::pair<const uint64_t*,unsigned>& blocks,Channel_Vector& fine_data_channel,
             Channel_Vector& coarse_data_channel,const int fine_level) const
    {
        auto fine_flags=hierarchy.Allocator(fine_level).template Get_Const_Array<Struct_type,unsigned>(&Struct_type::flags);

        auto ghost_vector_value_propagate=[&](uint64_t offset)
        {
            for(int e=0;e<Flag_array_mask::elements_per_block;++e,offset+=sizeof(Flags_type)) if(fine_flags(offset)&Cell_Type_Ghost){
                uint64_t coarse_offset=Flag_array_mask::DownsampleOffset(offset);
                for(int axis=0;axis<d;++axis){
                    auto fine_data=hierarchy.Allocator(fine_level).template Get_Array<Struct_type,T>(fine_data_channel(axis));
                    auto coarse_data=hierarchy.Allocator(fine_level+1).template Get_Const_Array<Struct_type,T>(coarse_data_channel(axis));
                    fine_data(offset)=coarse_data(coarse_offset);}}
        };

        SPGrid_Computations::Run_Parallel_Blocks(blocks,ghost_vector_value_propagate);
    }
};
}
#endif
