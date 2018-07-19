# Target for Geometry
add_library( _nova_geometry
  src/Basic_Geometry/Segment.cpp
  src/Spatial_Acceleration/Box_Hierarchy.cpp
  src/Topology/Simplex_Mesh.cpp
  src/Topology_Based_Geometry/Tetrahedralized_Volume.cpp
  src/Topology_Based_Geometry/Tetrahedralized_Volume_2D.cpp
  src/Topology_Based_Geometry/Tetrahedralized_Volume_3D.cpp
  src/Topology_Based_Geometry/Triangulated_Surface.cpp
  src/Topology_Based_Geometry/Segmented_Curve_2D.cpp
  )

if(USE_DOUBLES)
  target_compile_definitions(_nova_geometry PUBLIC COMPILE_WITH_DOUBLE_SUPPORT=1)
endif()

target_include_directories( _nova_geometry PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../Tools/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE src 
  )

target_compile_features( _nova_geometry
  PUBLIC
  cxx_variadic_templates
  cxx_nullptr
  )

target_link_libraries( _nova_geometry
  PUBLIC
  )

add_library( nova::geometry ALIAS _nova_geometry )

