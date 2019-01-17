//!#####################################################################
//! \file Read_Write_Matrix_1x1.h
//!#####################################################################
// Class Read_Write_Matrix_1x1
//###################################################################### 
#ifndef __Read_Write_Matrix_1x1__
#define __Read_Write_Matrix_1x1__

#include <nova/Tools/Read_Write/Utilities/Read_Write.h>
#include <nova/Tools/Matrices/Matrix_1x1.h>

namespace Nova{
template<class T>
class Read_Write<Matrix<T,1>>
{
  public:
    static void Read(std::istream& input,Matrix<T,1>& object)
    {Read_Write<T>::Read(input,object.x11);}

    static void Write(std::ostream& output,const Matrix<T,1>& object)
    {Read_Write<T>::Write(output,object.x11);}
};
}
#endif
