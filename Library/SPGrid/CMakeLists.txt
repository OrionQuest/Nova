set(ENABLE_SPGRID_SUPPORT OFF CACHE BOOL "Enable building the SPGrid framework.")

if(ENABLE_SPGRID_SUPPORT)
# Target for SPGrid
add_library( _nova_spgrid
  src/Core/SPGrid_Geometry.cpp
  src/Core/SPGrid_Utilities.cpp
  )

target_include_directories( _nova_spgrid PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../Geometry/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE src )

# Add required compiler features
# See here for more information:
# https://cmake.org/cmake/help/v3.1/prop_gbl/CMAKE_CXX_KNOWN_FEATURES.html#prop_gbl:CMAKE_CXX_KNOWN_FEATURES

target_compile_features( _nova_spgrid
  PRIVATE
  cxx_nullptr
  )

set(spgrid_use_haswell FALSE CACHE BOOL "use haswell-enabled bit-interleaving hardware instruction.")

if(spgrid_use_haswell)
  if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    target_compile_options( _nova_spgrid PUBLIC -mbmi2)
  endif()
  target_compile_definitions( _nova_spgrid PUBLIC HASWELL=1)
endif()

target_link_libraries( _nova_spgrid
  PUBLIC
  nova::tools
  )


add_library( nova::spgrid ALIAS _nova_spgrid )
endif()
