//!#####################################################################
//! \file Conjugate_Gradient.h
//!#####################################################################
// Class Conjugate_Gradient
//######################################################################
#ifndef __Conjugate_Gradient__
#define __Conjugate_Gradient__

#include <nova/Tools/Krylov_Solvers/Krylov_Solver.h>

namespace Nova{
template<class T>
class Conjugate_Gradient: public Krylov_Solver<T>
{
    using Base                  = Krylov_Solver<T>;
    using Vector_Base           = Krylov_Vector_Base<T>;
    using System_Base           = Krylov_System_Base<T>;

  public:
    using Base::restart_iterations;using Base::iterations_used;using Base::print_diagnostics;using Base::print_residuals;
    using Base::Solve;

    Conjugate_Gradient() {}

    virtual ~Conjugate_Gradient() {}

//######################################################################
    bool Solve(const System_Base& system,Vector_Base& x,const Vector_Base& b,Vector_Base& q,Vector_Base& s,Vector_Base& r,Vector_Base& k,Vector_Base& z,
               const T tolerance,const int min_iterations,const int max_iterations);
//######################################################################
};
}
#endif
