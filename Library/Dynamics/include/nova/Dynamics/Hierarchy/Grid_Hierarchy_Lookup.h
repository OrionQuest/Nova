//!#####################################################################
//! \file Grid_Hierarchy_Lookup.h
//!#####################################################################
// Class Grid_Hierarchy_Lookup
//######################################################################
#ifndef __Grid_Hierarchy_Lookup__
#define __Grid_Hierarchy_Lookup__

#include <nova/Dynamics/Hierarchy/Grid_Hierarchy.h>
#include <nova/Dynamics/Utilities/SPGrid_Flags.h>

namespace Nova{
template<class Struct_type,class T,int d>
class Grid_Hierarchy_Lookup
{
    enum {log2_struct=SPGrid::NextLogTwo<sizeof(Struct_type)>::value};
    using TV                    = Vector<T,d>;
    using TV2                   = Vector<T,d-1>;
    using T_INDEX               = Vector<int,d>;
    using Hierarchy             = Grid_Hierarchy<Struct_type,T,d>;
    using Allocator_type        = SPGrid::SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask       = typename Allocator_type::template Array_mask<unsigned>;

    template<class T_MASK> struct Cell_Parity_Helper {};
    template<int log2_field>
    struct Cell_Parity_Helper<SPGrid::SPGrid_Mask<log2_struct,log2_field,2> >
    {
        enum{
            x_mask=SPGrid::SPGrid_Mask<log2_struct,log2_field,2>::template LinearOffset<1,0>::value,
            y_mask=SPGrid::SPGrid_Mask<log2_struct,log2_field,2>::template LinearOffset<0,1>::value,
            z_mask=0UL
        };
    };    
    template<int log2_field>
    struct Cell_Parity_Helper<SPGrid::SPGrid_Mask<log2_struct,log2_field,3> >
    {
        enum{
            x_mask=SPGrid::SPGrid_Mask<log2_struct,log2_field,3>::template LinearOffset<1,0,0>::value,
            y_mask=SPGrid::SPGrid_Mask<log2_struct,log2_field,3>::template LinearOffset<0,1,0>::value,
            z_mask=SPGrid::SPGrid_Mask<log2_struct,log2_field,3>::template LinearOffset<0,0,1>::value
        };
    };

  public:

    static bool Cell_Lookup(const Hierarchy& hierarchy,const TV& X,uint64_t& flags_offset,int& level)
    {
        TV weights;
        return Cell_Lookup(hierarchy,X,flags_offset,level,weights);
    }

    static bool Cell_Lookup(const Hierarchy& hierarchy,const TV& X,uint64_t& flags_offset,int &level,TV& weights)
    {
        level=0;
        T_INDEX index=hierarchy.Lattice(level).Clamp_To_Cell(X);
        TV node_location=hierarchy.Lattice(level).Node(index);
        weights=(X-node_location)*hierarchy.Lattice(level).one_over_dX;
        flags_offset=Flag_array_mask::Linear_Offset(index._data);
        return Cell_Lookup(hierarchy,flags_offset,level,weights);
    }

    static bool Cell_Lookup(const Hierarchy& hierarchy,uint64_t& flags_offset,int& level,TV& weights)
    {
        if(hierarchy.template Set<unsigned>(level,&Struct_type::flags).Is_Set(flags_offset,Cell_Type_Interior|Cell_Type_Dirichlet)) return true;
        if(Cell_Lookup_Finer(hierarchy,flags_offset,level,weights)) return true;
        return Cell_Lookup_Coarser(hierarchy,flags_offset,level,weights);
    }

    static bool Cell_Lookup_Finer(const Hierarchy& hierarchy,uint64_t& flags_offset,int& level,TV& weights)
    {
        uint64_t candidate_flags_offset=flags_offset;
        TV candidate_weights=weights;

        for(int candidate_level=level-1;candidate_level>=0;--candidate_level){
            // upsample offset
            candidate_flags_offset=Flag_array_mask::UpsampleOffset(candidate_flags_offset);
            uint64_t child_offset=0UL;

            // adjust new weights and offset by old weights
            if(candidate_weights[0] >= (T).5){
                child_offset |= Cell_Parity_Helper<Flag_array_mask>::x_mask;
                candidate_weights[0]-=(T).5;}

            if(candidate_weights[1] >= (T).5){
                child_offset |= Cell_Parity_Helper<Flag_array_mask>::y_mask;
                candidate_weights[1]-=(T).5;}

            if(d==3 && candidate_weights[2] >= (T).5){
                child_offset |= Cell_Parity_Helper<Flag_array_mask>::z_mask;
                candidate_weights[2]-=(T).5;}
            candidate_flags_offset=Flag_array_mask::Packed_Add(candidate_flags_offset,child_offset);

            // upsample weights
            candidate_weights*=(T)2.;

            // check for this offset being interior
            if(hierarchy.template Set<unsigned>(candidate_level,&Struct_type::flags).Is_Set(candidate_flags_offset,Cell_Type_Interior|Cell_Type_Dirichlet)){
                flags_offset=candidate_flags_offset;
                level=candidate_level;
                weights=candidate_weights;
                return true;}}

        return false;
    }

    static bool Cell_Lookup_Coarser(const Hierarchy& hierarchy,uint64_t& flags_offset,int& level,TV& weights)
    {
        uint64_t candidate_flags_offset=flags_offset;
        TV candidate_weights=weights;

        for(unsigned candidate_level=level+1;candidate_level<hierarchy.Levels();++candidate_level){
            // downsample weights
            candidate_weights*=(T).5;
            if((flags_offset & Cell_Parity_Helper<Flag_array_mask>::x_mask) == 0) candidate_weights[0]+=(T).5;
            if((flags_offset & Cell_Parity_Helper<Flag_array_mask>::y_mask) == 0) candidate_weights[1]+=(T).5;
            if(d==3) if((flags_offset & Cell_Parity_Helper<Flag_array_mask>::z_mask) == 0) candidate_weights[2]+=(T).5;

            // downsample offset
            candidate_flags_offset=Flag_array_mask::DownsampleOffset(candidate_flags_offset);

            // check for this offset being interior
            if(hierarchy.template Set<unsigned>(candidate_level,&Struct_type::flags).Is_Set(candidate_flags_offset,Cell_Type_Interior|Cell_Type_Dirichlet)){
                flags_offset=candidate_flags_offset;
                level=candidate_level;
                weights=candidate_weights;
                return true;}}

        return false;
    }

    static bool Face_Lookup(const Hierarchy& hierarchy,unsigned long &flags_offset,int &level,TV2& weights,const unsigned mask,const int axis)
    {
        if(hierarchy.template Set<unsigned>(level,&Struct_type::flags).Is_Set(flags_offset,mask)) return true;
        return Face_Lookup_Finer(hierarchy,flags_offset,level,weights,mask,axis);
    }

    static bool Face_Lookup_Finer(const Hierarchy& hierarchy,uint64_t& flags_offset,int& level,TV2& weights,const unsigned mask,const int axis)
    {
        uint64_t candidate_flags_offset=flags_offset;
        TV2 candidate_weights=weights;

        for(int candidate_level=level-1;candidate_level>=0;--candidate_level){
            // upsample offset
            candidate_flags_offset=Flag_array_mask::UpsampleOffset(candidate_flags_offset);
            uint64_t child_offset=0UL;
            // adjust new weights and offset by old weights
            if(candidate_weights[0] >= (T).5){
                child_offset|=(axis==0)?Cell_Parity_Helper<Flag_array_mask>::y_mask:Cell_Parity_Helper<Flag_array_mask>::x_mask;
                candidate_weights[0]-=(T).5;}
            if(d==3) if(candidate_weights[1] >= (T).5){
                child_offset |= (axis==2)?Cell_Parity_Helper<Flag_array_mask>::y_mask:Cell_Parity_Helper<Flag_array_mask>::z_mask;
                candidate_weights[1]-=(T).5;}
            candidate_flags_offset=Flag_array_mask::Packed_Add(candidate_flags_offset,child_offset);

            // upsample weights
            candidate_weights*=(T)2.;

            // check for this offset being interior
            if(hierarchy.template Set<unsigned>(candidate_level,&Struct_type::flags).Is_Set(candidate_flags_offset,mask)){
                flags_offset=candidate_flags_offset;
                level=candidate_level;
                weights=candidate_weights;
                return true;}}

        return false;
    }
};
}
#endif
