//!#####################################################################
//! \file Grid_Iterator_Face.cpp
//!#####################################################################
#include <nova/Tools/Grids/Grid_Iterator_Face.h>
#include <cassert>
using namespace Nova;
//######################################################################
// Constructor
//######################################################################
template<class T,int d> Grid_Iterator_Face<T,d>::
Grid_Iterator_Face(const Grid<T,d>& grid_input,const int number_of_ghost_cells_input,const T_Region& region_type_input,const int side_input,int axis_input)
    :Base(grid_input),region_type(region_type_input),side(side_input),number_of_ghost_cells(number_of_ghost_cells_input)
{
    assert(side>=0 && side<=2*d);
    assert((region_type!=Grid<T,d>::Boundary_Interior_Region));
    if(region_type==Grid<T,d>::Boundary_Region && side) axis_input=(side+1)/2;
    if(axis_input)
    {
        single_axis=true;
        Reset_Axis(axis_input-1);
    }
    else
    {
        single_axis=false;
        Reset_Axis(0);
    }
}
//######################################################################
// Next_Helper
//######################################################################
template<class T,int d> void Grid_Iterator_Face<T,d>::
Next_Helper()
{
    Base::Next_Helper();
    if(!valid && !single_axis && axis<d-1) Reset_Axis(axis+1);
}
//######################################################################
// Reset_Axis
//######################################################################
template<class T,int d> void Grid_Iterator_Face<T,d>::
Reset_Axis(const int axis_input)
{
    axis=axis_input;
    Reset_Regions();
    Range<int,d> domain(grid.Cell_Indices(number_of_ghost_cells));

    switch(region_type)
    {
        case Grid<T,d>::Whole_Region:       assert(!side);
                                            ++domain.max_corner(axis);
                                            Add_Region(domain);
                                            break;

        case Grid<T,d>::Ghost_Region:       if(!side)
                                            {
                                                for(int side_iterator=1;side_iterator<=2*d;++side_iterator){
                                                    int axis_of_side=(side_iterator+1)/2;
                                                    if(side_iterator&1){
                                                        Range<int,d> domain_copy(domain);
                                                        ++domain_copy.max_corner(axis);
                                                        domain_copy.max_corner(axis_of_side-1)=domain_copy.min_corner(axis_of_side-1)+number_of_ghost_cells-1;
                                                        Add_Region(domain_copy);}
                                                    if(!(side_iterator&1)){
                                                        Range<int,d> domain_copy(domain);
                                                        ++domain_copy.max_corner(axis);
                                                        domain_copy.min_corner(axis_of_side-1)=domain_copy.max_corner(axis_of_side-1)-number_of_ghost_cells+1;
                                                        Add_Region(domain_copy);}}
                                            }
                                            else
                                            {
                                                int axis_of_side=(side+1)/2;
                                                if(side&1){
                                                    Range<int,d> domain_copy(domain);
                                                    ++domain_copy.max_corner(axis);
                                                    domain_copy.max_corner(axis_of_side-1)=domain_copy.min_corner(axis_of_side-1)+number_of_ghost_cells-1;
                                                    Add_Region(domain_copy);}
                                                if(!(side&1)){
                                                    Range<int,d> domain_copy(domain);
                                                    ++domain_copy.max_corner(axis);
                                                    domain_copy.min_corner(axis_of_side-1)=domain_copy.max_corner(axis_of_side-1)-number_of_ghost_cells+1;
                                                    Add_Region(domain_copy);}
                                            }
                                            break;

        case Grid<T,d>::Boundary_Region:    if(!side || side&1)
                                            {
                                                Range<int,d> domain_copy(domain);
                                                domain_copy.max_corner(axis)=domain.min_corner(axis);
                                                Add_Region(domain_copy);
                                            }
                                            if(!side || !(side&1))
                                            {
                                                Range<int,d> domain_copy(domain);
                                                domain_copy.min_corner(axis)=domain_copy.max_corner(axis)=domain.max_corner(axis)+1;
                                                Add_Region(domain_copy);
                                            }
                                            break;

        default:                            assert((region_type==Grid<T,d>::Interior_Region) && !side);
                                            ++domain.min_corner(axis);
                                            Add_Region(domain);
                                            break;
    }
    Reset();
}
//######################################################################
template class Nova::Grid_Iterator_Face<float,1>;
template class Nova::Grid_Iterator_Face<float,2>;
template class Nova::Grid_Iterator_Face<float,3>;
#ifdef COMPILE_WITH_DOUBLE_SUPPORT
template class Nova::Grid_Iterator_Face<double,1>;
template class Nova::Grid_Iterator_Face<double,2>;
template class Nova::Grid_Iterator_Face<double,3>;
#endif
