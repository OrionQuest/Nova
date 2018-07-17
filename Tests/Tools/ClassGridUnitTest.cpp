#include <nova/Tools/Grids/Grid.h>
#include <nova/Tools/Parsing/Parse_Args.h>
#include <nova/Tools/Utilities/File_Utilities.h>
#include <iostream>

using namespace Nova;

int main(int argc,char** argv)
{
    enum {d=3};
    typedef float T;
    using T_INDEX       = Vector<int,d>;

    Parse_Args parse_args;
    if(d==2) parse_args.Add_Vector_2D_Argument("-size",Vector<double,2>(64),"","Grid resolution");
    else parse_args.Add_Vector_3D_Argument("-size",Vector<double,3>(64),"","Grid resolution");
    parse_args.Add_String_Argument("-o",".","","Output directory");
    parse_args.Parse(argc,argv);

    T_INDEX counts;
    if(d==2){auto counts_2d=parse_args.Get_Vector_2D_Value("-size");for(int v=0;v<d;++v) counts(v)=counts_2d(v);}
    else{auto counts_3d=parse_args.Get_Vector_3D_Value("-size");for(int v=0;v<d;++v) counts(v)=counts_3d(v);}
    std::string output_directory=parse_args.Get_String_Value("-o");

    Log::cout<<"Counts: "<<counts<<std::endl;
    Grid<T,d> grid(counts,Range<T,d>::Unit_Box());
    File_Utilities::Write_To_File(output_directory+"/grid.grid",grid);

    return 0;
}
