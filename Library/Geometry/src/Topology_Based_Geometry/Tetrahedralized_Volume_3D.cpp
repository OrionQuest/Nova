//!#####################################################################
//! \file Tetrahedralized_Volume_3D.cpp
//!#####################################################################
#include <nova/Geometry/Topology_Based_Geometry/Tetrahedralized_Volume.h>
using namespace Nova;
//######################################################################
// Lattice
//######################################################################
static inline int Lattice(const int i,const int j,const int k,const int m,const int n,const int p)
{return i+m*(j-1)+m*n*(k-1)-1;}
//######################################################################
// Initialize_Cube_Mesh
//######################################################################
template<class T,int d> void Tetrahedralized_Volume<T,d>::
Initialize_Cube_Mesh(const Grid<T,d>& grid)
{
    points.Clear();elements.Clear();
    int m=grid.counts(0)+1,n=grid.counts(1)+1,p=grid.counts(2)+1;
    for(int i=1;i<=m-1;i++)for(int j=1;j<=n-1;j++)for(int k=1;k<=p-1;k++){
        Add_Element(INDEX({Lattice(i,j,k,m,n,p),Lattice(i+1,j,k,m,n,p),Lattice(i+1,j+1,k,m,n,p),Lattice(i+1,j+1,k+1,m,n,p)}));
        Add_Element(INDEX({Lattice(i,j,k,m,n,p),Lattice(i+1,j,k,m,n,p),Lattice(i+1,j+1,k+1,m,n,p),Lattice(i+1,j,k+1,m,n,p)}));
        Add_Element(INDEX({Lattice(i,j,k,m,n,p),Lattice(i+1,j,k+1,m,n,p),Lattice(i+1,j+1,k+1,m,n,p),Lattice(i,j,k+1,m,n,p)}));
        Add_Element(INDEX({Lattice(i,j,k,m,n,p),Lattice(i+1,j+1,k+1,m,n,p),Lattice(i,j+1,k+1,m,n,p),Lattice(i,j,k+1,m,n,p)}));
        Add_Element(INDEX({Lattice(i,j,k,m,n,p),Lattice(i+1,j+1,k+1,m,n,p),Lattice(i,j+1,k,m,n,p),Lattice(i,j+1,k+1,m,n,p)}));
        Add_Element(INDEX({Lattice(i,j,k,m,n,p),Lattice(i+1,j+1,k,m,n,p),Lattice(i,j+1,k,m,n,p),Lattice(i+1,j+1,k+1,m,n,p)}));}

    for(int k=1;k<=p;k++) for(int j=1;j<=n;j++) for(int i=1;i<=m;i++) Add_Vertex(grid.Node(T_INDEX({i,j,k})));
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
    return T_Element(points(e(0)),points(e(1)),points(e(2)),points(e(3)));
}
//######################################################################
template class Nova::Tetrahedralized_Volume<float,3>;
#ifdef COMPILE_WITH_DOUBLE_SUPPORT
template class Nova::Tetrahedralized_Volume<double,3>;
#endif
