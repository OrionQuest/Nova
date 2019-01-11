//!#####################################################################
//! \file Grid_Hierarchy_Averaging.h
//!#####################################################################
// Class Grid_Hierarchy_Averaging
//######################################################################
#ifndef __Grid_Hierarchy_Averaging__
#define __Grid_Hierarchy_Averaging__

#include <nova/Dynamics/Hierarchy/Grid_Hierarchy.h>
#include <nova/Dynamics/Hierarchy/Grid_Topology_Helper.h>
#include <nova/Dynamics/Utilities/SPGrid_Flags.h>
#include <nova/SPGrid/Tools/SPGrid_Clear.h>
#include <nova/SPGrid/Tools/SPGrid_Constrain_T_Junction_Nodes.h>
#include <nova/SPGrid/Tools/SPGrid_Downsample_Accumulate_Shared.h>
#include <nova/SPGrid/Tools/SPGrid_Masked_Average_Offset_Grid.h>
#include <nova/SPGrid/Tools/SPGrid_Masked_Normalize.h>
#include <nova/SPGrid/Tools/SPGrid_Upsample_Inject_Shared.h>
#include <nova/Tools/Log/Log.h>

namespace Nova{

extern int number_of_threads;

template<class Struct_type,class T,int d>
class Grid_Hierarchy_Averaging
{
    using Flags_type                = typename Struct_type::Flags_type;
    using Channel_Vector            = Vector<T Struct_type::*,d>;
    using Hierarchy                 = Grid_Hierarchy<Struct_type,T,d>;
    using Allocator_type            = SPGrid::SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask           = typename Allocator_type::template Array_mask<unsigned>;
    using Topology_Helper           = Grid_Topology_Helper<Flag_array_mask>;

    enum {nodes_per_cell            = Topology_Helper::number_of_nodes_per_cell};
    enum {nodes_per_face            = Topology_Helper::number_of_nodes_per_face};

  public:
    static void Average_Cell_Density_To_Nodes(Hierarchy& hierarchy,T Struct_type::* cell_channel,T Struct_type::* node_channel,T Struct_type::* weight_channel)
    {
        Log::Scope scope("Grid_Hierarchy_Averaging::Average_Cell_Density_To_Nodes");

        int nodes_of_cell_offsets[nodes_per_cell];
        Topology_Helper::Nodes_Of_Cell_Shadow_Grid_Offsets(nodes_of_cell_offsets);
        const int levels=hierarchy.Levels();

        // clear nodal values and weights, perform in-level averaging
        for(int level=0;level<levels;++level){const T weight=(T)(1u<<(levels-level-1));
            // clear
            SPGrid::Clear<Struct_type,T,d>(hierarchy.Allocator(level),hierarchy.Blocks(level),node_channel);
            SPGrid::Clear<Struct_type,T,d>(hierarchy.Allocator(level),hierarchy.Blocks(level),weight_channel);

            if(number_of_threads){SPGrid_Computations::Threading_Helper<Struct_type,d>(hierarchy.Allocator(level),hierarchy.Blocks(level)).Run_Parallel(
                SPGrid::Masked_Average_Offset_Grid<nodes_per_cell,Struct_type,T,unsigned,d>(cell_channel,node_channel,weight_channel,&Struct_type::flags,
                                                                nodes_of_cell_offsets,(unsigned)Cell_Type_Interior,weight),hierarchy.Red_Partition(level));
                SPGrid_Computations::Threading_Helper<Struct_type,d>(hierarchy.Allocator(level),hierarchy.Blocks(level)).Run_Parallel(
                    SPGrid::Masked_Average_Offset_Grid<nodes_per_cell,Struct_type,T,unsigned,d>(cell_channel,node_channel,weight_channel,&Struct_type::flags,
                                                                nodes_of_cell_offsets,(unsigned)Cell_Type_Interior,weight),hierarchy.Black_Partition(level));}
            else SPGrid::Masked_Average_Offset_Grid<nodes_per_cell,Struct_type,T,unsigned,d>(hierarchy.Allocator(level),hierarchy.Blocks(level),cell_channel,
                                                node_channel,weight_channel,&Struct_type::flags,nodes_of_cell_offsets,(unsigned)Cell_Type_Interior,weight);}

        // accumulate weights, normalize and update across all levels
        for(int level=0;level<levels-1;++level)
            SPGrid::Downsample_Accumulate_Shared<Struct_type,T,d>(hierarchy.Allocator(level),hierarchy.Blocks(level),hierarchy.Allocator(level+1),
                                                                  node_channel,weight_channel,(unsigned)Node_Coarse_Shared);

        for(int level=0;level<levels;++level)
            SPGrid::Masked_Normalize<Struct_type,T,d>(hierarchy.Allocator(level),hierarchy.Blocks(level),node_channel,weight_channel,(unsigned)Node_Active);

        for(int level=levels-2;level>=0;--level)
            SPGrid::Upsample_Inject_Shared<Struct_type,T,d>(hierarchy.Allocator(level),hierarchy.Blocks(level),hierarchy.Allocator(level+1),
                                                            node_channel,(unsigned)Node_Coarse_Shared);

        // fix T-junctions
        for(int level=0;level<levels;++level)
            SPGrid::Masked_Clear<Struct_type,T,d>(hierarchy.Allocator(level),hierarchy.Blocks(level),node_channel,(unsigned)Node_T_Junction);
        for(int level=levels-2;level>=0;--level)
            SPGrid::Constrain_T_Junction_Nodes<Struct_type,T,d>(hierarchy,hierarchy.Blocks(level),node_channel,level);
    }

    static void Average_Face_Velocities_To_Nodes(Hierarchy& hierarchy,Channel_Vector& face_channel,Channel_Vector& node_channel,T Struct_type::* weight_channel)
    {
        Log::Scope scope("Grid_Hierarchy_Averaging::Average_Face_Velocities_To_Nodes");

        const int levels=hierarchy.Levels();

        for(int axis=0;axis<d;++axis){int nodes_of_face_offsets[nodes_per_face];
            Topology_Helper::Nodes_Of_Face_Shadow_Grid_Offsets(nodes_of_face_offsets,axis);
            const unsigned face_valid_mask=Topology_Helper::Face_Valid_Mask(axis);

            // clear nodal values and weights, perform in-level averaging
            for(int level=0;level<levels;++level){const T weight=(T)(1u<<(levels-level-1));
                // clear
                SPGrid::Clear<Struct_type,T,d>(hierarchy.Allocator(level),hierarchy.Blocks(level),node_channel(axis));
                SPGrid::Clear<Struct_type,T,d>(hierarchy.Allocator(level),hierarchy.Blocks(level),weight_channel);

                // average
                if(number_of_threads){SPGrid_Computations::Threading_Helper<Struct_type,d>(hierarchy.Allocator(level),hierarchy.Blocks(level)).Run_Parallel(
                    SPGrid::Masked_Average_Offset_Grid<nodes_per_face,Struct_type,T,unsigned,d>(face_channel(axis),node_channel(axis),weight_channel,&Struct_type::flags,
                                                                                                nodes_of_face_offsets,face_valid_mask,weight),hierarchy.Red_Partition(level));

                    SPGrid_Computations::Threading_Helper<Struct_type,d>(hierarchy.Allocator(level),hierarchy.Blocks(level)).Run_Parallel(
                        SPGrid::Masked_Average_Offset_Grid<nodes_per_face,Struct_type,T,unsigned,d>(face_channel(axis),node_channel(axis),weight_channel,&Struct_type::flags,
                                                                                                nodes_of_face_offsets,face_valid_mask,weight),hierarchy.Black_Partition(level));}
                else SPGrid::Masked_Average_Offset_Grid<nodes_per_face,Struct_type,T,unsigned,d>(hierarchy.Allocator(level),hierarchy.Blocks(level),face_channel(axis),
                                                                        node_channel(axis),weight_channel,&Struct_type::flags,nodes_of_face_offsets,face_valid_mask,weight);}

            // accumulate weights, normalize and update across all levels
            for(int level=0;level<levels-1;++level)
                SPGrid::Downsample_Accumulate_Shared<Struct_type,T,d>(hierarchy.Allocator(level),hierarchy.Blocks(level),hierarchy.Allocator(level+1),
                                                                      node_channel(axis),weight_channel,(unsigned)Node_Coarse_Shared);

            for(int level=0;level<levels;++level)
                SPGrid::Masked_Normalize<Struct_type,T,d>(hierarchy.Allocator(level),hierarchy.Blocks(level),node_channel(axis),weight_channel,(unsigned)Node_Active);

            for(int level=levels-2;level>=0;--level)
                SPGrid::Upsample_Inject_Shared<Struct_type,T,d>(hierarchy.Allocator(level),hierarchy.Blocks(level),hierarchy.Allocator(level+1),
                                                                node_channel(axis),(unsigned)Node_Coarse_Shared);}
    }
};
}
#endif
