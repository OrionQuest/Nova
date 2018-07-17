//!#####################################################################
//! \file Timer.h
//!#####################################################################
// Class Timer
//######################################################################
#ifndef __Timer__
#define __Timer__

#include <nova/Tools/Arrays/Identity_Array.h>
#include <nova/Tools/Arrays/Array.h>
#include <nova/Tools/Utilities/Non_Copyable.h>

namespace Nova{

double Get_Current_Time();
double Initialize_Timer();

class Timer: public Non_Copyable
{
    struct Data
    {
        double start,elapsed,accumulator;
    };

    double resolution;
    Array<Data> timers;
    Identity_Array *free_timers;
    static Timer* singleton_instance;
    double overhead;

  public:
    Timer();

    ~Timer()
    {
        if(free_timers!=nullptr) delete free_timers;
    }

    static inline void Destroy_Singleton()
    {delete singleton_instance;singleton_instance=nullptr;}

    static inline Timer* Singleton()
    {
        if(singleton_instance==nullptr) singleton_instance=new Timer;
        return singleton_instance;
    }

    void Release_Timer(const int id)
    {free_timers->Append(id);}

    double Get_Time()
    {return Get_Current_Time()*resolution;}

    double Get_Total_Time_Since_Registration(const int id)
    {return (Get_Current_Time()-timers(id).start)*resolution;}

    double Peek_Time(const int id)
    {return (Get_Current_Time()-timers(id).elapsed)*resolution;}

    void Start(const int id)
    {timers(id).start=Get_Current_Time();}

    void Stop(const int id)
    {
        double time=Get_Current_Time();
        timers(id).accumulator+=time-timers(id).start-overhead;
    }

    double Get_Accumulated_Time(const int id)
    {return timers(id).accumulator;}

    void Reset_Accumulated_Time(const int id)
    {timers(id).accumulator=0;}

//######################################################################
    int Register_Timer();
    double Peek_And_Reset_Time(const int id);
    void Reset_Time(const int id);
    void Print_Stats(const int id,const char* str);
//######################################################################
};
}
#endif
