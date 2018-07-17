# Grid Class Test
add_executable (ClassGridUnitTest ClassGridUnitTest.cpp)
target_link_libraries (ClassGridUnitTest nova::tools)

add_test (NAME "Nova::Grid_UnitTest" COMMAND ClassGridUnitTest)
