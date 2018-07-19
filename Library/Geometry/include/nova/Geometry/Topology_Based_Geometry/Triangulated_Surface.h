//!#####################################################################
//! \file Triangulated_Surface.h
//!#####################################################################
// Class Triangulated_Surface
//######################################################################
#ifndef __Triangulated_Surface__
#define __Triangulated_Surface__

#include <nova/Geometry/Topology/Point_Cloud.h>
#include <nova/Geometry/Topology/Simplex_Mesh.h>
#include <nova/Tools/Utilities/File_Utilities.h>
#include <cassert>

namespace Nova{

template<class T,int d> class Sphere;

template<class T>
class Triangulated_Surface: public Point_Cloud<T,3>,public Simplex_Mesh<3>
{
    enum {d=3};
    using TV                    = Vector<T,d>;
    using INDEX                 = Vector<int,d>;
    using Mesh_Base             = Simplex_Mesh<d>;
    using Point_Base            = Point_Cloud<T,d>;

  public:
    using Point_Base::points;
    using Mesh_Base::elements;using Mesh_Base::number_of_nodes;
    using Point_Base::Add_Vertex;using Mesh_Base::Add_Element;

    Triangulated_Surface() {}

    Triangulated_Surface(const Array<TV>& points_input,const Array<INDEX>& elements_input)
        :Point_Base(points_input),Mesh_Base(points_input.size(),elements_input)
    {}

    void Write_OBJ(const std::string& filename)
    {
        std::ostream *output=File_Utilities::Safe_Open_Output(filename,false);
        if(output!=nullptr)
        {
            for(size_t i=0;i<points.size();++i) *output<<"v "<<points[i][0]<<" "<<points[i][1]<<" "<<points[i][2]<<std::endl;
            for(size_t i=0;i<elements.size();++i) *output<<"f "<<elements[i][0]+1<<" "<<elements[i][1]+1<<" "<<elements[i][2]+1<<std::endl;
        }
    }

//######################################################################
    void Initialize_Sphere_Tessellation(const Sphere<T,d>& sphere,const int levels=4);
    void Initialize_Ground_Plane(const T height=(T)0.);
//######################################################################
};
}
#include <nova/Geometry/Read_Write/Topology_Based_Geometry/Read_Write_Triangulated_Surface.h>
#endif
