//!#####################################################################
//! \file Driver.cpp
//!#####################################################################
#include <nova/Tools/Utilities/Driver.h>
#include <nova/Tools/Utilities/File_Utilities.h>
using namespace Nova;
//######################################################################
// Compute_Dt
//######################################################################
template<class T,int d> T Driver<T,d>::
Compute_Dt(const T time,const T target_time)
{
    T dt=target_time-time;
    example.Limit_Dt(dt,time);
    return dt;
}
//######################################################################
// Write_First_Frame
//######################################################################
template<class T,int d> void Driver<T,d>::
Write_First_Frame(const int frame) const
{
    if(frame==example.first_frame)
    {
        File_Utilities::Create_Directory(example.output_directory);
        File_Utilities::Create_Directory(example.output_directory+"/common");
        File_Utilities::Write_To_Text_File(example.output_directory+"/common/first_frame",std::to_string(frame));
    }
}
//######################################################################
// Write_Last_Frame
//######################################################################
template<class T,int d> void Driver<T,d>::
Write_Last_Frame(const int frame) const
{
    File_Utilities::Write_To_Text_File(example.output_directory+"/common/last_frame",std::to_string(frame));
    File_Utilities::Write_To_Text_File(example.output_directory+"/info.nova-animation",std::to_string(frame));
}
//######################################################################
// Write_Time
//######################################################################
template<class T,int d> void Driver<T,d>::
Write_Time(const int frame) const
{
    if(example.write_time) File_Utilities::Write_To_File<T>(example.output_directory+"/"+std::to_string(frame)+"/time",time);
}
//######################################################################
// Initialize
//######################################################################
template<class T,int d> void Driver<T,d>::
Initialize()
{
    if(example.restart){current_frame=example.restart_frame;Read_Time(current_frame);}
    else current_frame=example.first_frame;
    output_number=current_frame;
    time=example.Time_At_Frame(current_frame);

    example.output_number=output_number;
    example.current_frame=current_frame;
}
//######################################################################
// Execute_Main_Program
//######################################################################
template<class T,int d> void Driver<T,d>::
Execute_Main_Program()
{
    Initialize();
    Simulate_To_Frame(example.last_frame);
}
//######################################################################
// Read_Time
//######################################################################
template<class T,int d> void Driver<T,d>::
Read_Time(const int frame)
{
    time=example.Time_At_Frame(frame);
    std::string filename=example.output_directory+"/"+std::to_string(frame)+"/time";
    if(File_Utilities::File_Exists(filename))
    {
        T corrected_time;
        File_Utilities::Read_From_File<T>(filename,corrected_time);
        if(abs(time-corrected_time)>(T)1e-4*abs(time))
        {   
            // only adjust time if significantly different from default in order to get deterministic restarts
            time=corrected_time;
            // adjust initial time so that Simulate_To_Frame() returns correct time (essential when writing substeps)
            example.initial_time=time-(frame-example.first_frame)/example.frame_rate;
        }
    }
}
//######################################################################
// Read_Last_Frame
//######################################################################
template<class T,int d> void Driver<T,d>::
Read_Last_Frame()
{
    std::string filename=example.output_directory+"/common/last_frame";
    if(File_Utilities::File_Exists(filename))
        File_Utilities::Read_From_Text_File(filename,example.restart_frame);
}
//######################################################################
// Write_Output_Files
//######################################################################
template<class T,int d> void Driver<T,d>::
Write_Output_Files(const int frame)
{
    Write_First_Frame(frame);
    example.Write_Output_Files(frame);
    Write_Time(frame);
    Write_Last_Frame(frame);
}
//######################################################################
template class Nova::Driver<float,1>;
template class Nova::Driver<float,2>;
template class Nova::Driver<float,3>;
#ifdef COMPILE_WITH_DOUBLE_SUPPORT
template class Nova::Driver<double,1>;
template class Nova::Driver<double,2>;
template class Nova::Driver<double,3>;
#endif
