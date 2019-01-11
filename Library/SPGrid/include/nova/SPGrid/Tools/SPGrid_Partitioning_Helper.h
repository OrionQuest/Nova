//!#####################################################################
//! \file SPGrid_Partitioning_Helper.h
//!#####################################################################
// Class Partitioning_Helper
//######################################################################
#ifndef __Partitioning_Helper__
#define __Partitioning_Helper__

#include <nova/SPGrid/Core/SPGrid_Allocator.h>
#include <nova/SPGrid/Core/SPGrid_Set.h>
#include <nova/Tools/Arrays/Array.h>
#include <algorithm>
#include <vector>

namespace SPGrid{
template<class Struct_type,int d> struct Partitioning_Helper_Neighbor_Offsets;

template<class Struct_type>
struct Partitioning_Helper_Neighbor_Offsets<Struct_type,2>
{
    enum {d=2};
    enum {num_neighbors=9};

    using Allocator_type        = SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask       = typename Allocator_type::template Array_mask<unsigned>;

    static std::vector<uint64_t> Neighbor_Offsets(std::array<unsigned,d> block_size)
    {
        std::vector<uint64_t> neighbors;neighbors.resize(num_neighbors);
        int count=0;
        for(int i=-1;i<=1;i++) for(int j=-1;j<=1;j++)
            neighbors[count++]=Flag_array_mask::Linear_Offset(std::array<unsigned,d>({i*block_size[0],j*block_size[1]}));
        return neighbors;
    }
};

template<class Struct_type>
struct Partitioning_Helper_Neighbor_Offsets<Struct_type,3>
{
    enum {d=3};
    enum {num_neighbors=27};

    using Allocator_type        = SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask       = typename Allocator_type::template Array_mask<unsigned>;

    static std::vector<uint64_t> Neighbor_Offsets(std::array<unsigned,d> block_size)
    {
        std::vector<uint64_t> neighbors;neighbors.resize(num_neighbors);
        int count=0;
        for(int i=-1;i<=1;i++) for(int j=-1;j<=1;j++) for(int k=-1;k<=1;k++)
            neighbors[count++]=Flag_array_mask::Linear_Offset(std::array<unsigned,d>({i*block_size[0],j*block_size[1],k*block_size[2]}));
        return neighbors;
    }
};

template<class Struct_type,int d>
class Partitioning_Helper
{
    using T_Block               = std::pair<const uint64_t*,unsigned>;
    using TV_Block              = Nova::Array<T_Block>;
    using T_Partition           = std::pair<TV_Block,TV_Block>;
    using Allocator_type        = SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask       = typename Allocator_type::template Array_mask<unsigned>;
    using Flag_array_type       = typename Allocator_type::template Array_type<unsigned>;
    using Set_type              = SPGrid_Set<Flag_array_type>;

    Allocator_type& allocator;
    const Set_type& set;
    Nova::Array<uint64_t>& blocks;
    enum {ALL_ONES=0xFFFFFFFFu};

  private:
    static bool lex_compare_packed_offsets(const uint64_t a,const uint64_t b)
    {
        const std::array<int,d> a_index(Flag_array_mask::LinearToCoord(a));
        const std::array<int,d> b_index(Flag_array_mask::LinearToCoord(b));
        for(int v=0;v<d;v++) if(a_index[v]!=b_index[v]) return a_index[v]<b_index[v];
        return true;
    }

  public:
    Partitioning_Helper(Allocator_type& allocator_input,const Set_type& set_input,Nova::Array<uint64_t>& blocks_input)
        :allocator(allocator_input),set(set_input),blocks(blocks_input)
    {}

    void Generate_Red_Black_Partition(const int number_of_partitions,TV_Block& red_blocks,TV_Block& black_blocks)
    {
        const int size=blocks.size();
        if(!size) return;
        // init neighbor offsets
        const std::vector<uint64_t> neighbor_offsets(Partitioning_Helper_Neighbor_Offsets<Struct_type,d>::Neighbor_Offsets(allocator.Block_Size()));
        // sort new blocks *lexicographically*
        std::sort(blocks.begin(),blocks.end(),&Partitioning_Helper::lex_compare_packed_offsets);
        uint64_t minimum_offset=blocks[0];
        int current_index=0;
        for(int partition=0;partition<number_of_partitions;partition++){
            // find where this partition should end
            const int ideal_last_index_of_partition=(size/number_of_partitions)*(partition+1)+std::min(size%number_of_partitions,partition+1)-1;
            int last_index_of_partition=current_index;
            while((last_index_of_partition < (size-1)) &&
                  ((last_index_of_partition < ideal_last_index_of_partition) ||
                   (lex_compare_packed_offsets(blocks[last_index_of_partition],minimum_offset))))
                last_index_of_partition++;
            // create block
            T_Block block(&blocks[0]+current_index,last_index_of_partition-current_index+1);
            // resort by spgrid index locally
            std::sort(blocks.begin()+current_index,blocks.begin()+last_index_of_partition+1);
            // update current_index and min_index
            current_index=last_index_of_partition+1;
            for(unsigned i=0;i<block.second;i++){
                const uint64_t block_offset=*(block.first+i);
                for(int j=0;j<=neighbor_offsets.size();j++){
                    uint64_t neighbor=Flag_array_mask::Packed_Add(block_offset,neighbor_offsets[j]);
                    if(set.Is_Set(neighbor,ALL_ONES))
                        minimum_offset=std::max(minimum_offset,neighbor,&Partitioning_Helper::lex_compare_packed_offsets);}}
            // add block to red or black
            if(partition%2) black_blocks.Append(block);
            else red_blocks.Append(block);
            // dont go too far
            if(current_index >= size) break;}
    }
};
}
#endif
