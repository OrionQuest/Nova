# Target for Dynamics
add_library(_nova_dynamics INTERFACE)

#if(USE_DOUBLES)
#  target_compile_definitions(_nova_dynamics PUBLIC COMPILE_WITH_DOUBLE_SUPPORT=1)
#endif()

target_include_directories( _nova_dynamics INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../Tools/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../SPGrid/include>
  $<INSTALL_INTERFACE:include>
#  PRIVATE src 
  )

#target_compile_features( _nova_dynamics
#  PUBLIC
#  cxx_variadic_templates
#  cxx_nullptr
#  )

#target_link_libraries( _nova_dynamics
#  PUBLIC
#  nova::spgrid
#  nova::tools
#  )

add_library(nova::dynamics ALIAS _nova_dynamics)

