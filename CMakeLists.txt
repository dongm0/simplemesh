cmake_minimum_required(VERSION 3.10)

project(simplemesh)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")

set(IGL_INCLUDE_DIR "D:/software/libigl/include" CACHE PATH eigen)
set(EIGEN_INCLUDE_DIR D:\\software\\eigen-3.4-rc1 CACHE PATH eigen)
MESSAGE(STATUS ${INCLUDE_DIRECTORIES})

add_subdirectory(./src)

