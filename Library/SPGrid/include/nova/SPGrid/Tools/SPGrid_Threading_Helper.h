//!#####################################################################
//! \file SPGrid_Threading_Helper.h
//!#####################################################################
// Class SPGrid_Threading_Helper
//######################################################################
#ifndef __SPGrid_Threading_Helper__
#define __SPGrid_Threading_Helper__

#include <nova/SPGrid/Core/SPGrid_Allocator.h>
#include <nova/Tools/Utilities/Pthread_Queue.h>
#include <vector>

extern Pthread_Queue* pthread_queue;

namespace SPGrid_Computations{

using namespace SPGrid;

template<class Struct_type,int d,class T_Operation>
struct Threading_Operation_Helper: public Pthread_Queue::Task
{
    typedef std::pair<const unsigned long*,unsigned> T_Block;

    SPGrid_Allocator<Struct_type,d>& allocator;
    const T_Block blocks;
    const T_Operation& operation;

    Threading_Operation_Helper(SPGrid_Allocator<Struct_type,d>& allocator_input,const T_Block& blocks_input,const T_Operation& operation_input)
        :allocator(allocator_input),blocks(blocks_input),operation(operation_input)
    {}

    void Run(){operation.Run(allocator,blocks);}
};

template<class Struct_type,int d>
class Threading_Helper
{
    typedef std::pair<const unsigned long*,unsigned> T_Block;
    
    SPGrid_Allocator<Struct_type,d>& allocator;
    const T_Block& blocks;

  public:
    Threading_Helper(SPGrid_Allocator<Struct_type,d>& allocator_input,const T_Block& blocks_input)
        :allocator(allocator_input),blocks(blocks_input)
    {}

    template<class T_Operation>
    void Run_Parallel(const T_Operation& operation,std::vector<T_Block> list_of_partitions)
    {for(int partition=0;partition<list_of_partitions.size();partition++){     
     Threading_Operation_Helper<Struct_type,d,T_Operation>* task=
         new Threading_Operation_Helper<Struct_type,d,T_Operation>(allocator,list_of_partitions[partition],operation);     
     pthread_queue->Queue(task);}
     pthread_queue->Wait();}

    template<class T_Operation>
    void Run_Parallel(const T_Operation& operation,const int number_of_partitions)
    {const unsigned long* block_offsets=blocks.first;
    const int size=blocks.second;
    if(size<number_of_partitions*16){operation.Run(allocator,blocks);return;}
    for(int partition=0;partition<number_of_partitions;partition++){
        int first_index_of_partition=(size/number_of_partitions)*(partition)+std::min(size%number_of_partitions,partition);
        int block_size=(size/number_of_partitions)+((partition<size%number_of_partitions)?1:0);
        T_Block block(block_offsets+first_index_of_partition,block_size);
        Threading_Operation_Helper<Struct_type,d,T_Operation>* task=
            new Threading_Operation_Helper<Struct_type,d,T_Operation>(allocator,block,operation);
        pthread_queue->Queue(task);}
    pthread_queue->Wait();}
};

template<class Functor>
void Run_Parallel_Blocks(const std::pair<const uint64_t*,uint32_t>& blocks,Functor functor)
{
#pragma omp parallel for
    for(int b=0;b<blocks.second;b++)
        functor(blocks.first[b]);
}
}
#endif
