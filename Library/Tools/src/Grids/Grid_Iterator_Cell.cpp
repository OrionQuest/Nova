//!#####################################################################
//! \file Grid_Iterator_Cell.cpp
//!#####################################################################
#include <nova/Tools/Grids/Grid_Iterator_Cell.h>
#include <cassert>
using namespace Nova;
//######################################################################
// Constructor
//######################################################################
template<class T,int d> Grid_Iterator_Cell<T,d>::
Grid_Iterator_Cell(const Grid<T,d>& grid_input,const int number_of_ghost_cells,const T_Region& region_type,const int side)
    :Base(grid_input)
{
    assert(side>=0 && side<=2*d);
    assert((region_type!=Grid<T,d>::Boundary_Region && region_type!=Grid<T,d>::Interior_Region));
    Range<int,d> domain(grid.Cell_Indices(number_of_ghost_cells));

    switch(region_type)
    {
        case Grid<T,d>::Whole_Region:               assert(!side);
                                                    Add_Region(domain);
                                                    break;

        case Grid<T,d>::Boundary_Interior_Region:   if(!side)
                                                    {
                                                        Range<int,d> domain_copy(domain);
                                                        for(int axis=d-1;axis>=0;--axis)
                                                        {
                                                            domain.max_corner(axis)=domain.min_corner(axis);
                                                            Add_Region(domain);
                                                            domain.max_corner(axis)=domain.min_corner(axis)=domain_copy.max_corner(axis);
                                                            Add_Region(domain);
                                                            domain.max_corner(axis)=domain_copy.max_corner(axis)-1;
                                                            domain.min_corner(axis)=domain_copy.min_corner(axis)+1;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        int axis=(side-1)/2;
                                                        if(side&1) domain.max_corner(axis)=domain.min_corner(axis);
                                                        else domain.min_corner(axis)=domain.max_corner(axis);
                                                        Add_Region(domain);
                                                    }
                                                    break;

        default:                                    assert((region_type==Grid<T,d>::Ghost_Region && number_of_ghost_cells>0));
                                                    if(!side)
                                                    {
                                                        T_INDEX max_copy(domain.max_corner);
                                                        for(int axis=d-1;axis>=0;--axis)
                                                        {
                                                            domain.max_corner(axis)=0;
                                                            Add_Region(domain);
                                                            domain.max_corner(axis)=max_copy(axis);
                                                            domain.min_corner(axis)=grid.counts(axis)+1;
                                                            Add_Region(domain);
                                                            domain.max_corner(axis)=grid.counts(axis);
                                                            domain.min_corner(axis)=1;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        int axis=(side-1)/2;
                                                        if(side&1) domain.max_corner(axis)=0;
                                                        else domain.min_corner(axis)=grid.counts(axis)+1;
                                                        Add_Region(domain);
                                                    }
                                                    break;
    }
    Reset();
}
//######################################################################
template class Nova::Grid_Iterator_Cell<float,1>;
template class Nova::Grid_Iterator_Cell<float,2>;
template class Nova::Grid_Iterator_Cell<float,3>;
#ifdef COMPILE_WITH_DOUBLE_SUPPORT
template class Nova::Grid_Iterator_Cell<double,1>;
template class Nova::Grid_Iterator_Cell<double,2>;
template class Nova::Grid_Iterator_Cell<double,3>;
#endif
