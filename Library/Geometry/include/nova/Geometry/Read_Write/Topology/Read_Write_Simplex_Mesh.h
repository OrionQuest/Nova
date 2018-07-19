//!#####################################################################
//! \file Read_Write_Simplex_Mesh.h
//!#####################################################################
// Class Read_Write_Simplex_Mesh
//###################################################################### 
#ifndef __Read_Write_Simplex_Mesh__
#define __Read_Write_Simplex_Mesh__

#include <nova/Geometry/Topology/Simplex_Mesh.h>
#include <nova/Tools/Read_Write/Utilities/Read_Write.h>

namespace Nova{
template<int d>
class Read_Write<Simplex_Mesh<d>>
{
    typedef Vector<int,d> INDEX;

  public:
    static void Read(std::istream& input,Simplex_Mesh<d>& object)
    {
        size_t number_of_elements;
        Read_Write<size_t>::Read(input,object.number_of_nodes);
        Read_Write<size_t>::Read(input,number_of_elements);
        object.Resize_Elements(number_of_elements);
        for(size_t i=0;i<number_of_elements;++i)
            Read_Write<INDEX>::Read(input,object.elements[i]);
    }

    static void Write(std::ostream& output,const Simplex_Mesh<d>& object)
    {
        Read_Write<size_t>::Write(output,object.number_of_nodes);
        Read_Write<size_t>::Write(output,object.elements.size());
        for(size_t i=0;i<object.elements.size();++i) Read_Write<INDEX>::Write(output,object.elements[i]);
    }
};
}
#endif
