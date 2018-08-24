//!####################################################################
//! \file SPGrid_Geometry.cpp
//!####################################################################
// Class SPGrid_Geometry
//#####################################################################
#include <nova/SPGrid/Core/SPGrid_Geometry.h>
#include <nova/Tools/Log/Debug_Utilities.h>
using namespace Nova;
using namespace SPGrid;
//#####################################################################
// Function Check_Bounds
//#####################################################################
void SPGrid_Geometry<3>::
Check_Bounds(const ucoord_t i,const ucoord_t j,const ucoord_t k) const
{
    if(i>=xsize_padded || j>=ysize_padded || k>=zsize_padded)
        FATAL_ERROR("Array indices ("+Value_To_String(i)+","+Value_To_String(j)+","+Value_To_String(k)+") out of hard bounds");
    if(i>=xsize || j>=ysize || k>=zsize)
        FATAL_ERROR("Array indices ("+Value_To_String(i)+","+Value_To_String(j)+","+Value_To_String(k)+") out of soft bounds");
}
//#####################################################################
void SPGrid_Geometry<2>::
Check_Bounds(const ucoord_t i,const ucoord_t j) const
{
    if(i>=xsize_padded || j>=ysize_padded)
        FATAL_ERROR("Array indices ("+Value_To_String(i)+","+Value_To_String(j)+") out of hard bounds");
    if(i>=xsize || j>=ysize)
        FATAL_ERROR("Array indices ("+Value_To_String(i)+","+Value_To_String(j)+") out of soft bounds");
}
//#####################################################################
