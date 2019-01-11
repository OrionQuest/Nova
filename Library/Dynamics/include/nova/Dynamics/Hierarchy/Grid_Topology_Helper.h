//!#####################################################################
//! \file Grid_Topology_Helper.h
//!#####################################################################
// Class Grid_Topology_Helper
//######################################################################
#ifndef __Grid_Topology_Helper__
#define __Grid_Topology_Helper__

#include <nova/Dynamics/Utilities/SPGrid_Flags.h>
#include <nova/SPGrid/Core/SPGrid_Mask.h>
#include <nova/Tools/Utilities/Range.h>
#include <nova/Tools/Utilities/Range_Iterator.h>
#include <nova/Tools/Vectors/Vector.h>
#include <cassert>

namespace Nova{

namespace{
template<class T_MASK> class Axis_Vector_Offset_Helper;

template<int log2_struct,int log2_field>
class Axis_Vector_Offset_Helper<SPGrid::SPGrid_Mask<log2_struct,log2_field,2> >
{
  public:
    enum{
        x_mask=SPGrid::SPGrid_Mask<log2_struct,log2_field,2>::template LinearOffset<1,0>::value,
        y_mask=SPGrid::SPGrid_Mask<log2_struct,log2_field,2>::template LinearOffset<0,1>::value,
        z_mask=0UL
    };
};

template<int log2_struct,int log2_field>
class Axis_Vector_Offset_Helper<SPGrid::SPGrid_Mask<log2_struct,log2_field,3> >
{
  public:
    enum{
        x_mask=SPGrid::SPGrid_Mask<log2_struct,log2_field,3>::template LinearOffset<1,0,0>::value,
        y_mask=SPGrid::SPGrid_Mask<log2_struct,log2_field,3>::template LinearOffset<0,1,0>::value,
        z_mask=SPGrid::SPGrid_Mask<log2_struct,log2_field,3>::template LinearOffset<0,0,1>::value
    };
};
}

namespace{
template<class T_MASK> class Negative_Axis_Vector_Offset_Helper;

template<int log2_struct,int log2_field>
class Negative_Axis_Vector_Offset_Helper<SPGrid::SPGrid_Mask<log2_struct,log2_field,2> >
{
  public:
    enum{
        x_mask=SPGrid::SPGrid_Mask<log2_struct,log2_field,2>::template LinearOffset<-1,0>::value,
        y_mask=SPGrid::SPGrid_Mask<log2_struct,log2_field,2>::template LinearOffset<0,-1>::value,
        z_mask=0UL
    };
};

template<int log2_struct,int log2_field>
class Negative_Axis_Vector_Offset_Helper<SPGrid::SPGrid_Mask<log2_struct,log2_field,3> >
{
  public:
    enum{
        x_mask=SPGrid::SPGrid_Mask<log2_struct,log2_field,3>::template LinearOffset<-1,0,0>::value,
        y_mask=SPGrid::SPGrid_Mask<log2_struct,log2_field,3>::template LinearOffset<0,-1,0>::value,
        z_mask=SPGrid::SPGrid_Mask<log2_struct,log2_field,3>::template LinearOffset<0,0,-1>::value
    };
};
}

namespace{
template<class T_MASK> struct Shadow_Grid_Stencil_Offsets_Helper;        
template<int log2_struct,int log2_field>
struct Shadow_Grid_Stencil_Offsets_Helper<SPGrid::SPGrid_Mask<log2_struct,log2_field,2> >
{
    enum{d=2};
    typedef SPGrid::SPGrid_Mask<log2_struct,log2_field,d> T_MASK;
    enum{og_xsize = (1u << T_MASK::block_xbits)+2,og_ysize = (1u << T_MASK::block_ybits)+2};
    enum{x_shift=og_ysize,y_shift=1};
    static int Offset(std::array<int,d> vector){return x_shift*vector[0]+y_shift*vector[1];}
};
template<int log2_struct,int log2_field>
struct Shadow_Grid_Stencil_Offsets_Helper<SPGrid::SPGrid_Mask<log2_struct,log2_field,3> >
{
    enum{d=3};
    typedef SPGrid::SPGrid_Mask<log2_struct,log2_field,d> T_MASK;
    enum{og_xsize = (1u << T_MASK::block_xbits)+2,og_ysize = (1u << T_MASK::block_ybits)+2,og_zsize = (1u << T_MASK::block_zbits)+2};
    enum{x_shift=og_ysize*og_zsize,y_shift=og_zsize,z_shift=1};
    static int Offset(std::array<int,d> vector){return x_shift*vector[0]+y_shift*vector[1]+z_shift*vector[2];}
};
}

template<class T_MASK>
class Grid_Topology_Helper
{
  public:
    enum {d=T_MASK::dim};
    enum {number_of_nodes_per_cell=1<<d};
    enum {number_of_faces_per_cell=2*d};
    enum {number_of_nodes_per_face=1<<(d-1)};

    using T_INDEX                   = Vector<int,d>;
    using T_INDEX2                  = Vector<int,d-1>;

    static void Nodes_Of_Face_Offsets(uint64_t nodes_of_face_offsets[number_of_nodes_per_face],const int axis)
    {
        assert(axis>=0 && axis<d);
        int node=0;
        for(Range_Iterator<d> iterator(Range<int,d>(T_INDEX(),T_INDEX(1)-T_INDEX::Axis_Vector(axis)));iterator.Valid();iterator.Next())
            nodes_of_face_offsets[node++]=T_MASK::Linear_Offset(iterator.Index()._data);
    }

    static void Nodes_Of_Cell_Offsets(uint64_t nodes_of_cell_offsets[number_of_nodes_per_cell])
    {
        int node=0;
        for(Range_Iterator<d> iterator(Range<int,d>(T_INDEX(),T_INDEX(1)));iterator.Valid();iterator.Next())
            nodes_of_cell_offsets[node++]=T_MASK::Linear_Offset(iterator.Index()._data);
    }

    static void Nodes_Of_Face_Shadow_Grid_Offsets(int nodes_of_face_offsets[number_of_nodes_per_face],int axis)
    {
        assert(axis>=0 && axis<d);
        int node=0;
        for(Range_Iterator<d> iterator(Range<int,d>(T_INDEX(),T_INDEX(1)-T_INDEX::Axis_Vector(axis)));iterator.Valid();iterator.Next()){
            const T_INDEX& index=iterator.Index();
            nodes_of_face_offsets[node++]=Shadow_Grid_Stencil_Offsets_Helper<T_MASK>::Offset(index._data);}
    }

    static void Nodes_Of_Cell_Shadow_Grid_Offsets(int nodes_of_cell_offsets[number_of_nodes_per_cell])
    {
        int node=0;
        for(Range_Iterator<d> iterator(Range<int,d>::Unit_Box());iterator.Valid();iterator.Next()){
            const T_INDEX& index=iterator.Index();
            nodes_of_cell_offsets[node++]=Shadow_Grid_Stencil_Offsets_Helper<T_MASK>::Offset(index._data);}
    }

    static void Face_Neighbor_Offsets(uint64_t face_neighbor_offsets[number_of_faces_per_cell])
    {
        for(int axis=0,face=0;axis<d;++axis) for(int side=-1;side<=1;side+=2)
        {
            T_INDEX shift;
            shift[axis]=side;
            face_neighbor_offsets[face++]=T_MASK::Linear_Offset(shift._data);
        }
    }

    static void Child_Face_Offsets(uint64_t offset,const int axis,uint64_t child_face_offsets[number_of_nodes_per_face])
    {
        uint64_t base_child_offset=T_MASK::UpsampleOffset(offset);int face=0;
        for(Range_Iterator<d-1> face_iterator(Range<int,d-1>::Unit_Box());face_iterator.Valid();face_iterator.Next()){
            uint64_t face_offset=base_child_offset;int face_axis=0;
            T_INDEX2 face_child_index=face_iterator.Index();
            for(int axis2=0;axis2<d;++axis2) if(axis2!=axis){
                if(face_child_index(face_axis)==1) face_offset=T_MASK::Packed_Add(face_offset,Axis_Vector_Offset(axis2));
                ++face_axis;}
            child_face_offsets[face++]=face_offset;}
    }

    static unsigned Face_Valid_Mask(int axis)
    {
        assert(axis>=0 && axis<=2);
        if(axis==0) return Face_Type_X_Valid;
        if(axis==1) return Face_Type_Y_Valid;
        return Face_Type_Z_Valid;
    }

    static unsigned Face_Active_Mask(int axis)
    {
        assert(axis>=0 && axis<=2);
        if(axis==0) return Face_Type_X_Active;
        if(axis==1) return Face_Type_Y_Active;
        return Face_Type_Z_Active;
    }

    static unsigned Face_Plus_Minus_Active_Mask(int axis,int side)
    {
        assert(axis>=0 && axis<=2 && side>=0 && side<=1);
        if(side==0)
        {
            if(axis==0) return Face_Minus_X_Active;
            if(axis==1) return Face_Minus_Y_Active;
            return Face_Minus_Z_Active;
        }
        else
        {
            if(axis==0) return Face_Plus_X_Active;
            if(axis==1) return Face_Plus_Y_Active;
            return Face_Plus_Z_Active;
        }
    }

    static unsigned Face_Plus_Minus_Scaled_Mask(int axis,int side)
    {
        assert(axis>=0 && axis<=2 && side>=0 && side<=1);
        if(side==0)
        {
            if(axis==0) return Face_Minus_X_Scaled;
            if(axis==1) return Face_Minus_Y_Scaled;
            return Face_Minus_Z_Scaled;
        }
        else
        {
            if(axis==0) return Face_Plus_X_Scaled;
            if(axis==1) return Face_Plus_Y_Scaled;
            return Face_Plus_Z_Scaled;
        }
    }

    static void Face_Shift_Offsets(uint64_t face_shifts[number_of_faces_per_cell])
    {
        for(int axis=0,face=0;axis<d;++axis) for(int side=0;side<=1;++side)
        {
            T_INDEX shift;
            shift[axis]=side;
            face_shifts[face++]=T_MASK::Linear_Offset(shift._data);
        }
    }

    static uint64_t Axis_Vector_Offset(int axis)
    {
        assert(axis>=0 && axis<=2);
        if(axis==0) return Axis_Vector_Offset_Helper<T_MASK>::x_mask;
        if(axis==1) return Axis_Vector_Offset_Helper<T_MASK>::y_mask;
        return Axis_Vector_Offset_Helper<T_MASK>::z_mask;
    }

    static uint64_t Negative_Axis_Vector_Offset(int axis)
    {
        assert(axis>=0 && axis<=2);
        if(axis==0) return Negative_Axis_Vector_Offset_Helper<T_MASK>::x_mask;
        if(axis==1) return Negative_Axis_Vector_Offset_Helper<T_MASK>::y_mask;
        return Negative_Axis_Vector_Offset_Helper<T_MASK>::z_mask;
    }
};
}
#endif
