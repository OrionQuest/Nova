# Basic Project Information
cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 11)
project(nova)

# Add the cmake/Modules directory for external dependency location scripts
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/Compilers")
INCLUDE(FindPkgConfig)

# Store all completed executables and libraries just off the main build tree
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# Some options

#SET(ENABLE_SPGRID_SUPPORT OFF CACHE BOOL "Enable support for SPGrid module (Requires Intel C++ Compiler)")

set(USE_DOUBLES FALSE CACHE BOOL "Compile with double precision.")

OPTION(BUILD_TESTS "Build Tests" ON)
OPTION(BUILD_COVERAGE "Create test coverage report" OFF)
OPTION(USE_SYSTEM_GTEST "Use GTEST from system libraries" OFF)

if(USE_DOUBLES)
  target_compile_definitions( PUBLIC COMPILE_WITH_DOUBLE_SUPPORT=1)
endif()

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
  if( CMAKE_VERSION VERSION_LESS 3.6.0 )
    message(FATAL_ERROR "Error: Intel Compiler is not fully supported until CMake version 3.6.0. Please update Cmake.")
  endif()
endif()

# Use shared libraries by default
SET(BUILD_SHARED_LIBS ON) 

# Enable Cmake Unit testing support
IF(BUILD_TESTS)
    ENABLE_TESTING()
    ADD_SUBDIRECTORY(tests)
ENDIF()

# Add the Core library and Projects paths 
add_subdirectory(Library)
#add_subdirectory(Projects)


# add a target to generate API documentation with Doxygen/Sphinx
find_package(Doxygen)
find_package(Sphinx)
if(DOXYGEN_FOUND AND SPHINX_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs_src/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  add_custom_target(doxygen_doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
  add_custom_target(doc
    ${SPHINX_EXECUTABLE} -b html ${CMAKE_CURRENT_SOURCE_DIR}/docs_src ${CMAKE_CURRENT_SOURCE_DIR}/../Nova_Documentation/html
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Sphinx" VERBATIM
    )
  add_custom_target(prep_doc COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/docs )
  add_dependencies( doxygen_doc prep_doc )
  add_dependencies( doc doxygen_doc)
endif(DOXYGEN_FOUND AND SPHINX_FOUND)
