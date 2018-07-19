//!#####################################################################
//! \file Read_Write_Matrix.h
//!#####################################################################
// Class Read_Write_Matrix
//###################################################################### 
#ifndef __Read_Write_Matrix__
#define __Read_Write_Matrix__

#include <nova/Tools/Read_Write/Utilities/Read_Write.h>
#include <nova/Tools/Matrices/Matrix.h>

namespace Nova{
template<class T,int d1,int d2>
class Read_Write<Matrix<T,d1,d2>>
{
  public:
    static void Read(std::istream& input,Matrix<T,d1,d2>& object)
    {
        for(int i=0;i<d1;++i) for(int j=0;j<d2;++j) Read_Write<T>::Read(input,object(i,j));
    }

    static void Write(std::ostream& output,const Matrix<T,d1,d2>& object)
    {
        for(int i=0;i<d1;++i) for(int j=0;j<d2;++j) Read_Write<T>::Write(output,object(i,j));
    }
};
}
#endif
