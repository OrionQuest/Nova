//!#####################################################################
//! \file Segmented_Curve_2D.cpp
//!#####################################################################
#include <nova/Geometry/Basic_Geometry/Sphere.h>
#include <nova/Geometry/Topology_Based_Geometry/Segmented_Curve.h>
#include <nova/Tools/Utilities/Constants.h>
using namespace Nova;
//######################################################################
// Initialize_Sphere_Tessellation
//######################################################################
template<class T,int d> void Nova::Segmented_Curve<T,d>::
Initialize_Sphere_Tessellation(const Sphere<T,d>& sphere,const int levels)
{
    points.Clear();elements.Clear();

    int n=1<<levels;
    for(int i=1;i<=n;++i) Add_Vertex(TV({(T)cos(i*(T)two_pi/n),(T)sin(i*(T)two_pi/n)})*sphere.radius+sphere.center);
    for(int i=0;i<n-1;++i) Add_Element(INDEX({i,i+1}));
    Add_Element(INDEX({n-1,0}));
    number_of_nodes=points.size();
}
//######################################################################
template class Nova::Segmented_Curve<float,2>;
#ifdef COMPILE_WITH_DOUBLE_SUPPORT
template class Nova::Segmented_Curve<double,2>;
#endif
