//!#####################################################################
//! \file File_Utilities.cpp
//!#####################################################################
#include <nova/Tools/Utilities/File_Utilities.h>
#include <cstdio>
namespace Nova{
namespace File_Utilities
{
    std::string Get_Working_Directory()
    {return boost::filesystem::current_path().string();}

    bool Create_Directory(const std::string& directory)
    {
        std::string current_directory=Get_Working_Directory();
        std::string directory_name=current_directory+"/"+directory;
        boost::filesystem::path directory_path(directory_name);
        return boost::filesystem::create_directory(directory_path);
    }

    bool File_Exists(const std::string& filename,const bool use_absolute_path)
    {
        std::string absolute_filename=use_absolute_path?Get_Working_Directory()+"/"+filename:filename;
        boost::filesystem::path filename_path(absolute_filename);
        return boost::filesystem::exists(filename_path);
    }

    std::istream* Safe_Open_Input(const std::string& filename,bool binary)
    {
        std::ios_base::openmode flags=std::ios::in;
        if(binary) flags|=std::ios::binary;
        std::istream* input=new std::ifstream(filename.c_str(),flags);
        if(input!=nullptr) return input;
        std::cerr<<"Can't open "<<filename<<" for read "<<(binary?"binary":"")<<std::endl;
        exit(0);
    }

    FILE* Temporary_File()
    {return std::tmpfile();}

    std::ostream* Safe_Open_Output(const std::string& filename,bool binary)
    {
        std::ios_base::openmode flags=std::ios::out;
        if(binary) flags|=std::ios::binary;
        std::ostream* output=new std::ofstream(filename.c_str(),flags);
        if(output!=nullptr) return output;
        std::cerr<<"Can't open "<<filename<<" for write "<<(binary?"(binary)":"")<<std::endl;
        exit(0);
    }
}
}
