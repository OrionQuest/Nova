//!#####################################################################
//! \file Read_Write_Array_ND.h
//!#####################################################################
// Class Read_Write_Array_ND
//###################################################################### 
#ifndef __Read_Write_Array_ND__
#define __Read_Write_Array_ND__

#include <nova/Tools/Arrays/Array_ND.h>
#include <nova/Tools/Read_Write/Arrays/Read_Write_Array.h>

namespace Nova{
template<class T,int d>
class Read_Write<Array_ND<T,d>>
{
  public:
    static void Read(std::istream& input,Array_ND<T,d>& object)
    {
        Read_Write<Range<int,d>>::Read(input,object.domain);
        Read_Write<Vector<int,d>>::Read(input,object.counts);
        Read_Write<Array<T>>::Read(input,object.data);
    }

    static void Write(std::ostream& output,const Array_ND<T,d>& object)
    {
        Read_Write<Range<int,d>>::Write(output,object.domain);
        Read_Write<Vector<int,d>>::Write(output,object.counts);
        Read_Write<Array<T>>::Write(output,object.data);
    }
};
}
#endif
