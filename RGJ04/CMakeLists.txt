cmake_minimum_required(VERSION 2.8.0)

project(RGJ04)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-extras/")

#set(Boost_USE_STATIC_LIBS ON)
#set(Boost_USE_MULTITHREADED ON)

find_package(SFML 2 COMPONENTS audio graphics system window REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)

add_definitions(-Wall -Wextra -g -pedantic -fPIC -std=c++0x)
set(CMAKE_BUILD_TYPE "Debug")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(
	"${CMAKE_SOURCE_DIR}/src/"
	${Boost_INCLUDE_DIRS}
	${SFML_INCLUDE_DIRS}
)

aux_source_directory(src/ sources)

include_directories(src/)

add_executable(ameboid
    ${sources}
)

target_link_libraries(ameboid
    #${Boost_LIBRARIES}
	${SFML_LIBRARIES}
)
