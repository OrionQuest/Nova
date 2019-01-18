//!#####################################################################
//! \file Hierarchical_Uniform_Rasterizer.h
//!#####################################################################
// Class Hierarchical_Uniform_Rasterizer
//######################################################################
#ifndef __Hierarchical_Uniform_Rasterizer__
#define __Hierarchical_Uniform_Rasterizer__

#include <nova/Dynamics/Hierarchy/Rasterizers/Hierarchical_Rasterizer.h>
#include <nova/Dynamics/Utilities/SPGrid_Flags.h>

namespace Nova{
template<class Struct_type,class T,int d>
class Hierarchical_Uniform_Rasterizer: public Hierarchical_Rasterizer<Struct_type,T,d>
{
    using Base          = Hierarchical_Rasterizer<Struct_type,T,d>;
    using T_INDEX       = Vector<int,d>;
    using T_CELL        = std::pair<unsigned,T_INDEX>;
    using Hierarchy     = Grid_Hierarchy<Struct_type,T,d>;

  public:
    using Base::hierarchy;

    Hierarchical_Uniform_Rasterizer(Hierarchy& hierarchy_input)
        :Base(hierarchy_input)
    {}

    bool Consume(const T_CELL& cell) override
    {
        const unsigned level=cell.first;
        const T_INDEX& index=cell.second;

        if(level==0) hierarchy.Activate_Cell(level,index,Cell_Type_Interior);
        return (level>0);
    }
};
}
#endif
