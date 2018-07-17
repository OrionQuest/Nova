//!#####################################################################
//! \file Log_Scope.h
//!#####################################################################
// Class Log_Scope
//######################################################################
#ifndef __Log_Scope__
#define __Log_Scope__

#include <nova/Tools/Arrays/Array.h>
#include <nova/Tools/Log/Log_Entry.h>
#include <map>

namespace Nova{
namespace Log_Real{
class Log_Scope: public Log_Entry
{
  public:
    std::map<std::string,int> entries;
    Array<Log_Entry*> children;
    std::string scope_identifier;

    Log_Scope(Log_Entry* parent_input,int depth_input,int timer_id_input,const std::string& scope_identifier_input,const std::string& name_input,int& verbosity_level_input)
        :Log_Entry(parent_input,depth_input,timer_id_input,name_input,verbosity_level_input),scope_identifier(scope_identifier_input)
    {}

    virtual ~Log_Scope()
    {
        for(size_t i=0;i<children.size();++i) if(children(i)!=nullptr) delete children(i);
    }

    virtual Log_Entry* Get_Stop_Time(Log_Class& instance)
    {return this;}

    Log_Entry* Get_New_Scope(Log_Class& instance,const std::string& new_scope_identifier,const std::string& new_name)
    {
        int entry;end_on_separate_line=true;log_file_end_on_separate_line=true;
        if(entries.count(new_scope_identifier)!=0 && entries[new_scope_identifier]==entry){children(entry)->name=new_name;return children(entry);}
        Log_Entry* new_entry=new Log_Scope(this,depth+1,timer_id,new_scope_identifier,new_name,verbosity_level);
        children.Append(new_entry);
        entries[new_scope_identifier]=(int)children.size();
        return new_entry;
    }

    Log_Entry* Get_New_Item(Log_Class& instance,const std::string& new_name)
    {
        int entry;end_on_separate_line=true;log_file_end_on_separate_line=true;
        if(entries.count(new_name)!=0 && entries[new_name]==entry) return children(entry);
        Log_Entry* new_entry=new Log_Entry(this,depth+1,timer_id,new_name,verbosity_level);
        children.Append(new_entry);
        entries[new_name]=children.size();
        return new_entry;
    }

    Log_Entry* Get_Pop_Scope(Log_Class& instance)
    {Stop(instance);return parent;}

    void Start_XML(Log_Class& instance)
    {fprintf(instance.log_file,"%*s<scope id=\"%s\" name=\"%s\">",2*depth,"",scope_identifier.c_str(),name.c_str());}

    void Dump_Log(FILE* output)
    {
        fprintf(output,"%*s%-*s%8.4f\n",2*depth,"",50-2*depth,scope_identifier.c_str(),time);fflush(output);
        for(int i=0;i<children.size();++i) children(i)->Dump_Log(output);
    }

    void Dump_Names(FILE* output)
    {Log_Entry::Dump_Names(output);for(int i=0;i<children.size();++i)children(i)->Dump_Names(output);}
//######################################################################
};
}
}
#endif
