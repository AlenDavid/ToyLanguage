cmake_minimum_required(VERSION 3.25)
project(ToyLanguage)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message("LLVM_INCLUDE_DIRS=${LLVM_INCLUDE_DIRS}")
message("LLVM_DEFINITIONS=${LLVM_DEFINITIONS}")

llvm_map_components_to_libnames(LLVM_LIBS codegen target core orcjit native)

add_subdirectory(src/lib)
add_subdirectory(tools/toy)

include_directories("${CMAKE_CURRENT_LIST_DIR}")

target_link_libraries(Toy ${LLVM_LIBS})
