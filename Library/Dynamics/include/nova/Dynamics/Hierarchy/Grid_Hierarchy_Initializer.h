//!#####################################################################
//! \file Grid_Hierarchy_Initializer.h
//!#####################################################################
// Class Grid_Hierarchy_Initializer
//######################################################################
#ifndef __Grid_Hierarchy_Initializer__
#define __Grid_Hierarchy_Initializer__

#include <nova/Dynamics/Hierarchy/Grid_Hierarchy.h>
#include <nova/Dynamics/Hierarchy/Grid_Topology_Helper.h>
#include <nova/Dynamics/Utilities/SPGrid_Flags.h>
#include <nova/SPGrid/Tools/Parity_Helper.h>
#include <nova/SPGrid/Tools/SPGrid_Set_Iterator.h>
#include <nova/Tools/Log/Log.h>
#include <nova/Tools/Utilities/Range_Iterator.h>

namespace Nova{
template<class Struct_type,class T,int d>
class Grid_Hierarchy_Initializer
{
    using Hierarchy                 = Grid_Hierarchy<Struct_type,T,d>;
    using Allocator_type            = SPGrid::SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask           = typename Allocator_type::template Array_mask<unsigned>;
    using Flag_array_type           = typename Allocator_type::template Array_type<unsigned>;
    using Topology_Helper           = Grid_Topology_Helper<Flag_array_mask>;
    using Parity                    = SPGrid::Parity_Helper<Flag_array_mask,d>;
    using Set_Iterator              = SPGrid::SPGrid_Set_Iterator<Flag_array_type>;

    enum {number_of_nodes_per_cell  = Topology_Helper::number_of_nodes_per_cell};
    enum {number_of_faces_per_cell  = Topology_Helper::number_of_faces_per_cell};

  public:

    static void Flag_Ghost_Cells(Hierarchy& hierarchy)
    {
        Nova::Log::Scope scope("Grid_Hierarchy_Initializer::Flag_Ghost_Cells");

        uint64_t face_neighbor_offsets[number_of_faces_per_cell];
        Topology_Helper::Face_Neighbor_Offsets(face_neighbor_offsets);
        const int levels=hierarchy.Levels();

        for(int level=0;level<levels-1;++level){
            auto flags_set=hierarchy.template Set<unsigned>(level,&Struct_type::flags);
            for(Set_Iterator iterator(flags_set);iterator.Valid();iterator.Next()) if(iterator.Data()&Cell_Type_Interior)
                for(int neighbor=0;neighbor<number_of_faces_per_cell;++neighbor){
                    uint64_t neighbor_offset=iterator.Offset(face_neighbor_offsets[neighbor]),current_offset=neighbor_offset;
                    // search at current level and coarser resolutions for parent
                    int parent_level=-1;
                    for(int current_level=level;current_level<levels;++current_level){
                        if(hierarchy.template Set<unsigned>(current_level,&Struct_type::flags).Is_Set(current_offset,Cell_Type_Interior|Cell_Type_Ghost)){parent_level=current_level;break;}
                        current_offset=Flag_array_mask::DownsampleOffset(current_offset);}

                    // fill in ghost values (if parent level==0, nothing to be done!)
                    current_offset=neighbor_offset;
                    for(int current_level=level;current_level<parent_level;++current_level){
                        uint64_t parent_offset=Flag_array_mask::DownsampleOffset(current_offset);
                        hierarchy.Activate_Cell(current_level,current_offset,Cell_Type_Ghost);

                        // mask bits in parent mask
                        unsigned mask,pmask=current_offset&Parity::GHOST_111;
                        if(pmask==Parity::GHOST_000) mask=Ghost_Child_000;
                        else if(pmask==Parity::GHOST_010) mask=Ghost_Child_010;
                        else if(pmask==Parity::GHOST_100) mask=Ghost_Child_100;
                        else if(pmask==Parity::GHOST_110) mask=Ghost_Child_110;
                        else if(pmask==Parity::GHOST_001){assert(d==3);mask=Ghost_Child_001;}
                        else if(pmask==Parity::GHOST_011){assert(d==3);mask=Ghost_Child_011;}
                        else if(pmask==Parity::GHOST_101){assert(d==3);mask=Ghost_Child_101;}
                        else if(pmask==Parity::GHOST_111){assert(d==3);mask=Ghost_Child_111;}

                        hierarchy.Activate_Cell(current_level+1,parent_offset,mask);
                        current_offset=parent_offset;}}}
    }
};
}
#endif
