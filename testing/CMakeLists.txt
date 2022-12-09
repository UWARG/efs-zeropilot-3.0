cmake_minimum_required(VERSION 3.18)
project(my_project)
include(CMakePrintHelpers)

cmake_print_variables(PROJECT_SOURCE_DIR)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
# )

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

enable_testing()

# ZP related Lines Start Here 

include_directories(${PROJECT_SOURCE_DIR}/../Common/Inc)

# TM tests
set(TM_LOC ${PROJECT_SOURCE_DIR}/../TelemetryManager)

include_directories(${TM_LOC}/Inc)

file(GLOB TM_CODE
    ${TM_LOC}/tests/*.cpp # include all test cpp files

    # add any dependencies to the above tests here
    # ${TM_LOC}/Inc/CRC32.hpp
    # ${TM_LOC}/Src/CRC32.cpp
)

# AM tests
set(AM_LOC ${PROJECT_SOURCE_DIR}/../AttitudeManager)

include_directories(${AM_LOC}/Inc)

file(GLOB AM_CODE
    ${AM_LOC}/tests/*.cpp # include all test cpp files

    # add any dependencies to the above tests here
)

# PM tests
set(PM_LOC ${PROJECT_SOURCE_DIR}/../PathManager)

include_directories(${PM_LOC}/Inc)

file(GLOB PM_CODE
    ${PM_LOC}/tests/*.cpp # include all test cpp files

    # add any dependencies to the above tests here
)

# SM tests
set(SM_LOC ${PROJECT_SOURCE_DIR}/../SystemManager)

include_directories(${SM_LOC}/Inc)

file(GLOB SM_CODE
    ${SM_LOC}/tests/*.cpp # include all test cpp files

    # add any dependencies to the above tests here
)

# build the test executable
add_executable(
  test_proj
  ${TM_CODE}
  ${AM_CODE}
  ${PM_CODE}
  ${SM_CODE}
)

# ZP related stuff ends here

target_link_libraries(
  test_proj
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_proj)
