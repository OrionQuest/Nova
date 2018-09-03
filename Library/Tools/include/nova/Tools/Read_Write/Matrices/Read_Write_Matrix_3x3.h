//!#####################################################################
//! \file Read_Write_Matrix_3x3.h
//!#####################################################################
// Class Read_Write_Matrix_3x3
//###################################################################### 
#ifndef __Read_Write_Matrix_3x3__
#define __Read_Write_Matrix_3x3__

#include <nova/Tools/Read_Write/Utilities/Read_Write.h>
#include <nova/Tools/Matrices/Matrix_3x3.h>

namespace Nova{
template<class T>
class Read_Write<Matrix<T,3>>
{
  public:
    static void Read(std::istream& input,Matrix<T,3>& object)
    {
        for(int i=0;i<9;++i) Read_Write<T>::Read(input,object.x[i]);
    }

    static void Write(std::ostream& output,const Matrix<T,3>& object)
    {
        for(int i=0;i<9;++i) Read_Write<T>::Write(output,object.x[i]);
    }
};
}
#endif
