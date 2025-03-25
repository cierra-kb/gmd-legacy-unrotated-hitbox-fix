cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(ANDROID_ABI armeabi)

set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)

project(rotpatch)

add_library(rotpatch SHARED src/main.cc)

set(DOBBY_DEBUG OFF CACHE INTERNAL "" FORCE)
set(DOBBY_GENERATE_SHARED OFF CACHE INTERNAL "" FORCE)
add_definitions(-DLOGGING_DISABLE -DNDEBUG)
add_subdirectory(vendor/Dobby)

target_link_libraries(rotpatch PRIVATE dobby)
target_compile_options(rotpatch PUBLIC -fdebug-prefix-map=${CMAKE_SOURCE_DIR}=.)
