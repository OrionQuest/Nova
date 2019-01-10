//!#####################################################################
//! \file Shadow_Grid_Helper.h
//!#####################################################################
// Class Shadow_Grid_Helper
//######################################################################
#ifndef __Shadow_Grid_Helper__
#define __Shadow_Grid_Helper__

#include <nova/SPGrid/Core/SPGrid_Allocator.h>
#include <nova/SPGrid/Core/SPGrid_Utilities.h>

namespace SPGrid{
template<class Struct_type,class T,int d> class Shadow_Grid_Helper;

template<class Struct_type,class T>
class Shadow_Grid_Helper<Struct_type,T,2>
{
    enum {d=2};
    using Allocator_type            = SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask           = typename Allocator_type::template Array_mask<unsigned>;

    enum {
        prefetch_degree = 0,
        block_xsize = 1u << Flag_array_mask::block_xbits,
        block_ysize = 1u << Flag_array_mask::block_ybits,
        og_xsize = block_xsize+2,
        og_ysize = block_ysize+2,
        xmin = 1,
        ymin = 1,
        // Inclusive!!! give mins and maxs for actual block within shadow grid
        xmax = og_xsize-2,
        ymax = og_ysize-2,
    };

  public:
    static void Compute_Shadow_Grid(uint64_t* offset_grid_ptr,uint64_t packed_offset)
    {
        typedef uint64_t (&offset_grid_type)[og_xsize][og_ysize];
        offset_grid_type o_grid = reinterpret_cast<offset_grid_type>(*offset_grid_ptr);
        
        uint64_t simple_offset = 0;
        // Fill in simple offsets
        for (int i = xmin; i<=xmax; i++) for (int j = ymin; j<=ymax; j++){
          o_grid[i][j] = packed_offset + simple_offset;  // Can do simple addition here since addresses are within block
          simple_offset += sizeof(T);}

        // Vertices
        o_grid[xmin-1][ymin-1] = Flag_array_mask::template Packed_Offset<-1,-1>(o_grid[xmin][ymin]);
        o_grid[xmin-1][ymax+1] = Flag_array_mask::template Packed_Offset<-1, 1>(o_grid[xmin][ymax]);
        o_grid[xmax+1][ymin-1] = Flag_array_mask::template Packed_Offset< 1,-1>(o_grid[xmax][ymin]);
        o_grid[xmax+1][ymax+1] = Flag_array_mask::template Packed_Offset< 1, 1>(o_grid[xmax][ymax]);

        // 4 edges
        simple_offset = o_grid[xmin][ymin-1] = Flag_array_mask::template Packed_Offset< 0,-1>(o_grid[xmin][ymin]);
        for( int i = xmin+1; i <= xmax; i++ ) o_grid[i][ymin-1] = simple_offset + (i-1)*(sizeof(T) * (block_ysize));
        simple_offset = o_grid[xmin][ymax+1] = Flag_array_mask::template Packed_Offset< 0, 1>(o_grid[xmin][ymax]);
        for( int i = xmin+1; i <= xmax; i++ ) o_grid[i][ymax+1] = simple_offset + (i-1)*(sizeof(T) * (block_ysize));

        simple_offset = o_grid[xmin-1][ymin] = Flag_array_mask::template Packed_Offset<-1, 0>(o_grid[xmin][ymin]);
        for( int j = ymin+1; j <= ymax; j++ ) o_grid[xmin-1][j] = simple_offset + (j-1)*(sizeof(T));
        simple_offset = o_grid[xmax+1][ymin] = Flag_array_mask::template Packed_Offset< 1, 0>(o_grid[xmax][ymin]);
        for( int j = ymin+1; j <= ymax; j++ ) o_grid[xmax+1][j] = simple_offset + (j-1)*(sizeof(T));

        // First let's do the starting points
        o_grid[xmin-1][ymin] =  Flag_array_mask::template Packed_OffsetXdim<-1>(o_grid[xmin][ymin]);
        o_grid[xmax+1][ymin] =  Flag_array_mask::template Packed_OffsetXdim< 1>(o_grid[xmax][ymin]);

        o_grid[xmin][ymin-1] =  Flag_array_mask::template Packed_OffsetYdim<-1>(o_grid[xmin][ymin]);
        o_grid[xmin][ymax+1] =  Flag_array_mask::template Packed_OffsetYdim< 1>(o_grid[xmin][ymax]);

        // Fill in edge offsets (cube faces, but not edges will be correct after this)
        // This is ok for 6 neighbors, but one more pass will be needed for kernels that use edges
        {
          // Left and Right face
          for (int i=xmin-1; i<=xmax+1; i+= (xmax-xmin)+2){
            simple_offset = o_grid[i][ymin];
            for (int j=ymin; j<=ymax; j++){
              o_grid[i][j] = simple_offset;
              simple_offset += sizeof(T);}}     // Simple addition (going through neighboring block in same manner)
        }
        
        {
          // Top and bottom face
          for (int j = ymin-1; j<=ymax+1; j+= (ymax-ymin)+2){
            simple_offset = o_grid[xmin][j];
            for (int i=xmin; i<=xmax; i++){
              o_grid[i][j] = simple_offset;
              simple_offset += sizeof(T) * (block_ysize);}}
        }
    }
};

template<class Struct_type,class T>
class Shadow_Grid_Helper<Struct_type,T,3>
{
    enum {d=3};
    using Allocator_type            = SPGrid_Allocator<Struct_type,d>;
    using Flag_array_mask           = typename Allocator_type::template Array_mask<unsigned>;

    enum {
        prefetch_degree = 0,
        block_xsize = 1u << Flag_array_mask::block_xbits,
        block_ysize = 1u << Flag_array_mask::block_ybits,
        block_zsize = 1u << Flag_array_mask::block_zbits,
        og_xsize = block_xsize+2,
        og_ysize = block_ysize+2,
        og_zsize = block_zsize+2,
        xmin = 1,
        ymin = 1,
        zmin = 1,
        // Inclusive!!! give mins and maxs for actual block within shadow grid
        xmax = og_xsize-2,
        ymax = og_ysize-2,
        zmax = og_zsize-2
    };

  public:
    static void Compute_Shadow_Grid(uint64_t* offset_grid_ptr,uint64_t packed_offset)
    {
        typedef uint64_t (&offset_grid_type)[og_xsize][og_ysize][og_zsize];
        offset_grid_type o_grid = reinterpret_cast<offset_grid_type>(*offset_grid_ptr);

        // clear o_grid
        for(int i=0;i<og_xsize;i++) for(int j=0;j<og_ysize;j++) for(int k=0;k<og_zsize;k++) o_grid[i][j][k]=packed_offset;

        uint64_t simple_offset = 0;
        // Fill in simple offsets
        for (int i = xmin; i<=xmax; i++) for (int j = ymin; j<=ymax; j++) for (int k = zmin; k<=zmax; k++){
          o_grid[i][j][k] = packed_offset + simple_offset;  // Can do simple addition here since addresses are within block
          simple_offset += sizeof(T);}

        // Vertices
        o_grid[xmin-1][ymin-1][zmin-1] = Flag_array_mask::template Packed_Offset<-1,-1,-1>(o_grid[xmin][ymin][zmin]);
        o_grid[xmin-1][ymin-1][zmax+1] = Flag_array_mask::template Packed_Offset<-1,-1, 1>(o_grid[xmin][ymin][zmax]);
        o_grid[xmin-1][ymax+1][zmin-1] = Flag_array_mask::template Packed_Offset<-1, 1,-1>(o_grid[xmin][ymax][zmin]);
        o_grid[xmin-1][ymax+1][zmax+1] = Flag_array_mask::template Packed_Offset<-1, 1, 1>(o_grid[xmin][ymax][zmax]);
        o_grid[xmax+1][ymin-1][zmin-1] = Flag_array_mask::template Packed_Offset< 1,-1,-1>(o_grid[xmax][ymin][zmin]);
        o_grid[xmax+1][ymin-1][zmax+1] = Flag_array_mask::template Packed_Offset< 1,-1, 1>(o_grid[xmax][ymin][zmax]);
        o_grid[xmax+1][ymax+1][zmin-1] = Flag_array_mask::template Packed_Offset< 1, 1,-1>(o_grid[xmax][ymax][zmin]);
        o_grid[xmax+1][ymax+1][zmax+1] = Flag_array_mask::template Packed_Offset< 1, 1, 1>(o_grid[xmax][ymax][zmax]);

        // 12 edges
        simple_offset = o_grid[xmin][ymin-1][zmin-1] = Flag_array_mask::template Packed_Offset< 0,-1,-1>(o_grid[xmin][ymin][zmin]);
        for( int i = xmin+1; i <= xmax; i++ ) o_grid[i][ymin-1][zmin-1] = simple_offset + (i-1)*(sizeof(T) * (block_ysize) * (block_zsize));
        simple_offset = o_grid[xmin][ymin-1][zmax+1] = Flag_array_mask::template Packed_Offset< 0,-1, 1>(o_grid[xmin][ymin][zmax]);
        for( int i = xmin+1; i <= xmax; i++ ) o_grid[i][ymin-1][zmax+1] = simple_offset + (i-1)*(sizeof(T) * (block_ysize) * (block_zsize));
        simple_offset = o_grid[xmin][ymax+1][zmin-1] = Flag_array_mask::template Packed_Offset< 0, 1,-1>(o_grid[xmin][ymax][zmin]);
        for( int i = xmin+1; i <= xmax; i++ ) o_grid[i][ymax+1][zmin-1] = simple_offset + (i-1)*(sizeof(T) * (block_ysize) * (block_zsize));
        simple_offset = o_grid[xmin][ymax+1][zmax+1] = Flag_array_mask::template Packed_Offset< 0, 1, 1>(o_grid[xmin][ymax][zmax]);
        for( int i = xmin+1; i <= xmax; i++ ) o_grid[i][ymax+1][zmax+1] = simple_offset + (i-1)*(sizeof(T) * (block_ysize) * (block_zsize));
        simple_offset = o_grid[xmin-1][ymin][zmin-1] = Flag_array_mask::template Packed_Offset<-1, 0,-1>(o_grid[xmin][ymin][zmin]);
        for( int j = ymin+1; j <= ymax; j++ ) o_grid[xmin-1][j][zmin-1] = simple_offset + (j-1)*(block_zsize*sizeof(T));
        simple_offset = o_grid[xmin-1][ymin][zmax+1] = Flag_array_mask::template Packed_Offset<-1, 0, 1>(o_grid[xmin][ymin][zmax]);
        for( int j = ymin+1; j <= ymax; j++ ) o_grid[xmin-1][j][zmax+1] = simple_offset + (j-1)*(block_zsize*sizeof(T));
        simple_offset = o_grid[xmax+1][ymin][zmin-1] = Flag_array_mask::template Packed_Offset< 1, 0,-1>(o_grid[xmax][ymin][zmin]);
        for( int j = ymin+1; j <= ymax; j++ ) o_grid[xmax+1][j][zmin-1] = simple_offset + (j-1)*(block_zsize*sizeof(T));
        simple_offset = o_grid[xmax+1][ymin][zmax+1] = Flag_array_mask::template Packed_Offset< 1, 0, 1>(o_grid[xmax][ymin][zmax]);
        for( int j = ymin+1; j <= ymax; j++ ) o_grid[xmax+1][j][zmax+1] = simple_offset + (j-1)*(block_zsize*sizeof(T));
        simple_offset = o_grid[xmin-1][ymin-1][zmin] = Flag_array_mask::template Packed_Offset<-1,-1, 0>(o_grid[xmin][ymin][zmin]);
        for( int k = zmin+1; k <= zmax; k++ ) o_grid[xmin-1][ymin-1][k] = simple_offset + (k-1)*sizeof(T);
        simple_offset = o_grid[xmin-1][ymax+1][zmin] = Flag_array_mask::template Packed_Offset<-1, 1, 0>(o_grid[xmin][ymax][zmin]);
        for( int k = zmin+1; k <= zmax; k++ ) o_grid[xmin-1][ymax+1][k] = simple_offset + (k-1)*sizeof(T);
        simple_offset = o_grid[xmax+1][ymin-1][zmin] = Flag_array_mask::template Packed_Offset< 1,-1, 0>(o_grid[xmax][ymin][zmin]);
        for( int k = zmin+1; k <= zmax; k++ ) o_grid[xmax+1][ymin-1][k] = simple_offset + (k-1)*sizeof(T);
        simple_offset = o_grid[xmax+1][ymax+1][zmin] = Flag_array_mask::template Packed_Offset< 1, 1, 0>(o_grid[xmax][ymax][zmin]);
        for( int k = zmin+1; k <= zmax; k++ ) o_grid[xmax+1][ymax+1][k] = simple_offset + (k-1)*sizeof(T);

        // First let's do the starting points
        o_grid[xmin-1][ymin][zmin] =  Flag_array_mask::template Packed_OffsetXdim<-1>(o_grid[xmin][ymin][zmin]);
        o_grid[xmax+1][ymin][zmin] =  Flag_array_mask::template Packed_OffsetXdim< 1>(o_grid[xmax][ymin][zmin]);

        o_grid[xmin][ymin][zmin-1] =  Flag_array_mask::template Packed_OffsetZdim<-1>(o_grid[xmin][ymin][zmin]);
        o_grid[xmin][ymin][zmax+1] =  Flag_array_mask::template Packed_OffsetZdim< 1>(o_grid[xmin][ymin][zmax]);

        o_grid[xmin][ymin-1][zmin] =  Flag_array_mask::template Packed_OffsetYdim<-1>(o_grid[xmin][ymin][zmin]);
        o_grid[xmin][ymax+1][zmin] =  Flag_array_mask::template Packed_OffsetYdim< 1>(o_grid[xmin][ymax][zmin]);

        // Fill in edge offsets (cube faces, but not edges will be correct after this)
        // This is ok for 6 neighbors, but one more pass will be needed for kernels that use edges
        {
          // Left and Right face
          for (int i=xmin-1; i<=xmax+1; i+= (xmax-xmin)+2){
            simple_offset = o_grid[i][ymin][zmin];
            for (int j=ymin; j<=ymax; j++) for (int k=zmin; k<=zmax; k++){
              o_grid[i][j][k] = simple_offset;
              simple_offset += sizeof(T);}}     // Simple addition (going through neighboring block in same manner)
        }

        {
          // Front and Back face
          for (int k=zmin-1; k<=zmax+1; k+= (zmax-zmin)+2){
            simple_offset = o_grid[xmin][ymin][k];
            for (int i=xmin; i<=xmax; i++) for (int j=ymin; j<=ymax; j++){
              o_grid[i][j][k] = simple_offset;
              simple_offset += block_zsize*sizeof(T);}}
        }
        
        {
          // Top and bottom face
          for (int j=ymin-1; j<=ymax+1; j+= (ymax-ymin)+2){
            simple_offset = o_grid[xmin][j][zmin];
            for (int i=xmin; i<=xmax; i++){
              for (int k=zmin; k<=zmax; k++){
                o_grid[i][j][k] = simple_offset;
                simple_offset += sizeof(T);}
              simple_offset += sizeof(T) * (block_ysize-1) * (block_zsize);}}
        }
    }
};
}
#endif
