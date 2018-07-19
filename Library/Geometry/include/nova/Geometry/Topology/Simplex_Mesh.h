//!#####################################################################
//! \file Simplex_Mesh.h
//!#####################################################################
// Class Simplex_Mesh
//######################################################################
#ifndef __Simplex_Mesh__
#define __Simplex_Mesh__

#include <nova/Tools/Arrays/Array.h>
#include <nova/Tools/Vectors/Vector.h>

namespace Nova{
template<int d>
class Simplex_Mesh
{
    using INDEX                 = Vector<int,d>;

  public:
    size_t number_of_nodes;
    Array<INDEX> elements;

    // acceleration structures
    Array<Array<int>>* incident_elements;
    Array<Array<int>>* neighbor_nodes;
    Array<Array<int>>* adjacent_elements;

    Simplex_Mesh()
        :number_of_nodes(0),incident_elements(nullptr),neighbor_nodes(nullptr),adjacent_elements(nullptr)
    {}

    Simplex_Mesh(const size_t number_of_nodes_input)
        :number_of_nodes(number_of_nodes_input),incident_elements(nullptr),neighbor_nodes(nullptr),adjacent_elements(nullptr)
    {}

    Simplex_Mesh(const size_t number_of_nodes_input,const Array<INDEX>& elements_input)
        :number_of_nodes(number_of_nodes_input),elements(elements_input),incident_elements(nullptr),neighbor_nodes(nullptr),
        adjacent_elements(nullptr)
    {}

    ~Simplex_Mesh()
    {
        if(incident_elements!=nullptr) delete incident_elements;
        if(neighbor_nodes!=nullptr) delete neighbor_nodes;
        if(adjacent_elements!=nullptr) delete adjacent_elements;
    }

    void Resize_Elements(const size_t size)
    {elements.resize(size);}

    unsigned Add_Element(const INDEX& e)
    {
        elements.Append(e);
        return elements.size();
    }

    template<int d2>
    bool Nodes_In_Simplex(const Vector<int,d2>& nodes,const int simplex) const
    {
        static_assert(d2<=d+1,"Dimension mismatch");
        const INDEX& element=elements(simplex);
        for(size_t i=0;i<d2;++i) if(!element.Contains(nodes(i))) return false;
        return true;
    }

//######################################################################
    void Initialize_Incident_Elements();
    void Initialize_Neighbor_Nodes();
    void Initialize_Adjacent_Elements();
  protected:
    void Find_And_Append_Adjacent_Elements(const int simplex,const Vector<int,d-1>& face);
//######################################################################
};
}
#include <nova/Geometry/Read_Write/Topology/Read_Write_Simplex_Mesh.h>
#endif
