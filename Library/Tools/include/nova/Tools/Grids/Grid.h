//!#####################################################################
//! \file Grid.h
//!#####################################################################
// Class Grid
//######################################################################
#ifndef __Grid__
#define __Grid__

#include <nova/Tools/Utilities/Range.h>

namespace Nova{
template<class T,int d>
class Grid
{
    typedef Vector<T,d> TV;typedef Vector<int,d> T_INDEX;

  public:
    enum Region {Whole_Region,Ghost_Region,Boundary_Region,Interior_Region,Boundary_Interior_Region};       // for iterators
    enum {number_of_nodes_per_cell=1<<d};
    enum {number_of_faces_per_cell=2*d};

    T_INDEX counts;
    Range<T,d> domain;
    TV dX,one_over_dX;

    Grid()
    {
        Initialize(T_INDEX(),Range<T,d>::Unit_Box());
    }

    Grid(const T_INDEX& counts_input,const Range<T,d>& domain_input)
    {
        Initialize(counts_input,domain_input);
    }

    const T_INDEX& Number_Of_Cells() const
    {return counts;}

    T_INDEX Number_Of_Nodes() const
    {return counts+1;}

    TV Node(const T_INDEX& index) const
    {
        return domain.min_corner+TV(index-1)*dX;
    }

    TV Center(const T_INDEX& index) const
    {
        return domain.min_corner+(TV(index)-(T).5)*dX;
    }

    TV Face(const int axis,const T_INDEX& index) const
    {
        TV shifted_index(TV(index)-(T).5);
        shifted_index(axis)-=(T).5;
        return domain.min_corner+shifted_index*dX;
    }

    Range<T,d> Cell_Domain(const T_INDEX& index) const
    {
        TV corner=domain.min_corner+TV(index)*dX;
        return Range<T,d>(corner-dX,corner);
    }

    bool Inside_Domain(const T_INDEX& index,const int number_of_ghost_cells=0) const
    {return Cell_Indices(number_of_ghost_cells).Inside(index);}

    Range<int,d> Cell_Indices(const int number_of_ghost_cells=0) const
    {return Range<int,d>(T_INDEX(1),counts).Thickened(number_of_ghost_cells);}

    Range<int,d> Node_Indices(const int number_of_ghost_cells=0) const
    {return Range<int,d>(T_INDEX(1),Number_Of_Nodes()).Thickened(number_of_ghost_cells);}

    T_INDEX Clamp_To_Cell(const TV& X) const
    {
        T_INDEX index=T_INDEX((X-domain.min_corner)*one_over_dX)+1;
        return Cell_Indices().Clamp(index);
    }

    T_INDEX Cell(const TV& location,const int number_of_ghost_cells) const
    {
        T_INDEX index;
        Cell(location,index,number_of_ghost_cells);
        return index;
    }

    void Cell(const TV& location,T_INDEX& index,const int number_of_ghost_cells) const
    {
        int number_of_ghost_cells_plus_one=number_of_ghost_cells+1;
        index=T_INDEX((location-domain.min_corner)*one_over_dX+number_of_ghost_cells_plus_one)-number_of_ghost_cells;
    }

//##################################################################### 
    void Initialize(const T_INDEX& counts_input,const Range<T,d>& domain_input);
    void Nodes_In_Cell_From_Minimum_Corner_Node(const T_INDEX& minimum_corner_node,T_INDEX nodes[number_of_nodes_per_cell]) const;
//##################################################################### 
};
}
#include <nova/Tools/Read_Write/Grids/Read_Write_Grid.h>
#endif
