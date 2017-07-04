CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7)

### Below code was used to verify GNU compiler
### version on travis-ci servers
if (CMAKE_COMPILER_IS_GNUCC)
    execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
        OUTPUT_VARIABLE GCC_VERSION)
    string(REGEX MATCHALL "[0-9]+" GCC_VERSION_COMPONENTS
        ${GCC_VERSION})
    list(GET GCC_VERSION_COMPONENTS 0 GCC_MAJOR)
    list(GET GCC_VERSION_COMPONENTS 1 GCC_MINOR)

    message(STATUS "Using GNUCC Compiler Verson ${GCC_MAJOR}.${GCC_MINOR}")
endif()

###############################################################################
### Pulled the section enclosed by hashed lines from the following project.
### https://github.com/arrayfire/arrayfire/blob/devel/test/CMakeLists.txt
FIND_PACKAGE(Threads REQUIRED)
IF(CMAKE_USE_PTHREADS_INIT AND NOT "${APPLE}")
    SET(THREAD_LIB_FLAG "-pthread")
ELSE()
    SET(THREAD_LIB_FLAG ${CMAKE_THREAD_LIB_INIT})
ENDIF()

IF(USE_SYSTEM_GTEST)
    FIND_PACKAGE(GTest REQUIRED)
ELSE(USE_SYSTEM_GTEST)
    INCLUDE("${CMAKE_SOURCE_DIR}/cmake/Modules/build_gtest.cmake")
ENDIF(USE_SYSTEM_GTEST)

#IF(${USE_RELATIVE_TEST_DIR})
#    SET(RELATIVE_TEST_DATA_DIR "../resources" CACHE STRING "Relative Test Data Directory")
#    SET(TESTDATA_SOURCE_DIR ${RELATIVE_TEST_DATA_DIR})
#ELSE(${USE_RELATIVE_TEST_DIR})
#    SET(TESTDATA_SOURCE_DIR "${PROJECT_SOURCE_DIR}/resources")
#ENDIF(${USE_RELATIVE_TEST_DIR})

IF(${CMAKE_GENERATOR} STREQUAL "Xcode")
    ADD_DEFINITIONS("-D TEST_DIR=\"\\\\\"${TESTDATA_SOURCE_DIR}\\\\\"\"")
ELSE(${CMAKE_GENERATOR} STREQUAL "Xcode")
    ADD_DEFINITIONS("-D TEST_DIR=\"\\\"${TESTDATA_SOURCE_DIR}\\\"\"")
ENDIF(${CMAKE_GENERATOR} STREQUAL "Xcode")

IF(NOT ${USE_RELATIVE_TEST_DIR})
# Check if data exists
IF(EXISTS "${TESTDATA_SOURCE_DIR}" AND IS_DIRECTORY "${TESTDATA_SOURCE_DIR}"
    AND EXISTS "${TESTDATA_SOURCE_DIR}/README.md")
    # Test data is available
    # Do Nothing
ELSE(EXISTS "${TESTDATA_SOURCE_DIR}" AND IS_DIRECTORY "${TESTDATA_SOURCE_DIR}"
    AND EXISTS "${TESTDATA_SOURCE_DIR}/README.md")
    MESSAGE(WARNING "Test Data is not available. Tests will build but fail when run.")
    MESSAGE("Clone the repository properly and make sure there is a folder known as resources under it")
ENDIF()
ENDIF(NOT ${USE_RELATIVE_TEST_DIR})
###############################################################################

IF(BUILD_COVERAGE)
    # debug, no optimisation
    # enabling coverage
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage -fprofile-arcs -ftest-coverage")
ENDIF()

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GTEST_INCLUDE_DIRS}
    )

FILE(GLOB FILES "*.cpp" "*.c")

FOREACH(FILE ${FILES})
    GET_FILENAME_COMPONENT(FNAME ${FILE} NAME_WE)
    SET(TEST_NAME ${FNAME})
    ADD_TEST(Test_${TEST_NAME} ${TEST_NAME})
    FILE(GLOB TEST_FILE "${FNAME}.cpp" "${FNAME}.c")
    ADD_EXECUTABLE(${TEST_NAME} ${TEST_FILE})
    ## Following commented out changes work only on CMAKE version >=3.0
    TARGET_LINK_LIBRARIES(${TEST_NAME}  INTERFACE nova
                                        PRIVATE ${THREAD_LIB_FLAG}
                                        PRIVATE ${GTEST_LIBRARIES})
    SET_TARGET_PROPERTIES(${TEST_NAME} PROPERTIES FOLDER "Tests")
ENDFOREACH()
