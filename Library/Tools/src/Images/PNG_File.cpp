//!#####################################################################
//! \file PNG_File.h
//!#####################################################################
#include <nova/Tools/Images/Image.h>
#include <nova/Tools/Images/PNG_File.h>
#include <nova/Tools/Log/Debug_Utilities.h>
#include <nova/Tools/Utilities/Exceptions.h>
#include <png.h>
using namespace Nova;
//######################################################################
// Read
//######################################################################
template<class T> void PNG_File<T>::
Read(const std::string& filename,Array_ND<Vector<T,3>,2>& image)
{
    FILE* file=fopen(filename.c_str(),"rb");
    if(!file) throw READ_ERROR("Failed to open "+filename+" for reading");

    png_structp png_ptr=png_create_read_struct(PNG_LIBPNG_VER_STRING,0,0,0);
    if(!png_ptr) throw READ_ERROR("Error reading png file "+filename);
    png_infop info_ptr=png_create_info_struct(png_ptr);
    if(!info_ptr) throw READ_ERROR("Error reading png file "+filename);
    if(setjmp(png_jmpbuf(png_ptr))) throw READ_ERROR("Error reading png file "+filename);
    png_init_io(png_ptr,file);
    png_read_png(png_ptr,info_ptr,PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_STRIP_ALPHA | PNG_TRANSFORM_PACKING,0);
    int width=png_get_image_width(png_ptr,info_ptr),height=png_get_image_height(png_ptr,info_ptr);
    int color_type=png_get_color_type(png_ptr,info_ptr);
    if(color_type!=PNG_COLOR_TYPE_RGB && color_type!=PNG_COLOR_TYPE_RGBA) FATAL_ERROR("PNG read only supports RGB and RGBA");
    image.Resize(Range<int,2>(T_INDEX({0,width-1}),T_INDEX({0,height-1})));
    Vector<unsigned char,3>** row_pointers=(Vector<unsigned char,3>**)png_get_rows(png_ptr,info_ptr);
    for(int i=0;i<width;++i)for(int j=0;j<height;++j) image(T_INDEX({i,j}))=Image<T>::Byte_Color_To_Scalar_Color(row_pointers[height-j-1][i]);
        
    png_destroy_read_struct(&png_ptr,&info_ptr,0);
    fclose(file);
    return;
}
//######################################################################
// Write
//######################################################################
template<class T> template<int d> void PNG_File<T>::
Write(const std::string& filename,const Array_ND<Vector<T,d>,2>& image)
{
    FILE* file=fopen(filename.c_str(),"wb");
    if(!file) FATAL_ERROR("Failed to open "+filename+" for writing");

    png_structp png_ptr=png_create_write_struct(PNG_LIBPNG_VER_STRING,0,0,0);
    if(!png_ptr) FATAL_ERROR("Error writing png file "+filename);
    png_infop info_ptr=png_create_info_struct(png_ptr);
    if(!info_ptr) FATAL_ERROR("Error writing png file "+filename);
    if(setjmp(png_jmpbuf(png_ptr))) FATAL_ERROR("Error writing png file "+filename);
    png_init_io(png_ptr,file);
    int color_type;
    switch(d){
        case 3:color_type=PNG_COLOR_TYPE_RGB;
               break;
        case 4:color_type=PNG_COLOR_TYPE_RGBA;
               break;
        default:FATAL_ERROR("Invalid number of channels for png write");
                break;}
    png_set_IHDR(png_ptr,info_ptr,image.counts(0),image.counts(1),8,color_type,PNG_INTERLACE_NONE,PNG_COMPRESSION_TYPE_DEFAULT,PNG_FILTER_TYPE_DEFAULT);

    Vector<unsigned char,d>* byte_data=new Vector<unsigned char,d>[image.counts(1)*image.counts(0)];
    Vector<unsigned char,d>** row_pointers=new Vector<unsigned char,d>*[image.counts(1)];
    for(int j=0;j<image.counts(1);++j){
        row_pointers[image.counts(1)-j-1]=byte_data+image.counts(0)*(image.counts(1)-j-1);
        for(int i=0;i<image.counts(0);++i) row_pointers[image.counts(1)-j-1][i]=Image<T>::Scalar_Color_To_Byte_Color(image(T_INDEX({i,j})));}
    png_set_rows(png_ptr,info_ptr,(png_byte**)row_pointers);
    png_write_png(png_ptr,info_ptr,PNG_TRANSFORM_IDENTITY,0);
    delete[] row_pointers;delete[] byte_data;

    png_destroy_write_struct(&png_ptr,&info_ptr);
    fclose(file);
    return;
}
//######################################################################
template class Nova::PNG_File<float>;
template void Nova::PNG_File<float>::Write(const std::string&,const Array_ND<Vector<float,3>,2>&);
template void Nova::PNG_File<float>::Write(const std::string&,const Array_ND<Vector<float,4>,2>&);
#ifdef COMPILE_WITH_DOUBLE_SUPPORT
template class Nova::PNG_File<double>;
template void Nova::PNG_File<double>::Write(const std::string&,const Array_ND<Vector<double,3>,2>&);
template void Nova::PNG_File<double>::Write(const std::string&,const Array_ND<Vector<double,4>,2>&);
#endif
