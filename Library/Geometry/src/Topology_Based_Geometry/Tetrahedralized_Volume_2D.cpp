//!#####################################################################
//! \file Tetrahedralized_Volume_2D.cpp
//!#####################################################################
#include <nova/Geometry/Topology_Based_Geometry/Tetrahedralized_Volume.h>
using namespace Nova;
//######################################################################
// Initialize_Cube_Mesh
//######################################################################
template<class T,int d> void Nova::Tetrahedralized_Volume<T,d>::
Initialize_Cube_Mesh(const Grid<T,d>& grid)
{
    points.Clear();elements.Clear();
    int m=grid.counts(0)+1,n=grid.counts(1)+1;
    for(int i=1;i<=m-1;++i) for(int j=1;j<=n-1;++j){    // counter-clockwise node ordering
        if(i%2){Add_Element(INDEX({i-1+m*(j-1),i+m*(j-1),i-1+m*j}));
            Add_Element(INDEX({i+m*(j-1),i+m*j,i-1+m*j}));}
        else{Add_Element(INDEX({i-1+m*(j-1),i+m*(j-1),i+m*j}));
            Add_Element(INDEX({i-1+m*(j-1),i+m*j,i-1+m*j}));}}

    for(int j=1;j<=n;++j) for(int i=1;i<=m;++i) Add_Vertex(grid.Node(T_INDEX({i,j})));
    number_of_nodes=points.size();
}
//######################################################################
// Get_Element
//######################################################################
template<class T,int d> typename Nova::Tetrahedralized_Volume_Policy<T,d>::T_Element Nova::Tetrahedralized_Volume<T,d>::
Get_Element(const int id) const
{
    assert(id>=0 && id<elements.size());
    const INDEX& e=elements(id);
    return T_Element(points(e(0)),points(e(1)),points(e(2)));
}
//######################################################################
template class Nova::Tetrahedralized_Volume<float,2>;
#ifdef COMPILE_WITH_DOUBLE_SUPPORT
template class Nova::Tetrahedralized_Volume<double,2>;
#endif
