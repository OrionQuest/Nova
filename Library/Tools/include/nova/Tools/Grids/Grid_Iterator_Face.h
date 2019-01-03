//!#####################################################################
//! \file Grid_Iterator_Face.h
//!#####################################################################
// Class Grid_Iterator_Face
//######################################################################
#ifndef __Grid_Iterator_Face__
#define __Grid_Iterator_Face__

#include <nova/Tools/Grids/Grid_Iterator.h>

namespace Nova{
template<class T,int d>
class Grid_Iterator_Face: public Grid_Iterator<T,d>
{
    using TV                = Vector<T,d>;
    using Base              = Grid_Iterator<T,d>;
    using T_INDEX           = Vector<int,d>;
    using T_Region          = typename Grid<T,d>::Region;

  protected:
    T_Region region_type;
    int axis,side;
    bool single_axis;
    int number_of_ghost_cells;

  public:
    using Base::grid;using Base::index;using Base::valid;using Base::region;
    using Base::Add_Region;using Base::Reset;using Base::Reset_Regions;

    Grid_Iterator_Face(const Grid<T,d>& grid_input,const int number_of_ghost_cells_input=0,const T_Region& region_type_input=Grid<T,d>::Whole_Region,
                       const int side_input=0,int axis_input=0);

    int Axis() const
    {return axis;}

    const T_INDEX& Face_Index() const
    {return index;}

    void Next()
    {
        if(index(d-1)<region.max_corner(d-1)) ++index(d-1);
        else Next_Helper();
    }

  private:
    void Reset_Axis(const int axis_input);
    void Next_Helper();
};
}
#endif
