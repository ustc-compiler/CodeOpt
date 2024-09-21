cmake_minimum_required(VERSION 3.5)

project(SysYFCompiler)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -Wextra -Wno-unused -Wshadow -g -pedantic")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# add macros to generate Bison & Flex Cpp code from grammar
# find_package(BISON REQUIRED)
# find_package(FLEX REQUIRED)

# BISON_TARGET(SysYFParser ${CMAKE_CURRENT_SOURCE_DIR}/grammar/SysYFParser.yy ${CMAKE_CURRENT_BINARY_DIR}/SysYFParser.cpp
#              COMPILE_FLAGS "-Wall -Wcounterexamples --defines=${CMAKE_CURRENT_BINARY_DIR}/SysYFParser.h")
# FLEX_TARGET(SysYFScanner ${CMAKE_CURRENT_SOURCE_DIR}/grammar/SysYFScanner.ll  ${CMAKE_CURRENT_BINARY_DIR}/SysYFScanner.cpp
#             COMPILE_FLAGS "--debug --c++")
# ADD_FLEX_BISON_DEPENDENCY(SysYFScanner SysYFParser)

# include generated files in project environment
include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/AST)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/ErrorReporter)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Frontend)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/SysYFIR)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/SysYFIRBuilder)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Log)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Optimize)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/AST)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/ErrorReporter)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/Frontend)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/SysYFIR)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/SysYFIRBuilder)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/Log)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/Optimize)

# add_library(
#   BISON_FLEX_LIB STATIC
#   ${BISON_SysYFParser_OUTPUTS}
#   ${FLEX_SysYFScanner_OUTPUTS}
# )

add_executable(
  compiler
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

target_link_libraries(
  compiler
  SysYFPass
  SysYFIRBuilder
  IRLib
  Driver
  Checker
  ASTPrinter
  ErrReporter
  Log
  # BISON_FLEX_LIB
)

add_executable(
        test_logging
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Log/test_logging.cpp
)

target_link_libraries(
        test_logging
        Log
)