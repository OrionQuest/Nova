//!#####################################################################
//! \file Density_Advection_Helper.h
//!#####################################################################
// Class Density_Advection_Helper
//######################################################################
#ifndef __Density_Advection_Helper__
#define __Density_Advection_Helper__

#include <nova/Dynamics/Hierarchy/Advection/Grid_Hierarchy_Backtrace.h>
#include <nova/Dynamics/Hierarchy/Interpolation/Grid_Hierarchy_Interpolation.h>
#include <nova/SPGrid/Tools/SPGrid_Threading_Helper.h>
#include <nova/Tools/Utilities/Range_Iterator.h>

namespace Nova{
template<class Struct_type,class T,int d>
class Density_Advection_Helper
{
    using TV                        = Vector<T,d>;
    using T_INDEX                   = Vector<int,d>;
    using Flags_type                = typename Struct_type::Flags_type;
    using Channel_Vector            = Vector<T Struct_type::*,d>;
    using Hierarchy                 = Grid_Hierarchy<Struct_type,T,d>;
    using Allocator_type            = SPGrid::SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask           = typename Allocator_type::template Array_mask<unsigned>;
    using Topology_Helper           = Grid_Topology_Helper<Flag_array_mask>;
    using Hierarchy_Interpolation   = Grid_Hierarchy_Interpolation<Struct_type,T,d>;

    enum {number_of_nodes_per_cell  = Topology_Helper::number_of_nodes_per_cell};

  public:
    Density_Advection_Helper(Hierarchy& hierarchy,const std::pair<const uint64_t*,unsigned>& blocks,T Struct_type::* cell_channel,T Struct_type::* node_channel,
                             T Struct_type::* result_channel,Channel_Vector& node_velocity_channels,uint64_t* nodes_of_cell_offsets,const TV& intra_cell_dX,const int level,
                             const T dt,const unsigned mask)
    {Run(hierarchy,blocks,cell_channel,node_channel,result_channel,node_velocity_channels,nodes_of_cell_offsets,intra_cell_dX,level,dt,mask);}

    void Run(Hierarchy& hierarchy,const std::pair<const uint64_t*,unsigned>& blocks,T Struct_type::* cell_channel,T Struct_type::* node_channel,T Struct_type::* result_channel,
             Channel_Vector& node_velocity_channels,uint64_t* nodes_of_cell_offsets,const TV& intra_cell_dX,const int level,const T dt,const unsigned mask) const
    {
        auto block_size=hierarchy.Allocator(level).Block_Size();
        auto flags=hierarchy.Allocator(level).template Get_Const_Array<Struct_type,unsigned>(&Struct_type::flags);
        auto result=hierarchy.Allocator(level).template Get_Array<Struct_type,T>(result_channel);

        auto density_advection_helper=[&](uint64_t offset)
        {
            Range_Iterator<d> range_iterator(T_INDEX(),*reinterpret_cast<T_INDEX*>(&block_size)-1);
            T_INDEX base_index(Flag_array_mask::LinearToCoord(offset));

            for(unsigned e=0;e<Flag_array_mask::elements_per_block;++e,offset+=sizeof(Flags_type)){const T_INDEX index=base_index+range_iterator.Index();
                if(flags(offset)&mask){TV velocity;
                    for(int node=0;node<number_of_nodes_per_cell;++node){uint64_t node_offset=Flag_array_mask::Packed_Add(offset,nodes_of_cell_offsets[node]);
                        for(int axis=0;axis<d;++axis) velocity(axis)+=hierarchy.Channel(level,node_velocity_channels(axis))(node_offset);}
                    velocity/=(T)number_of_nodes_per_cell;

                    // backtrace
                    TV dX=-velocity*dt;
                    uint64_t new_offset=offset;int new_level=level;
                    TV weights=Grid_Hierarchy_Backtrace<Struct_type,T,d>::Backtrace(hierarchy,new_level,index,new_offset,intra_cell_dX,dX);
                    
                    T value=Hierarchy_Interpolation::Cell_Interpolation_Helper(hierarchy,nodes_of_cell_offsets,new_level,new_offset,weights,cell_channel,node_channel);
                    // if(value>(T)0.) Log::cout<<"velocity: "<<velocity<<", weights: "<<weights<<", value: "<<value<<std::endl;
                    result(offset)=value;}
                range_iterator.Next();}
        };

        SPGrid_Computations::Run_Parallel_Blocks(blocks,density_advection_helper);
    }
};
}
#endif
