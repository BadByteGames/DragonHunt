# At LEAST 3.11
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

#require c++11
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set where dependencies will be installed
#set(HUNTER_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../hunter)

#use hunter to manage dependencies
#include("cmake/HunterGate.cmake")

#download hunter
#HunterGate(
#    URL "https://github.com/ruslo/hunter/archive/v0.23.27.tar.gz"
#    SHA1 "70d75a25f895df3da00d8382417e8c2a08c93173"
#)

project(DragonHunt VERSION 1.1 LANGUAGES CXX)

#install boost filesystem to test
#hunter_add_package(Boost COMPONENTS filesystem)
#install tinyxml2 for parsing xml
#hunter_add_package(Tinyxml2)

#time to add the packages
#find_package(Boost CONFIG REQUIRED filesystem)
#find_package(Tinyxml2 REQUIRED)

#the executable
add_subdirectory(thirdparty/Tinyxml2)
add_subdirectory(DragonHunt)
