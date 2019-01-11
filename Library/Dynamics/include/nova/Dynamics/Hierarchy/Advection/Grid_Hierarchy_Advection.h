//!#####################################################################
//! \file Grid_Hierarchy_Advection.h
//!#####################################################################
// Class Grid_Hierarchy_Advection
//######################################################################
#ifndef __Grid_Hierarchy_Advection__
#define __Grid_Hierarchy_Advection__

#include <nova/Dynamics/Hierarchy/Advection/Density_Advection_Helper.h>
#include <nova/Dynamics/Hierarchy/Advection/Face_Velocity_Advection_Helper.h>
#include <nova/Dynamics/Utilities/SPGrid_Flags.h>
#include <nova/SPGrid/Tools/SPGrid_Clear.h>
#include <nova/SPGrid/Tools/SPGrid_Copy.h>

namespace Nova{
template<class Struct_type,class T,int d>
class Grid_Hierarchy_Advection
{
    using TV                        = Vector<T,d>;
    using Flags_type                = typename Struct_type::Flags_type;
    using Channel_Vector            = Vector<T Struct_type::*,d>;
    using Hierarchy                 = Grid_Hierarchy<Struct_type,T,d>;
    using Allocator_type            = SPGrid::SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask           = typename Allocator_type::template Array_mask<unsigned>;
    using Topology_Helper           = Grid_Topology_Helper<Flag_array_mask>;

    enum {number_of_nodes_per_cell  = Topology_Helper::number_of_nodes_per_cell};
    enum {number_of_nodes_per_face  = Topology_Helper::number_of_nodes_per_face};

  public:
    static void Advect_Density(Hierarchy& hierarchy,Channel_Vector& node_velocity_channels,T Struct_type::* density_channel,
                               T Struct_type::* node_density_channel,T Struct_type::* temp_channel,const T dt)
    {
        Log::Scope scope("Grid_Hierarchy_Advection::Advect_Density");

        uint64_t nodes_of_cell_offsets[number_of_nodes_per_cell];
        Topology_Helper::Nodes_Of_Cell_Offsets(nodes_of_cell_offsets);
        const int levels=hierarchy.Levels();

        // clear temporary channel
        for(int level=0;level<levels;++level)
            SPGrid::Clear<Struct_type,T,d>(hierarchy.Allocator(level),hierarchy.Blocks(level),temp_channel);

        const TV intra_cell_dX=TV(.5);

        // advect
        for(int level=0;level<levels;++level)
            Density_Advection_Helper<Struct_type,T,d>(hierarchy,hierarchy.Blocks(level),density_channel,node_density_channel,temp_channel,node_velocity_channels,
                                                      nodes_of_cell_offsets,intra_cell_dX,level,dt,(unsigned)Cell_Type_Interior);

        // copy result
        for(int level=0;level<levels;++level)
            SPGrid::Masked_Copy<Struct_type,T,d>(hierarchy.Allocator(level),hierarchy.Blocks(level),temp_channel,
                                                 density_channel,(unsigned)Cell_Type_Interior);
    }

    static void Advect_Face_Velocities(Hierarchy& hierarchy,Channel_Vector& face_velocity_channels,Channel_Vector& node_velocity_channels,
                                       T Struct_type::* temp_channel,const T dt)
    {
        Log::Scope scope("Grid_Hierarchy_Advection::Advect_Face_Velocities");

        Vector<uint64_t,d> other_face_offsets;
        for(int v=0;v<d;++v) other_face_offsets(v)=Topology_Helper::Axis_Vector_Offset(v);
        const int levels=hierarchy.Levels();

        // advect velocities along each axis
        for(int axis=0;axis<d;++axis){unsigned face_active_mask=Topology_Helper::Face_Active_Mask(axis);
            uint64_t nodes_of_face_offsets[number_of_nodes_per_face];
            Topology_Helper::Nodes_Of_Face_Offsets(nodes_of_face_offsets,axis);

            // clear temporary channel
            for(int level=0;level<levels;++level)
                SPGrid::Clear<Struct_type,T,d>(hierarchy.Allocator(level),hierarchy.Blocks(level),temp_channel);

            TV intra_cell_dX=TV(.5)-TV::Axis_Vector(axis)*(T).5;

            // advect
            for(int level=0;level<levels;++level)
                Face_Velocity_Advection_Helper<Struct_type,T,d>(hierarchy,hierarchy.Blocks(level),face_velocity_channels,node_velocity_channels,temp_channel,
                                                                nodes_of_face_offsets,other_face_offsets,intra_cell_dX,level,dt,face_active_mask,axis);

            // copy result
            for(int level=0;level<levels;++level)
                SPGrid::Masked_Copy<Struct_type,T,d>(hierarchy.Allocator(level),hierarchy.Blocks(level),temp_channel,face_velocity_channels(axis),face_active_mask);}
    }
};
}
#endif
