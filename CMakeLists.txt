cmake_minimum_required(VERSION 2.6)
file(TO_CMAKE_PATH "$ENV{IMAGINEPP_ROOT}/CMake" p)
list(APPEND CMAKE_MODULE_PATH "${p}") #For old Imagine++
list(APPEND CMAKE_SYSTEM_FRAMEWORK_PATH /Library/Frameworks) #Mac, why not auto?
find_package(Imagine REQUIRED)

project(Superpixels)

add_executable(Superpixels
               main.cpp
               energie.cpp
               energie.h
               partition.h
               partition.cpp
               point.cpp
               point.h)

ImagineUseModules(Superpixels Images)
