cmake_minimum_required(VERSION 3.14)
project(MyOStream
        VERSION 2.8
        DESCRIPTION "MyOStream header-only lib.")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

install(FILES "include/myostream.h" DESTINATION include)

option(BUILD_TEST "Build test." FALSE)
if (BUILD_TEST)
  enable_testing()
  add_subdirectory(test)
endif()
