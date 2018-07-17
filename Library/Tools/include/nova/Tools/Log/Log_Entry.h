//!#####################################################################
//! \file Log_Entry.h
//!#####################################################################
// Class Log_Entry
//######################################################################
#ifndef __Log_Entry__
#define __Log_Entry__

#include <nova/Tools/Arrays/Array.h>
#include <nova/Tools/Log/Log.h>
#include <nova/Tools/Utilities/Non_Copyable.h>
#include <nova/Tools/Utilities/Timer.h>
#include <string>

namespace Nova{
namespace Log_Real{
class Log_Entry: public Non_Copyable
{
  public:
    Log_Entry* parent;
    int depth,timer_id;
    double time,timer_start_time;
    std::string name;
    bool end_on_separate_line,log_file_end_on_separate_line;
    static bool start_on_separate_line,log_file_start_on_separate_line,needs_indent,log_file_needs_indent;
    int& verbosity_level;

    Log_Entry(Log_Entry* parent_input,const int depth_input,const int timer_id_input,const std::string& name_input,int& verbosity_level_input)
        :parent(parent_input),depth(depth_input),timer_id(timer_id_input),time(0),name(name_input),verbosity_level(verbosity_level_input)
    {
        end_on_separate_line=false;log_file_end_on_separate_line=false;
        timer_start_time=Timer::Singleton()->Peek_Time(timer_id);
    }

    virtual ~Log_Entry() {}

    void Start(const Log_Class& instance)
    {
        if(depth<=verbosity_level)
        {
            if(start_on_separate_line) putchar('\n');
            start_on_separate_line=needs_indent=true;
            printf("%*s%-*s",2*depth,"",50-2*depth,name.c_str());fflush(stdout);
        }

        if(instance.log_file)
        {
            if(log_file_start_on_separate_line) putc('\n',instance.log_file);
            log_file_start_on_separate_line=log_file_needs_indent=true;
            if(instance.xml) Start_XML(instance);
            else fprintf(instance.log_file,"%*s%-*s",2*depth,"",50-2*depth,name.c_str());
            fflush(instance.log_file);
        }

        timer_start_time=Timer::Singleton()->Peek_Time(timer_id);
    }

    virtual void Start_XML(const Log_Class& instance)
    {fprintf(instance.log_file,"%*s<scope name=\"%s\">",2*depth,"",name.c_str());}

    void Stop(Log_Class& instance)
    {
        double time_since_start=(Timer::Singleton()->Peek_Time(timer_id)-timer_start_time)/1000;

        if(depth<=verbosity_level)
        {
            if(end_on_separate_line){
                if(start_on_separate_line) putchar('\n');
                printf("%*sEND %-*s",2*depth,"",50-2*depth-4,name.c_str());}
            end_on_separate_line=false;start_on_separate_line=needs_indent=true;
            printf("%8.4f s",time_since_start);fflush(stdout);
        }

        if(instance.log_file)
        {
            if(instance.xml){
                if(log_file_end_on_separate_line){
                    if(log_file_start_on_separate_line) putc('\n',instance.log_file);
                    fprintf(instance.log_file,"%*s",2*depth,"");}
                log_file_end_on_separate_line=false;log_file_start_on_separate_line=log_file_needs_indent=true;
                fprintf(instance.log_file,"<time value=\"%f\"/></scope>",time_since_start);fflush(instance.log_file);}
            else{
                if(log_file_end_on_separate_line){
                    if(log_file_start_on_separate_line) putc('\n',instance.log_file);
                    fprintf(instance.log_file,"%*sEND %-*s",2*depth,"",50-2*depth-4,name.c_str());}
                log_file_end_on_separate_line=false;log_file_start_on_separate_line=log_file_needs_indent=true;
                fprintf(instance.log_file,"%8.4f s",time_since_start);fflush(instance.log_file);}
        }

        time+=time_since_start;
    }

    virtual Log_Entry* Get_Stop_Time(Log_Class& instance)
    {Stop(instance);return parent;}

    virtual Log_Entry* Get_New_Scope(Log_Class& instance,const std::string& new_scope_identifier,const std::string& new_name)
    {Stop(instance);return parent->Get_New_Scope(instance,new_scope_identifier,new_name);}

    virtual Log_Entry* Get_New_Item(Log_Class& instance,const std::string& new_name)
    {Stop(instance);return parent->Get_New_Item(instance,new_name);}

    virtual Log_Entry* Get_Pop_Scope(Log_Class& instance)
    {Stop(instance);return parent->Get_Pop_Scope(instance);}

    virtual void Dump_Log(FILE* output)
    {fprintf(output,"%*s%-*s%8.4f s\n",2*depth,"",50-2*depth,name.c_str(),time);fflush(output);}

    virtual void Dump_Names(FILE* output)
    {fprintf(output,"%*s%-*s",2*depth,"",50-2*depth,name.c_str());fflush(output);}
//######################################################################
};
}
}
#endif
