cmake_minimum_required(VERSION 3.14)
project(MyOstream
        VERSION 2.2
        DESCRIPTION "MyOstream header-only lib.")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

install(FILES "include/myostream.h" DESTINATION include)

option(BUILD_TEST "Build test." FALSE)
if (BUILD_TEST)
  add_subdirectory(test)
endif()