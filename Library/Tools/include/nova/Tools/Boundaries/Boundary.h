//!#####################################################################
//! \file Boundary.h
//!#####################################################################
// Class Boundary
//######################################################################
#ifndef __Boundary__
#define __Boundary__

#include <nova/Tools/Utilities/Non_Copyable.h>
#include <nova/Tools/Vectors/Vector.h>

namespace Nova{
template<class T,int d>
class Boundary: public Non_Copyable
{
    using TV_Bool2          = Vector<bool,2>;
    using TV_Sides          = Vector<TV_Bool2,d>;

  public:
    bool use_fixed_boundary,clamp_below,clamp_above;
    T fixed_boundary_value,lower_threshold,upper_threshold;
    TV_Sides constant_extrapolation;

    Boundary()
    {
        Set_Constant_Extrapolation();
        Set_Fixed_Boundary(false);
        Limit_Minimum_Boundary_Value(false);
        Limit_Maximum_Boundary_Value(false);
    }

    virtual ~Boundary() {}

    virtual void Set_Constant_Extrapolation(const TV_Sides& constant_extrapolation_input=TV_Sides({TV_Bool2(true)}))
    {constant_extrapolation=constant_extrapolation_input;}

    virtual bool Constant_Extrapolation(const int side) const
    {
        assert(0<=side && side <6);
        int axis=side/2;
        return constant_extrapolation(axis)((side%2==0)?1:0);
    }

    virtual void Set_Fixed_Boundary(const bool use_fixed_boundary_input=true,const T fixed_boundary_value_input=T())
    {use_fixed_boundary=use_fixed_boundary_input;fixed_boundary_value=fixed_boundary_value_input;
    if(use_fixed_boundary) clamp_above=clamp_below=false;}

    void Limit_Minimum_Boundary_Value(const bool clamp_below_input=true,const T lower_threshold_input=T())
    {clamp_below=clamp_below_input;lower_threshold=lower_threshold_input;}

    void Limit_Maximum_Boundary_Value(const bool clamp_above_input=true,const T upper_threshold_input=T())
    {clamp_above=clamp_above_input;upper_threshold=upper_threshold_input;}
};
}
#endif
