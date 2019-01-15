//!#####################################################################
//! \file Rigid_Geometry_Policy.h
//!#####################################################################
// Class Rigid_Geometry_Policy
//######################################################################
#ifndef __Rigid_Geometry_Policy__
#define __Rigid_Geometry_Policy__

#include <nova/Geometry/Implicit_Objects/Implicit_Object.h>
#include <nova/Geometry/Topology_Based_Geometry/Triangulated_Surface.h>
#include <nova/Geometry/Topology_Based_Geometry/Segmented_Curve.h>

namespace Nova{
template<class T,int d> class Rigid_Geometry_Policy;

template<class T>
class Rigid_Geometry_Policy<T,2>
{
  public:
    typedef Segmented_Curve<T,2> T_Simplicial_Object;
};

template<class T>
class Rigid_Geometry_Policy<T,3>
{
  public:
    typedef Triangulated_Surface<T> T_Simplicial_Object;
};
}
#endif
