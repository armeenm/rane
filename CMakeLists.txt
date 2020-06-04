cmake_minimum_required(VERSION 3.15...3.17 FATAL_ERROR)

include(cmake/BuildType.cmake)
include(cmake/ExportCompileCommands.cmake)

project(rane
  VERSION 0.0.1
  DESCRIPTION "Control station for ilanta-powered robots"
  LANGUAGES CXX)

option(FORCE_COLORED_OUTPUT
  "Always produce ANSI-colored output (GNU/Clang only)" 
  OFF)

option(STATIC_ANALYSIS
  "Enable static analysis tools"
  OFF)

set(CONAN_SYSTEM_INCLUDES ON)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

file(GLOB_RECURSE SRCS
  LIST_DIRECTORIES false
  CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/rane/*.cpp")

file(GLOB BINDINGS
  LIST_DIRECTORIES false
  CONFIGURE_DEPENDS
  "${CMAKE_BINARY_DIR}/bindings/*.cpp")

file(GLOB VERT_SHADERS
  LIST_DIRECTORIES false
  CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/rane/assets/*.vert")

file(GLOB FRAG_SHADERS
  LIST_DIRECTORIES false
  CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/rane/assets/*.frag")

set(SHADERS ${FRAG_SHADERS} ${VERT_SHADERS})

set_source_files_properties(${SRCS}
  PROPERTIES
  COMPILE_OPTIONS "\
-Wshadow;\
-Warray-bounds;\
-Wstringop-overflow;\
-Wduplicated-cond;\
-Wlogical-op;\
-Wduplicated-branches;\
-Wnull-dereference;\
-Wold-style-cast;\
-Wuseless-cast;\
-Wformat=2;\
-Wall;\
-Wextra;\
-Wconversion;\
-Wpedantic;\
-Werror=return-type")

add_executable(rane ${SRCS} ${BINDINGS} ${SHADERS})

target_include_directories(rane PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(rane SYSTEM PRIVATE
  ${CMAKE_BINARY_DIR}/bindings
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party)

find_library(VULKAN_LIB NAMES vulkan vulkan-1)

target_link_libraries(rane PRIVATE ${CONAN_LIBS} ${VULKAN_LIB})

target_compile_definitions(rane PUBLIC VK_PROTOTYPES)
target_compile_features(rane PUBLIC cxx_std_17)
target_compile_options(rane PRIVATE -flto -pipe)

if(CMAKE_BUILD_TYPE MATCHES DEBUG)
  target_compile_options(rane PRIVATE -fsanitize=address,undefined)
  target_link_libraries(rane PRIVATE asan ubsan)
endif()

if(FORCE_COLORED_OUTPUT OR CMAKE_GENERATOR STREQUAL "Ninja")
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(rane PRIVATE -fdiagnostics-color=always)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(rane PRIVATE -fcolor-diagnostics)
  endif()
endif()

if(STATIC_ANALYSIS)
  set_target_properties(rane PROPERTIES
    CXX_CLANG_TIDY "clang-tidy;-checks=*;-header-filter=.*"
    CXX_CPPCHECK "cppcheck;--std=c++11")
endif()
