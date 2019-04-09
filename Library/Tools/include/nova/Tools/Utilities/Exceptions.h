//!#####################################################################
//! \file Exceptions.h
//!#####################################################################
// Class Exceptions
//######################################################################
#ifndef __Exceptions__
#define __Exceptions__

#include <stdexcept>

namespace Nova{

// Note: destructors must be in .cpp to avoid shared library name lookup issues

#define Nova_Simple_Exception(Error,Base)   \
    struct Error: public Base               \
    {                                       \
        Error(const std::string& message)   \
            :Base(message)                  \
        {}                                  \
                                            \
        virtual ~Error() throw ();          \
    };

// base class for all Nova exceptions
Nova_Simple_Exception(NOVA_ERROR,std::runtime_error)

Nova_Simple_Exception(READ_ERROR,NOVA_ERROR)
Nova_Simple_Exception(FILESYSTEM_ERROR,NOVA_ERROR)
Nova_Simple_Exception(LOOKUP_ERROR,NOVA_ERROR)
    Nova_Simple_Exception(INDEX_ERROR,LOOKUP_ERROR)
    Nova_Simple_Exception(KEY_ERROR,LOOKUP_ERROR)
Nova_Simple_Exception(TYPE_ERROR,NOVA_ERROR)
Nova_Simple_Exception(VALUE_ERROR,NOVA_ERROR)
Nova_Simple_Exception(NOT_IMPLEMENTED_ERROR,NOVA_ERROR)
Nova_Simple_Exception(ASSERTION_ERROR,NOVA_ERROR)
Nova_Simple_Exception(FLOATING_POINT_ERROR,NOVA_ERROR)

#undef Nova_Simple_Exception
}
#endif
