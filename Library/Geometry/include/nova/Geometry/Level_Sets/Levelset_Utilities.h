//!#####################################################################
//! \file Levelset_Utilities.h
//!#####################################################################
// Class Levelset_Utilities
//######################################################################
#ifndef __Levelset_Utilities__
#define __Levelset_Utilities__

#include <nova/Tools/Utilities/Non_Copyable.h>

namespace Nova{
template<class T>
class Levelset_Utilities: public Non_Copyable
{
  protected:
    Levelset_Utilities() {}
    virtual ~Levelset_Utilities() {}

  public:
    static T Sign(const T phi)
    {if(phi<=0) return -1;else return 1;}

    static bool Interface(const T phi_1,const T phi_2)
    {if((phi_1>0 && phi_2<=0) || (phi_1<=0 && phi_2>0)) return true;else return false;}

    static T Theta(const T phi_left,const T phi_right)
    {
        assert(phi_left != phi_right);
        return phi_left/(phi_left-phi_right);
    }
};
}
#endif
