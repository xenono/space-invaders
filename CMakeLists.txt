cmake_minimum_required(VERSION 3.17)
project(space_invaders C)

set(CMAKE_C_STANDARD 11)
# add extra include directories
include_directories(chipmunk/include)
include_directories(CSFML/include)

# add extra lib directories

# link_directories(chipmunk/msvc/VS2015/chipmunk/x64/Debug)
link_directories(CSFML/lib/msvc/)

# link each of the lib library files u need like below

link_libraries(csfml-graphics.lib)
link_libraries(csfml-system.lib)
link_libraries(csfml-audio.lib)
link_libraries(csfml-window.lib)
# link_libraries(chipmunk.lib)

# copy the runtime dll files to the project’s binary directory

file(COPY CSFML/bin/ DESTINATION ${PROJECT_BINARY_DIR})

add_executable(space_invaders main.c objects.h utils.c utils.h)