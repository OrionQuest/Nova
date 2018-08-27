//!#####################################################################
//! \file Grid_Hierarchy.h
//!#####################################################################
// Class Grid_Hierarchy
//######################################################################
#ifndef __Grid_Hierarchy__
#define __Grid_Hierarchy__

#include <nova/SPGrid/Core/SPGrid_Allocator.h>
#include <nova/SPGrid/Core/SPGrid_Page_Map.h>
#include <nova/SPGrid/Core/SPGrid_Set.h>
#include <nova/SPGrid/Tools/SPGrid_Block_Iterator.h>
#include <nova/Tools/Arrays/Array.h>
#include <nova/Tools/Grids/Grid.h>
#include <nova/Tools/Utilities/File_Utilities.h>

namespace Nova{
template<class Struct_type,class T,int d>
class Grid_Hierarchy
{
    using T_INDEX               = Vector<int,d>;
    using Page_Map_type         = SPGrid::SPGrid_Page_Map;
    using Index_type            = std::array<SPGrid::ucoord_t,d>;
    using Allocator_type        = SPGrid::SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask       = typename Allocator_type::template Array_mask<unsigned>;
    using Flag_array_type       = typename Allocator_type::template Array_type<unsigned>;
    using Block_Iterator        = SPGrid::SPGrid_Block_Iterator<Flag_array_mask>;
    using Set_type              = SPGrid::SPGrid_Set<Flag_array_type>;

    Array<Grid<T,d>> grids;
    Array<Index_type> sizes;
    Array<Page_Map_type*> page_maps;
    Array<Allocator_type*> allocators;

    void Initialize_Allocators()
    {
        const unsigned levels=Levels();
        allocators.resize(levels);
        for(unsigned level=0;level<levels;++level){
            Vector<SPGrid::ucoord_t,d> current_size(sizes[level]);current_size+=2;          // size+2 to accommodate boundaries
            allocators[level]=new Allocator_type(current_size._data);}
    }

    void Initialize_Page_Maps()
    {
        const unsigned levels=Levels();
        page_maps.resize(levels);
        for(unsigned level=0;level<levels;++level) page_maps[level]=new Page_Map_type(*allocators[level]);
    }

  public:

    Grid_Hierarchy() {}

    Grid_Hierarchy(const Grid<T,d>& grid,const unsigned levels)
    {Initialize(grid.counts,grid.domain,levels);}

    Grid_Hierarchy(const T_INDEX& counts,const Range<T,d>& domain,const unsigned levels)
    {Initialize(counts,domain,levels);}

    ~Grid_Hierarchy()
    {
        for(auto a: allocators) delete a;
        for(auto p: page_maps) delete p;
    }

    void Initialize(const T_INDEX& counts,const Range<T,d>& domain,const unsigned levels)
    {
        T_INDEX cell_counts=counts;
        Index_type base_size=*reinterpret_cast<Index_type*>(&cell_counts);
        grids.resize(levels);sizes.resize(levels);
        for(unsigned level=0;level<levels;++level){sizes[level]=base_size;
            for(auto &s:base_size) s/=2;
            grids[level].Initialize(*reinterpret_cast<T_INDEX*>(&sizes[level]),domain);}

        Initialize_Allocators();
        Initialize_Page_Maps();
    }

    unsigned Levels() const
    {return sizes.size();}

    const Grid<T,d>& Lattice(const unsigned level) const
    {return grids[level];}

    // access to allocators
    const Allocator_type& Allocator(const unsigned level) const
    {return *allocators[level];}

    Allocator_type& Allocator(const unsigned level)
    {return *allocators[level];}

    // access to blocks (for optimized iteration)
    std::pair<const uint64_t*,unsigned> Blocks(const unsigned level) const
    {return page_maps[level]->Get_Blocks();}

    // access to page maps
    const Page_Map_type& Page_Map(const unsigned level) const
    {return *page_maps[level];}

    Page_Map_type& Page_Map(const unsigned level)
    {return *page_maps[level];}

    void Update_Block_Offsets()
    {
        const unsigned levels=Levels();
        for(unsigned level=0;level<levels;++level) page_maps[level]->Update_Block_Offsets();
    }

    // access to sets (more often use blocks)
    template<class T_FIELD>
    Set_type Set(const unsigned level,T_FIELD Struct_type::* field) const
    {return Set_type(*page_maps[level],allocators[level]->Get_Array(field));}

    void Activate_Cell(const unsigned level,const unsigned long offset,unsigned long mask)
    {Set(level,&Struct_type::flags).Mask(offset,mask);}

    void Activate_Cell(const unsigned level,const T_INDEX& index,unsigned long mask)
    {Set(level,&Struct_type::flags).Mask(*reinterpret_cast<const Index_type*>(&index),mask);}

    void Write_Hierarchy(const std::string& output_directory,const int frame)
    {
        if(frame==0){Write_Metadata(output_directory);
            File_Utilities::Write_To_File(output_directory+"/common/fine_grid",grids[0]);}
        Write_Block_Offsets(output_directory+"/"+std::to_string(frame)+"/block_offsets");
        Write_Channel<unsigned>(output_directory+"/"+std::to_string(frame)+"/flags",&Struct_type::flags);
    }

    void Write_Metadata(const std::string& output_directory)
    {
        std::ostream* output=(d==3)?File_Utilities::Safe_Open_Output(output_directory+"/common/hierarchy.struct3d"):File_Utilities::Safe_Open_Output(output_directory+"/common/hierarchy.struct2d");
        Read_Write<int>::Write(*output,Levels());
        Read_Write<int>::Write(*output,Flag_array_mask::elements_per_block);
        Index_type block_size=Allocator(0).Block_Size();
        for(int v=0;v<d;++v) Read_Write<int>::Write(*output,block_size[v]);
        delete output;
    }

    template<class T_FIELD>
    void Write_Channel(const std::string& filename,T_FIELD Struct_type::* channel)
    {
        std::ostream* output=File_Utilities::Safe_Open_Output(filename);
        for(unsigned level=0;level<Levels();++level){
            auto data=Allocator(level).Get_Const_Array(channel);
            for(Block_Iterator iterator(Page_Map(level).Get_Blocks());iterator.Valid();iterator.Next())
                Read_Write<T_FIELD>::Write(*output,data(iterator.Offset()));}
        delete output;
    }

    void Write_Block_Offsets(const std::string& filename)
    {
        std::ostream* output=File_Utilities::Safe_Open_Output(filename);
        for(unsigned level=0;level<Levels();++level){
            auto blocks=Page_Map(level).Get_Blocks();
            Read_Write<int>::Write(*output,blocks.second);
            for(unsigned block=0;block<blocks.second;++block){
                uint64_t offset=blocks.first[block];
                std::array<int,d> base_index_s=Flag_array_mask::LinearToCoord(offset);
                T_INDEX base_index=*reinterpret_cast<T_INDEX*>(&base_index_s);
                Read_Write<T_INDEX>::Write(*output,base_index);}}
        delete output;
    }
};
}
#endif
