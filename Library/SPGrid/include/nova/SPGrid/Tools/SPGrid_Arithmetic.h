//!#####################################################################
//! \file SPGrid_Arithmetic.h
//!#####################################################################
// Various classes for SPGrid arithmetic.
//######################################################################
#ifndef __SPGrid_Arithmetic__
#define __SPGrid_Arithmetic__

#include <nova/SPGrid/Core/SPGrid_Allocator.h>
#include <nova/SPGrid/Tools/SPGrid_Threading_Helper.h>

namespace SPGrid{
template<class Struct_type,class T,int d>
class Masked_Add
{
    using Flags_type            = typename Struct_type::Flags_type;
    using Allocator_type        = SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask       = typename Allocator_type::template Array_mask<unsigned>;

  public:
    Masked_Add(Allocator_type& allocator,const std::pair<const uint64_t*,unsigned>& blocks,T Struct_type::* source1_channel,
               T Struct_type::* source2_channel,T Struct_type::* destination_channel,const unsigned mask)
    {Run(allocator,blocks,source1_channel,source2_channel,destination_channel,mask);}

    void Run(Allocator_type& allocator,const std::pair<const uint64_t*,unsigned>& blocks,T Struct_type::* source1_channel,
             T Struct_type::* source2_channel,T Struct_type::* destination_channel,const unsigned mask) const
    {
        auto source1=allocator.template Get_Const_Array<Struct_type,T>(source1_channel);
        auto source2=allocator.template Get_Const_Array<Struct_type,T>(source2_channel);
        auto destination=allocator.template Get_Array<Struct_type,T>(destination_channel);
        auto flags=allocator.template Get_Const_Array<Struct_type,unsigned>(&Struct_type::flags);

        auto masked_add=[&](uint64_t offset)
        {
            for(int e=0;e<Flag_array_mask::elements_per_block;++e,offset+=sizeof(Flags_type))
                if(flags(offset)&mask) destination(offset)=source1(offset)+source2(offset);
        };

        SPGrid_Computations::Run_Parallel_Blocks(blocks,masked_add);
    }
};

template<class Struct_type,class T,int d>
class Masked_Subtract
{
    using Flags_type            = typename Struct_type::Flags_type;
    using Allocator_type        = SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask       = typename Allocator_type::template Array_mask<unsigned>;

  public:
    Masked_Subtract(Allocator_type& allocator,const std::pair<const uint64_t*,unsigned>& blocks,T Struct_type::* source1_channel,
                    T Struct_type::* source2_channel,T Struct_type::* destination_channel,const unsigned mask)
    {Run(allocator,blocks,source1_channel,source2_channel,destination_channel,mask);}

    void Run(Allocator_type& allocator,const std::pair<const uint64_t*,unsigned>& blocks,T Struct_type::* source1_channel,
             T Struct_type::* source2_channel,T Struct_type::* destination_channel,const unsigned mask) const
    {
        auto source1=allocator.template Get_Const_Array<Struct_type,T>(source1_channel);
        auto source2=allocator.template Get_Const_Array<Struct_type,T>(source2_channel);
        auto destination=allocator.template Get_Array<Struct_type,T>(destination_channel);
        auto flags=allocator.template Get_Const_Array<Struct_type,unsigned>(&Struct_type::flags);

        auto masked_subtract=[&](uint64_t offset)
        {
            for(int e=0;e<Flag_array_mask::elements_per_block;++e,offset+=sizeof(Flags_type))
                if(flags(offset)&mask) destination(offset)=source1(offset)-source2(offset);
        };

        SPGrid_Computations::Run_Parallel_Blocks(blocks,masked_subtract);
    }
};

template<class Struct_type,class T,int d>
class Masked_Scalar_Multiply
{
    using Flags_type            = typename Struct_type::Flags_type;
    using Allocator_type        = SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask       = typename Allocator_type::template Array_mask<unsigned>;

  public:
    Masked_Scalar_Multiply(Allocator_type& allocator,const std::pair<const uint64_t*,unsigned>& blocks,T Struct_type::* source_channel,
                           const T a,T Struct_type::* destination_channel,const unsigned mask)
    {Run(allocator,blocks,source_channel,a,destination_channel,mask);}

    void Run(Allocator_type& allocator,const std::pair<const uint64_t*,unsigned>& blocks,T Struct_type::* source_channel,
             const T a,T Struct_type::* destination_channel,const unsigned mask) const
    {
        auto source=allocator.template Get_Const_Array<Struct_type,T>(source_channel);
        auto destination=allocator.template Get_Array<Struct_type,T>(destination_channel);
        auto flags=allocator.template Get_Const_Array<Struct_type,unsigned>(&Struct_type::flags);

        auto masked_scalar_multiply=[&](uint64_t offset)
        {
            for(int e=0;e<Flag_array_mask::elements_per_block;++e,offset+=sizeof(Flags_type))
                if(flags(offset)&mask) destination(offset)=source(offset)*a;
        };

        SPGrid_Computations::Run_Parallel_Blocks(blocks,masked_scalar_multiply);
    }
};

template<class Struct_type,class T,int d>
class Masked_Saxpy
{
    using Flags_type            = typename Struct_type::Flags_type;
    using Allocator_type        = SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask       = typename Allocator_type::template Array_mask<unsigned>;

  public:
    Masked_Saxpy(Allocator_type& allocator,const std::pair<const uint64_t*,unsigned>& blocks,const T alpha,T Struct_type::* source1_channel,
                 T Struct_type::* source2_channel,T Struct_type::* destination_channel,const unsigned mask)
    {Run(allocator,blocks,alpha,source1_channel,source2_channel,destination_channel,mask);}

    void Run(Allocator_type& allocator,const std::pair<const uint64_t*,unsigned>& blocks,const T alpha,T Struct_type::* source1_channel,
             T Struct_type::* source2_channel,T Struct_type::* destination_channel,const unsigned mask) const
    {
        auto source1=allocator.template Get_Const_Array<Struct_type,T>(source1_channel);
        auto source2=allocator.template Get_Const_Array<Struct_type,T>(source2_channel);
        auto destination=allocator.template Get_Array<Struct_type,T>(destination_channel);
        auto flags=allocator.template Get_Const_Array<Struct_type,unsigned>(&Struct_type::flags);

        auto masked_saxpy=[&](uint64_t offset)
        {
            for(int e=0;e<Flag_array_mask::elements_per_block;++e,offset+=sizeof(Flags_type))
                if(flags(offset)&mask) destination(offset)=alpha*source1(offset)+source2(offset);
        };

        SPGrid_Computations::Run_Parallel_Blocks(blocks,masked_saxpy);
    }
};
}
#endif
