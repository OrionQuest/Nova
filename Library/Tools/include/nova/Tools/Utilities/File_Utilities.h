//!#####################################################################
//! \file File_Utilities.h
//!#####################################################################
// Class File_Utilities
//######################################################################
#ifndef __File_Utilities__
#define __File_Utilities__

#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#undef BOOST_NO_CXX11_SCOPED_ENUMS
#include <nova/Tools/Read_Write/Utilities/Read_Write.h>
#include <fstream>
#include <iostream>

namespace Nova{
namespace File_Utilities
{
    std::string Get_Working_Directory();
    bool Create_Directory(const std::string& directory);
    bool File_Exists(const std::string& filename,const bool use_absolute_path=true);
    std::istream* Safe_Open_Input(const std::string& filename,bool binary=true);
    std::ostream* Safe_Open_Output(const std::string& filename,bool binary=true);

    template<class T> inline void Read_From_File(const std::string& filename,T& object)
    {
        std::istream* input=Safe_Open_Input(filename);
        Read_Write<T>::Read(*input,object);
        delete input;
    }

    FILE* Temporary_File();

    template<class T> inline void Write_To_File(const std::string& filename,const T& object)
    {
        std::ostream* output=Safe_Open_Output(filename);
        Read_Write<T>::Write(*output,object);
        delete output;
    }

    template<class T> inline void Write_To_Text_File(const std::string& filename,const T& object)
    {
        std::ostream* output=Safe_Open_Output(filename,false);
        *output<<object;
        delete output;
    }

    template<class T> inline void Read_From_Text_File(const std::string& filename,T& object)
    {
        std::istream* input=Safe_Open_Input(filename,false);
        *input>>object;
        delete input;
    }
}
}
#endif
