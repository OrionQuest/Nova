//!#####################################################################
//! \file Grid_Hierarchy_Visualization.h
//!#####################################################################
// Class Grid_Hierarchy_Visualization
//######################################################################
#ifndef __Grid_Hierarchy_Visualization__
#define __Grid_Hierarchy_Visualization__

#include <boost/functional/hash.hpp>
#include <nova/Dynamics/Hierarchy/Grid_Hierarchy.h>
#include <nova/Dynamics/Utilities/SPGrid_Flags.h>
#include <nova/Geometry/Topology_Based_Geometry/Triangulated_Surface.h>
#include <nova/Tools/Arrays/Array.h>
#include <nova/Tools/Utilities/Range_Iterator.h>
#include <cassert>
#include <set>
#include <unordered_map>
#include <tuple>

namespace Nova{
template<class Struct_type,class T>
class Grid_Hierarchy_Visualization
{
  public:
    static void Visualize_Heightfield(Grid_Hierarchy<Struct_type,T,2>& hierarchy,T Struct_type::* channel,
                                      const std::string& output_directory,const int frame,const T scale=(T)1.)
    {
        enum {d=2};
        using TV                        = Vector<T,d>;
        using T_INDEX                   = Vector<int,d>;
        using Pair                      = std::pair<int,uint64_t>;
        using Flags_type                = typename Struct_type::Flags_type;
        using Cell_Info                 = std::tuple<unsigned,int,T_INDEX>;
        using Cell_Hash                 = std::unordered_map<Pair,Cell_Info,boost::hash<Pair>>;
        using Hierarchy                 = Grid_Hierarchy<Struct_type,T,d>;
        using Allocator_type            = SPGrid::SPGrid_Allocator<Struct_type,d>;
        using Flag_array_mask           = typename Allocator_type::template Array_mask<unsigned>;

        // create cell hash and map to non-ghost parents
        Cell_Hash cell_hash;
        Array<int> non_ghost_parent_of_cell;
        for(int level=hierarchy.Levels()-1;level>=0;--level){
            auto blocks=hierarchy.Blocks(level);
            auto block_size=hierarchy.Allocator(level).Block_Size();
            auto flags=hierarchy.Allocator(level).template Get_Const_Array<Struct_type,unsigned>(&Struct_type::flags);
            
            for(unsigned block=0;block<blocks.second;++block){uint64_t offset=blocks.first[block];
                Range_Iterator<d> range_iterator(T_INDEX(),*reinterpret_cast<T_INDEX*>(&block_size)-1);
                T_INDEX base_index(Flag_array_mask::LinearToCoord(offset));

                for(int e=0;e<Flag_array_mask::elements_per_block;++e,offset+=sizeof(Flags_type)){const T_INDEX index=base_index+range_iterator.Index();
                    if(hierarchy.template Set<unsigned>(level,&Struct_type::flags).Is_Set(offset,Cell_Type_Interior|Cell_Type_Ghost|Cell_Type_Dirichlet)){
                        // iterate over all cells, add to cell hash
                        Cell_Info info=std::make_tuple(cell_hash.size(),level,index);
                        cell_hash.insert({Pair(level,offset),info});

                        // find parents of each cell
                        if(hierarchy.template Set<unsigned>(level,&Struct_type::flags).Is_Set(offset,Cell_Type_Ghost)){
                            T_INDEX current_index=index;uint64_t parent_offset=offset;int parent_level;
                            for(parent_level=level+1;parent_level<hierarchy.Levels();++parent_level){
                                current_index=(current_index-1)/2+1;parent_offset=Flag_array_mask::DownsampleOffset(parent_offset);
                                if(hierarchy.template Set<unsigned>(parent_level,&Struct_type::flags).Is_Set(parent_offset,Cell_Type_Interior)) break;}

                            if(parent_level<hierarchy.Levels()) non_ghost_parent_of_cell.Append(std::get<0>(cell_hash[Pair(parent_level,parent_offset)]));
                            else non_ghost_parent_of_cell.Append(-1);}
                        else non_ghost_parent_of_cell.Append(std::get<0>(info));}
                    range_iterator.Next();}}}

        assert(non_ghost_parent_of_cell.size()==cell_hash.size());

        // fill in node info
        Cell_Hash node_hash;
        for(typename Cell_Hash::iterator it=cell_hash.begin();it!=cell_hash.end();++it){
            const int level=std::get<1>(it->second);const T_INDEX index=std::get<2>(it->second);const uint64_t offset=it->first.second;
            auto flags=hierarchy.Allocator(level).template Get_Const_Array<Struct_type,unsigned>(&Struct_type::flags);

            if(flags(offset) & (Cell_Type_Interior|Cell_Type_Dirichlet)){
                // node (0,0)
                node_hash.insert({Pair(level,offset),std::make_tuple(node_hash.size(),level,index)});

                // node (1,0)
                uint64_t node_offset=Flag_array_mask::Packed_Add(offset,Flag_array_mask::template LinearOffset<1,0>::value);
                node_hash.insert({Pair(level,node_offset),std::make_tuple(node_hash.size(),level,index+T_INDEX({1,0}))});

                // node (1,1)
                node_offset=Flag_array_mask::Packed_Add(offset,Flag_array_mask::template LinearOffset<1,1>::value);
                node_hash.insert({Pair(level,node_offset),std::make_tuple(node_hash.size(),level,index+T_INDEX({1,1}))});

                // node (0,1)
                node_offset=Flag_array_mask::Packed_Add(offset,Flag_array_mask::template LinearOffset<0,1>::value);
                node_hash.insert({Pair(level,node_offset),std::make_tuple(node_hash.size(),level,index+T_INDEX({0,1}))});}}

        // fill in node neighbors
        typedef Vector<int,4> Node_Neighbors;
        Array<Node_Neighbors> node_neighbors(node_hash.size(),Nova::Vector<int,4>(-1));
        for(typename Cell_Hash::iterator it=cell_hash.begin();it!=cell_hash.end();++it){
            const int cell_id=std::get<0>(it->second);const int level=std::get<1>(it->second);
            const T_INDEX index=std::get<2>(it->second);const uint64_t offset=it->first.second;
            auto flags=hierarchy.Allocator(level).template Get_Array<Struct_type,unsigned>(&Struct_type::flags);

            if(flags(offset) & (Cell_Type_Interior|Cell_Type_Ghost|Cell_Type_Dirichlet)){
                uint64_t node_offset_00=offset;
                uint64_t node_offset_01=Flag_array_mask::Packed_Add(offset,Flag_array_mask::template LinearOffset<0,1>::value);
                uint64_t node_offset_10=Flag_array_mask::Packed_Add(offset,Flag_array_mask::template LinearOffset<1,0>::value);
                uint64_t node_offset_11=Flag_array_mask::Packed_Add(offset,Flag_array_mask::template LinearOffset<1,1>::value);

                unsigned n_00=(node_hash.count(Pair(level,node_offset_00))!=0)?std::get<0>(node_hash[Pair(level,node_offset_00)]):-1;
                unsigned n_01=(node_hash.count(Pair(level,node_offset_01))!=0)?std::get<0>(node_hash[Pair(level,node_offset_01)]):-1;
                unsigned n_10=(node_hash.count(Pair(level,node_offset_10))!=0)?std::get<0>(node_hash[Pair(level,node_offset_10)]):-1;
                unsigned n_11=(node_hash.count(Pair(level,node_offset_11))!=0)?std::get<0>(node_hash[Pair(level,node_offset_11)]):-1;

                if(n_00 != -1) node_neighbors(n_00)(2)=non_ghost_parent_of_cell(cell_id);
                if(n_01 != -1) node_neighbors(n_01)(1)=non_ghost_parent_of_cell(cell_id);
                if(n_10 != -1) node_neighbors(n_10)(3)=non_ghost_parent_of_cell(cell_id);
                if(n_11 != -1) node_neighbors(n_11)(0)=non_ghost_parent_of_cell(cell_id);}}

        // create triangulated surface
        Triangulated_Surface<T> surface;
        surface.Resize_Vertices(cell_hash.size());
        // add vertices
        for(typename Cell_Hash::iterator it=cell_hash.begin();it!=cell_hash.end();++it){
            const int level=std::get<1>(it->second);const uint64_t offset=it->first.second;const T_INDEX index=std::get<2>(it->second);
            auto data=hierarchy.Allocator(level).template Get_Array<Struct_type,T>(channel);

            surface.Set_Vertex(std::get<0>(it->second),hierarchy.Lattice(level).Center(index).Insert(scale*data(offset),2));}

        // add triangles
        for(int node_id=0;node_id<node_neighbors.size();++node_id){
            Array<int> neighbor_particles;std::set<int> neighbor_set;
            for(int i=0;i<4;++i){const int cell_id=node_neighbors(node_id)(i);
                if(cell_id!=-1 && neighbor_set.count(cell_id)==0){neighbor_particles.Append(cell_id);neighbor_set.insert(cell_id);}}

            switch(neighbor_particles.size()){
                case 3: surface.Add_Element(Vector<int,3>({neighbor_particles(0),neighbor_particles(1),neighbor_particles(2)}));
                        break;

                case 4: surface.Add_Element(Vector<int,3>({neighbor_particles(0),neighbor_particles(1),neighbor_particles(2)}));
                        surface.Add_Element(Vector<int,3>({neighbor_particles(0),neighbor_particles(2),neighbor_particles(3)}));
                        break;

                default: break;}}

        std::string filename=output_directory+"/"+std::to_string(frame)+"/heightfield.obj";
        surface.Write_OBJ(filename);
    }

    static void Visualize_Heightfield(Grid_Hierarchy<Struct_type,T,3>& hierarchy,T Struct_type::* channel,
                                      const std::string& output_directory,const int frame,const T scale=(T)1.)
    {}
};
}
#endif
