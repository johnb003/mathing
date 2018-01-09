cmake_minimum_required(VERSION 3.10)

include(extern_gtest.cmake)

add_executable(testmath
    src/main.cpp)

target_link_libraries(testmath
    mathing
    ${GTEST_LIBRARY}
)

include(GoogleTest)

gtest_discover_tests(testmath TEST_LIST testmath_targets)
#gtest_add_tests(TARGET testmath TEST_LIST testmath_targets)

# set each target to timeout if not finished within 1 sec
set_tests_properties(${testmath_targets} PROPERTIES TIMEOUT 1)
