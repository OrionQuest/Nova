//!#####################################################################
//! \file Triangulated_Surface.cpp
//!#####################################################################
#include <nova/Geometry/Basic_Geometry/Sphere.h>
#include <nova/Geometry/Topology_Based_Geometry/Triangulated_Surface.h>
using namespace Nova;
//######################################################################
// Initialize_Sphere_Tessellation
//######################################################################
template<class T> void Nova::Triangulated_Surface<T>::
Initialize_Sphere_Tessellation(const Sphere<T,d>& sphere,const int levels)
{
    points.Clear();elements.Clear();

    Add_Vertex(TV({-1, 0, 0}));
    Add_Vertex(TV({ 1, 0, 0}));
    Add_Vertex(TV({ 0,-1, 0}));
    Add_Vertex(TV({ 0, 1, 0}));
    Add_Vertex(TV({ 0, 0,-1}));
    Add_Vertex(TV({ 0, 0, 1}));

    Array<INDEX> *current_elements=new Array<INDEX>(8);
    current_elements->Append(INDEX({0,5,3}));
    current_elements->Append(INDEX({0,2,5}));
    current_elements->Append(INDEX({5,1,3}));
    current_elements->Append(INDEX({5,2,1}));
    current_elements->Append(INDEX({4,0,3}));
    current_elements->Append(INDEX({4,2,0}));
    current_elements->Append(INDEX({1,2,4}));
    current_elements->Append(INDEX({1,4,3}));

    for(int level=0;level<levels;++level){Array<INDEX> *new_elements=new Array<INDEX>();
        for(size_t i=0;i<current_elements->size();++i){const INDEX& e=(*current_elements)(i);
            TV p=(T).5*(points(e(0))+points(e(1))),q=(T).5*(points(e(1))+points(e(2))),r=(T).5*(points(e(0))+points(e(2)));
            int a=Add_Vertex(p)-1,b=Add_Vertex(q)-1,c=Add_Vertex(r)-1;

            new_elements->Append(INDEX({e(0),a,c}));
            new_elements->Append(INDEX({a,e(1),b}));
            new_elements->Append(INDEX({a,b,c}));
            new_elements->Append(INDEX({c,b,e(2)}));}
        delete current_elements;current_elements=new_elements;}

    for(size_t t=0;t<current_elements->size();++t) Add_Element((*current_elements)(t));

    for(size_t i=0;i<points.size();++i) points(i)=sphere.center+sphere.radius*points(i).Normalized();
    number_of_nodes=points.size();
}
//######################################################################
// Initialize_Ground_Plane
//
// Assumes the normal is (0,1,0)
//######################################################################
template<class T> void Nova::Triangulated_Surface<T>::
Initialize_Ground_Plane(const T height)
{
    points.Clear();elements.Clear();

    Add_Vertex(TV({-100, height, 100}));
    Add_Vertex(TV({ 100, height, 100}));
    Add_Vertex(TV({ 100, height,-100}));
    Add_Vertex(TV({-100, height,-100}));

    Add_Element(INDEX({0,1,3}));
    Add_Element(INDEX({1,2,3}));
    number_of_nodes=points.size();
}
//######################################################################
template class Nova::Triangulated_Surface<float>;
#ifndef COMPILE_WITH_DOUBLE_SUPPORT
template class Nova::Triangulated_Surface<double>;
#endif
