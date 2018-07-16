//!#####################################################################
//! \file Read_Write_Range.h
//!#####################################################################
// Class Read_Write_Range
//###################################################################### 
#ifndef __Read_Write_Range__
#define __Read_Write_Range__

#include <nova/Tools/Read_Write/Utilities/Read_Write.h>
#include <nova/Tools/Utilities/Range.h>

namespace Nova{
template<class T,int d>
class Read_Write<Range<T,d>>
{
    typedef Vector<T,d> TV;
  public:
    static void Read(std::istream& input,Range<T,d>& object)
    {
        for(int i=0;i<d;++i){
            Read_Write<T>::Read(input,object.min_corner(i));
            Read_Write<T>::Read(input,object.max_corner(i));}
    }

    static void Write(std::ostream& output,const Range<T,d>& object)
    {
        for(int i=0;i<d;++i){
            Read_Write<T>::Write(output,object.min_corner(i));
            Read_Write<T>::Write(output,object.max_corner(i));}
    }
};
}
#endif
