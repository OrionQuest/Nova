//!#####################################################################
//! \file Tetrahedralized_Volume.h
//!#####################################################################
// Class Tetrahedralized_Volume
//######################################################################
#ifndef __Tetrahedralized_Volume__
#define __Tetrahedralized_Volume__

#include <nova/Geometry/Spatial_Acceleration/Box_Hierarchy.h>
#include <nova/Geometry/Topology/Point_Cloud.h>
#include <nova/Geometry/Topology/Simplex_Mesh.h>
#include <nova/Geometry/Topology_Based_Geometry/Tetrahedralized_Volume_Policy.h>
#include <nova/Tools/Grids/Grid.h>
#include <cassert>

namespace Nova{
template<class T,int d>
class Tetrahedralized_Volume: public Point_Cloud<T,d>,public Simplex_Mesh<d+1>
{
    using TV                    = Vector<T,d>;
    using INDEX                 = Vector<int,d+1>;
    using T_INDEX               = Vector<int,d>;
    using Mesh_Base             = Simplex_Mesh<d+1>;
    using Point_Base            = Point_Cloud<T,d>;
  public:
    using T_Element             = typename Tetrahedralized_Volume_Policy<T,d>::T_Element;
    using T_Surface             = typename Tetrahedralized_Volume_Policy<T,d>::T_Surface;

    using Point_Base::points;
    using Mesh_Base::elements;using Mesh_Base::incident_elements;using Mesh_Base::number_of_nodes;
    using Point_Base::Add_Vertex;
    using Mesh_Base::Initialize_Incident_Elements;using Mesh_Base::Nodes_In_Simplex;using Mesh_Base::Add_Element;

    T_Surface* boundary_mesh;
    Box_Hierarchy<T,d>* hierarchy;

    Tetrahedralized_Volume()
        :boundary_mesh(nullptr),hierarchy(nullptr)
    {}

    Tetrahedralized_Volume(const Array<TV>& points_input,const Array<INDEX>& elements_input)
        :Point_Base(points_input),Mesh_Base(points_input.size(),elements_input),boundary_mesh(nullptr),hierarchy(nullptr)
    {
        Initialize_Boundary_Mesh();
    }

    ~Tetrahedralized_Volume()
    {
        if(boundary_mesh!=nullptr) delete boundary_mesh;
        if(hierarchy!=nullptr) delete hierarchy;
    }

//######################################################################
    void Initialize_Cube_Mesh(const Grid<T,d>& grid);
    void Initialize_Boundary_Mesh();
    void Initialize_Hierarchy();
    T_Element Get_Element(const int id) const;
//######################################################################
};
}
#include <nova/Geometry/Read_Write/Topology_Based_Geometry/Read_Write_Tetrahedralized_Volume.h>
#endif
