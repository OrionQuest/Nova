//!#####################################################################
//! \file Grid_Iterator.h
//!#####################################################################
// Class Grid_Iterator
//######################################################################
#ifndef __Grid_Iterator__
#define __Grid_Iterator__

#include <nova/Tools/Grids/Grid.h>

namespace Nova{
template<class T,int d>
class Grid_Iterator
{
    typedef Vector<int,d> T_INDEX;

  public:
    const Grid<T,d>& grid;
    int number_of_regions,current_region;
    Range<int,d> region,regions[1<<d];          // at most 2^d distinct regions for iteration
    T_INDEX index;
    bool valid;

    Grid_Iterator(const Grid<T,d>& grid_input)
        :grid(grid_input),number_of_regions(0)
    {}

    Grid_Iterator(const Grid<T,d>& grid_input,const Range<int,d>& region_input)
        :grid(grid_input),number_of_regions(0)
    {
        Add_Region(region_input);
        Reset();
    }

    void Reset_Regions()
    {number_of_regions=0;}

    void Add_Region(const Range<int,d>& region_input)
    {
        if(d==0) return;
        if(region_input.Edge_Lengths().Min()>=0) regions[number_of_regions++]=region_input;
    }

    void Reset(const int region_index=0)
    {
        if(d==0){valid=true;index=T_INDEX();return;}

        valid=(region_index<number_of_regions);
        if(valid)
        {
            current_region=region_index;
            region=regions[current_region];
            index=region.min_corner;
        }
    }

    bool Valid() const
    {return valid;}

    void Next()
    {
        if(d==0){valid=false;return;}
        if(index(d-1)<region.max_corner(d-1)) ++index(d-1);
        else Next_Helper();
    }

  protected:
    void Next_Helper()
    {
        index(d-1)=region.min_corner(d-1);
        for(int i=d-2;i>=0;--i)
        {
            if(index(i)<region.max_corner(i)){++index(i);return;}
            index(i)=region.min_corner(i);
        }
        Reset(current_region+1);
    }
};
}
#endif
