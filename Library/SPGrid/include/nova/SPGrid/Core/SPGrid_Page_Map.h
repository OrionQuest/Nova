//!#####################################################################
//! \file SPGrid_Page_Map.h
//!#####################################################################
// Class SPGrid_Page_Map
//######################################################################
#ifndef __SPGrid_Page_Map__
#define __SPGrid_Page_Map__

#include <nova/SPGrid/Core/SPGrid_Geometry.h>
#include <stdint.h>
#include <vector>

namespace SPGrid{
class SPGrid_Page_Map
{
  public:
    const uint64_t map_size;                // Size of the page map, in uint64_t units. Each entry corresponds to 64 4KB pages.
    uint64_t* page_map;                     // The actual page map - a bitmap structured as an array of 64-bit entries.
    bool dirty;                             // Indicates that block offsets are inconsistent with the page map (perhaps for a good reason, if only one of them is used).
    std::vector<uint64_t> block_offsets;    // Alternative representation as a list of linearized offsets. Created on demand.

    // Make the pagemap class noncopyable
    SPGrid_Page_Map(const SPGrid_Page_Map&) = delete;
    SPGrid_Page_Map& operator=(const SPGrid_Page_Map&) = delete;

    template<int d>
    SPGrid_Page_Map(const SPGrid_Geometry<d>& geometry)
        :map_size((geometry.Padded_Volume()/geometry.Elements_Per_Block()+0x3fUL)>>6)
    {
        page_map = static_cast<uint64_t*>(Raw_Allocate(map_size*sizeof(uint64_t)));
        dirty = false;
    }
    
    ~SPGrid_Page_Map()
    {
        Raw_Deallocate(page_map,map_size*sizeof(uint64_t));
    }

    void Clear_Page_Map()
    {
        Raw_Deallocate(page_map,map_size*sizeof(uint64_t));
        page_map = static_cast<uint64_t*>(Raw_Allocate(map_size*sizeof(uint64_t)));
        dirty = true;
    }

    void Clear_Blocks()
    {
        std::vector<uint64_t>().swap(block_offsets);
        dirty = true;
    }

    void Clear()
    {
        Clear_Page_Map();
        Clear_Blocks();
        dirty = false;
    }

    void Append_Pages(const std::vector<uint64_t>& page_offsets)
    {
        for(auto it=page_offsets.cbegin();it!=page_offsets.cend();++it) Set_Page(*it);
        Update_Block_Offsets();
    }

    void Set_Page(const uint64_t offset)
    {
        uint64_t mask = 1UL << (offset>>12 & 0x3f);
        uint64_t& entry = page_map[offset>>18];
        if(mask & ~entry){
#pragma omp atomic
            entry |= mask;
        }
        if(!dirty) dirty = true; // Important to avoid unnecessary write sharing
    }

    bool Test_Page(const uint64_t offset) const
    {
        uint64_t mask = 1UL << (offset>>12 & 0x3f);
        const uint64_t& entry = page_map[offset>>18];
        return entry&mask;
    }

    std::pair<const uint64_t*,unsigned> Get_Blocks() const
    {
        if(block_offsets.size())
            return std::pair<const uint64_t*,unsigned>(&block_offsets[0],block_offsets.size());
        else
            return std::pair<const uint64_t*,unsigned>((const uint64_t*)0,0);
    }

    void Update_Block_Offsets()
    {
        if(dirty)
        {
            std::vector<uint64_t> new_block_offsets(Generate_Block_Offsets());
            block_offsets.swap(new_block_offsets);
        }
        dirty = false;
    }

    // This implementation is currently suboptimal in that it will touch the entirety of the page map.
    // It should perferably be implemented using mincore() instead, to selectively query only resident pages.
    std::vector<uint64_t> Generate_Block_Offsets()
    {
        std::vector<uint64_t> block_offsets;
        for(uint64_t entry=0;entry<map_size;entry++)
            if(page_map[entry])
                for(uint64_t pos=0;pos<64;pos++)
                    if(page_map[entry]&(1UL<<pos))
                        block_offsets.push_back((entry<<18)|(pos<<12));
        return block_offsets;
    }
//######################################################################
};
}
#endif
