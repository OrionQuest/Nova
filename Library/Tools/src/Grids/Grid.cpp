//!#####################################################################
//! \file Grid.cpp
//!#####################################################################
#include <nova/Tools/Grids/Grid.h>
#include <nova/Tools/Utilities/Range_Iterator.h>
using namespace Nova;
//######################################################################
// Initialize
//######################################################################
template<class T,int d> void Grid<T,d>::
Initialize(const T_INDEX& counts_input,const Range<T,d>& domain_input)
{
    counts=counts_input;
    domain=domain_input;
    if(counts.Min()>0)
    {
        dX=domain.Edge_Lengths()/TV(counts);
        one_over_dX=(T)1./dX;
    }
}
//######################################################################
// Nodes_In_Cell_From_Minimum_Corner_Node
//######################################################################
template<class T,int d> void Grid<T,d>::
Nodes_In_Cell_From_Minimum_Corner_Node(const T_INDEX& minimum_corner_node,T_INDEX nodes[number_of_nodes_per_cell]) const
{
    int node=0;
    for(Range_Iterator<d> iterator(Range<int,d>(T_INDEX(),T_INDEX(1)));iterator.Valid();iterator.Next())
        nodes[node++]=minimum_corner_node+iterator.Index();
}
//######################################################################
template class Nova::Grid<float,1>;
template class Nova::Grid<float,2>;
template class Nova::Grid<float,3>;
#ifdef COMPILE_WITH_DOUBLE_SUPPORT
template class Nova::Grid<double,1>;
template class Nova::Grid<double,2>;
template class Nova::Grid<double,3>;
#endif
