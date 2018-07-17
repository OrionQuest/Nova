//!#####################################################################
//! \file Read_Write_Array.h
//!#####################################################################
// Class Read_Write_Array
//###################################################################### 
#ifndef __Read_Write_Array__
#define __Read_Write_Array__

#include <nova/Tools/Arrays/Array.h>
#include <nova/Tools/Read_Write/Utilities/Read_Write.h>

namespace Nova{
template<class T>
class Read_Write<Array<T>>
{
  public:
    static void Read(std::istream& input,Array<T>& object)
    {
        size_t size;
        Read_Write<size_t>::Read(input,size);
        object.resize(size);
        for(size_t i=0;i<size;++i) Read_Write<T>::Read(input,object[i]);
    }

    static void Write(std::ostream& output,const Array<T>& object)
    {
        Read_Write<size_t>::Write(output,object.size());
        for(size_t i=0;i<object.size();++i) Read_Write<T>::Write(output,object[i]);
    }
};

template<class T>
class Read_Write<Array<Array<T>>>
{
  public:
    static void Read(std::istream& input,Array<Array<T>>& object)
    {
        size_t size;
        Read_Write<size_t>::Read(input,size);
        object.resize(size);
        for(size_t i=0;i<size;++i) Read_Write<Array<T>>::Read(input,object[i]);
    }

    static void Write(std::ostream& output,const Array<Array<T>>& object)
    {
        Read_Write<size_t>::Write(output,object.size());
        for(size_t i=0;i<object.size();++i) Read_Write<Array<T>>::Write(output,object[i]);
    }
};
}
#endif
