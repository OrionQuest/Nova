//!#####################################################################
//! \file Read_Write_Complex.h
//!#####################################################################
// Class Read_Write_Complex
//###################################################################### 
#ifndef __Read_Write_Complex__
#define __Read_Write_Complex__

#include <nova/Tools/Read_Write/Utilities/Read_Write.h>
#include <nova/Tools/Vectors/Complex.h>

namespace Nova{
template<class T>
class Read_Write<Complex<T>>
{
  public:
    static void Read(std::istream& input,Complex<T>& object)
    {
        Read_Write<T>::Read(input,object.re);
        Read_Write<T>::Read(input,object.im);
    }

    static void Write(std::ostream& output,const Complex<T>& object)
    {
        Read_Write<T>::Write(output,object.re);
        Read_Write<T>::Write(output,object.im);
    }
};
}
#endif
