//!#####################################################################
//! \file Tetrahedralized_Volume_Policy.h
//!#####################################################################
// Class Tetrahedralized_Volume_Policy
//######################################################################
#ifndef __Tetrahedralized_Volume_Policy__
#define __Tetrahedralized_Volume_Policy__

#include <nova/Geometry/Basic_Geometry/Tetrahedron.h>
#include <nova/Geometry/Topology_Based_Geometry/Triangulated_Surface.h>
#include <nova/Geometry/Topology_Based_Geometry/Segmented_Curve.h>

namespace Nova{
template<class T,int d> class Tetrahedralized_Volume_Policy;

template<class T>
class Tetrahedralized_Volume_Policy<T,2>
{
  public:
    typedef Segmented_Curve<T,2> T_Surface;
    typedef Triangle<T,2> T_Element;
};

template<class T>
class Tetrahedralized_Volume_Policy<T,3>
{
  public:
    typedef Triangulated_Surface<T> T_Surface;
    typedef Tetrahedron<T> T_Element;
};
}
#endif
