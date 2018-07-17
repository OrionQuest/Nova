//!#####################################################################
//! \file Parse_Args.cpp
//!#####################################################################
#include <nova/Tools/Parsing/Parse_Args.h>
#include <nova/Tools/Utilities/File_Utilities.h>
#include <cassert>
#include <cstring>
using namespace Nova;
//######################################################################
// Constructor
//######################################################################
Parse_Args::
Parse_Args()
    :num_expected_extra_args(0),use_help_option(true),extra_usage_callback(0)
{}
//######################################################################
// Add_Option_Argument
//######################################################################
void Parse_Args::
Add_Option_Argument(const std::string& arg_str,const std::string& desc)
{
    arg_data_list.push_back(Arg_Data(arg_str,desc));
}
//######################################################################
// Add_Integer_Argument
//######################################################################
void Parse_Args::
Add_Integer_Argument(const std::string& arg_str,int default_value,const std::string& val_name,const std::string& desc)
{
    arg_data_list.push_back(Arg_Data(arg_str,val_name,desc,default_value));
}
//######################################################################
// Add_Double_Argument
//######################################################################
void Parse_Args::
Add_Double_Argument(const std::string& arg_str,double default_value,const std::string& val_name,const std::string& desc)
{
    arg_data_list.push_back(Arg_Data(arg_str,val_name,desc,default_value));
}
//######################################################################
// Add_Vector_2D_Argument
//######################################################################
void Parse_Args::
Add_Vector_2D_Argument(const std::string& arg_str,const Vector<double,2> &default_value,const std::string& val_name,const std::string& desc)
{
    arg_data_list.push_back(Arg_Data(arg_str,val_name,desc,default_value));
}
//######################################################################
// Add_Vector_3D_Argument
//######################################################################
void Parse_Args::
Add_Vector_3D_Argument(const std::string& arg_str,const Vector<double,3> &default_value,const std::string& val_name,const std::string& desc)
{
    arg_data_list.push_back(Arg_Data(arg_str,val_name,desc,default_value));
}
//######################################################################
// Add_String_Argument
//######################################################################
void Parse_Args::
Add_String_Argument(const std::string& arg_str,const std::string& default_value,const std::string& val_name,const std::string& desc)
{
    arg_data_list.push_back(Arg_Data(arg_str,val_name,desc,default_value));
}
//######################################################################
// Set_Extra_Arguments
// num=-1 for arbitrary extra arguments
//######################################################################
void Parse_Args::
Set_Extra_Arguments(int num,const std::string& synopsis,const std::string& desc)
{
    num_expected_extra_args=(num!=-1)?num_expected_extra_args+1:num;
    if(synopsis.length()) extra_args_synopsis_list.push_back(synopsis);
    if(desc.length()) extra_args_desc_list.push_back(desc);
}
//######################################################################
// Parse
//######################################################################
void Parse_Args::
Parse(int argc,char* argv[])
{
    program_name=argv[0];
    int current_arg=1;
    extra_arg_list.clear();
    while(current_arg<argc)
    {
        if(use_help_option && !strcmp(argv[current_arg],"--help")) Print_Usage(true);                       // print help
        int match=Find_Match(argv[current_arg]);
        if(match<0)
        {
            if(argv[current_arg][0]=='-') Print_Usage(true);
            else extra_arg_list.push_back(argv[current_arg++]);
        }
        else if(!arg_data_list[match].Parse_Value(argc,argv,current_arg))
            Print_Usage(true);
        else arg_data_list[match].value_set=true;
    }

    if(num_expected_extra_args!=-1 && extra_arg_list.size()!=num_expected_extra_args) Print_Usage(true);     // didn't get the expected number of extra args
    for(int i=current_arg;i<argc;i++) extra_arg_list.push_back(argv[i]);
}
//######################################################################
// Get_Option_Value
//######################################################################
bool Parse_Args::
Get_Option_Value(const std::string& arg_str) const
{
    return arg_data_list[Find_Match(arg_str,Arg_Data::OPTION)].boolean_value;
}
//######################################################################
// Get_Integer_Value
//######################################################################
int Parse_Args::
Get_Integer_Value(const std::string& arg_str) const
{
    return arg_data_list[Find_Match(arg_str,Arg_Data::INTEGER)].integer_value;
}
//######################################################################
// Get_Double_Value
//######################################################################
double Parse_Args::
Get_Double_Value(const std::string& arg_str) const
{
    return arg_data_list[Find_Match(arg_str,Arg_Data::DOUBLE)].double_value;
}
//######################################################################
// Get_Vector_2D_Value
//######################################################################
Vector<double,2> Parse_Args::
Get_Vector_2D_Value(const std::string& arg_str) const
{
    return arg_data_list[Find_Match(arg_str,Arg_Data::VECTOR2)].vector_2d_value;
}
//######################################################################
// Get_Vector_3D_Value
//######################################################################
Vector<double,3> Parse_Args::
Get_Vector_3D_Value(const std::string& arg_str) const
{
    return arg_data_list[Find_Match(arg_str,Arg_Data::VECTOR3)].vector_3d_value;
}
//######################################################################
// Get_String_Value
//######################################################################
const std::string& Parse_Args::
Get_String_Value(const std::string& arg_str) const
{
    return arg_data_list[Find_Match(arg_str,Arg_Data::STRING)].string_value;
}
//######################################################################
// Is_Value_Set
//######################################################################
bool Parse_Args::
Is_Value_Set(const std::string& arg_str) const
{
    int match=Find_Match(arg_str);
    if(match<0)
    {
        Log::cerr<<"Argument "<<arg_str<<" undeclared"<<std::endl;
        exit(-1);
    }
    return arg_data_list[match].value_set;
}
//######################################################################
// Override_String_Value
//######################################################################
void Parse_Args::
Override_String_Value(const std::string& arg_str,const std::string& value)
{
    arg_data_list[Find_Match(arg_str,Arg_Data::STRING)].string_value=value;
}
//######################################################################
// Extra_Arg
//######################################################################
const std::string& Parse_Args::
Extra_Arg(int i) const
{
    assert(0<=i && i<extra_arg_list.size());
    return extra_arg_list[i];
}
//######################################################################
// Find_And_Remove
//######################################################################
bool Parse_Args::
Find_And_Remove(const char *str,int& argc,char** argv)
{
    int i;
    for(i=0;i<argc;i++) if(!strcmp(str,argv[i])) break;
    if(i<argc)
    {
        for(;i<argc-1;i++) argv[i]=argv[i+1];
        argc--;
        argv[argc]=0;
        return true;
    }
    return false;
}
//######################################################################
// Find_And_Remove_Integer
//######################################################################
int Parse_Args::
Find_And_Remove_Integer(const char *str,int& argc,char** argv)
{
    int value,i;
    for(i=0;i<argc;i++) if(!strcmp(str,argv[i])) break;
    if(i+1<argc)
    {
        value=atoi(argv[i+1]);
        for(;i<argc-2;i++) argv[i]=argv[i+2];
        argc--;
        argv[argc]=0;
        argc--;
        argv[argc]=0;
        return value;
    }
    return 0;
}
//######################################################################
// Find_And_Remove_Double
//######################################################################
double Parse_Args::
Find_And_Remove_Double(const char *str,int& argc,char** argv)
{
    double value;
    int i;
    for(i=0;i<argc;i++) if(!strcmp(str,argv[i])) break;
    if(i+1<argc)
    {
        value=atof(argv[i+1]);
        for(;i<argc-2;i++) argv[i]=argv[i+2];
        argc--;
        argv[argc]=0;
        argc--;
        argv[argc]=0;
        return value;
    }
    return 0.;
}
//######################################################################
// Print_Usage
//######################################################################
void Parse_Args::
Print_Usage(bool do_exit) const
{
    int i;
    Log::cerr<<"Usage: "<<program_name<<" ";
    for(i=0;i<arg_data_list.size();i++)
    {
        arg_data_list[i].Print_Synopsis();
        Log::cerr<<" ";
    }
    for(i=0;i<extra_args_synopsis_list.size();i++) Log::cerr<<extra_args_synopsis_list[i]<<" ";
    Log::cerr<<std::endl;

    int width=0,len;
    for(i=0;i<arg_data_list.size();i++)
    {
        len=(int)arg_data_list[i].str.length();
        if(len>width) width=len;
    }
    for(i=0;i<extra_args_desc_list.size();i++)
    {
        len=(int)extra_args_synopsis_list[i].length();
        if(len>width) width=len;
    }
    for(i=0;i<arg_data_list.size();i++)
    {
        arg_data_list[i].Print_Description(width+2);
        Log::cerr<<std::endl;
    }
    for(i=0;i<extra_args_desc_list.size();i++)
    {
        Log::cerr<<extra_args_synopsis_list[i];
        for(unsigned j=1;j<=width+2-extra_args_synopsis_list[i].length();j++) Log::cerr<<" ";
        Log::cerr<<extra_args_desc_list[i]<<std::endl;
    }
    if(extra_usage_callback) extra_usage_callback();
    if(do_exit) exit(-1);
}
//######################################################################
// Find_Match
//######################################################################
int Parse_Args::
Find_Match(const std::string& str) const
{
    for(int i=0;i<arg_data_list.size();i++) if(arg_data_list[i].str==str) return i;
    return -1;
}
//######################################################################
// Find_Match
//######################################################################
int Parse_Args::
Find_Match(const std::string& str,const Arg_Data::Type& type) const
{
    int match=Find_Match(str);
    if(match<0)
    {
        Log::cout<<"Argument "<<str<<" undeclared"<<std::endl;
        exit(-1);
    }

    if(arg_data_list[match].type!=type)
    {
        Log::cout<<"Type mismatch in Find_Match("<<str<<")"<<std::endl;
        exit(-1);
    }
    return match;
}
//######################################################################
// Print_Arguments
//######################################################################
std::string Parse_Args::
Print_Arguments(int argc,char* argv[])
{
    std::string s="command = ";
    for(int i=0;i<argc;i++)
    {
        s+=argv[i];
        s+=' ';
    }
    s+="\nworking directory = "+File_Utilities::Get_Working_Directory()+"\n";
    return s;
}
//######################################################################
