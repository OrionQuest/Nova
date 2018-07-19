//!#####################################################################
//! \file Conjugate_Gradient.cpp
//!#####################################################################
#include <nova/Tools/Krylov_Solvers/Conjugate_Gradient.h>
#include <nova/Tools/Log/Log.h>
#include <cfloat>
#include <limits>
using namespace Nova;
//######################################################################
// Solve
//######################################################################
template<class T> bool Conjugate_Gradient<T>::
Solve(const System_Base& system,Vector_Base& x,const Vector_Base& b,Vector_Base& q,Vector_Base& s,Vector_Base& r,Vector_Base& k,Vector_Base& z,
      const T tolerance,const int min_iterations,const int max_iterations)
{
    static const T small_number=std::numeric_limits<T>::epsilon();
    system.Set_Boundary_Conditions(x);
    T rho_old=(T)FLT_MAX;T convergence_norm=0;
    int iterations;for(iterations=0;;iterations++){
        bool restart=!iterations || (restart_iterations && iterations%restart_iterations==0);
        if(restart){
            if(print_residuals) Log::cout<<"restarting cg"<<std::endl;
            r=b;system.Multiply(x,q);r-=q;system.Project(r);}
        // stopping conditions
        system.Project_Nullspace(r);
        convergence_norm=system.Convergence_Norm(r);
        if(print_residuals) Log::cout<<convergence_norm<<std::endl;
        if(convergence_norm<=tolerance && (iterations>=min_iterations || convergence_norm<small_number)){
            if(print_diagnostics) Log::Stat("cg iterations",iterations);if(iterations_used) *iterations_used=iterations;return true;}
        if(iterations==max_iterations) break;
        // actual iteration
        const Vector_Base& mr=system.Precondition(r,z);
        T rho=(T)system.Inner_Product(mr,r);
        if(restart) s=mr;
        else s.Copy(rho/rho_old,s,mr);
        system.Multiply(s,q);
        system.Project(q);
        T s_dot_q=(T)system.Inner_Product(s,q);
        if(s_dot_q<=0) Log::cout<<"CG: matrix appears indefinite or singular, s_dot_q/s_dot_s="<<s_dot_q/(T)system.Inner_Product(s,s)<<std::endl;
        T alpha=s_dot_q?rho/s_dot_q:(T)FLT_MAX;
        x.Copy(alpha,s,x);
        r.Copy(-alpha,q,r);
        rho_old=rho;}

    if(print_diagnostics) Log::Stat("cg iterations",iterations);if(iterations_used) *iterations_used=iterations;
    if(print_diagnostics) Log::cout<<"cg not converged after "<<max_iterations<<" iterations, error = "<<convergence_norm<<std::endl;
    return false;
}
//######################################################################
template class Nova::Conjugate_Gradient<float>;
#ifdef COMPILE_WITH_DOUBLE_SUPPORT
template class Nova::Conjugate_Gradient<double>;
#endif
