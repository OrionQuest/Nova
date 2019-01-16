//!#####################################################################
//! \file Segmented_Curve.h
//!#####################################################################
// Class Segmented_Curve
//######################################################################
#ifndef __Segmented_Curve__
#define __Segmented_Curve__

#include <nova/Geometry/Topology/Point_Cloud.h>
#include <nova/Geometry/Topology/Simplex_Mesh.h>

namespace Nova{

template<class T,int d> class Box;
template<class T,int d> class Sphere;

template<class T,int d>
class Segmented_Curve: public Point_Cloud<T,d>,public Simplex_Mesh<2>
{
    using TV                    = Vector<T,d>;
    using INDEX                 = Vector<int,2>;
    using Mesh_Base             = Simplex_Mesh<2>;
    using Point_Base            = Point_Cloud<T,d>;

  public:
    using Point_Base::points;
    using Mesh_Base::elements;using Mesh_Base::number_of_nodes;
    using Point_Base::Add_Vertex;using Mesh_Base::Add_Element;

    Segmented_Curve() {}

    Segmented_Curve(const Array<TV>& points_input,const Array<INDEX>& elements_input)
        :Point_Base(points_input),Mesh_Base(points_input.size(),elements_input)
    {}

//######################################################################
    void Initialize_Box_Tessellation(const Box<T,d>& box);
    void Initialize_Sphere_Tessellation(const Sphere<T,d>& sphere,const int levels=4);
//######################################################################
};
}
#include <nova/Geometry/Read_Write/Topology_Based_Geometry/Read_Write_Segmented_Curve.h>
#endif
