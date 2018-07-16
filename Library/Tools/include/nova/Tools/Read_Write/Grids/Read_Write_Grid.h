//!#####################################################################
//! \file Read_Write_Grid.h
//!#####################################################################
// Class Read_Write_Grid
//###################################################################### 
#ifndef __Read_Write_Grid__
#define __Read_Write_Grid__

#include <nova/Tools/Read_Write/Utilities/Read_Write.h>
#include <nova/Tools/Grids/Grid.h>

namespace Nova{
template<class T,int d>
class Read_Write<Grid<T,d>>
{
    typedef Vector<int,d> T_INDEX;

  public:
    static void Read(std::istream& input,Grid<T,d>& object)
    {
        Read_Write<T_INDEX>::Read(input,object.counts);
        Read_Write<Range<T,d>>::Read(input,object.domain);
        object.Initialize(object.counts,object.domain);
    }

    static void Write(std::ostream& output,const Grid<T,d>& object)
    {
        Read_Write<T_INDEX>::Write(output,object.counts);
        Read_Write<Range<T,d>>::Write(output,object.domain);
        Read_Write<T>::Write(output,(T).5);
    }
};
}
#endif
