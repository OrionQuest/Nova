//!#####################################################################
//! \file Driver.h
//!#####################################################################
// Class Driver
//######################################################################
#ifndef __Driver__
#define __Driver__

#include <nova/Tools/Utilities/Example.h>
#include <nova/Tools/Utilities/File_Utilities.h>

namespace Nova{
template<class T,int d>
class Driver
{
  protected:
    T time;
  public:
    Example<T,d>& example;
    int current_frame,output_number;

    Driver(Example<T,d>& example_input)
        :example(example_input),output_number(example.first_frame)
    {}

    virtual ~Driver() {}

    T Time() const
    {return time;}

    void Set_Time(const T time_input) 
    {time=time_input;}

//######################################################################
    virtual void Initialize();
    virtual void Advance_To_Target_Time(const T target_time)=0;
    virtual void Simulate_To_Frame(const int frame)=0;
    virtual void Execute_Main_Program();
    virtual void Read_Time(const int frame);
    virtual void Read_Last_Frame();
//######################################################################
    T Compute_Dt(const T time,const T target_time);
    void Write_First_Frame(const int frame) const;
    void Write_Last_Frame(const int frame) const;
    void Write_Time(const int frame) const;
    void Write_Output_Files(const int frame);
//######################################################################
};
}
#endif
