//!#####################################################################
//! \file Read_Write_Symmetric_Matrix_2x2.h
//!#####################################################################
// Class Read_Write_Symmetric_Matrix_2x2
//###################################################################### 
#ifndef __Read_Write_Symmetric_Matrix_2x2__
#define __Read_Write_Symmetric_Matrix_2x2__

#include <nova/Tools/Read_Write/Utilities/Read_Write.h>
#include <nova/Tools/Matrices/Symmetric_Matrix_2x2.h>

namespace Nova{
template<class T>
class Read_Write<Symmetric_Matrix<T,2>>
{
  public:
    static void Read(std::istream& input,Symmetric_Matrix<T,2>& object)
    {
        Read_Write<T>::Read(input,object.x11);
        Read_Write<T>::Read(input,object.x21);
        Read_Write<T>::Read(input,object.x22);
    }

    static void Write(std::ostream& output,const Symmetric_Matrix<T,2>& object)
    {
        Read_Write<T>::Write(output,object.x11);
        Read_Write<T>::Write(output,object.x21);
        Read_Write<T>::Write(output,object.x22);
    }
};
}
#endif
