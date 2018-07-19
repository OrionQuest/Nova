//!#####################################################################
//! \file Krylov_Solver.h
//!#####################################################################
// Class Krylov_Solver
//######################################################################
#ifndef __Krylov_Solver__
#define __Krylov_Solver__

#include <nova/Tools/Krylov_Solvers/Krylov_System_Base.h>
#include <cassert>

namespace Nova{

enum Krylov_Solver_Type {krylov_solver_cg,krylov_solver_cr,krylov_solver_symmqmr};

template<class T>
class Krylov_Solver
{
    using Vector_Base           = Krylov_Vector_Base<T>;
    using System_Base           = Krylov_System_Base<T>;

  public:
    bool print_diagnostics,print_residuals;
    int* iterations_used;
    int restart_iterations;

    Krylov_Solver()
        :print_diagnostics(true),print_residuals(false),iterations_used(nullptr),restart_iterations(0)
    {}

    virtual ~Krylov_Solver()
    {
        if(iterations_used!=nullptr) delete iterations_used;
    }

    // version without preconditioning
    bool Solve(const System_Base& system,Vector_Base& x,const Vector_Base& b,Vector_Base& p,Vector_Base& ap,Vector_Base& ar,Vector_Base& r,
               const T tolerance,const int min_iterations,const int max_iterations)
    {
        Vector_Base* z=nullptr;
        assert(!system.use_preconditioner);
        return Solve(system,x,b,p,ap,ar,r,*z,tolerance,min_iterations,max_iterations);
    }

//######################################################################
    virtual bool Solve(const System_Base& system,Vector_Base& x,const Vector_Base& b,Vector_Base& q,Vector_Base& s,Vector_Base& r,Vector_Base& k,Vector_Base& z,
                       const T tolerance,const int min_iterations,const int max_iterations)=0;
//######################################################################
};
}
#endif
