//!#####################################################################
//! \file Ghost_Vector_Value_Accumulate.h
//!#####################################################################
// Class Ghost_Vector_Value_Accumulate
//######################################################################
#ifndef __Ghost_Vector_Value_Accumulate__
#define __Ghost_Vector_Value_Accumulate__

#include <nova/Dynamics/Hierarchy/Grid_Hierarchy.h>
#include <nova/Dynamics/Utilities/SPGrid_Flags.h>
#include <nova/SPGrid/Tools/SPGrid_Threading_Helper.h>
#include <cassert>

namespace Nova{
template<class Struct_type,class T,int d>
class Ghost_Vector_Value_Accumulate
{
    using Flags_type                = typename Struct_type::Flags_type;
    using Hierarchy                 = Grid_Hierarchy<Struct_type,T,d>;
    using Channel_Vector            = Vector<T Struct_type::*,d>;
    using Allocator_type            = SPGrid::SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask           = typename Allocator_type::template Array_mask<unsigned>;

  public:
    Ghost_Vector_Value_Accumulate(Hierarchy& hierarchy,const std::pair<const uint64_t*,unsigned>& blocks,Channel_Vector& fine_data_channel,
                                  Channel_Vector& coarse_data_channel,const int coarse_level)
    {
        assert(coarse_level>=1 && coarse_level<hierarchy.Levels());
        Run(hierarchy,blocks,fine_data_channel,coarse_data_channel,coarse_level);
    }

    void Run(Grid_Hierarchy<Struct_type,T,2>& hierarchy,const std::pair<const uint64_t*,unsigned>& blocks,
             Channel_Vector& fine_data_channel,Channel_Vector& coarse_data_channel,const int coarse_level) const
    {
        auto coarse_flags=hierarchy.Allocator(coarse_level).template Get_Const_Array<Struct_type,unsigned>(&Struct_type::flags);

        auto ghost_vector_value_accumulate=[&](uint64_t offset)
        {
            for(int e=0;e<Flag_array_mask::elements_per_block;++e,offset+=sizeof(Flags_type)){
                uint64_t fine_offset=Flag_array_mask::UpsampleOffset(offset);

                if(coarse_flags(offset)&Ghost_Child_000){
                    uint64_t neighbor_offset=Flag_array_mask::template Packed_Offset<1,1>(fine_offset);
                    for(int axis=0;axis<d;++axis){
                        auto fine_data=hierarchy.Allocator(coarse_level-1).template Get_Const_Array<Struct_type,T>(fine_data_channel(axis));
                        auto coarse_data=hierarchy.Allocator(coarse_level).template Get_Array<Struct_type,T>(coarse_data_channel(axis));
                        coarse_data(offset)+=fine_data(neighbor_offset);}}

                if(coarse_flags(offset)&Ghost_Child_010){
                    uint64_t neighbor_offset=Flag_array_mask::template Packed_Offset<1,0>(fine_offset);
                    for(int axis=0;axis<d;++axis){
                        auto fine_data=hierarchy.Allocator(coarse_level-1).template Get_Const_Array<Struct_type,T>(fine_data_channel(axis));
                        auto coarse_data=hierarchy.Allocator(coarse_level).template Get_Array<Struct_type,T>(coarse_data_channel(axis));
                        coarse_data(offset)+=fine_data(neighbor_offset);}}

                if(coarse_flags(offset)&Ghost_Child_100){
                    uint64_t neighbor_offset=Flag_array_mask::template Packed_Offset<0,1>(fine_offset);
                    for(int axis=0;axis<d;++axis){
                        auto fine_data=hierarchy.Allocator(coarse_level-1).template Get_Const_Array<Struct_type,T>(fine_data_channel(axis));
                        auto coarse_data=hierarchy.Allocator(coarse_level).template Get_Array<Struct_type,T>(coarse_data_channel(axis));
                        coarse_data(offset)+=fine_data(neighbor_offset);}}

                if(coarse_flags(offset)&Ghost_Child_110){
                    uint64_t neighbor_offset=Flag_array_mask::template Packed_Offset<0,0>(fine_offset);
                    for(int axis=0;axis<d;++axis){
                        auto fine_data=hierarchy.Allocator(coarse_level-1).template Get_Const_Array<Struct_type,T>(fine_data_channel(axis));
                        auto coarse_data=hierarchy.Allocator(coarse_level).template Get_Array<Struct_type,T>(coarse_data_channel(axis));
                        coarse_data(offset)+=fine_data(neighbor_offset);}}}
        };

        SPGrid_Computations::Run_Parallel_Blocks(blocks,ghost_vector_value_accumulate);
    }

    void Run(Grid_Hierarchy<Struct_type,T,3>& hierarchy,const std::pair<const uint64_t*,unsigned>& blocks,
             Channel_Vector& fine_data_channel,Channel_Vector& coarse_data_channel,const int coarse_level) const
    {
        auto coarse_flags=hierarchy.Allocator(coarse_level).template Get_Const_Array<Struct_type,unsigned>(&Struct_type::flags);

        auto ghost_vector_value_accumulate=[&](uint64_t offset)
        {
            for(int e=0;e<Flag_array_mask::elements_per_block;++e,offset+=sizeof(Flags_type)){
                uint64_t fine_offset=Flag_array_mask::UpsampleOffset(offset);

                if(coarse_flags(offset)&Ghost_Child_000){
                    uint64_t neighbor_offset=Flag_array_mask::template Packed_Offset<1,1,1>(fine_offset);
                    for(int axis=0;axis<d;++axis){
                        auto fine_data=hierarchy.Allocator(coarse_level-1).template Get_Const_Array<Struct_type,T>(fine_data_channel(axis));
                        auto coarse_data=hierarchy.Allocator(coarse_level).template Get_Array<Struct_type,T>(coarse_data_channel(axis));
                        coarse_data(offset)+=fine_data(neighbor_offset);}}

                if(coarse_flags(offset)&Ghost_Child_001){
                    uint64_t neighbor_offset=Flag_array_mask::template Packed_Offset<1,1,0>(fine_offset);
                    for(int axis=0;axis<d;++axis){
                        auto fine_data=hierarchy.Allocator(coarse_level-1).template Get_Const_Array<Struct_type,T>(fine_data_channel(axis));
                        auto coarse_data=hierarchy.Allocator(coarse_level).template Get_Array<Struct_type,T>(coarse_data_channel(axis));
                        coarse_data(offset)+=fine_data(neighbor_offset);}}

                if(coarse_flags(offset)&Ghost_Child_010){
                    uint64_t neighbor_offset=Flag_array_mask::template Packed_Offset<1,0,1>(fine_offset);
                    for(int axis=0;axis<d;++axis){
                        auto fine_data=hierarchy.Allocator(coarse_level-1).template Get_Const_Array<Struct_type,T>(fine_data_channel(axis));
                        auto coarse_data=hierarchy.Allocator(coarse_level).template Get_Array<Struct_type,T>(coarse_data_channel(axis));
                        coarse_data(offset)+=fine_data(neighbor_offset);}}

                if(coarse_flags(offset)&Ghost_Child_011){
                    uint64_t neighbor_offset=Flag_array_mask::template Packed_Offset<1,0,0>(fine_offset);
                    for(int axis=0;axis<d;++axis){
                        auto fine_data=hierarchy.Allocator(coarse_level-1).template Get_Const_Array<Struct_type,T>(fine_data_channel(axis));
                        auto coarse_data=hierarchy.Allocator(coarse_level).template Get_Array<Struct_type,T>(coarse_data_channel(axis));
                        coarse_data(offset)+=fine_data(neighbor_offset);}}

                if(coarse_flags(offset)&Ghost_Child_100){
                    uint64_t neighbor_offset=Flag_array_mask::template Packed_Offset<0,1,1>(fine_offset);
                    for(int axis=0;axis<d;++axis){
                        auto fine_data=hierarchy.Allocator(coarse_level-1).template Get_Const_Array<Struct_type,T>(fine_data_channel(axis));
                        auto coarse_data=hierarchy.Allocator(coarse_level).template Get_Array<Struct_type,T>(coarse_data_channel(axis));
                        coarse_data(offset)+=fine_data(neighbor_offset);}}

                if(coarse_flags(offset)&Ghost_Child_101){
                    uint64_t neighbor_offset=Flag_array_mask::template Packed_Offset<0,1,0>(fine_offset);
                    for(int axis=0;axis<d;++axis){
                        auto fine_data=hierarchy.Allocator(coarse_level-1).template Get_Const_Array<Struct_type,T>(fine_data_channel(axis));
                        auto coarse_data=hierarchy.Allocator(coarse_level).template Get_Array<Struct_type,T>(coarse_data_channel(axis));
                        coarse_data(offset)+=fine_data(neighbor_offset);}}

                if(coarse_flags(offset)&Ghost_Child_110){
                    uint64_t neighbor_offset=Flag_array_mask::template Packed_Offset<0,0,1>(fine_offset);
                    for(int axis=0;axis<d;++axis){
                        auto fine_data=hierarchy.Allocator(coarse_level-1).template Get_Const_Array<Struct_type,T>(fine_data_channel(axis));
                        auto coarse_data=hierarchy.Allocator(coarse_level).template Get_Array<Struct_type,T>(coarse_data_channel(axis));
                        coarse_data(offset)+=fine_data(neighbor_offset);}}

                if(coarse_flags(offset)&Ghost_Child_111){
                    uint64_t neighbor_offset=Flag_array_mask::template Packed_Offset<0,0,0>(fine_offset);
                    for(int axis=0;axis<d;++axis){
                        auto fine_data=hierarchy.Allocator(coarse_level-1).template Get_Const_Array<Struct_type,T>(fine_data_channel(axis));
                        auto coarse_data=hierarchy.Allocator(coarse_level).template Get_Array<Struct_type,T>(coarse_data_channel(axis));
                        coarse_data(offset)+=fine_data(neighbor_offset);}}}
        };

        SPGrid_Computations::Run_Parallel_Blocks(blocks,ghost_vector_value_accumulate);
    }
};
}
#endif
