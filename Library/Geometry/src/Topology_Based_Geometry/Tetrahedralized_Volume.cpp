//!#####################################################################
//! \file Tetrahedralized_Volume.cpp
//!#####################################################################
#include <nova/Geometry/Topology_Based_Geometry/Tetrahedralized_Volume.h>
using namespace Nova;
//######################################################################
// Initialize_Boundary_Mesh
//######################################################################
template<class T,int d> void Nova::Tetrahedralized_Volume<T,d>::
Initialize_Boundary_Mesh()
{
    if(boundary_mesh!=nullptr){delete boundary_mesh;boundary_mesh=nullptr;}
    boundary_mesh=new T_Surface(points,Array<T_INDEX>());

    bool incident_elements_defined=(incident_elements!=nullptr);
    if(!incident_elements_defined) Initialize_Incident_Elements();

    for(size_t e=0;e<elements.size();++e){const INDEX& element=elements(e);
        for(int i=0;i<d+1;++i){T_INDEX face=element.Remove_Index(i);
            if(i==1) std::swap(face(0),face(1));        // ensure cyclic order
            if(i==3) std::swap(face(0),face(2));
            const Array<int>& incident_elements_to_face1=(*incident_elements)(face(0));
            bool another_element_on_face=false;
            for(size_t j=0;j<incident_elements_to_face1.size() && !another_element_on_face;++j)
                another_element_on_face=(incident_elements_to_face1(j)!=e && Nodes_In_Simplex(face,incident_elements_to_face1(j)));
            if(!another_element_on_face) boundary_mesh->Add_Element(face);}}
    if(!incident_elements_defined){delete incident_elements;incident_elements=nullptr;}
}
//######################################################################
// Initialize_Hierarchy
//######################################################################
template<class T,int d> void Nova::Tetrahedralized_Volume<T,d>::
Initialize_Hierarchy()
{
    if(hierarchy!=nullptr) delete hierarchy;
    hierarchy=new Box_Hierarchy<T,d>();

    Array<Box<T,d>> boxes;
    for(size_t t=0;t<elements.size();++t){const INDEX& e=elements(t);
        // compute bounding box of the element
        TV min_corner=points[e[0]],max_corner=points[e[0]];
        for(int i=1;i<d+1;++i) for(int axis=0;axis<d;++axis){
            min_corner(axis)=std::min(min_corner(axis),points(e(i))(axis));
            max_corner(axis)=std::max(max_corner(axis),points(e(i))(axis));}
        boxes.Append(Box<T,d>(min_corner,max_corner));}

    hierarchy->Construct_From_Leaf_Boxes(boxes.size(),&boxes[0]);
}
//######################################################################
template class Nova::Tetrahedralized_Volume<float,2>;
template class Nova::Tetrahedralized_Volume<float,3>;
#ifdef COMPILE_WITH_DOUBLE_SUPPORT
template class Nova::Tetrahedralized_Volume<double,2>;
template class Nova::Tetrahedralized_Volume<double,3>;
#endif
