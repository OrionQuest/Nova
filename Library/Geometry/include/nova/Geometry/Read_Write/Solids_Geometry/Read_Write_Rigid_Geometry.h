//!#####################################################################
//! \file Read_Write_Rigid_Geometry.h
//!#####################################################################
// Class Read_Write_Rigid_Geometry
//###################################################################### 
#ifndef __Read_Write_Rigid_Geometry__
#define __Read_Write_Rigid_Geometry__

#include <nova/Geometry/Solids_Geometry/Rigid_Geometry.h>
#include <nova/Tools/Read_Write/Utilities/Read_Write.h>

namespace Nova{
template<class T,int d>
class Read_Write<Rigid_Geometry<T,d>>
{
    using TV                            = Vector<T,d>;
    using T_Spin                        = typename Rotation<T,d>::T_Spin;
    using T_Simplicial_Object           = typename Rigid_Geometry_Policy<T,d>::T_Simplicial_Object;

  public:
    static void Read(std::istream& input,Rigid_Geometry<T,d>& geometry)
    {
        if(geometry.object!=nullptr) delete geometry.object;
        geometry.object=new T_Simplicial_Object();

        Read_Write<T_Simplicial_Object>::Read(input,*geometry.object);
        Read_Write<Frame<T,d>>::Read(input,geometry.f);
        Read_Write<TV>::Read(input,geometry.V);
        Read_Write<T_Spin>::Read(input,geometry.angular_velocity);
    }

    static void Write(std::ostream& output,const Rigid_Geometry<T,d>& geometry)
    {
        Read_Write<T_Simplicial_Object>::Write(output,*geometry.object);
        Read_Write<Frame<T,d>>::Write(output,geometry.f);
        Read_Write<TV>::Write(output,geometry.V);
        Read_Write<T_Spin>::Write(output,geometry.angular_velocity);
    }
};
}
#endif
