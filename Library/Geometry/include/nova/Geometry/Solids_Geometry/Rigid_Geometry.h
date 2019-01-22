//!#####################################################################
//! \file Rigid_Geometry.h
//!#####################################################################
// Class Rigid_Geometry
//######################################################################
#ifndef __Rigid_Geometry__
#define __Rigid_Geometry__

#include <nova/Geometry/Implicit_Objects/Implicit_Object.h>
#include <nova/Geometry/Solids_Geometry/Rigid_Geometry_Policy.h>
#include <nova/Tools/Matrices/Frame.h>

namespace Nova{
template<class T,int d>
class Rigid_Geometry
{
    using TV                                    = Vector<T,d>;
    using T_Spin                                = typename Rotation<T,d>::T_Spin;
    using T_Implicit_Object                     = Implicit_Object<T,d>;
    using T_Simplicial_Object                   = typename Rigid_Geometry_Policy<T,d>::T_Simplicial_Object;

  public:
    T_Simplicial_Object *object;
    T_Implicit_Object *implicit_object;
    
    TV V;
    Frame<T,d> f;
    T_Spin angular_velocity;

    Rigid_Geometry()
        :object(new T_Simplicial_Object()),implicit_object(nullptr)
    {}

    Rigid_Geometry(T_Simplicial_Object *object_input,T_Implicit_Object *implicit_object_input)
        :object(object_input),implicit_object(implicit_object_input)
    {Initialize_Simplicial_Object(object);}

    ~Rigid_Geometry()
    {
        if(object!=nullptr) delete object;
        if(implicit_object!=nullptr) delete implicit_object;
    }

    void Set_Position(const TV& x)
    {f.t=x;}

    void Set_Rotation(const Rotation<T,d>& r)
    {f.r=r;}

    void Set_Frame(const TV& x,const Rotation<T,d>& r)
    {f.t=x;f.r=r;}

    TV& X()
    {return f.t;}

    const TV& X() const
    {return f.t;}

    TV Object_Space_Point(const TV& world_space_point) const
    {return f.Inverse_Times(world_space_point);}

    TV Object_Space_Vector(const TV& world_space_vector) const
    {return f.r.Inverse_Rotate(world_space_vector);}

    TV World_Space_Point(const TV& object_space_point) const
    {return f*object_space_point;}

    TV World_Space_Vector(const TV& object_space_vector) const
    {return f.r.Rotate(object_space_vector);}

    TV Pointwise_Object_Velocity(const TV& X) const
    {return V+Cross_Product(angular_velocity,X-f.t);}

    void Initialize_Simplicial_Object(T_Simplicial_Object *object_input)
    {
        if(object!=nullptr) delete object;
        object=object_input;

        // compute center of mass
        TV center_of_mass;
        for(int i=0;i<(int)object->points.size();++i)
            center_of_mass+=object->points[i];
        center_of_mass/=(T)object->points.size();

        // renormalize all points
        for(int i=0;i<(int)object->points.size();++i)
            object->points[i]-=center_of_mass;
    }

    void Initialize_Implicit_Object(T_Implicit_Object *implicit_object_input)
    {implicit_object=implicit_object_input;}
};
}
#include <nova/Geometry/Read_Write/Solids_Geometry/Read_Write_Rigid_Geometry.h>
#endif
