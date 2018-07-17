//!#####################################################################
//! \file Log.cpp
//!#####################################################################
#include <nova/Tools/Arrays/Array.h>
#include <nova/Tools/Log/Debug_Utilities.h>
#include <nova/Tools/Log/Log_Scope.h>
#include <nova/Tools/Utilities/File_Utilities.h>
#include <iostream>
using namespace Nova;
Nova::Log_Null::log_null_class cout;
Nova::Log_Null::log_null_class cerr;
using namespace Log_Real;
namespace{
Log_Class* private_instance=nullptr;
std::streambuf* cout_buffer=nullptr;
std::streambuf* cerr_buffer=nullptr;
bool cout_initialized=false;
bool cerr_initialized=false;

template<class T>
struct Initialization_Helper
{
    T* object;
    bool& initialized;

    Initialization_Helper(T* object_input,bool& initialized_input)
        :object(object_input),initialized(initialized_input)
    {initialized=true;}

    ~Initialization_Helper()
    {initialized=false;delete object;}
};

void Dump_Log_Helper(Log_Class& instance,const bool suppress_cout);
}
//######################################################################
// Class Log_Cout_Buffer
//######################################################################
class Log_Cout_Buffer: public std::stringbuf
{
    int sync()
    {
        if(!Instance()->suppress_cout && Instance()->current_entry->depth<Instance()->verbosity_level){
            if(Log_Entry::start_on_separate_line) putchar('\n');
            std::string buffer=str();
            for(size_t start=0;start<buffer.length();){
                size_t end=buffer.find('\n',start);
                if(Log_Entry::needs_indent){printf("%*s",2*Instance()->current_entry->depth+2,"");Log_Entry::needs_indent=false;}
                fputs(buffer.substr(start,end-start).c_str(),stdout);
                if(end!=std::string::npos){putchar('\n');Log_Entry::needs_indent=true;start=end+1;}
                else break;}
            Log_Entry::start_on_separate_line=false;Instance()->current_entry->end_on_separate_line=true;
            fflush(stdout);}

        if(Instance()->log_file){
            if(Log_Entry::log_file_start_on_separate_line) putc('\n',Instance()->log_file);
            std::string buffer=str();
            for(size_t start=0;start<buffer.length();){
                size_t end=buffer.find('\n',start);
                if(Log_Entry::log_file_needs_indent){
                    fprintf(Instance()->log_file,"%*s",2*Instance()->current_entry->depth+2,"");Log_Entry::log_file_needs_indent=false;
                    if(Instance()->xml) fputs("<print>",Instance()->log_file);}
                fputs(buffer.substr(start,end-start).c_str(),Instance()->log_file);
                if(end!=std::string::npos){
                    if(Instance()->xml) fputs("</print>",Instance()->log_file);
                    putc('\n',Instance()->log_file);Log_Entry::log_file_needs_indent=true;start=end+1;}
                else break;}
            Log_Entry::log_file_start_on_separate_line=false;Instance()->current_entry->log_file_end_on_separate_line=true;
            fflush(Instance()->log_file);}
        str("");return std::stringbuf::sync();
    }
};
//######################################################################
// Class Log_Cerr_Buffer
//######################################################################
class Log_Cerr_Buffer: public std::stringbuf
{
    int sync()
    {
        if(!Instance()->suppress_cerr){
            if(Log_Entry::start_on_separate_line) putchar('\n');Log_Entry::start_on_separate_line=false;
            fputs(str().c_str(),stderr);}
        if(Instance()->log_file){
            if(Log_Entry::log_file_start_on_separate_line) putc('\n',Instance()->log_file);Log_Entry::log_file_start_on_separate_line=false;
            std::string buffer=str();
            for(size_t start=0;start<buffer.length();){
                size_t end=buffer.find('\n',start);
                if(Instance()->xml) fputs("<error><!--",Instance()->log_file);
                fputs(buffer.substr(start,end-start).c_str(),Instance()->log_file);
                if(Instance()->xml) fputs("--></error>",Instance()->log_file);
                putc('\n',Instance()->log_file);
                if(end!=std::string::npos) start=end+1;
                else break;}}
        str("");return std::stringbuf::sync();
    }
};
//######################################################################
// Constructor
//######################################################################
Log_Class::Log_Class(const bool suppress_cout_input,const bool suppress_cerr_input,const bool suppress_timing_input,const int verbosity_level_input,const bool cache_initial_output)
    :timer_singleton(Timer::Singleton()),suppress_cout(suppress_cout_input),suppress_cerr(suppress_cerr_input),suppress_timing(suppress_timing_input),log_file(0),verbosity_level(verbosity_level_input),xml(true)
{
    if(private_instance!=nullptr) delete private_instance;private_instance=this;
    
    if(cache_initial_output){
        log_file=File_Utilities::Temporary_File();
        if(log_file==nullptr) FATAL_ERROR("Could not create temporary log file!");
        log_file_temporary=true;}

    timer_id=timer_singleton->Register_Timer();
    if(cout_buffer) delete cout_buffer;cout_buffer=new Log_Cout_Buffer;cout.rdbuf(cout_buffer);
    if(cerr_buffer) delete cerr_buffer;cerr_buffer=new Log_Cerr_Buffer;cerr.rdbuf(cerr_buffer);
    root=new Log_Scope(0,0,timer_id,"SIMULATION","Simulation",verbosity_level);
    current_entry=root;root->Start(*this);
}
//######################################################################
// Destructor
//######################################################################
Log_Class::~Log_Class()
{
    while(current_entry!=nullptr) current_entry=current_entry->Get_Pop_Scope(*this);
    Dump_Log_Helper(*this,suppress_cout);
    if(log_file) fclose(log_file);
    if(cerr_initialized) cout.rdbuf(std::cout.rdbuf());
    if(cerr_initialized) cerr.rdbuf(std::cerr.rdbuf());
    timer_singleton->Release_Timer(timer_id);
    delete cout_buffer;cout_buffer=nullptr;
    delete cerr_buffer;cerr_buffer=nullptr;
    delete root;root=nullptr;
}
//######################################################################
// cout_Helper
//######################################################################
std::ostream& Nova::Log::cout_Helper()
{
    static Initialization_Helper<std::ostream> helper(new std::ostream(std::cout.rdbuf()),cout_initialized);
    return *helper.object;
}
//######################################################################
// cerr_Helper
//######################################################################
std::ostream& Nova::Log::cerr_Helper()
{
    static Initialization_Helper<std::ostream> helper(new std::ostream(std::cerr.rdbuf()),cerr_initialized);
    return *helper.object;
}
//######################################################################
// Instance
//######################################################################
Log_Class* Nova::Log::Instance()
{
    if(private_instance==nullptr) Initialize_Logging();
    return private_instance;
}
//######################################################################
// Initialize_Logging
//######################################################################
void Nova::Log::Initialize_Logging(const bool suppress_cout_input,const bool suppress_timing_input,const int verbosity_level_input,const bool cache_initial_output)
{
    new Log_Class(suppress_cout_input,false,suppress_timing_input,verbosity_level_input-1,cache_initial_output);
}
//######################################################################
// Copy_Log_To_File
//######################################################################
void Log_Class::
Copy_Log_To_File(const std::string& filename,const bool append)
{
    FILE* temporary_file=0;
    if(log_file && log_file_temporary){
        temporary_file=log_file;log_file=0;}
    if(log_file){
        if(Log_Entry::log_file_start_on_separate_line) putc('\n',log_file);
        Instance()->root->Dump_Log(log_file);
        fclose(log_file);log_file=0;}
    if(!filename.empty()){
        if(append){
            log_file=fopen(filename.c_str(),"a");
            if(log_file==nullptr) FATAL_ERROR("Cannot open log file "+filename+" for append!");
            putc('\n',log_file);}
        else{
            log_file=fopen(filename.c_str(),"w");
            if(log_file==nullptr) FATAL_ERROR("Cannot open log file "+filename+" for writing!");}
        if(!temporary_file){
            Instance()->root->Dump_Names(log_file);
            Log_Entry::log_file_start_on_separate_line=Log_Entry::log_file_needs_indent=Instance()->current_entry->log_file_end_on_separate_line=true;}
        else{
            fflush(temporary_file);fseek(temporary_file,0,SEEK_SET);
            Array<char> buffer(4096);
            for(;;){
                int n=(int)fread(buffer.Get_Array_Pointer(),sizeof(char),buffer.size(),temporary_file);
                fwrite(buffer.Get_Array_Pointer(),sizeof(char),n,log_file);
                if(n<buffer.size()) break;}
            fflush(log_file);}}
    if(temporary_file) fclose(temporary_file);
    log_file_temporary=false;
}
//######################################################################
// Finish_Logging
//######################################################################
void Nova::Log::Finish_Logging()
{
    if(private_instance!=nullptr){delete private_instance;private_instance=nullptr;}
    Timer::Destroy_Singleton();
}
//######################################################################
// Time_Helper
//######################################################################
void Log_Class::
Time_Helper(const std::string& label)
{
    Log_Class* instance=Log::Instance();
    instance->current_entry=instance->current_entry->Get_New_Item(*instance,label);
    instance->current_entry->Start(*instance);
}
//######################################################################
// Stop_Time
//######################################################################
void Nova::Log::Stop_Time()
{
    Log_Class* instance=Log::Instance();
    if(instance->suppress_timing) return;
    instance->current_entry=instance->current_entry->Get_Stop_Time(*instance);
}
//######################################################################
// Stat_Helper
//######################################################################
void Nova::Log::Stat_Helper(const std::string& label,const std::stringstream& s)
{
    Log_Class* instance=Log::Instance();
    if(instance->suppress_timing) return;
    if(instance->current_entry->depth<instance->verbosity_level){
        if(Log_Entry::start_on_separate_line) putchar('\n');
        if(Log_Entry::needs_indent) printf("%*s",2*instance->current_entry->depth+2,"");
        printf("%s = %s\n",label.c_str(),s.str().c_str());
        Log_Entry::start_on_separate_line=false;Log_Entry::needs_indent=instance->current_entry->end_on_separate_line=true;}
    if(instance->log_file){
        if(Log_Entry::log_file_start_on_separate_line) putc('\n',instance->log_file);
        if(Log_Entry::log_file_needs_indent) fprintf(instance->log_file,"%*s",2*instance->current_entry->depth+2,"");
        if(instance->xml) fprintf(instance->log_file,"<stat name=\"%s\" value=\"%s\"/>\n",label.c_str(),s.str().c_str());
        else fprintf(instance->log_file,"%s = %s\n",label.c_str(),s.str().c_str());
        Log_Entry::log_file_start_on_separate_line=false;Log_Entry::log_file_needs_indent=instance->current_entry->log_file_end_on_separate_line=true;}
}
//######################################################################
// Push_Scope
//######################################################################
void Log_Class::
Push_Scope(const std::string& scope_identifier,const std::string& scope_name)
{
    Log_Class* instance=Log::Instance();
    if(instance->suppress_timing) return;
    instance->current_entry=instance->current_entry->Get_New_Scope(*instance,scope_identifier,scope_name);
    instance->current_entry->Start(*instance);
}
//######################################################################
// Pop_Scope
//######################################################################
void Log_Class::
Pop_Scope()
{
    Log_Class* instance=Log::Instance();
    if(instance->suppress_timing) return;
    instance->current_entry=instance->current_entry->Get_Pop_Scope(*instance);
    if(instance->current_entry==nullptr){Log::cerr<<"Could not pop scope. Defaulting to root"<<std::endl;instance->current_entry=instance->root;}
}
//######################################################################
// Reset
//######################################################################
void Nova::Log::Reset()
{
    Log_Class* instance=Log::Instance();
    if(instance->suppress_timing) return;
    delete instance->root;
    instance->root=new Log_Scope(0,0,instance->timer_id,"SIMULATION","Simulation",instance->verbosity_level);instance->current_entry=instance->root;
}
//#####################################################################
// Dump_Log
//#####################################################################
namespace{
void
Dump_Log_Helper(Log_Class& instance,const bool suppress_cout)
{
    if(instance.suppress_timing) return;
    if(Log_Entry::start_on_separate_line){putchar('\n');Log_Entry::start_on_separate_line=false;}
    // TODO: Figure out why below does not accumulate scope names.
#if 0
    if(!suppress_cout) instance.root->Dump_Log(stdout);
    if(instance.log_file){
        if(Log_Entry::log_file_start_on_separate_line){putc('\n',instance.log_file);Log_Entry::log_file_start_on_separate_line=false;}
        if(!instance.xml) instance.root->Dump_Log(instance.log_file);}
#endif
}
}
void Dump_Log()
{
    Log_Class& instance=*Log::Instance();
    Dump_Log_Helper(instance,instance.suppress_cout);
}
//#####################################################################
// Destructor
//#####################################################################
Scope::
~Scope()
{if(active) Log_Class::Pop_Scope();}
