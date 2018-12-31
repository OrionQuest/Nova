//!#####################################################################
//! \file SPGrid_Multiple_Allocator_Masked_Plus_Equals_Helper.h
//!#####################################################################
// Class Multiple_Allocator_Masked_Plus_Equals_Helper
//######################################################################
#ifndef __Multiple_Allocator_Masked_Plus_Equals_Helper__
#define __Multiple_Allocator_Masked_Plus_Equals_Helper__

#include <nova/SPGrid/Core/SPGrid_Allocator.h>
#include <nova/SPGrid/Tools/SPGrid_Block_Iterator.h>
#include <nova/Tools/Utilities/Pthread_Queue.h>

extern Pthread_Queue* pthread_queue;

namespace SPGrid{
template<class Struct_type,class Data_type,class Flags_type,int d> class Multiple_Allocator_Masked_Plus_Equals_Helper;
//######################################################################
// Multiple_Allocator_Masked_Plus_Equals_Threading_Operation_Helper
//######################################################################
template<class Struct_type,class Data_type,class Flags_type,int d>
struct Multiple_Allocator_Masked_Plus_Equals_Threading_Operation_Helper: public Pthread_Queue::Task
{
    using Block_type            = std::pair<const uint64_t*,unsigned>;

    const Block_type blocks;
    Multiple_Allocator_Masked_Plus_Equals_Helper<Struct_type,Data_type,Flags_type,d>* const obj;

    Multiple_Allocator_Masked_Plus_Equals_Threading_Operation_Helper(const Block_type& blocks_input,Multiple_Allocator_Masked_Plus_Equals_Helper<Struct_type,Data_type,Flags_type,d>* const obj_input)
        :blocks(blocks_input),obj(obj_input)
    {}

    void Run()
    {obj->Operation(blocks);}
};
//######################################################################
// Multiple_Allocator_Masked_Plus_Equals_Helper
//######################################################################
template<class Struct_type,class Data_type,class Flags_type,int d>
class Multiple_Allocator_Masked_Plus_Equals_Helper
{
    using Block_type            = std::pair<const uint64_t*,unsigned>;
    using Allocator_type        = SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask       = typename Allocator_type::template Array_mask<unsigned>;
    using Block_Iterator        = SPGrid_Block_Iterator<Flag_array_mask>;
    using Data_Channel          = Data_type Struct_type::*;
    using Flags_Channel         = Flags_type Struct_type::*;

    Allocator_type& source1_allocator;
    Allocator_type& source2_allocator;
    Allocator_type& destination_allocator;
    Allocator_type& flags_allocator;
    const Block_type blocks;
    Data_Channel source1_field;
    Data_Channel source2_field;
    Data_Channel destination_field;
    Flags_Channel flags_field;
    const unsigned long mask;

  public:
    Multiple_Allocator_Masked_Plus_Equals_Helper(Allocator_type& source1_allocator_input,Allocator_type& source2_allocator_input,Allocator_type& destination_allocator_input,
                                                 Allocator_type& flags_allocator_input,const std::pair<const uint64_t*,unsigned>& blocks_input,Data_Channel source1_field_input,
                                                 Data_Channel source2_field_input,Data_Channel destination_field_input,Flags_Channel flags_field_input,const unsigned long mask_input)
        :source1_allocator(source1_allocator_input),source2_allocator(source2_allocator_input),destination_allocator(destination_allocator_input),flags_allocator(flags_allocator_input),
        blocks(blocks_input),source1_field(source1_field_input),source2_field(source2_field_input),destination_field(destination_field_input),flags_field(flags_field_input),mask(mask_input)
    {}

    void Operation(const std::pair<const uint64_t*,unsigned>& blocks_in) const
    {
        auto source1=source1_allocator.template Get_Const_Array<Struct_type,Data_type>(source1_field);
        auto source2=source2_allocator.template Get_Const_Array<Struct_type,Data_type>(source2_field);
        auto destination=destination_allocator.template Get_Array<Struct_type,Data_type>(destination_field);
        auto flags=flags_allocator.template Get_Const_Array<Struct_type,Flags_type>(flags_field);    

        for(Block_Iterator iterator(blocks_in);iterator.Valid();iterator.Next())
            if(iterator.Data(flags)&mask) iterator.Data(destination)=iterator.Data(source1)+iterator.Data(source2);
    }

    void Run()
    {Operation(blocks);}

    void Run_Parallel(const int number_of_partitions)
    {
        const uint64_t* block_offsets=blocks.first;const int size=blocks.second;
        if(size<number_of_partitions*16){Operation(blocks);return;}
        for(int partition=0;partition<number_of_partitions;partition++){
            int first_index_of_partition=(size/number_of_partitions)*(partition)+std::min(size%number_of_partitions,partition);
            int block_size=(size/number_of_partitions)+((partition<size%number_of_partitions)?1:0);
            Block_type block(block_offsets+first_index_of_partition,block_size);
            Multiple_Allocator_Masked_Plus_Equals_Threading_Operation_Helper<Struct_type,Data_type,Flags_type,d>* task=
                new Multiple_Allocator_Masked_Plus_Equals_Threading_Operation_Helper<Struct_type,Data_type,Flags_type,d>(block,this);
            pthread_queue->Queue(task);}
        pthread_queue->Wait();
    }
};
}
#endif
