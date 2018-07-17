//!#####################################################################
//! \file Log.h
//!#####################################################################
// Class Log
//######################################################################
#ifndef __Log__
#define __Log__

#include <nova/Tools/Utilities/Non_Copyable.h>
#include <nova/Tools/Utilities/Timer.h>
#include <cassert>
#include <ostream>
#include <sstream>

namespace Nova{

#define Log_Real Log

namespace Log_Null{
struct log_null_class{
    template<class T> log_null_class& operator<<(const T&) {return *this;}
    log_null_class& operator<<(std::ostream& (*)(std::ostream&)) {return *this;}
    template<class T> void flags(const T&);
    template<class T> void width(const T&);
    template<class A,class B> void Copy_Log_To_File(const A&,const B&) {}
};
extern log_null_class cout;
extern log_null_class cerr;
template<class T> inline void Time(const T&) {}
template<class A,class B> inline void Stat(const A&,const B&) {}
inline void Stop_Time() {}
inline void Finish_Logging() {}
template<class A,class B,class C,class D> inline void Initialize_Logging(const A&,const B&,const C&,const D&) {}
inline log_null_class* Instance() {return &cout;}

struct Scope
{
    template<class A> Scope(const A&) {}
    template<class A,class B> Scope(const A&,const B&) {}
    template<class A,class B,class C> Scope(const A&,const B&,const C&) {}
    template<class A,class B,class C,class D> Scope(const A&,const B&,const C&,const D&) {}
    template<class A,class B,class C,class D,class E> Scope(const A&,const B&,const C&,const D&,const E&) {}
    void Pop() {}
};
}

namespace Log_Real{

class Log_Entry;
class Log_Scope;

class Log_Class
{
    friend class Log_Entry;
    friend class Log_Scope;
    friend class Log_Cout_Buffer;
    friend class Log_Cerr_Buffer;
    friend void Reset();
    friend void Dump_Log();

  public:
    Timer* timer_singleton;
    int timer_id;
    bool suppress_cout,suppress_cerr;
    bool suppress_timing;
    FILE* log_file;
    int verbosity_level;
    bool log_file_temporary;
    bool xml;

    Log_Entry* root;
    Log_Entry* current_entry;

    Log_Class(const bool suppress_cout,const bool suppress_cerr,const bool suppress_timing,const int verbosity_level,const bool cache_initial_output);
    ~Log_Class();

    static void Push_Scope(const std::string& scope_identifier,const std::string& scope_name);
    static void Pop_Scope();
    static void Time_Helper(const std::string& label);
    void Copy_Log_To_File(const std::string& filename,const bool append);
};

class Scope: public Non_Copyable
{
    bool active;

  public:
    Scope()
        :active(false)
    {}

    Scope(const std::string& scope_identifier)
        :active(true)
    {
        Log_Class::Push_Scope(scope_identifier,scope_identifier);
    }

    Scope(const std::string& scope_identifier,const std::string& scope_name)
        :active(true)
    {
        Log_Class::Push_Scope(scope_identifier,scope_name);
    }

    ~Scope();

    void Push(const std::string& scope_identifier,const std::string& scope_name)
    {assert(!active);active=true;Log_Class::Push_Scope(scope_identifier,scope_name);}

    void Pop()
    {assert(active);active=false;Log_Class::Pop_Scope();}
};

Log_Class* Instance();
std::ostream& cout_Helper();
std::ostream& cerr_Helper();
namespace{
    static std::ostream& cout = ::Nova::Log_Real::cout_Helper();
    static std::ostream& cerr = ::Nova::Log_Real::cerr_Helper();
}

void Initialize_Logging(const bool suppress_cout_input=false,const bool suppress_timing_input=false,const int verbosity_level_input=1<<30,const bool cache_initial_output=false);
void Finish_Logging();
void Stop_Time();
void Stat_Helper(const std::string& label,const std::stringstream& s);

template<class T_VALUE> void
Stat(const std::string& label,const T_VALUE& value)
{std::stringstream s;s<<value;Stat_Helper(label,s);}
void Reset();
void Dump_log();

inline void Time(const std::string& format)
{
    if(Instance()->suppress_timing) return;
    Log_Class::Time_Helper(format);
}
}
}
#endif
