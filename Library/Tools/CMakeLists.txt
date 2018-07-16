# Target for Tools
add_library( _nova_tools
  src/Grids/Grid.cpp
  src/Vectors/Vector_2D.cpp
  src/Vectors/Vector_3D.cpp
  )

find_package(Boost 1.54 REQUIRED COMPONENTS filesystem)

if(USE_DOUBLES)
  target_compile_definitions(_nova_tools PUBLIC COMPILE_WITH_DOUBLE_SUPPORT=1)
endif()

target_include_directories( _nova_tools PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE src )

target_compile_features( _nova_tools
  PUBLIC
  cxx_variadic_templates
  cxx_nullptr
  )

target_link_libraries( _nova_tools
  PUBLIC
  ${Boost_LIBRARIES}
  )

add_library( nova::tools ALIAS _nova_tools )
