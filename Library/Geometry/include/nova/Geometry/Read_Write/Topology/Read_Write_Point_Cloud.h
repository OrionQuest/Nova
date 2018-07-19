//!#####################################################################
//! \file Read_Write_Point_Cloud.h
//!#####################################################################
// Class Read_Write_Point_Cloud
//###################################################################### 
#ifndef __Read_Write_Point_Cloud__
#define __Read_Write_Point_Cloud__

#include <nova/Geometry/Topology/Point_Cloud.h>
#include <nova/Tools/Read_Write/Utilities/Read_Write.h>

namespace Nova{
template<class T,int d>
class Read_Write<Point_Cloud<T,d>>
{
    typedef Vector<T,d> TV;

  public:
    static void Read(std::istream& input,Point_Cloud<T,d>& object)
    {
        size_t number_of_points;
        Read_Write<size_t>::Read(input,number_of_points);
        object.Resize_Vertices(number_of_points);
        for(size_t i=0;i<number_of_points;++i)
            Read_Write<TV>::Read(input,object.points[i]);
    }

    static void Write(std::ostream& output,const Point_Cloud<T,d>& object)
    {
        Read_Write<size_t>::Write(output,object.points.size());
        for(size_t i=0;i<object.points.size();++i) Read_Write<TV>::Write(output,object.points[i]);
    }
};
}
#endif
