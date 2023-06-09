cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(GTest REQUIRED)

file(GLOB SOURCE_FILES "*.cpp")
add_executable(test_myostream ${SOURCE_FILES})

target_include_directories(test_myostream PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/
        ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_include_directories(test_myostream PUBLIC ${GTEST_INCLUDE_DIR})
target_link_libraries(test_myostream PUBLIC ${GTEST_LIBRARIES})

add_test(NAME test_myostream COMMAND test_myostream)
