//!#####################################################################
//! \file SPGrid_Masked_Average_Offset_Grid.h
//!#####################################################################
// Class Masked_Average_Offset_Grid
//######################################################################
#ifndef __Masked_Average_Offset_Grid__
#define __Masked_Average_Offset_Grid__

#include <nova/SPGrid/Tools/Shadow_Grid_Helper.h>
#include <nova/SPGrid/Tools/SPGrid_Block_Iterator.h>
#include <stdlib.h>

namespace SPGrid{
template<int stencil_size,class Struct_type,class Data_type,class Flags_type,int d> struct Masked_Average_Offset_Grid_Helper;

template<int stencil_size,class Struct_type,class Data_type,class Flags_type>
struct Masked_Average_Offset_Grid_Helper<stencil_size,Struct_type,Data_type,Flags_type,2>
{
    enum {d=2};
    using Allocator_type            = SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask           = typename Allocator_type::template Array_mask<unsigned>;

    enum {
        block_xsize = 1u << Flag_array_mask::block_xbits,
        block_ysize = 1u << Flag_array_mask::block_ybits,
        og_xsize = block_xsize+2,
        og_ysize = block_ysize+2,
        xmin = 1,
        ymin = 1,
        xmax = og_xsize-2,
        ymax = og_ysize-2};
    typedef uint64_t (&offset_grid_type)[og_xsize][og_ysize];

    static inline void Run_Block(const Data_type* const source,const uint64_t destination_base_addr,const uint64_t weights_base_addr,
        const Flags_type* const flags,const Data_type weight,const Flags_type mask,const uint64_t offset,const int (&stencil)[stencil_size])
    {
        uint64_t* offset_grid_ptr = (uint64_t*)malloc( (og_xsize) * (og_ysize) * sizeof(uint64_t));
        offset_grid_type o_grid = reinterpret_cast<offset_grid_type>(*offset_grid_ptr);
        Shadow_Grid_Helper<Struct_type,Data_type,d>::Compute_Shadow_Grid(offset_grid_ptr,offset);
        int cur_index = 0;
        // Actually process elements
        for(int i=xmin;i<=xmax;i++) for(int j=ymin;j<=ymax;j++){unsigned flag = flags[cur_index];
            if( flag & mask ){const Data_type value=weight*source[cur_index];
                for(int s=0;s<stencil_size;s++){
                    (*reinterpret_cast<Data_type*>(destination_base_addr + *(&o_grid[i][j]+stencil[s]) ))+=value;
                    (*reinterpret_cast<Data_type*>(weights_base_addr     + *(&o_grid[i][j]+stencil[s]) ))+=weight;}}
            cur_index++;}
        free(offset_grid_ptr);
    }
};

template<int stencil_size,class Struct_type,class Data_type,class Flags_type>
struct Masked_Average_Offset_Grid_Helper<stencil_size,Struct_type,Data_type,Flags_type,3>
{
    enum {d=3};
    using Allocator_type            = SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask           = typename Allocator_type::template Array_mask<unsigned>;

    enum {
        block_xsize = 1u << Flag_array_mask::block_xbits,
        block_ysize = 1u << Flag_array_mask::block_ybits,
        block_zsize = 1u << Flag_array_mask::block_zbits,
        og_xsize = block_xsize+2,
        og_ysize = block_ysize+2,
        og_zsize = block_zsize+2,
        xmin = 1,
        ymin = 1,
        zmin = 1,
        xmax = og_xsize-2,
        ymax = og_ysize-2,
        zmax = og_zsize-2};
    typedef uint64_t (&offset_grid_type)[og_xsize][og_ysize][og_zsize];

    static inline void Run_Block(const Data_type* const source,const uint64_t destination_base_addr,const uint64_t weights_base_addr,
        const Flags_type* const flags,const Data_type weight,const Flags_type mask,const uint64_t offset,const int (&stencil)[stencil_size])
    {
        uint64_t* offset_grid_ptr = (uint64_t*)malloc( (og_xsize) * (og_ysize) * (og_zsize) * sizeof(uint64_t));
        offset_grid_type o_grid = reinterpret_cast<offset_grid_type>(*offset_grid_ptr);
        Shadow_Grid_Helper<Struct_type,Data_type,d>::Compute_Shadow_Grid(offset_grid_ptr,offset);
        int cur_index = 0;
        // Actually process elements
        for(int i=xmin;i<=xmax;i++) for(int j=ymin;j<=ymax;j++) for(int k=zmin;k<=zmax;k++){unsigned flag = flags[cur_index];
            if( flag & mask ){const Data_type value=weight*source[cur_index];
                for(int s=0;s<stencil_size;s++){
                    (*reinterpret_cast<Data_type*>(destination_base_addr + *(&o_grid[i][j][k]+stencil[s]) ))+=value;
                    (*reinterpret_cast<Data_type*>(weights_base_addr     + *(&o_grid[i][j][k]+stencil[s]) ))+=weight;}}
            cur_index++;}
        free(offset_grid_ptr);
    }
};

template<int stencil_size,class Struct_type,class Data_type,class Flags_type,int d>
class Masked_Average_Offset_Grid
{
    using Data_Field                = Data_type Struct_type::*;
    using Flags_Field               = Flags_type Struct_type::*;
    using Allocator_type            = SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask           = typename Allocator_type::template Array_mask<unsigned>;
    using Block_Iterator            = SPGrid_Block_Iterator<Flag_array_mask>;

    Data_Field source_field;
    Data_Field destination_field;
    Data_Field weights_field;
    Flags_Field flags_field;
    const int (&stencil)[stencil_size];
    const Flags_type mask;
    const Data_type weight;

  public:
    Masked_Average_Offset_Grid(Data_Field source_field_input,Data_Field destination_field_input,Data_Field weights_field_input,
                               Flags_Field flags_field_input,const int (&stencil_input)[stencil_size],const Flags_type mask_input,
                               const Data_type weight_input)
        :source_field(source_field_input),destination_field(destination_field_input),weights_field(weights_field_input),
        flags_field(flags_field_input),stencil(stencil_input),mask(mask_input),weight(weight_input)
    {}

    Masked_Average_Offset_Grid(Allocator_type& allocator,const std::pair<const uint64_t*,unsigned>& blocks,Data_Field source_field_input,
                               Data_Field destination_field_input,Data_Field weights_field_input,Flags_Field flags_field_input,
                               const int (&stencil_input)[stencil_size],const Flags_type mask_input,const Data_type weight_input)
        :source_field(source_field_input),destination_field(destination_field_input),weights_field(weights_field_input),
        flags_field(flags_field_input),stencil(stencil_input),mask(mask_input),weight(weight_input)
    {Run(allocator,blocks);}

    void Run(Allocator_type& allocator,const std::pair<const uint64_t*,unsigned>& blocks) const
    {
        auto source=allocator.template Get_Const_Array<Struct_type,Data_type>(source_field);
        auto destination=allocator.template Get_Array<Struct_type,Data_type>(destination_field);
        auto weights=allocator.template Get_Array<Struct_type,Data_type>(weights_field);
        auto flags=allocator.template Get_Const_Array<Struct_type,Flags_type>(flags_field);
        uint64_t destination_base_addr = reinterpret_cast<uint64_t>(destination.Get_Data_Ptr());
        uint64_t weights_base_addr = reinterpret_cast<uint64_t>(weights.Get_Data_Ptr());

        for(Block_Iterator iterator(blocks);iterator.Valid();iterator.Next_Block())
            Masked_Average_Offset_Grid_Helper<stencil_size,Struct_type,Data_type,Flags_type,d>::Run_Block(
                &iterator.Data(source),destination_base_addr,weights_base_addr,&iterator.Data(flags),weight,mask,iterator.Offset(),stencil);
    }
};
}
#endif
