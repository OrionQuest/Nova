//!#####################################################################
//! \file Debug_Utilities.cpp
//!#####################################################################
#include <nova/Tools/Log/Debug_Utilities.h>
#include <nova/Tools/Log/Log.h>
#include <iostream>
#include <stdexcept>
using namespace Nova;
//######################################################################
// Fatal_Error
//######################################################################
void Nova::Fatal_Error(const char* function,const char* file,int line)
{
    Fatal_Error(function,file,line,"Fatal error");
}
void Nova::Fatal_Error(const char* function,const char* file,int line,const char* message)
{
    Fatal_Error(function,file,line,std::string(message));
}
void Nova::Fatal_Error(const char* function,const char* file,int line,const std::string& message)
{
    static char buffer[2048];
    sprintf(buffer,"%s:%s:%d: %s",file,function,line,message.c_str());
    std::string error(buffer);
    Log::cout<<std::flush;Log::cerr<<"\n";
    Log::cerr<<"\n*** ERROR: "<<error<<'\n'<<std::endl;
    throw std::runtime_error(error);
}
//######################################################################
// Not_Implemented
//######################################################################
void Nova::Not_Implemented(const char* function,const char* file,unsigned int line)
{
    Not_Implemented(function,file,line,"Not implemented");
}
void Nova::Not_Implemented(const char* function,const char* file,unsigned int line,const char* message)
{
    Not_Implemented(function,file,line,std::string(message));
}
void Nova::Not_Implemented(const char* function,const char* file,unsigned int line,const std::string& message)
{
    static char buffer[2048];
    sprintf(buffer,"%s:%s:%d: Not implemented: %s",file,function,line,message.c_str());
    std::string error(buffer);
    Log::cout<<std::flush;Log::cerr<<"\n";
    Log::cerr<<"\n*** ERROR: "<<error<<'\n'<<std::endl;
    throw std::runtime_error(error);
}
//######################################################################
// Function_Is_Not_Defined
//######################################################################
void Nova::Function_Is_Not_Defined(const char* function,const char* file,unsigned int line,const std::type_info& type)
{
    static char buffer[2048];
    sprintf(buffer,"%s:%s:%d: Function not defined by %s",file,function,line,type.name());
    std::string error(buffer);
    Log::cout<<std::flush;Log::cerr<<"\n";
    Log::cerr<<"\n*** ERROR: "<<error<<'\n'<<std::endl;
    throw std::runtime_error(error);
}
//######################################################################
