//!#####################################################################
//! \file Example.cpp
//!#####################################################################
#include <nova/Tools/Utilities/Example.h>
#include <nova/Tools/Utilities/File_Utilities.h>
using namespace Nova;
//######################################################################
// Constructor
//######################################################################
template<class T,int d> Example<T,d>::
Example(std::ostream& output_i)
    :initial_time(0),frame_rate(24),first_frame(0),last_frame(120),restart_frame(0),write_substeps_level(-1),test_number(1),frame_title(""),
    output_directory("output"),write_time(true),restart(false),write_output_files(true),write_frame_title(true),parse_args(nullptr),output(&output_i)
{}
//######################################################################
// Destructor
//######################################################################
template<class T,int d> Example<T,d>::
~Example()
{
    if(parse_args!=nullptr) delete parse_args;
}
//######################################################################
// Clamp_Time_Step_With_Target_Time
//######################################################################
template<class T,int d> void Example<T,d>::
Clamp_Time_Step_With_Target_Time(const T time,const T target_time,T& dt,bool& done)
{
    if(time+dt>=target_time){dt=target_time-time;done=true;}
    else if(time+2*dt>=target_time) dt=std::min(dt,(T).51*(target_time-time));
}
//######################################################################
// Write_Frame_Title
//######################################################################
template<class T,int d> void Example<T,d>::
Write_Frame_Title(const int frame) const
{
    if(write_frame_title) File_Utilities::Write_To_Text_File(output_directory+"/"+std::to_string(frame)+"/frame_title",frame_title);
}
//######################################################################
// Write_Substep
//######################################################################
template<class T,int d> void Example<T,d>::
Write_Substep(const std::string& title,const int substep,const int level)
{
    if(level<=write_substeps_level)
    {
        frame_title=title;
        *output<<"Writing substep ["<<frame_title<<"]: output_number="<<output_number+1<<", frame="<<current_frame<<", substep="<<substep<<std::endl;
        Write_Output_Files(++output_number);frame_title="";
    }
}
//######################################################################
// Log_Parameters
//######################################################################
template<class T,int d> void Example<T,d>::
Log_Parameters() const
{
    *output<<"initial_time="<<initial_time<<std::endl;
    *output<<"frame_rate="<<frame_rate<<std::endl;
    *output<<"first_frame="<<first_frame<<std::endl;
    *output<<"last_frame="<<last_frame<<std::endl;
    *output<<"restart_frame="<<restart_frame<<std::endl;
    *output<<"write_substeps_level="<<write_substeps_level<<std::endl;
    *output<<"frame_title="<<frame_title<<std::endl;
    *output<<"output_directory="<<output_directory<<std::endl;
    *output<<"write_time="<<write_time<<std::endl;
    *output<<"restart="<<restart<<std::endl;
    *output<<"write_output_files="<<write_output_files<<std::endl;
    *output<<"write_frame_title="<<write_frame_title<<std::endl;
}
//######################################################################
// Register_Options
//######################################################################
template<class T,int d> void Example<T,d>::
Register_Options()
{
    if(!parse_args) return;
    parse_args->Add_String_Argument("-o","","output directory");
    parse_args->Add_Integer_Argument("-restart",0,"frame","restart frame");
    parse_args->Add_Integer_Argument("-test_number",1,"test","test number");
    parse_args->Add_Integer_Argument("-substeps",-1,"level","substep output level");
    parse_args->Add_Integer_Argument("-first_frame",0,"frame","first frame");
    parse_args->Add_Integer_Argument("-last_frame",0,"frame","last frame");
    parse_args->Add_Double_Argument("-framerate",24,"frame rate");
}
//######################################################################
// Parse_Options
//######################################################################
template<class T,int d> void Example<T,d>::
Parse_Options()
{
    if(!parse_args) return;
    if(parse_args->Is_Value_Set("-test_number")) test_number=parse_args->Get_Integer_Value("-test_number");
    if(parse_args->Is_Value_Set("-o")) output_directory=parse_args->Get_String_Value("-o");
    if(parse_args->Is_Value_Set("-restart")){restart=true;restart_frame=parse_args->Get_Integer_Value("-restart");}
    if(parse_args->Is_Value_Set("-substeps")) Set_Write_Substeps_Level(parse_args->Get_Integer_Value("-substeps"));
    if(parse_args->Is_Value_Set("-first_frame")) first_frame=parse_args->Get_Integer_Value("-first_frame");
    if(parse_args->Is_Value_Set("-framerate")) frame_rate=(T)parse_args->Get_Double_Value("-framerate");
    if(parse_args->Is_Value_Set("-last_frame")) last_frame=parse_args->Get_Integer_Value("-last_frame");
}
//######################################################################
// Parse
//######################################################################
template<class T,int d> void Example<T,d>::
Parse(int argc,char* argv[])
{
    parse_args=new Parse_Args;
    Register_Options();

    parse_args->Parse(argc,argv);
    std::string print_args=parse_args->Print_Arguments(argc,argv);
    *output<<print_args<<std::endl;

    Parse_Options();
}
//######################################################################
template class Nova::Example<float,1>;
template class Nova::Example<float,2>;
template class Nova::Example<float,3>;
#ifdef COMPILE_WITH_DOUBLE_SUPPORT
template class Nova::Example<double,1>;
template class Nova::Example<double,2>;
template class Nova::Example<double,3>;
#endif
