//!#####################################################################
//! \file Grid_Iterator_Cell.h
//!#####################################################################
// Class Grid_Iterator_Cell
//######################################################################
#ifndef __Grid_Iterator_Cell__
#define __Grid_Iterator_Cell__

#include <nova/Tools/Grids/Grid_Iterator.h>

namespace Nova{
template<class T,int d>
class Grid_Iterator_Cell: public Grid_Iterator<T,d>
{
    using TV                = Vector<T,d>;
    using Base              = Grid_Iterator<T,d>;
    using T_INDEX           = Vector<int,d>;

  public:
    using T_Region          = typename Grid<T,d>::Region;

    using Base::grid;using Base::index;
    using Base::Add_Region;using Base::Reset;

    /*! Base constructor which initializes the cell iterator.
     *
     * \param grid_input The grid over which we will iterate.
     * \param number_of_ghost_cells The number of ghost cells which provide an extra layer of padding.
     * \param region_type Different region types as enumerated in the Grid class.
     * \param side The different sides of the grid over which we will iterate (side=0 represents entire domain).
     */
    Grid_Iterator_Cell(const Grid<T,d>& grid_input,const int number_of_ghost_cells=0,const T_Region& region_type=Grid<T,d>::Whole_Region,const int side=0);

    Grid_Iterator_Cell(const Grid<T,d>& grid_input,const Range<int,d>& region_input)
        :Base(grid_input,region_input)
    {}

    const T_INDEX& Cell_Index() const
    {return index;}

    TV Location() const
    {return grid.Center(index);}
};
}
#endif
