//!#####################################################################
//! \file Read_Write_Tetrahedralized_Volume.h
//!#####################################################################
// Class Read_Write_Tetrahedralized_Volume
//###################################################################### 
#ifndef __Read_Write_Tetrahedralized_Volume__
#define __Read_Write_Tetrahedralized_Volume__

#include <nova/Geometry/Topology_Based_Geometry/Tetrahedralized_Volume.h>
#include <nova/Tools/Read_Write/Utilities/Read_Write.h>

namespace Nova{
template<class T,int d>
class Read_Write<Tetrahedralized_Volume<T,d>>
{
  public:
    static void Read(std::istream& input,Tetrahedralized_Volume<T,d>& object)
    {
        Read_Write<Point_Cloud<T,d>>::Read(input,object);
        Read_Write<Simplex_Mesh<d+1>>::Read(input,object);
    }

    static void Write(std::ostream& output,const Tetrahedralized_Volume<T,d>& object)
    {
        Read_Write<Point_Cloud<T,d>>::Write(output,object);
        Read_Write<Simplex_Mesh<d+1>>::Write(output,object);
    }
};

template<class T,int d>
class Read_Write<Array<std::shared_ptr<Tetrahedralized_Volume<T,d>>>>
{
    using T_Volume          = Tetrahedralized_Volume<T,d>;
    using Volume_Pointer    = std::shared_ptr<T_Volume>;

  public:
    static void Read(std::istream& input,Array<Volume_Pointer>& objects)
    {
        for(size_t i=0;i<objects.size();++i){
            Read_Write<Point_Cloud<T,d>>::Read(input,*objects[i]);
            Read_Write<Simplex_Mesh<d+1>>::Read(input,*objects[i]);}
    }

    static void Write(std::ostream& output,const Array<Volume_Pointer>& objects)
    {
        for(size_t i=0;i<objects.size();++i){
            Read_Write<Point_Cloud<T,d>>::Write(output,*objects[i]);
            Read_Write<Simplex_Mesh<d+1>>::Write(output,*objects[i]);}
    }
};
}
#endif
