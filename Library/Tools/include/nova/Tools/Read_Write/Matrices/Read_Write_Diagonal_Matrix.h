//!#####################################################################
//! \file Read_Write_Diagonal_Matrix.h
//!#####################################################################
// Class Read_Write_Diagonal_Matrix
//###################################################################### 
#ifndef __Read_Write_Diagonal_Matrix__
#define __Read_Write_Diagonal_Matrix__

#include <nova/Tools/Read_Write/Utilities/Read_Write.h>
#include <nova/Tools/Matrices/Diagonal_Matrix.h>

namespace Nova{
template<class T,int d>
class Read_Write<Diagonal_Matrix<T,d>>
{
    using TV            = Vector<T,d>;

  public:
    static void Read(std::istream& input,Diagonal_Matrix<T,d>& object)
    {Read_Write<TV>::Read(input,object._data);}

    static void Write(std::ostream& output,const Diagonal_Matrix<T,d>& object)
    {Read_Write<TV>::Write(output,object._data);}
};
}
#endif
