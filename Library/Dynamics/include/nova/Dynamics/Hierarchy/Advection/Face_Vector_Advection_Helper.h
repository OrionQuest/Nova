//!#####################################################################
//! \file Face_Vector_Advection_Helper.h
//!#####################################################################
// Class Face_Vector_Advection_Helper
//######################################################################
#ifndef __Face_Vector_Advection_Helper__
#define __Face_Vector_Advection_Helper__

#include <nova/Dynamics/Hierarchy/Advection/Grid_Hierarchy_Backtrace.h>
#include <nova/Dynamics/Hierarchy/Interpolation/Grid_Hierarchy_Interpolation.h>
#include <nova/SPGrid/Tools/SPGrid_Threading_Helper.h>
#include <nova/Tools/Utilities/Range_Iterator.h>

namespace Nova{
template<class Struct_type,class T,int d>
class Face_Vector_Advection_Helper
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
    enum {number_of_nodes_per_face  = Topology_Helper::number_of_nodes_per_face};

  public:
    Face_Vector_Advection_Helper(Hierarchy& hierarchy,const std::pair<const uint64_t*,unsigned>& blocks,Channel_Vector& face_vector_channels,Channel_Vector& node_vector_channels,Channel_Vector& face_velocity_channels,Channel_Vector& node_velocity_channels,
                                   T Struct_type::* result_channel,uint64_t* nodes_of_face_offsets,const Vector<uint64_t,d>& other_face_offsets,const TV& intra_cell_dX,const int level,
                                   const T dt,const unsigned mask,const int axis)
    {Run(hierarchy,blocks,face_vector_channels,node_vector_channels,face_velocity_channels,node_velocity_channels,result_channel,nodes_of_face_offsets,other_face_offsets,intra_cell_dX,level,dt,mask,axis);}

    void Run(Hierarchy& hierarchy,const std::pair<const uint64_t*,unsigned>& blocks,Channel_Vector& face_vector_channels,Channel_Vector& node_vector_channels,Channel_Vector& face_velocity_channels,Channel_Vector& node_velocity_channels,
             T Struct_type::* result_channel,uint64_t* nodes_of_face_offsets,const Vector<uint64_t,d>& other_face_offsets,const TV& intra_cell_dX,const int level,
             const T dt,const unsigned mask,const int axis) const
    {
        auto block_size=hierarchy.Allocator(level).Block_Size();
        auto flags=hierarchy.Allocator(level).template Get_Const_Array<Struct_type,unsigned>(&Struct_type::flags);
        auto result=hierarchy.Allocator(level).template Get_Array<Struct_type,T>(result_channel);
        const T one_over_nodes_per_face=(T)1./number_of_nodes_per_face;

        auto face_vector_advection_helper=[&](uint64_t offset)
        {
            Range_Iterator<d> range_iterator(T_INDEX(),*reinterpret_cast<T_INDEX*>(&block_size)-1);
            T_INDEX base_index(Flag_array_mask::LinearToCoord(offset));

            for(unsigned e=0;e<Flag_array_mask::elements_per_block;++e,offset+=sizeof(Flags_type)){const T_INDEX index=base_index+range_iterator.Index();
                if(flags(offset)&mask){TV velocity;
                    velocity(axis)=hierarchy.Channel(level,face_velocity_channels(axis))(offset);
                    for(int v=((axis+1)%d);v!=axis;v=(v+1)%d){
                        // average from 2^(d-1) nodes
                        for(int node=0;node<number_of_nodes_per_face;++node){uint64_t node_offset=Flag_array_mask::Packed_Add(offset,nodes_of_face_offsets[node]);
                            velocity(v)+=hierarchy.Channel(level,node_velocity_channels(v))(node_offset);}
                        velocity(v)*=one_over_nodes_per_face;}

                    // backtrace
                    TV dX=-velocity*dt;
                    // Log::cout<<"velocity: "<<velocity<<", dX: "<<dX<<std::endl;
                    uint64_t new_offset=offset;int new_level=level;
                    TV weights=Grid_Hierarchy_Backtrace<Struct_type,T,d>::Backtrace(hierarchy,new_level,index,new_offset,intra_cell_dX,dX);
                    uint64_t other_offset=Flag_array_mask::Packed_Add(new_offset,other_face_offsets[axis]);
                    T val1=Hierarchy_Interpolation::Face_Interpolation_Helper(hierarchy,nodes_of_face_offsets,axis,new_level,new_offset,weights.Remove_Index(axis),
                                                                              face_vector_channels,node_vector_channels);
                    T val2=Hierarchy_Interpolation::Face_Interpolation_Helper(hierarchy,nodes_of_face_offsets,axis,new_level,other_offset,weights.Remove_Index(axis),
                                                                              face_vector_channels,node_vector_channels);
                    result(offset)=Linear_Interpolation<T,T>::Linear(val1,val2,weights(axis));}
                range_iterator.Next();}
        };

        SPGrid_Computations::Run_Parallel_Blocks(blocks,face_vector_advection_helper);
    }
};
}
#endif
