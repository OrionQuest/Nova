//!#####################################################################
//! \file Boundary_Uniform.cpp
//!#####################################################################
#include <nova/Tools/Boundaries/Boundary_Uniform.h>
#include <nova/Tools/Grids/Grid_Iterator_Cell.h>
#include <nova/Tools/Utilities/Utilities.h>
using namespace Nova;
//######################################################################
// Fill_Ghost_Cells
//######################################################################
template<class T,int d> void Boundary_Uniform<T,d>::
Fill_Ghost_Cells(const T_Grid& grid,const T_Arrays& u,T_Arrays& u_ghost,const T dt,const T time,const int number_of_ghost_cells)
{
    using Cell_Iterator         = Grid_Iterator_Cell<T,d>;

    for(Cell_Iterator iterator(grid);iterator.Valid();iterator.Next()){const T_Index& index=iterator.Cell_Index();
        u_ghost(index)=u(index);}
    Array<Range<int,d> > regions;
    Find_Ghost_Regions(grid,regions,number_of_ghost_cells);
    for(int side=0;side<T_Grid::number_of_faces_per_cell;++side) Fill_Single_Ghost_Region(grid,u_ghost,side,dt,time,regions(side));
}
//######################################################################
// Find_Ghost_Regions
//######################################################################
static inline void Find_Ghost_Regions_Helper(Array<Range<int,1> >& regions,const Range<int,1>& domain,const Range<int,1>& ghost)
{
    regions(0)=Range<int,1>(ghost.min_corner(0),domain.min_corner(0)-1);    // left
    regions(1)=Range<int,1>(domain.max_corner(0)+1,ghost.max_corner(0));    // right
}
static inline void Find_Ghost_Regions_Helper(Array<Range<int,2> >& regions,const Range<int,2>& domain,const Range<int,2>& ghost)
{
    regions(0)=Range<int,2>(ghost.min_corner(0),domain.min_corner(0)-1,domain.min_corner(1),domain.max_corner(1));  // left
    regions(1)=Range<int,2>(domain.max_corner(0)+1,ghost.max_corner(0),domain.min_corner(1),domain.max_corner(1));  // right
    regions(2)=Range<int,2>(ghost.min_corner(0),ghost.max_corner(0),ghost.min_corner(1),domain.min_corner(1)-1);    // bottom
    regions(3)=Range<int,2>(ghost.min_corner(0),ghost.max_corner(0),domain.max_corner(1)+1,ghost.max_corner(1));    // top
}
static inline void Find_Ghost_Regions_Helper(Array<Range<int,3> >& regions,const Range<int,3>& domain,const Range<int,3>& ghost)
{
    regions(0)=Range<int,3>(ghost.min_corner(0),domain.min_corner(0)-1,domain.min_corner(1),domain.max_corner(1),domain.min_corner(2),domain.max_corner(2));    // left
    regions(1)=Range<int,3>(domain.max_corner(0)+1,ghost.max_corner(0),domain.min_corner(1),domain.max_corner(1),domain.min_corner(2),domain.max_corner(2));    // right
    regions(2)=Range<int,3>(ghost.min_corner(0),ghost.max_corner(0),ghost.min_corner(1),domain.min_corner(1)-1,domain.min_corner(2),domain.max_corner(2));      // bottom
    regions(3)=Range<int,3>(ghost.min_corner(0),ghost.max_corner(0),domain.max_corner(1)+1,ghost.max_corner(1),domain.min_corner(2),domain.max_corner(2));      // top
    regions(4)=Range<int,3>(ghost.min_corner(0),ghost.max_corner(0),ghost.min_corner(1),ghost.max_corner(1),ghost.min_corner(2),domain.min_corner(2)-1);        // front
    regions(5)=Range<int,3>(ghost.min_corner(0),ghost.max_corner(0),ghost.min_corner(1),ghost.max_corner(1),domain.max_corner(2)+1,ghost.max_corner(2));        // back
}
template<class T,int d> void Boundary_Uniform<T,d>::
Find_Ghost_Regions(const T_Grid& grid,Array<Range<int,d> >& regions,const int number_of_ghost_cells) const
{
    Range<int,d> domain=grid.Cell_Indices(),ghost=domain.Thickened(number_of_ghost_cells);
    regions.resize(T_Grid::number_of_faces_per_cell);
    Find_Ghost_Regions_Helper(regions,domain,ghost);
}
//######################################################################
// Fill_Single_Ghost_Region
//######################################################################
template<class T,int d> void Boundary_Uniform<T,d>::
Fill_Single_Ghost_Region(const T_Grid& grid,T_Arrays& u_ghost,const int side,const T dt,const T time,const Range<int,d>& region) const
{
    using Cell_Iterator         = Grid_Iterator_Cell<T,d>;

    int axis=side/2,boundary=(side%2==0)?region.max_corner[axis]+1:region.min_corner[axis]-1;
    Cell_Iterator iterator(grid,region);
    if(use_fixed_boundary) for(;iterator.Valid();iterator.Next()){T_Index index=iterator.Cell_Index();
        u_ghost(index)=fixed_boundary_value;}
    else if(clamp_below&&clamp_above) for(;iterator.Valid();iterator.Next()){T_Index index=iterator.Cell_Index(),boundary_index=index;boundary_index[axis]=boundary;
        u_ghost(index)=Nova_Utilities::Clamp<T>(u_ghost(boundary_index),lower_threshold,upper_threshold);}
    else if(clamp_below) for(;iterator.Valid();iterator.Next()){T_Index index=iterator.Cell_Index(),boundary_index=index;boundary_index[axis]=boundary;
        u_ghost(index)=std::max(u_ghost(boundary_index),lower_threshold);}
    else if(clamp_above) for(;iterator.Valid();iterator.Next()){T_Index index=iterator.Cell_Index(),boundary_index=index;boundary_index[axis]=boundary;
        u_ghost(index)=std::min(u_ghost(boundary_index),upper_threshold);}
    else for(;iterator.Valid();iterator.Next()){T_Index index=iterator.Cell_Index(),boundary_index=index;boundary_index[axis]=boundary;
        u_ghost(index)=u_ghost(boundary_index);}
}
//######################################################################
template class Nova::Boundary_Uniform<float,1>;
template class Nova::Boundary_Uniform<float,2>;
template class Nova::Boundary_Uniform<float,3>;
#ifdef COMPILE_WITH_DOUBLE_SUPPORT
template class Nova::Boundary_Uniform<double,1>;
template class Nova::Boundary_Uniform<double,2>;
template class Nova::Boundary_Uniform<double,3>;
#endif
