//!#####################################################################
//! \file Parse_Args.h
//!#####################################################################
// Class Parse_Args
//######################################################################
#ifndef __Parse_Args__
#define __Parse_Args__

#include <nova/Tools/Parsing/Arg_Data.h>
#include <nova/Tools/Utilities/Non_Copyable.h>

namespace Nova{
class Parse_Args: public Non_Copyable
{
    std::vector<Arg_Data> arg_data_list;
    std::vector<std::string> extra_arg_list,extra_args_synopsis_list,extra_args_desc_list;
    int num_expected_extra_args;
    std::string program_name;
    bool use_help_option;
    void (*extra_usage_callback)();

  public:
    Parse_Args();
    ~Parse_Args() {}

    void Use_Help_Option(bool use_it)
    {use_help_option=use_it;}

    void Set_Extra_Usage_Callback(void (*extra_usage_callback_input)())
    {extra_usage_callback=extra_usage_callback_input;}

    size_t Num_Extra_Args() const
    {return extra_arg_list.size();}

    const std::string& Get_Program_Name() const
    {return program_name;}

//#####################################################################
    static bool Find_And_Remove(const char *str,int& argc,char** argv);
    static int Find_And_Remove_Integer(const char *str,int& argc,char** argv);
    static double Find_And_Remove_Double(const char *str,int& argc,char** argv);
    static std::string Print_Arguments(int argc,char* argv[]);
//#####################################################################
    void Add_Option_Argument(const std::string& arg_str,const std::string& desc="");
    void Add_Integer_Argument(const std::string& arg_str,int default_value,const std::string& val_name="",const std::string& desc="");
    void Add_Double_Argument(const std::string& arg_str,double default_value,const std::string& val_name="",const std::string& desc="");
    void Add_Vector_2D_Argument(const std::string& arg_str,const Vector<double,2>& default_value,const std::string& val_name="",const std::string& desc="");
    void Add_Vector_3D_Argument(const std::string& arg_str,const Vector<double,3>& default_value,const std::string& val_name="",const std::string& desc="");
    void Add_String_Argument(const std::string& arg_str,const std::string& default_value,const std::string& val_name="",const std::string& desc="");
    void Set_Extra_Arguments(int num,const std::string& synopsis="",const std::string& desc="");
    void Parse(int argc,char* argv[]);
    bool Get_Option_Value(const std::string& arg_str) const;
    int Get_Integer_Value(const std::string& arg_str) const;
    double Get_Double_Value(const std::string& arg_str) const;
    Vector<double,2> Get_Vector_2D_Value(const std::string& arg_str) const;
    Vector<double,3> Get_Vector_3D_Value(const std::string& arg_str) const;
    const std::string& Get_String_Value(const std::string& arg_str) const;
    bool Is_Value_Set(const std::string& arg_str) const;
    void Override_String_Value(const std::string& arg_str,const std::string& value);
    const std::string& Extra_Arg(int i) const;
    int Find_Match(const std::string& str) const;
    int Find_Match(const std::string& str,const Arg_Data::Type& type) const;
    void Print_Usage(bool do_exit=false) const;
//#####################################################################
};
}
#endif
