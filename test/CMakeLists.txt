cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(GTest REQUIRED)
if (NOT GTEST_FOUND)
    message(FATAL_ERROR "GTest not found! Please install GTest first.")
endif()

file(GLOB SOURCE_FILES "*.cpp")
add_executable(myostream_test ${SOURCE_FILES})

target_include_directories(myostream_test PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/
        ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_include_directories(myostream_test PUBLIC ${GTEST_INCLUDE_DIR})
target_link_libraries(myostream_test PUBLIC ${GTEST_LIBRARIES})

add_test(NAME myostream_test COMMAND myostream_test)
