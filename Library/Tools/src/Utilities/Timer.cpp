//!#####################################################################
//! \file Timer.cpp
//!#####################################################################
#include <nova/Tools/Log/Log.h>
#include <nova/Tools/Utilities/Timer.h>
#include <cstdio>
#include <iostream>
#include <sys/resource.h>
#include <sys/time.h>
using namespace Nova;
//######################################################################
// Get_Current_Time
//######################################################################
double Nova::Get_Current_Time()
{
    timeval tv;
    gettimeofday(&tv,0);
    return tv.tv_sec+1e-6*tv.tv_usec;
}
//######################################################################
// Initialize_Timer
//######################################################################
double Nova::Initialize_Timer()
{
    return 1e3;     // to convert seconds to milliseconds
}

Timer* Timer::singleton_instance;

//######################################################################
// Constructor
//######################################################################
Timer::Timer()
{
    timers.resize(512);
    resolution=Initialize_Timer();
    timers(0).elapsed=timers(0).start=0;
    free_timers=new Identity_Array(timers.size());
    overhead=0;timers(0).accumulator=0;
    for(int i=0;i<100;++i){Start(0);Stop(0);}
    overhead=timers(0).accumulator*.01;
}
//######################################################################
// Register_Timer
//######################################################################
int Timer::
Register_Timer()
{
    if(free_timers->size()==0)
    {
        Log::cerr<<"No more timers available, timing information may be incorrect!"<<std::endl;
        return 0;
    }

    int id=free_timers->Pop();
    double time=Get_Current_Time();
    timers(id).start=time;
    timers(id).elapsed=time;
    timers(id).accumulator=0;
    return id;
}
//######################################################################
// Peek_And_Reset_Time
//######################################################################
double Timer::
Peek_And_Reset_Time(const int id)
{
    double time=Get_Current_Time();
    double time_elapsed=(time-timers(id).elapsed)*resolution;
    timers(id).elapsed=time;
    return time_elapsed;
}
//######################################################################
// Reset_Time
//######################################################################
void Timer::
Reset_Time(const int id)
{
    double time=Get_Current_Time();
    timers(id).elapsed=time;
}
//######################################################################
// Print_Stats
//######################################################################
void Timer::
Print_Stats(const int id,const char* str)
{
    Log::cout<<"Total time elapsed for "<<str<<" is "<<timers(id).accumulator*resolution*.001<<" s."<<std::endl;
}
//######################################################################
