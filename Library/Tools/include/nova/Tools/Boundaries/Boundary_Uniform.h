//!#####################################################################
//! \file Boundary_Uniform.h
//!#####################################################################
// Class Boundary_Uniform
//######################################################################
#ifndef __Boundary_Uniform__
#define __Boundary_Uniform__

#include <nova/Tools/Arrays/Array_ND.h>
#include <nova/Tools/Boundaries/Boundary.h>
#include <nova/Tools/Grids/Grid.h>

namespace Nova{
template<class T,int d>
class Boundary_Uniform: public Boundary<T,d>
{
    using Base          = Boundary<T,d>;
    using T_Grid        = Grid<T,d>;
    using T_Index       = Vector<int,d>;
    using T_Arrays      = Array_ND<T,d>;

  public:
    using Base::use_fixed_boundary;using Base::clamp_below;using Base::clamp_above;using Base::fixed_boundary_value;
    using Base::lower_threshold;using Base::upper_threshold;

    Boundary_Uniform() {}
    virtual ~Boundary_Uniform() {}

  protected:
    int Boundary_Side(const int side,const Range<int,d>& region) const
    {int axis=side/2;return (side%2==0)?region.max_corner[axis]+1:region.min_corner[axis]-1;}

  public:
    void Fill_Ghost_Cells_Cell(const T_Grid& grid,const T_Arrays& u,T_Arrays& u_ghost,const T time,const int number_of_ghost_cells)
    {Fill_Ghost_Cells(grid,u,u_ghost,0,time,number_of_ghost_cells);}

//######################################################################
    virtual void Apply_Boundary_Condition(const T_Grid& grid,T_Arrays& u,const T time) {}
    virtual void Apply_Boundary_Condition_Single_Side(const T_Grid& grid,T_Arrays& u,const int side,const T time) const {}
//######################################################################
    virtual void Fill_Ghost_Cells(const T_Grid& grid,const T_Arrays& u,T_Arrays& u_ghost,const T dt,const T time,const int number_of_ghost_cells);
    virtual void Fill_Single_Ghost_Region(const T_Grid& grid,T_Arrays& u_ghost,const int side,const T dt,const T time,const Range<int,d>& region) const;
    void Find_Ghost_Regions(const T_Grid& grid,Array<Range<int,d> >& regions,const int number_of_ghost_cells) const;
//######################################################################
};
}
#endif
