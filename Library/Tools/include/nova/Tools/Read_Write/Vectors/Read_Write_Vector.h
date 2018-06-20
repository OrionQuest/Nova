//!#####################################################################
//! \file Read_Write_Vector.h
//!#####################################################################
// Class Read_Write_Vector
//###################################################################### 
#ifndef __Read_Write_Vector__
#define __Read_Write_Vector__

#include <nova/Tools/Read_Write/Utilities/Read_Write.h>
#include <nova/Tools/Vectors/Vector.h>

namespace Nova{
template<class T,int d>
class Read_Write<Vector<T,d>>
{
  public:
    /*! Read Function
     *
     * Reads the arguments into a vector.
     * \param input Input stream.
     * \param object Vector created with the arguments.
     */
    static void Read(std::istream& input,Vector<T,d>& object)
    {
        for(int v=0;v<d;++v) Read_Write<T>::Read(input,object(v));
    }

    /*! Write Function
     *
     * Writes the arguments of a vector.
     * \param output Output stream.
     * \param object Vector to be written.
     */
    static void Write(std::ostream& output,const Vector<T,d>& object)
    {
        for(int v=0;v<d;++v) Read_Write<T>::Write(output,object(v));
    }
};
}
#endif
