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
#include <nova/Tools/Arrays/Array_ND.h>
#include <nova/Tools/Log/Log.h>
#include <nova/Tools/Utilities/Range_Iterator.h>

namespace Nova{
template<class Struct_type,class T,int d>
class Grid_Hierarchy_Initializer
{
    using T_INDEX                   = Vector<int,d>;
    using Hierarchy                 = Grid_Hierarchy<Struct_type,T,d>;
    using Allocator_type            = SPGrid::SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask           = typename Allocator_type::template Array_mask<unsigned>;
    using Flag_array_type           = typename Allocator_type::template Array_type<unsigned>;
    using Topology_Helper           = Grid_Topology_Helper<Flag_array_mask>;
    using Parity                    = SPGrid::Parity_Helper<Flag_array_mask,d>;
    using Set_Iterator              = SPGrid::SPGrid_Set_Iterator<Flag_array_type>;

    enum {number_of_nodes_per_cell  = Topology_Helper::number_of_nodes_per_cell};
    enum {number_of_faces_per_cell  = Topology_Helper::number_of_faces_per_cell};
    enum {number_of_nodes_per_face  = Topology_Helper::number_of_nodes_per_face};

  public:

    static void Flag_Ghost_Cells(Hierarchy& hierarchy)
    {
        Log::Scope scope("Grid_Hierarchy_Initializer::Flag_Ghost_Cells");

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

    static void Flag_Valid_Faces(Hierarchy& hierarchy)
    {
        Log::Scope scope("Grid_Hierarchy_Initializer::Flag_Valid_Faces");

        int child=0;
        Array_ND<uint64_t,d> ghost_child_masks(Range<int,d>::Unit_Box());
        for(Range_Iterator<d> iterator(Range<int,d>::Unit_Box());iterator.Valid();iterator.Next(),++child){const T_INDEX& index=iterator.Index();
            if(d==3) ghost_child_masks(index)=Ghost_Child_000<<child;
            else ghost_child_masks(index)=Ghost_Child_000<<(2*child);}

        uint64_t face_neighbor_offsets[number_of_faces_per_cell],face_shifts[number_of_faces_per_cell],face_ghost_masks[number_of_faces_per_cell];
        Topology_Helper::Face_Neighbor_Offsets(face_neighbor_offsets);
        Topology_Helper::Face_Shift_Offsets(face_shifts);
        const int levels=hierarchy.Levels();

        for(int axis=0,face_number=0;axis<d;++axis) for(int axis_shift=0;axis_shift<=1;++axis_shift){face_ghost_masks[face_number]=0;
            for(Range_Iterator<d-1> iterator(Range<int,d-1>::Unit_Box());iterator.Valid();iterator.Next()){
                const T_INDEX index=T_INDEX(1)-iterator.Index().Insert(axis_shift,axis);
                face_ghost_masks[face_number]|=ghost_child_masks(index);}
            ++face_number;}

        for(int level=0;level<levels;++level){auto flags_set=hierarchy.template Set<unsigned>(level,&Struct_type::flags);
            for(Set_Iterator iterator(flags_set);iterator.Valid();iterator.Next()) if(iterator.Data()&Cell_Type_Interior)
                for(int axis=0,face_number=0;axis<d;++axis) for(int axis_shift=0;axis_shift<=1;++axis_shift){
                    const uint64_t face_offset=Flag_array_mask::Packed_Add(iterator.Offset(),face_shifts[face_number]);
                    const uint64_t neighbor_offset=Flag_array_mask::Packed_Add(iterator.Offset(),face_neighbor_offsets[face_number]);
                    if(!((iterator.Data()&face_ghost_masks[face_number]) == face_ghost_masks[face_number]))
                        hierarchy.Activate_Cell(level,face_offset,Topology_Helper::Face_Valid_Mask(axis));
                    else if(hierarchy.template Set<unsigned>(level,&Struct_type::flags).Is_Set(neighbor_offset,Cell_Type_Dirichlet|Cell_Type_Interior|Cell_Type_Ghost))
                        hierarchy.Activate_Cell(level,face_offset,Topology_Helper::Face_Valid_Mask(axis));
                    ++face_number;}}
    }

    static void Flag_Active_Faces(Hierarchy& hierarchy)
    {
        Log::Scope scope("Grid_Hierarchy_Initializer::Flag_Active_Faces");

        const int levels=hierarchy.Levels();
        Vector<uint64_t,d> left_cell_offsets;
        for(int axis=0;axis<d;++axis) left_cell_offsets(axis)=Topology_Helper::Negative_Axis_Vector_Offset(axis);

        for(int level=0;level<levels;++level){auto flags_set=hierarchy.template Set<unsigned>(level,&Struct_type::flags);
            for(int axis=0;axis<d;++axis){unsigned face_valid_mask=Topology_Helper::Face_Valid_Mask(axis);
                for(Set_Iterator iterator(flags_set);iterator.Valid();iterator.Next()) if(iterator.Data()&face_valid_mask){
                    uint64_t left_cell_offset=Flag_array_mask::Packed_Add(iterator.Offset(),left_cell_offsets(axis));
                    bool left_cell_not_exterior=(hierarchy.template Set<unsigned>(level,&Struct_type::flags).Is_Set(left_cell_offset,Cell_Type_Interior|Cell_Type_Dirichlet|Cell_Type_Ghost));
                    bool right_cell_not_exterior=(hierarchy.template Set<unsigned>(level,&Struct_type::flags).Is_Set(iterator.Offset(),Cell_Type_Interior|Cell_Type_Dirichlet|Cell_Type_Ghost));
                    if(left_cell_not_exterior && right_cell_not_exterior) iterator.Data()|=Topology_Helper::Face_Active_Mask(axis);}}}
    }

    static void Flag_Active_Nodes(Hierarchy& hierarchy)
    {
        Log::Scope scope("Grid_Hierarchy_Initializer::Flag_Active_Nodes");

        uint64_t nodes_of_cell_offsets[number_of_nodes_per_cell];
        Topology_Helper::Nodes_Of_Cell_Offsets(nodes_of_cell_offsets);
        const uint64_t odd_bits=Flag_array_mask::Linear_Offset(T_INDEX(1)._data);
        const int levels=hierarchy.Levels();

        // flag active nodes at individual levels
        for(int level=0;level<levels;++level){auto flags_set=hierarchy.template Set<unsigned>(level,&Struct_type::flags);
            for(Set_Iterator iterator(flags_set);iterator.Valid();iterator.Next()) if(iterator.Data()&Cell_Type_Interior)
                for(int node=0;node<number_of_nodes_per_cell;++node){
                    uint64_t node_offset=Flag_array_mask::Packed_Add(iterator.Offset(),nodes_of_cell_offsets[node]);
                    hierarchy.Activate_Cell(level,node_offset,Node_Active);}}

        // flag active nodes at intermediate levels
        for(int level=0;level<levels-1;++level){auto flags_set=hierarchy.template Set<unsigned>(level,&Struct_type::flags);
            for(Set_Iterator iterator(flags_set);iterator.Valid();iterator.Next()) if(iterator.Data()&Node_Active){
                Array<uint64_t> offsets;                                // candidate offsets at in-between levels. First one starts at level+1
                bool found=false;                                       // indicates a previously marked active node has been found at a coarser level

                for(int new_level=level+1,new_offset=iterator.Offset();new_level<levels;++new_level){
                    if((new_offset&odd_bits)!=odd_bits) break;          // no co-located node exists at the coarser level
                    new_offset=Flag_array_mask::DownsampleOffset(new_offset);
                    if(hierarchy.template Set<unsigned>(level,&Struct_type::flags).Is_Set(new_offset,Node_Active)){found=true;break;}
                    offsets.Append(new_offset);}

                if(found) for(int dlevel=0;dlevel<offsets.size();++dlevel) hierarchy.Activate_Cell(level+dlevel+1,offsets(dlevel),Node_Active);}}
    }

    static void Flag_Shared_Nodes(Hierarchy& hierarchy)
    {
        Log::Scope scope("Grid_Hierarchy_Initializer::Flag_Shared_Nodes");

        const uint64_t odd_bits=Flag_array_mask::Linear_Offset(T_INDEX(1)._data);
        for(int level=0;level<hierarchy.Levels()-1;++level){auto flags_set=hierarchy.template Set<unsigned>(level,&Struct_type::flags);
            for(Set_Iterator iterator(flags_set);iterator.Valid();iterator.Next()) if(iterator.Data()&Node_Active){uint64_t offset=iterator.Offset();
                if((offset & odd_bits)==odd_bits){uint64_t coarse_offset=Flag_array_mask::DownsampleOffset(offset);
                    if(hierarchy.template Set<unsigned>(level+1,&Struct_type::flags).Is_Set(coarse_offset,Node_Active))
                        iterator.Data()|=Node_Coarse_Shared;}}}
    }

    static void Flag_Ghost_Nodes(Hierarchy& hierarchy)
    {
        Log::Scope scope("Grid_Hierarchy_Initializer::Flag_Ghost_Nodes");

        uint64_t face_neighbor_offsets[d][2];
        for(int axis=0;axis<d;++axis) for(int axis_shift=-1;axis_shift<=1;axis_shift+=2){
            const int side=(axis_shift+1)/2;
            T_INDEX shift;shift(axis)=axis_shift;
            face_neighbor_offsets[axis][side]=Flag_array_mask::Linear_Offset(shift._data);}

        uint64_t nodes_of_face_offsets[d][number_of_nodes_per_face];
        for(int axis=0;axis<d;++axis) Topology_Helper::Nodes_Of_Face_Offsets(nodes_of_face_offsets[axis],axis);
        Vector<uint64_t,d> axis_vector_offsets;
        for(int v=0;v<d;++v) axis_vector_offsets(v)=Topology_Helper::Axis_Vector_Offset(v);

        for(int level=0;level<hierarchy.Levels();++level){auto flags_set=hierarchy.template Set<unsigned>(level,&Struct_type::flags);
            for(Set_Iterator iterator(flags_set);iterator.Valid();iterator.Next()) if(iterator.Data()&Cell_Type_Interior)
                for(int axis=0;axis<d;++axis) for(int side=0;side<2;++side){int current_level=level;
                    uint64_t neighbor_offset=iterator.Offset(face_neighbor_offsets[axis][side]);
                    while(hierarchy.template Set<unsigned>(current_level,&Struct_type::flags).Is_Set(neighbor_offset,Cell_Type_Ghost)){
                        const uint64_t face_offset=(side==0)?Flag_array_mask::Packed_Add(neighbor_offset,axis_vector_offsets(axis)):neighbor_offset;
                        for(int node=0;node<number_of_nodes_per_face;++node){
                            const uint64_t node_offset=Flag_array_mask::Packed_Add(face_offset,nodes_of_face_offsets[axis][node]);
                            hierarchy.Activate_Cell(current_level,node_offset,Node_Ghost);}
                        neighbor_offset=Flag_array_mask::DownsampleOffset(neighbor_offset);
                        ++current_level;}}}
    }

    static void Flag_T_Junction_Nodes(Hierarchy& hierarchy)
    {
        Log::Scope scope("Grid_Hierarchy_Initializer::Flag_T_Junction_Nodes");

        Array_ND<uint64_t,d> nodes_of_cell_offsets(Range<int,d>::Unit_Box());
        for(Range_Iterator<d> node_iterator(Range<int,d>::Unit_Box());node_iterator.Valid();node_iterator.Next()){const T_INDEX& index=node_iterator.Index();
            nodes_of_cell_offsets(index)=Flag_array_mask::Linear_Offset(index._data);}
        Vector<uint64_t,d> axis_vector_offsets;
        for(int v=0;v<d;++v) axis_vector_offsets(v)=Topology_Helper::Axis_Vector_Offset(v);

        static const unsigned active_node_mask=(Node_Active|Node_Ghost);
        for(int level=0;level<hierarchy.Levels();++level){auto flags_set=hierarchy.template Set<unsigned>(level,&Struct_type::flags);
            for(Set_Iterator iterator(flags_set);iterator.Valid();iterator.Next()) if(iterator.Data()&Cell_Type_Ghost){const uint64_t offset=iterator.Offset();
                int coarse_level=level;uint64_t coarse_offset=offset;T_INDEX parity,index_relative_to_parent;
                while(!hierarchy.template Set<unsigned>(coarse_level,&Struct_type::flags).Is_Set(coarse_offset,Cell_Type_Interior)){
                    for(int v=0;v<d;++v) parity(v)=(coarse_offset&axis_vector_offsets(v))?0:1;
                    index_relative_to_parent+=(1<<(coarse_level-level))*parity;
                    coarse_level++;coarse_offset=Flag_array_mask::DownsampleOffset(coarse_offset);}
                const int parent_size=1<<(coarse_level-level);
                for(Range_Iterator<d> node_iterator(Range<int,d>::Unit_Box());node_iterator.Valid();node_iterator.Next()){
                    const T_INDEX& node_index=node_iterator.Index();const uint64_t node_offset=nodes_of_cell_offsets(node_index);
                    if(!hierarchy.template Set<unsigned>(level,&Struct_type::flags).Is_Set(Flag_array_mask::Packed_Add(offset,node_offset),active_node_mask)) continue;
                    bool is_corner_of_parent=true;
                    const T_INDEX node_index_in_coarse=node_index+index_relative_to_parent;
                    for(int v=0;v<d;++v) if(node_index_in_coarse(v)!=0&&node_index_in_coarse(v)!=parent_size){is_corner_of_parent=false;break;}
                    if(!is_corner_of_parent) iterator.Data(node_offset)|=Node_T_Junction;}}}
    }
};
}
#endif
