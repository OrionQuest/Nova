//!#####################################################################
//! \file Read_Write_Rotation.h
//!#####################################################################
// Class Read_Write_Rotation
//###################################################################### 
#ifndef __Read_Write_Rotation__
#define __Read_Write_Rotation__

#include <nova/Tools/Matrices/Rotation.h>
#include <nova/Tools/Read_Write/Utilities/Read_Write.h>

namespace Nova{
template<class T,int d>
class Read_Write<Rotation<T,d>>
{
  public:
    static void Read(std::istream& input,Rotation<T,2>& object)
    {
        Read_Write<Complex<T>>::Read(input,object.c);
    }

    static void Read(std::istream& input,Rotation<T,3>& object)
    {
        Read_Write<Quaternion<T>>::Read(input,object.q);
    }

    static void Write(std::ostream& output,const Rotation<T,2>& object)
    {
        Read_Write<Complex<T>>::Write(output,object.c);
    }

    static void Write(std::ostream& output,const Rotation<T,3>& object)
    {
        Read_Write<Quaternion<T>>::Write(output,object.q);
    }
};
}
#endif
