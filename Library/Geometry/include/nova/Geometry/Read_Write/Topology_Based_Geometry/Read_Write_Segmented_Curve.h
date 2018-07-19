//!#####################################################################
//! \file Read_Write_Segmented_Curve.h
//!#####################################################################
// Class Read_Write_Segmented_Curve
//###################################################################### 
#ifndef __Read_Write_Segmented_Curve__
#define __Read_Write_Segmented_Curve__

#include <nova/Geometry/Topology_Based_Geometry/Segmented_Curve.h>
#include <nova/Tools/Read_Write/Utilities/Read_Write.h>

namespace Nova{
template<class T,int d>
class Read_Write<Segmented_Curve<T,d>>
{
    typedef Vector<T,d> TV;typedef Vector<int,2> INDEX;

  public:
    static void Read(std::istream& input,Segmented_Curve<T,d>& object)
    {
        Read_Write<Point_Cloud<T,d>>::Read(input,object);
        Read_Write<Simplex_Mesh<2>>::Read(input,object);
    }

    static void Write(std::ostream& output,const Segmented_Curve<T,d>& object)
    {
        Read_Write<Point_Cloud<T,d>>::Write(output,object);
        Read_Write<Simplex_Mesh<2>>::Write(output,object);
    }
};
}
#endif
