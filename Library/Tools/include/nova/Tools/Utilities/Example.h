//!#####################################################################
//! \file Example.h
//!#####################################################################
// Class Example
//######################################################################
#ifndef __Example__
#define __Example__

#include <nova/Tools/Parsing/Parse_Args.h>

namespace Nova{
template<class T,int d>
class Example
{
  public:
    T initial_time,frame_rate;
    int first_frame,last_frame,restart_frame,write_substeps_level,test_number;
    int current_frame,output_number;
    std::string frame_title,output_directory;
    bool write_time,restart,write_output_files,write_frame_title;
    Parse_Args *parse_args;
    std::ostream *output;

    Example(std::ostream& output_i=Log::cout);
    virtual ~Example();

    T Time_At_Frame(const int frame) const
    {return initial_time+(frame-first_frame)/frame_rate;}

    void Set_Write_Substeps_Level(const int level)
    {write_substeps_level=level;}

//######################################################################
    virtual void Limit_Dt(T& dt,const T time)=0;
    virtual void Write_Output_Files(const int frame) const=0;
//######################################################################
    virtual void Log_Parameters() const;
    virtual void Register_Options();            // call parent first
    virtual void Parse_Options();               // call parent first
//######################################################################
    static void Clamp_Time_Step_With_Target_Time(const T time,const T target_time,T& dt,bool& done);
    void Write_Substep(const std::string& title,const int substep,const int level=0);
    void Write_Frame_Title(const int frame) const;
    void Parse(int argc,char* argv[]);
//######################################################################
};
}
#endif
