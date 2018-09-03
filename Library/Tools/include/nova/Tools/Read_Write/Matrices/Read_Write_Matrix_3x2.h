//!#####################################################################
//! \file Read_Write_Matrix_3x2.h
//!#####################################################################
// Class Read_Write_Matrix_3x2
//###################################################################### 
#ifndef __Read_Write_Matrix_3x2__
#define __Read_Write_Matrix_3x2__

#include <nova/Tools/Read_Write/Utilities/Read_Write.h>
#include <nova/Tools/Matrices/Matrix_3x2.h>

namespace Nova{
template<class T>
class Read_Write<Matrix<T,3,2>>
{
  public:
    static void Read(std::istream& input,Matrix<T,3,2>& object)
    {
        for(int i=0;i<6;++i) Read_Write<T>::Read(input,object.x[i]);
    }

    static void Write(std::ostream& output,const Matrix<T,3,2>& object)
    {
        for(int i=0;i<6;++i) Read_Write<T>::Write(output,object.x[i]);
    }
};
}
#endif
