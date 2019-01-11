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
#include <nova/SPGrid/Tools/SPGrid_Partitioning_Helper.h>
#include <nova/Tools/Arrays/Array.h>
#include <nova/Tools/Grids/Grid.h>
#include <nova/Tools/Log/Log.h>
#include <nova/Tools/Utilities/File_Utilities.h>
#include <nova/Tools/Utilities/Range_Iterator.h>

namespace Nova{
template<class Struct_type,class T,int d>
class Grid_Hierarchy
{
    using T_INDEX                   = Vector<int,d>;
    using Page_Map_type             = SPGrid::SPGrid_Page_Map;
    using Index_type                = std::array<SPGrid::ucoord_t,d>;
    using Flags_type                = typename Struct_type::Flags_type;
    using Allocator_type            = SPGrid::SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask           = typename Allocator_type::template Array_mask<unsigned>;
    using Flag_array_type           = typename Allocator_type::template Array_type<unsigned>;
    using Const_flag_array_type     = typename Allocator_type::template Array_type<const unsigned>;
    using Data_array_type           = typename Allocator_type::template Array_type<T>;
    using Const_data_array_type     = typename Allocator_type::template Array_type<const T>;
    using Block_Iterator            = SPGrid::SPGrid_Block_Iterator<Flag_array_mask>;
    using Set_type                  = SPGrid::SPGrid_Set<Flag_array_type>;

    Array<Grid<T,d>> grids;
    Array<Index_type> sizes;
    Array<Page_Map_type*> page_maps;
    Array<Allocator_type*> allocators;
    Array<Array<uint64_t>> boundary_blocks,copy_of_blocks;
    Array<Array<std::pair<const uint64_t*,unsigned>>> red_blocks,black_blocks;

    void Initialize_Allocators()
    {
        const int levels=Levels();
        allocators.resize(levels);
        for(int level=0;level<levels;++level){
            Vector<SPGrid::ucoord_t,d> current_size(sizes[level]);current_size+=2;          // size+2 to accommodate boundaries
            allocators[level]=new Allocator_type(current_size._data);}
    }

    void Initialize_Page_Maps()
    {
        const int levels=Levels();
        page_maps.resize(levels);
        for(int level=0;level<levels;++level) page_maps[level]=new Page_Map_type(*allocators[level]);
    }

  public:

    Grid_Hierarchy() {}

    Grid_Hierarchy(const Grid<T,d>& grid,const int levels)
    {Initialize(grid.counts,grid.domain,levels);}

    Grid_Hierarchy(const T_INDEX& counts,const Range<T,d>& domain,const int levels)
    {Initialize(counts,domain,levels);}

    ~Grid_Hierarchy()
    {
        for(auto a: allocators) delete a;
        for(auto p: page_maps) delete p;
    }

    void Initialize(const T_INDEX& counts,const Range<T,d>& domain,const int levels)
    {
        T_INDEX cell_counts=counts;
        Index_type base_size=*reinterpret_cast<Index_type*>(&cell_counts);
        grids.resize(levels);sizes.resize(levels);
        for(int level=0;level<levels;++level){sizes[level]=base_size;
            for(auto &s:base_size) s/=2;
            grids[level].Initialize(*reinterpret_cast<T_INDEX*>(&sizes[level]),domain);}

        Initialize_Allocators();
        Initialize_Page_Maps();
    }

    int Levels() const
    {return sizes.size();}

    const Grid<T,d>& Lattice(const int level) const
    {return grids[level];}

    // access to allocators
    const Allocator_type& Allocator(const int level) const
    {return *allocators[level];}

    Allocator_type& Allocator(const int level)
    {return *allocators[level];}

    // access to blocks (for optimized iteration)
    std::pair<const uint64_t*,unsigned> Blocks(const int level) const
    {return page_maps[level]->Get_Blocks();}

    // access to boundary blocks (for optimized iteration)
    std::pair<const uint64_t*,unsigned> Boundary_Blocks(const int level) const
    {return std::pair<const uint64_t*,unsigned>(&(boundary_blocks(level)(0)),boundary_blocks(level).size());}

    // access to page maps
    const Page_Map_type& Page_Map(const int level) const
    {return *page_maps[level];}

    Page_Map_type& Page_Map(const int level)
    {return *page_maps[level];}

    Data_array_type Channel(const int level,T Struct_type::* field)
    {return allocators[level]->Get_Array(field);}

    Const_data_array_type Channel(const int level,const T Struct_type::* field) const
    {return allocators[level]->Get_Const_Array(field);}

    Flag_array_type Channel(const int level,unsigned Struct_type::* field)
    {return allocators[level]->Get_Array(field);}

    Const_flag_array_type Channel(const int level,const unsigned Struct_type::* field) const
    {return allocators[level]->Get_Const_Array(field);}

    void Update_Block_Offsets()
    {
        const int levels=Levels();
        for(int level=0;level<levels;++level) page_maps[level]->Update_Block_Offsets();
    }

    void Initialize_Boundary_Blocks(const unsigned boundary_mask)
    {
        const int levels=Levels();
        boundary_blocks.resize(levels);
        for(int level=0;level<levels;++level) boundary_blocks(level).resize(0);
        for(int level=0;level<levels;++level){auto flags=allocators[level]->Get_Const_Array(&Struct_type::flags);
            for(Block_Iterator iterator(Blocks(level));iterator.Valid();iterator.Next_Block()){
                uint64_t block_offset=iterator.Offset(),offset=block_offset;
                for(int e=0;e<Flag_array_mask::elements_per_block;++e,offset+=sizeof(Flags_type))
                    if(flags(offset)&boundary_mask){boundary_blocks(level).Append(block_offset);break;}}}
    }

    void Initialize_Red_Black_Partition(const int number_of_partitions)
    {
        const int levels=Levels();

        // copy over blocks
        copy_of_blocks.resize(levels);
        for(int level=0;level<levels;++level){
            copy_of_blocks(level).resize(Blocks(level).second);
            for(unsigned i=0;i<copy_of_blocks(level).size();i++)
                copy_of_blocks(level)[i]=(Blocks(level).first)[i];}

        // set up read black partition
        red_blocks.resize(levels);
        black_blocks.resize(levels);
        for(int level=0;level<levels;++level){
            SPGrid::Partitioning_Helper<Struct_type,d> partitioning_helper(Allocator(level),Set<unsigned>(level,&Struct_type::flags),copy_of_blocks(level));
            partitioning_helper.Generate_Red_Black_Partition(number_of_partitions,red_blocks(level),black_blocks(level));}

        for(int level=0;level<levels;++level){
            Log::cout<<"Level: "<<level<<std::endl;
            Log::cout<<"    Red Blocks:"<<std::endl;
            for(unsigned i=0;i<red_blocks(level).size();++i)
                Log::cout<<"        # Blocks : "<<red_blocks(level)[i].second<<std::endl;
            Log::cout<<"    Black Blocks:"<<std::endl;
            for(unsigned i=0;i<black_blocks(level).size();++i)
                Log::cout<<"        # Blocks : "<<black_blocks(level)[i].second<<std::endl;}
    }

    // access to sets (more often use blocks)
    template<class T_FIELD>
    Set_type Set(const int level,T_FIELD Struct_type::* field) const
    {return Set_type(*page_maps[level],allocators[level]->Get_Array(field));}

    void Activate_Cell(const int level,const unsigned long offset,unsigned long mask)
    {Set(level,&Struct_type::flags).Mask(offset,mask);}

    void Activate_Cell(const int level,const T_INDEX& index,unsigned long mask)
    {Set(level,&Struct_type::flags).Mask(*reinterpret_cast<const Index_type*>(&index),mask);}

    void Write_Hierarchy(const std::string& output_directory,const int frame)
    {
        if(frame==0){Write_Metadata(output_directory);
            File_Utilities::Write_To_File(output_directory+"/common/fine_grid",grids[0]);}
        Write_Block_Offsets(output_directory+"/"+std::to_string(frame)+"/block_offsets");
        Write_Channel<unsigned>(output_directory+"/"+std::to_string(frame)+"/flags",&Struct_type::flags);
    }

    void Read_Hierarchy(const std::string& input_directory,const int frame)
    {
        Read_Metadata(input_directory);

        const int levels=Levels();
        std::istream* input1=File_Utilities::Safe_Open_Input(input_directory+"/"+std::to_string(frame)+"/flags");
        std::istream* input2=File_Utilities::Safe_Open_Input(input_directory+"/"+std::to_string(frame)+"/block_offsets");
        for(int level=0;level<levels;++level){unsigned number_of_blocks=0;
            auto block_size=Allocator(level).Block_Size();
            Read_Write<unsigned>::Read(*input2,number_of_blocks);
            for(unsigned block=0;block < number_of_blocks;++block){T_INDEX base_index;
                Read_Write<T_INDEX>::Read(*input2,base_index);
                Range_Iterator<d,T_INDEX> range_iterator(T_INDEX(),*reinterpret_cast<T_INDEX*>(&block_size)-1);
                for(unsigned e=0;e<Flag_array_mask::elements_per_block;++e){
                    unsigned flag;const T_INDEX index=base_index+range_iterator.Index();
                    uint64_t offset=Flag_array_mask::Linear_Offset(index._data);
                    Read_Write<unsigned>::Read(*input1,flag);
                    Set<unsigned>(level,&Struct_type::flags).Mask(offset,flag);
                    range_iterator.Next();}}}

        delete input2;delete input1;
        Update_Block_Offsets();
    }

    void Write_Metadata(const std::string& output_directory)
    {
        std::ostream* output=(d==3)?File_Utilities::Safe_Open_Output(output_directory+"/common/hierarchy.struct3d"):File_Utilities::Safe_Open_Output(output_directory+"/common/hierarchy.struct2d");
        Read_Write<int>::Write(*output,Levels());
        Read_Write<unsigned>::Write(*output,Flag_array_mask::elements_per_block);
        Index_type block_size=Allocator(0).Block_Size();
        for(int v=0;v<d;++v) Read_Write<unsigned>::Write(*output,block_size[v]);
        delete output;
    }

    void Read_Metadata(const std::string& input_directory)
    {
        std::istream* input=(d==3)?File_Utilities::Safe_Open_Input(input_directory+"/common/hierarchy.struct3d"):File_Utilities::Safe_Open_Input(input_directory+"/common/hierarchy.struct2d");
        int levels;Read_Write<int>::Read(*input,levels);
        unsigned elements_per_block;Read_Write<unsigned>::Read(*input,elements_per_block);
        Index_type block_size;
        for(int v=0;v<d;++v) Read_Write<unsigned>::Read(*input,block_size[v]);
        delete input;
        Grid<T,d> grid;
        File_Utilities::Read_From_File(input_directory+"/common/fine_grid",grid);
        Initialize(grid.counts,grid.domain,levels);
    }

    template<class T_FIELD>
    void Write_Channel(const std::string& filename,T_FIELD Struct_type::* channel)
    {
        std::ostream* output=File_Utilities::Safe_Open_Output(filename);
        for(int level=0;level<Levels();++level){
            auto data=Allocator(level).Get_Const_Array(channel);
            for(Block_Iterator iterator(Page_Map(level).Get_Blocks());iterator.Valid();iterator.Next())
                Read_Write<T_FIELD>::Write(*output,data(iterator.Offset()));}
        delete output;
    }

    template<class T_FIELD>
    void Read_Channel(const std::string& filename,T_FIELD Struct_type::* channel)
    {
        std::istream* input=File_Utilities::Safe_Open_Input(filename);
        for(int level=0;level<Levels();++level){
            auto data=Allocator(level).Get_Array(channel);
            for(Block_Iterator iterator(Page_Map(level).Get_Blocks());iterator.Valid();iterator.Next())
                Read_Write<T_FIELD>::Read(*input,data(iterator.Offset()));}
        delete input;
    }

    void Write_Block_Offsets(const std::string& filename)
    {
        std::ostream* output=File_Utilities::Safe_Open_Output(filename);
        for(int level=0;level<Levels();++level){
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
