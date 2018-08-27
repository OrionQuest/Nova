//!####################################################################
//! \file SPGrid_Flags.h
//!####################################################################
#ifndef __SPGrid_Flags__
#define __SPGrid_Flags__

namespace Nova{
enum{
    Cell_Type_Interior              = 0x00000001u,              // Set by user
    Cell_Type_Dirichlet             = 0x00000002u,              // Set by user
    Cell_Type_Ghost                 = 0x00000004u,              // Set by user
    Cell_Type_Active                = 0x00000008u,              // Generated automatically

    Face_Type_X_Valid               = 0x00000010u,              // Generated automatically - A face is valid if :
    Face_Type_Y_Valid               = 0x00000020u,              //   (a) it is the face of an interior cell, AND
    Face_Type_Z_Valid               = 0x00000040u,              //   (b) is undivided (not a part of a larger face)

    Face_Type_X_Active              = 0x00000080u,              // Active faces are initialized to be all the valid faces that don't touch exterior cells
    Face_Type_Y_Active              = 0x00000100u,              // These flags can be overridden by user, to specify additional Neumann regions
    Face_Type_Z_Active              = 0x00000200u,              // NOTE: The user can only *deactivate* active faces, not vice versa

    Node_Active                     = 0x00000400u,
    Node_Coarse_Shared              = 0x00000800u,
    Node_T_Junction                 = 0x00001000u,
    Node_Ghost                      = 0x00004000u,

    Face_Minus_X_Active             = Face_Type_X_Active,       // aliased
    Face_Minus_Y_Active             = Face_Type_Y_Active,       // aliased
    Face_Minus_Z_Active             = Face_Type_Z_Active,       // aliased
    Face_Plus_X_Active              = 0x00002000u,
    Face_Plus_Y_Active              = 0x00008000u,
    Face_Plus_Z_Active              = 0x00020000u,

    Face_Minus_X_Scaled             = 0x00040000u,
    Face_Plus_X_Scaled              = 0x00080000u,
    Face_Minus_Y_Scaled             = 0x00100000u,
    Face_Plus_Y_Scaled              = 0x00200000u,
    Face_Minus_Z_Scaled             = 0x00400000u,
    Face_Plus_Z_Scaled              = 0x00800000u,

    Ghost_Child_000                 = 0x01000000u,
    Ghost_Child_001                 = 0x02000000u,
    Ghost_Child_010                 = 0x04000000u,
    Ghost_Child_011                 = 0x08000000u,
    Ghost_Child_100                 = 0x10000000u,
    Ghost_Child_101                 = 0x20000000u,
    Ghost_Child_110                 = 0x40000000u,
    Ghost_Child_111                 = 0x80000000u
};
enum{MG_Boundary=Node_Coarse_Shared};                           // careful!
}
#endif
