//!#####################################################################
//! \file PNG_File.h
//!#####################################################################
#include <nova/Tools/Utilities/Exceptions.h>
namespace Nova{

#define INSTANTIATE(ERROR) \
    ERROR::~ERROR() throw () {}

INSTANTIATE(NOVA_ERROR)

INSTANTIATE(READ_ERROR)
INSTANTIATE(FILESYSTEM_ERROR)
INSTANTIATE(LOOKUP_ERROR)
INSTANTIATE(INDEX_ERROR)
INSTANTIATE(KEY_ERROR)
INSTANTIATE(TYPE_ERROR)
INSTANTIATE(VALUE_ERROR)
INSTANTIATE(NOT_IMPLEMENTED_ERROR)
INSTANTIATE(ASSERTION_ERROR)
INSTANTIATE(FLOATING_POINT_ERROR)

}
