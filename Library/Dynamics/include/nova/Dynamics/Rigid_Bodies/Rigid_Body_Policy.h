//!#####################################################################
//! \file Rigid_Body_Policy.h
//!#####################################################################
// Class Rigid_Body_Policy
//######################################################################
#ifndef __Rigid_Body_Policy__
#define __Rigid_Body_Policy__

#include <nova/Tools/Matrices/Diagonal_Matrix.h>
#include <nova/Tools/Matrices/Matrix_1x1.h>

namespace Nova{
template<class T,int d> class Rigid_Body_Policy;

template<class T>
class Rigid_Body_Policy<T,2>
{
  public:
    typedef Matrix<T,1> T_Inertia_Tensor;
};

template<class T>
class Rigid_Body_Policy<T,3>
{
  public:
    typedef Diagonal_Matrix<T,3> T_Inertia_Tensor;
};
}
#endif
