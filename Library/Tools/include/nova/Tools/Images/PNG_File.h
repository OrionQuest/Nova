//!#####################################################################
//! \file PNG_File.h
//!#####################################################################
// Class PNG_File
//######################################################################
#ifndef __PNG_File__
#define __PNG_File__

#include <nova/Tools/Arrays/Array_ND.h>
#include <nova/Tools/Vectors/Vector.h>
#include <string>

namespace Nova{
template<class T>
class PNG_File
{
    using T_INDEX           = Vector<int,2>;

  public:
    PNG_File() {}
    ~PNG_File() {}

//######################################################################
    static void Read(const std::string& filename,Array_ND<Vector<T,3>,2>& image);
    template<int d> static void Write(const std::string& filename,const Array_ND<Vector<T,d>,2>& image);
//######################################################################
};
}
#endif
