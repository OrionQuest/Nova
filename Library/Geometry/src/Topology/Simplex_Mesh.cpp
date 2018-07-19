//!#####################################################################
//! \file Simplex_Mesh.cpp
//!#####################################################################
#include <nova/Geometry/Topology/Simplex_Mesh.h>
using namespace Nova;
//######################################################################
// Initialize_Incident_Elements
//######################################################################
template<int d> void Nova::Simplex_Mesh<d>::
Initialize_Incident_Elements()
{
    if(incident_elements!=nullptr) delete incident_elements;
    incident_elements=new Array<Array<int>>(number_of_nodes);
    for(size_t t=0;t<elements.size();++t){const INDEX& element=elements(t);
        for(int i=0;i<d;++i) (*incident_elements)(element[i]).Append(t);}
}
//######################################################################
// Initialize_Neighbor_Nodes
//######################################################################
template<int d> void Nova::Simplex_Mesh<d>::
Initialize_Neighbor_Nodes()
{
    if(neighbor_nodes!=nullptr) delete neighbor_nodes;
    neighbor_nodes=new Array<Array<int>>(number_of_nodes);
    for(size_t t=0;t<elements.size();++t){const INDEX& element=elements(t);
        for(int i=0;i<d;++i) for(int j=0;j<d;++j) if(i!=j) (*neighbor_nodes)(element[i]).Append_Unique(element[j]);}
}
//######################################################################
// Initialize_Adjacent_Elements
//######################################################################
template<int d> void Nova::Simplex_Mesh<d>::
Initialize_Adjacent_Elements()
{
    if(adjacent_elements!=nullptr) delete adjacent_elements;
    adjacent_elements=new Array<Array<int>>(elements.size());
    
    bool incident_elements_defined=(incident_elements!=nullptr);
    if(!incident_elements_defined) Initialize_Incident_Elements();

    for(size_t t=0;t<elements.size();++t){const INDEX& element=elements(t);
        for(int i=0;i<d;++i) Find_And_Append_Adjacent_Elements(t,element.Remove_Index(i));}
    if(!incident_elements_defined){delete incident_elements;incident_elements=nullptr;}
}
//######################################################################
// Find_And_Append_Adjacent_Elements
//######################################################################
template<int d> void Nova::Simplex_Mesh<d>::
Find_And_Append_Adjacent_Elements(const int simplex,const Vector<int,d-1>& face)
{
    int first_node=face(0);Vector<int,d-2> other_nodes=face.Remove_Index(0);
    for(size_t t=0;t<(*incident_elements)(first_node).size();++t){
        int simplex2=(*incident_elements)(first_node)(t);
        if(simplex2!=simplex && Nodes_In_Simplex(other_nodes,simplex2))
            (*adjacent_elements)(simplex).Append_Unique(simplex2);}
}
//######################################################################
template class Nova::Simplex_Mesh<2>;
template class Nova::Simplex_Mesh<3>;
template class Nova::Simplex_Mesh<4>;
