//!#####################################################################
//! \file Read_Write_Quaternion.h
//!#####################################################################
// Class Read_Write_Quaternion
//###################################################################### 
#ifndef __Read_Write_Quaternion__
#define __Read_Write_Quaternion__

#include <nova/Tools/Matrices/Quaternion.h>
#include <nova/Tools/Read_Write/Utilities/Read_Write.h>

namespace Nova{
template<class T>
class Read_Write<Quaternion<T>>
{
  public:
    static void Read(std::istream& input,Quaternion<T>& object)
    {
        Read_Write<T>::Read(input,object.s);
        Read_Write<Vector<T,3>>::Read(input,object.v);
    }

    static void Write(std::ostream& output,const Quaternion<T>& object)
    {
        Read_Write<T>::Write(output,object.s);
        Read_Write<Vector<T,3>>::Write(output,object.v);
    }
};
}
#endif
