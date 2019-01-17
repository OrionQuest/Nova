//!#####################################################################
//! \file Rigid_Body.h
//!#####################################################################
// Class Rigid_Body
//######################################################################
#ifndef __Rigid_Body__
#define __Rigid_Body__

#include <nova/Dynamics/Rigid_Bodies/Rigid_Body_Policy.h>
#include <nova/Geometry/Solids_Geometry/Rigid_Geometry.h>

namespace Nova{
template<class T,int d>
class Rigid_Body: public Rigid_Geometry<T,d>
{
    using Base                                  = Rigid_Geometry<T,d>;
    using T_Implicit_Object                     = Implicit_Object<T,d>;
    using T_Inertia_Tensor                      = typename Rigid_Body_Policy<T,d>::T_Inertia_Tensor;
    using T_Simplicial_Object                   = typename Rigid_Geometry_Policy<T,d>::T_Simplicial_Object;

  public:
    T mass;
    T_Inertia_Tensor inertia_tensor;

    Rigid_Body()
        :Base()
    {}

    Rigid_Body(T_Simplicial_Object *object,T_Implicit_Object *implicit_object)
        :Base(object,implicit_object)
    {}

    Rigid_Body(T_Simplicial_Object *object,T_Implicit_Object *implicit_object,const T mass_input,const T_Inertia_Tensor& inertia_tensor_input)
        :Base(object,implicit_object),mass(mass_input),inertia_tensor(inertia_tensor_input)
    {}

    ~Rigid_Body() {}

    void Set_Mass(const T mass_input)
    {mass=mass_input;}

    void Set_Inertia_Tensor(const T_Inertia_Tensor& inertia_tensor_input)
    {inertia_tensor=inertia_tensor_input;}
};
}
#include <nova/Dynamics/Read_Write/Rigid_Bodies/Read_Write_Rigid_Body.h>
#endif
