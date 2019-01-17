//!#####################################################################
//! \file Read_Write_Rigid_Body.h
//!#####################################################################
// Class Read_Write_Rigid_Body
//###################################################################### 
#ifndef __Read_Write_Rigid_Body__
#define __Read_Write_Rigid_Body__

#include <nova/Dynamics/Rigid_Bodies/Rigid_Body.h>
#include <nova/Tools/Read_Write/Utilities/Read_Write.h>

namespace Nova{
template<class T,int d>
class Read_Write<Rigid_Body<T,d>>
{
    using T_Inertia_Tensor              = typename Rigid_Body_Policy<T,d>::T_Inertia_Tensor;

  public:
    static void Read(std::istream& input,Rigid_Body<T,d>& body)
    {
        Read_Write<Rigid_Geometry<T,d>>::Read(input,body);
        Read_Write<T>::Read(input,body.mass);
        Read_Write<T_Inertia_Tensor>::Read(input,body.inertia_tensor);
    }

    static void Write(std::ostream& output,const Rigid_Body<T,d>& body)
    {
        Read_Write<Rigid_Geometry<T,d>>::Write(output,body);
        Read_Write<T>::Write(output,body.mass);
        Read_Write<T_Inertia_Tensor>::Write(output,body.inertia_tensor);
    }
};
}
#endif
