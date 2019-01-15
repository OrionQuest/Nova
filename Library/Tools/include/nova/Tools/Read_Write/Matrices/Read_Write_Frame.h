//!#####################################################################
//! \file Read_Write_Frame.h
//!#####################################################################
// Class Read_Write_Frame
//###################################################################### 
#ifndef __Read_Write_Frame__
#define __Read_Write_Frame__

#include <nova/Tools/Matrices/Frame.h>
#include <nova/Tools/Read_Write/Utilities/Read_Write.h>

namespace Nova{
template<class T,int d>
class Read_Write<Frame<T,d>>
{
    using TV            = Vector<T,d>;

  public:
    static void Read(std::istream& input,Frame<T,d>& object)
    {
        Read_Write<TV>::Read(input,object.t);
        Read_Write<Rotation<T,d>>::Read(input,object.r);
    }

    static void Write(std::ostream& output,const Frame<T,d>& object)
    {
        Read_Write<TV>::Write(output,object.t);
        Read_Write<Rotation<T,d>>::Write(output,object.r);
    }
};
}
#endif
