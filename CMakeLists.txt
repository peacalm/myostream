cmake_minimum_required(VERSION 3.14)
project(MyOStream
        VERSION 2.8
        DESCRIPTION "MyOStream header-only lib.")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

SET(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include")
SET(LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib")
SET(CMAKECONFIG_INSTALL_DIR "${LIB_INSTALL_DIR}/cmake/${PROJECT_NAME}")

configure_file("${PROJECT_NAME}Config.cmake.in" "${PROJECT_NAME}Config.cmake"
        @ONLY)
install(FILES "${CMAKE_SOURCE_DIR}/include/myostream.h"
        DESTINATION ${INCLUDE_INSTALL_DIR})
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        DESTINATION ${CMAKECONFIG_INSTALL_DIR})

option(BUILD_TEST "Build test." FALSE)
if (BUILD_TEST)
  enable_testing()
  add_subdirectory(test)
endif()
