//!#####################################################################
//! \file Box_Hierarchy.h
//!#####################################################################
// Class Box_Hierarchy
//######################################################################
#ifndef __Box_Hierarchy__
#define __Box_Hierarchy__

#include <nova/Geometry/Basic_Geometry/Box.h>
#include <nova/Tools/Arrays/Array.h>

namespace Nova{
template<class T,int d>
class Box_Hierarchy
{
    using TV            = Vector<T,d>;

  public:
    Box<T,d> box;
    Array<int> index;
    Array<Box_Hierarchy*> children;

    Box_Hierarchy() {}

    virtual ~Box_Hierarchy()
    {Clear();}

    void Clear()
    {
        index.Clear();
        for(unsigned i=0;i<children.size();++i) if(children[i]!=nullptr) delete children[i];
        children.Clear();
    }

  private:
    /*! Since this class allocates its own storage (children nodes), we cannot
     * allow the default copy constructor or assignment operator.
     */
    Box_Hierarchy(const Box_Hierarchy& other)
    {assert(false);}

    Box_Hierarchy& operator=(const Box_Hierarchy& other)
    {assert(false);return *this;}

//######################################################################
  public:
    void Construct_From_Leaf_Boxes(unsigned n,const Box<T,d>* input_box,unsigned max_depth=30,unsigned boxes_per_leaf=1);
    void Intersection_List(const TV& X,Array<int>& intersection_list);
    void Intersection_List(const Box<T,d>& input_box,Array<int>& intersection_list,const T thickness_over_two=0);
  protected:
    void Construct_Recursively(unsigned n,const Box<T,d>* input_box,unsigned max_depth,unsigned boxes_per_leaf,
                               unsigned current_depth,unsigned num_indices,unsigned* local_index,TV* local_centroid);
//######################################################################
};
}
#endif
