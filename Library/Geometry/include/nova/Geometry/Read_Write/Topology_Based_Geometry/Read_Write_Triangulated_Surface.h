//!#####################################################################
//! \file Read_Write_Triangulated_Surface.h
//!#####################################################################
// Class Read_Write_Triangulated_Surface
//###################################################################### 
#ifndef __Read_Write_Triangulated_Surface__
#define __Read_Write_Triangulated_Surface__

#include <nova/Geometry/Topology_Based_Geometry/Triangulated_Surface.h>
#include <nova/Tools/Read_Write/Utilities/Read_Write.h>

namespace Nova{
template<class T>
class Read_Write<Triangulated_Surface<T>>
{
    enum {d=3};
    typedef Vector<T,d> TV;typedef Vector<int,d> INDEX;

  public:
    static void Read(std::istream& input,Triangulated_Surface<T>& object)
    {
        Read_Write<Point_Cloud<T,d>>::Read(input,object);
        Read_Write<Simplex_Mesh<d>>::Read(input,object);
    }

    static void Write(std::ostream& output,const Triangulated_Surface<T>& object)
    {
        Read_Write<Point_Cloud<T,d>>::Write(output,object);
        Read_Write<Simplex_Mesh<d>>::Write(output,object);
    }
};

template<class T>
class Read_Write<Array<std::shared_ptr<Triangulated_Surface<T>>>>
{
    enum {d=3};
    using T_Surface         = Triangulated_Surface<T>;
    using Surface_Pointer   = std::shared_ptr<T_Surface>;

  public:
    static void Read(std::istream& input,Array<Surface_Pointer>& objects)
    {
        for(size_t i=0;i<objects.size();++i){
            Read_Write<Point_Cloud<T,d>>::Read(input,*objects[i]);
            Read_Write<Simplex_Mesh<d>>::Read(input,*objects[i]);}
    }

    static void Write(std::ostream& output,const Array<Surface_Pointer>& objects)
    {
        for(size_t i=0;i<objects.size();++i){
            Read_Write<Point_Cloud<T,d>>::Write(output,*objects[i]);
            Read_Write<Simplex_Mesh<d>>::Write(output,*objects[i]);}
    }
};
}
#endif
