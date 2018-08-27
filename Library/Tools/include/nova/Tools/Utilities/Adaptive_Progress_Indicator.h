//!#####################################################################
//! \file Adaptive_Progress_Indicator.h
//!#####################################################################
// Class Adaptive_Progress_Indicator
//##################################################################### 
#ifndef __Adaptive_Progress_Indicator__
#define __Adaptive_Progress_Indicator__

#include <iostream>

namespace Nova{
class Adaptive_Progress_Indicator
{
    const unsigned long total;
    const unsigned long increment;
    unsigned long units_done;
    unsigned long increments_done;
    unsigned long increments_left;

  public:
    Adaptive_Progress_Indicator(const unsigned long total_input,const unsigned long increment_input)
        :total(total_input),increment(increment_input)
    {
        units_done=0;
        increments_done=0;
        increments_left=(total+increment-1)/increment;
        printf("\n[  0.000%%]");fflush(stdout);
    }

    bool Progress(const unsigned long by=1)
    {
        unsigned long old_increments_done=increments_done,old_increments_left=increments_left;
        units_done+=by;increments_done=units_done/increment;increments_left=(total-units_done+increment-1)/increment;
        if(increments_done != old_increments_done || increments_left != old_increments_left){
            double percent_done=100.l*((double)units_done/(double)total);
            printf("\r[%7.3f%%]",percent_done);fflush(stdout);if(units_done>=total) printf("\n");
            return false;}
        return true;
    }
};
}
#endif
