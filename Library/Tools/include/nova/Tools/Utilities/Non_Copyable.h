//!#####################################################################
//! \file Non_Copyable.h
//!#####################################################################
// Class Non_Copyable
//###################################################################### 
#ifndef __Non_Copyable__
#define __Non_Copyable__

namespace Nova{
class Non_Copyable
{
  protected:
    Non_Copyable() {}
    ~Non_Copyable() {}

  private:
    Non_Copyable(const Non_Copyable&);
    void operator=(const Non_Copyable&);
};
}
#endif
