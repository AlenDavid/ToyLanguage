cmake_minimum_required(VERSION 3.25)
project(ToyLanguage)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message("LLVM_INCLUDE_DIRS=${LLVM_INCLUDE_DIRS}")
message("LLVM_DEFINITIONS=${LLVM_DEFINITIONS}")

add_subdirectory(src)
add_subdirectory(tools)
