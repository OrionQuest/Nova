//!#####################################################################
//! \file Image.h
//!#####################################################################
// Class Image
//######################################################################
#ifndef __Image__
#define __Image__

#include <nova/Tools/Vectors/Vector.h>

namespace Nova{
template<class T>
class Image
{
  public:
    Image() {}
    ~Image() {}

    template<int d> static Vector<T,d> Byte_Color_To_Scalar_Color(const Vector<unsigned char,d>& color_in)
    {return (Vector<T,d>(color_in)+(T).5*Vector<T,d>(1))/(T)256;}

    static T Byte_Color_To_Scalar_Color(const unsigned char color_in)
    {return ((T)color_in+(T).5)/(T)256;}

    template<int d> static Vector<unsigned char,d> Scalar_Color_To_Byte_Color(const Vector<T,d>& color_in)
    {
        Vector<int,d> ret((T)256*color_in);
        for(int axis=0;axis<d;++axis){
            ret(axis)=std::max(ret(axis),(int)0);
            ret(axis)=std::min(ret(axis),(int)255);}
        return Vector<unsigned char,d>(ret);
    }

    static unsigned char Scalar_Color_To_Byte_Color(const T color_in)
    {
        int ret=(int)((T)256*color_in);
        ret=std::max(ret,(int)0);
        ret=std::min(ret,(int)255);
        return ret;
    }
};
}
#endif
