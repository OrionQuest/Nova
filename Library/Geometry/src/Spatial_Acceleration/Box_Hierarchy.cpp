//!#####################################################################
//! \file Box_Hierarchy.cpp
//!#####################################################################
#include <nova/Geometry/Spatial_Acceleration/Box_Hierarchy.h>
#include <nova/Tools/Log/Log.h>
using namespace Nova;
//######################################################################
// Construct_From_Leaf_Boxes
//######################################################################
template<class T,int d> void Box_Hierarchy<T,d>::
Construct_From_Leaf_Boxes(unsigned n,const Box<T,d>* input_box,unsigned max_depth,unsigned boxes_per_leaf)
{
    Log::Scope scope("Construct_From_Leaf_Boxes");

    assert(n>=1 && input_box && boxes_per_leaf>0);
    Clear();

    // set up a list of all point indices (initially in order 0,1,...,n-1) and box centroids
    unsigned *global_index=new unsigned[n];
    TV *global_centroid=new TV[n];
    for(unsigned i=0;i<n;++i){global_index[i]=i;
        global_centroid[i]=(T).5*(input_box[i].min_corner+input_box[i].max_corner);}

    // now hand this off to the recursive function which does the actual work
    Construct_Recursively(n,input_box,max_depth,boxes_per_leaf,0,n,global_index,global_centroid);

    // clean up
    delete[] global_index;
    delete[] global_centroid;
}
//######################################################################
// Intersection_List
//######################################################################
template<class T,int d> void Box_Hierarchy<T,d>::
Intersection_List(const TV& X,Array<int>& intersection_list)
{
    Array<const Box_Hierarchy<T,d>*> stack;
    stack.Append(this);
    while(!stack.Empty()){const Box_Hierarchy<T,d>* node=stack.Back();
        stack.Pop_Back();
        // check if node is a leaf and if there is an intersection
        if(node->box.Inside(X)){
            for(size_t i=0;i<node->index.size();++i) intersection_list.Append(node->index[i]);

            // push children of the node on the stack
            for(size_t i=0;i<node->children.size();++i) stack.Append(node->children[i]);}}
}
//######################################################################
// Intersection_List
//######################################################################
template<class T,int d> void Box_Hierarchy<T,d>::
Intersection_List(const Box<T,d>& input_box,Array<int>& intersection_list,const T thickness_over_two)
{
    Array<const Box_Hierarchy<T,d>*> stack;
    stack.Append(this);
    while(!stack.Empty()){const Box_Hierarchy<T,d>* node=stack.Back();
        stack.Pop_Back();
        // check if node is a leaf and if there is an intersection
        if(input_box.Intersection(node->box,thickness_over_two)){
            for(size_t i=0;i<node->index.size();++i) intersection_list.Append(node->index[i]);

            // push children of the node on the stack
            for(size_t i=0;i<node->children.size();++i) stack.Append(node->children[i]);}}
}
//######################################################################
// Construct_Recursively
//######################################################################
template<class T,int d> void Box_Hierarchy<T,d>::
Construct_Recursively(unsigned n,const Box<T,d>* input_box,unsigned max_depth,unsigned boxes_per_leaf,
                      unsigned current_depth,unsigned num_indices,unsigned* local_index,TV* centroid)
{
    assert(num_indices>0);
    // find bounding box for everything contained in this node
    box=input_box[local_index[0]];
    for(unsigned i=1;i<num_indices;++i)
        box.Enlarge_To_Include_Box(input_box[local_index[i]]);

    // check for base case or recurse
    if(current_depth==max_depth || num_indices<=boxes_per_leaf)
    {
        // base case of recursion (keep *this as a leaf, with the given points)
        index.resize(num_indices);
        for(unsigned i=0;i<num_indices;++i) index[i]=local_index[i];
    }
    else
    {
        // see if we can split up the indices between two child nodes
        // figure out which axis to split along - from bounding box of centroids
        Box<T,d> centroid_box(centroid[local_index[0]]);
        TV mean(centroid[local_index[0]]);
        for(unsigned i=1;i<num_indices;++i){
            centroid_box.Enlarge_To_Include_Point(centroid[local_index[i]]);
            mean+=centroid[local_index[i]];}
        mean*=(T)1./num_indices;

        // check if all the centroids were equal
        if(centroid_box.max_corner==centroid_box.min_corner)
        {
            // leave this is as a leaf, since we can't tell the centroids apart
            index.resize(num_indices);
            for(unsigned i=0;i<num_indices;++i) index[i]=local_index[i];
        }
        else
        {
            // otherwise, split along a selected axis
            int axis=0;
            for(unsigned a=1;a<d;++a)
                if(centroid_box.max_corner(a)-centroid_box.min_corner(a) > centroid_box.max_corner(axis)-centroid_box.min_corner(axis)) axis=a;

            // pick mean as a splitting value (since axis bounds were tight, there must be at least one point on either side of split)
            T split=mean(axis);
            if(split<=centroid_box.min_corner(axis) || split>=centroid_box.max_corner(axis))
                split=(T).5*(centroid_box.min_corner(axis)+centroid_box.max_corner(axis));

            // do the quicksort-style in-place split of the indices
            int i=0,j=num_indices-1;
            for(;;){
                while((unsigned)i<num_indices && centroid[local_index[i]][axis]<=split) ++i;
                while(j>=0 && centroid[local_index[j]][axis]>split) --j;
                if(i<j)
                {
                    std::swap(local_index[i],local_index[j]);
                    ++i;
                    --j;
                }
                else break;}

            assert((unsigned)i<num_indices);
            // now indices[0] to indices[i-1] inclusive should all be located <=split (i boxes),
            // and indices[i] to indices[num_indices-1] inclusive should be located >split (num_indices-i boxes)
            children.resize(2,0);
            children[0]=new Box_Hierarchy<T,d>();
            children[0]->Construct_Recursively(n,input_box,max_depth,boxes_per_leaf,current_depth+1,i,local_index,centroid);
            children[1]=new Box_Hierarchy<T,d>();
            children[1]->Construct_Recursively(n,input_box,max_depth,boxes_per_leaf,current_depth+1,num_indices-i,local_index+i,centroid);
        }
    }
}
//######################################################################
template class Nova::Box_Hierarchy<float,2>;
template class Nova::Box_Hierarchy<float,3>;
#ifdef COMPILE_WITH_DOUBLE_SUPPORT
template class Nova::Box_Hierarchy<double,2>;
template class Nova::Box_Hierarchy<double,3>;
#endif
