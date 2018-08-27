//!#####################################################################
//! \file Hierarchical_Rasterizer.h
//!#####################################################################
// Class Hierarchical_Rasterizer
//######################################################################
#ifndef __Hierarchical_Rasterizer__
#define __Hierarchical_Rasterizer__

#include <nova/Dynamics/Hierarchy/Grid_Hierarchy.h>
#include <utility>

namespace Nova{
template<class Struct_type,class T,int d>
class Hierarchical_Rasterizer
{
    using T_INDEX           = Vector<int,d>;
    using T_CELL            = std::pair<unsigned,T_INDEX>;
    using Hierarchy         = Grid_Hierarchy<Struct_type,T,d>;

  public:
    Hierarchy& hierarchy;

    Hierarchical_Rasterizer(Hierarchy& hierarchy_input)
        :hierarchy(hierarchy_input)
    {}

    ~Hierarchical_Rasterizer() {}

//######################################################################
    virtual bool Consume(const T_CELL& cell)=0;
//######################################################################
};
}
#endif
