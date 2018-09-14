//!#####################################################################
//! \file Debug_Utilities.h
//!#####################################################################
// Class Debug_Utilities
//######################################################################
#ifndef __Debug_Utilities__
#define __Debug_Utilities__

#include <string>
#include <typeinfo>

namespace Nova{
#define FATAL_ERROR(...) \
    Fatal_Error((const char*)__FUNCTION__,__FILE__,__LINE__,##__VA_ARGS__)

#define FUNCTION_IS_NOT_DEFINED() \
    Function_Is_Not_Defined((const char*)__FUNCTION__,__FILE__,__LINE__,typeid(*this))

#define FUNCTION_NOT_IMPLEMENTED(...) \
    Not_Implemented((const char*)__FUNCTION__,__FILE__,__LINE__,##__VA_ARGS__)

void Fatal_Error(const char* function,const char* file,int line) __attribute__ ((noreturn));
void Fatal_Error(const char* function,const char* file,int line,const char* message) __attribute__ ((noreturn));
void Fatal_Error(const char* function,const char* file,int line,const std::string& message) __attribute__ ((noreturn));
void Not_Implemented(const char* function,const char* file,unsigned int line) __attribute__ ((noreturn));
void Not_Implemented(const char* function,const char* file,unsigned int line,const char* message) __attribute__ ((noreturn));
void Not_Implemented(const char* function,const char* file,unsigned int line,const std::string& message) __attribute__ ((noreturn));
void Function_Is_Not_Defined(const char* function,const char* file,unsigned int line,const std::type_info& type) __attribute__ ((noreturn));
}
#endif
