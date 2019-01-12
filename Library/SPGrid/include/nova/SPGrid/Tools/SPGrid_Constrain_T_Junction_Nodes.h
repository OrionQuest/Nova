//!#####################################################################
//! \file SPGrid_Constrain_T_Junction_Nodes.h
//!#####################################################################
// Class Constrain_T_Junction_Nodes
//######################################################################
#ifndef __Constrain_T_Junction_Nodes__
#define __Constrain_T_Junction_Nodes__

#include <nova/Dynamics/Hierarchy/Grid_Hierarchy.h>
#include <nova/Dynamics/Utilities/SPGrid_Flags.h>
#include <nova/SPGrid/Tools/SPGrid_Block_Iterator.h>
#include <nova/Tools/Log/Debug_Utilities.h>

namespace SPGrid{

template<class T_Mask,int d> struct Constrain_T_Junction_Nodes_Helper;

template<class T_Mask>
struct Constrain_T_Junction_Nodes_Helper<T_Mask,2>
{
    enum{odd_bits=T_Mask::template LinearOffset<1,1>::value,
        Parity_00=T_Mask::template LinearOffset<0,0>::value,
        Parity_01=T_Mask::template LinearOffset<0,1>::value,
        Parity_10=T_Mask::template LinearOffset<1,0>::value,
        Parity_11=T_Mask::template LinearOffset<1,1>::value};

    enum{plus_x =T_Mask::template LinearOffset< 1, 0>::value,
        plus_y  =T_Mask::template LinearOffset< 0, 1>::value,
        minus_x =T_Mask::template LinearOffset<-1, 0>::value,
        minus_y =T_Mask::template LinearOffset< 0,-1>::value,};

    static inline void Find_Neighboring_Nodes(const uint64_t offset,uint64_t* neighbor_offsets,int& m)
    {
        const uint64_t parity=(offset&odd_bits);
        const uint64_t coarse_offset=T_Mask::DownsampleOffset(offset);

        if(parity==Parity_11){m=1;
            neighbor_offsets[0]=coarse_offset;}
        else if(parity==Parity_10){m=2;
            neighbor_offsets[0]=coarse_offset;
            neighbor_offsets[1]=T_Mask::Packed_Add(coarse_offset,plus_y);}
        else if(parity==Parity_01){m=2;
            neighbor_offsets[0]=coarse_offset;
            neighbor_offsets[1]=T_Mask::Packed_Add(coarse_offset,plus_x);}
        else if(parity==Parity_00) Nova::FATAL_ERROR("Invalid arrangement for T-Junction node!");
        else Nova::FATAL_ERROR("Should never reach this point!");
    }
};

template<class T_Mask>
struct Constrain_T_Junction_Nodes_Helper<T_Mask,3>
{
    enum{odd_bits=T_Mask::template LinearOffset<1,1,1>::value,
        Parity_000=T_Mask::template LinearOffset<0,0,0>::value,
        Parity_001=T_Mask::template LinearOffset<0,0,1>::value,
        Parity_010=T_Mask::template LinearOffset<0,1,0>::value,
        Parity_011=T_Mask::template LinearOffset<0,1,1>::value,
        Parity_100=T_Mask::template LinearOffset<1,0,0>::value,
        Parity_101=T_Mask::template LinearOffset<1,0,1>::value,
        Parity_110=T_Mask::template LinearOffset<1,1,0>::value,
        Parity_111=T_Mask::template LinearOffset<1,1,1>::value};

    enum{plus_x       = T_Mask::template LinearOffset< 1, 0, 0>::value,
        plus_y        = T_Mask::template LinearOffset< 0, 1, 0>::value,
        plus_z        = T_Mask::template LinearOffset< 0, 0, 1>::value,
        plus_x_and_y  = T_Mask::template LinearOffset< 1, 1, 0>::value,
        plus_x_and_z  = T_Mask::template LinearOffset< 1, 0, 1>::value,
        plus_y_and_z  = T_Mask::template LinearOffset< 0, 1, 1>::value,
        minus_x       = T_Mask::template LinearOffset<-1, 0, 0>::value,
        minus_y       = T_Mask::template LinearOffset< 0,-1, 0>::value,
        minus_z       = T_Mask::template LinearOffset< 0, 0,-1>::value,
        minus_x_and_y = T_Mask::template LinearOffset<-1,-1, 0>::value,
        minus_x_and_z = T_Mask::template LinearOffset<-1, 0,-1>::value,
        minus_y_and_z = T_Mask::template LinearOffset< 0,-1,-1>::value};

    static inline void Find_Neighboring_Nodes(const uint64_t offset,uint64_t* neighbor_offsets,int& m)
    {
        const uint64_t parity=(offset&odd_bits);
        const uint64_t coarse_offset=T_Mask::DownsampleOffset(offset);

        if(parity==Parity_111){m=1;
            neighbor_offsets[0]=coarse_offset;}
        else if(parity==Parity_110){m=2;
            neighbor_offsets[0]=coarse_offset;
            neighbor_offsets[1]=T_Mask::Packed_Add(coarse_offset,plus_z);}
        else if(parity==Parity_101){m=2;
            neighbor_offsets[0]=coarse_offset;
            neighbor_offsets[1]=T_Mask::Packed_Add(coarse_offset,plus_y);}
        else if(parity==Parity_011){m=2;
            neighbor_offsets[0]=coarse_offset;
            neighbor_offsets[1]=T_Mask::Packed_Add(coarse_offset,plus_x);}
        else if(parity==Parity_100){m=4;
            neighbor_offsets[0]=coarse_offset;
            neighbor_offsets[1]=T_Mask::Packed_Add(coarse_offset,plus_y);
            neighbor_offsets[2]=T_Mask::Packed_Add(coarse_offset,plus_z);
            neighbor_offsets[3]=T_Mask::Packed_Add(coarse_offset,plus_y_and_z);}
        else if(parity==Parity_010){m=4;
            neighbor_offsets[0]=coarse_offset;
            neighbor_offsets[1]=T_Mask::Packed_Add(coarse_offset,plus_x);
            neighbor_offsets[2]=T_Mask::Packed_Add(coarse_offset,plus_z);
            neighbor_offsets[3]=T_Mask::Packed_Add(coarse_offset,plus_x_and_z);}
        else if(parity==Parity_001){m=4;
            neighbor_offsets[0]=coarse_offset;
            neighbor_offsets[1]=T_Mask::Packed_Add(coarse_offset,plus_x);
            neighbor_offsets[2]=T_Mask::Packed_Add(coarse_offset,plus_y);
            neighbor_offsets[3]=T_Mask::Packed_Add(coarse_offset,plus_x_and_y);}
        else if(parity==Parity_000) Nova::FATAL_ERROR("Invalid arrangement for T-Junction node!");
        else Nova::FATAL_ERROR("Should never reach this point!");
    }
};

template<class Struct_type,class T,int d>
class Constrain_T_Junction_Nodes
{
    using Flags_type                = typename Struct_type::Flags_type;
    using Hierarchy                 = Nova::Grid_Hierarchy<Struct_type,T,d>;
    using Allocator_type            = SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask           = typename Allocator_type::template Array_mask<unsigned>;
    using Block_Iterator            = SPGrid_Block_Iterator<Flag_array_mask>;

    static const int max_neighbors  = (d==2)?2:4;
    uint64_t *neighbor_offsets;

  public:
    Constrain_T_Junction_Nodes(Hierarchy& hierarchy,const std::pair<const uint64_t*,unsigned>& blocks,T Struct_type::* node_channel,const int level)
    {
        neighbor_offsets=new uint64_t[max_neighbors];
        Run(hierarchy,blocks,node_channel,level);
    }

    void Run(Hierarchy& hierarchy,const std::pair<const uint64_t*,unsigned>& blocks,T Struct_type::* node_channel,const int level) const
    {
        auto data=hierarchy.Allocator(level).template Get_Array<Struct_type,T>(node_channel);
        auto flags=hierarchy.Allocator(level).template Get_Const_Array<Struct_type,unsigned>(&Struct_type::flags);

        for(Block_Iterator iterator(blocks);iterator.Valid();iterator.Next()){uint64_t offset=iterator.Offset();
            if(flags(offset)&(Nova::Node_T_Junction)){int m=0;
                Constrain_T_Junction_Nodes_Helper<Flag_array_mask,d>::Find_Neighboring_Nodes(offset,neighbor_offsets,m);
                double value=(double)0;
                for(int node=0;node<m;node++) value+=(double)hierarchy.Channel(level+1,node_channel)(neighbor_offsets[node]);
                value/=(double)m;
                data(offset)=value;}}
    }
};
}
#endif
