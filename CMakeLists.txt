project( face )

cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options( -Wall -Wextra -pedantic -Werror -Wfatal-errors )
find_package( OpenCV REQUIRED )

add_executable( face src/main.cpp src/cli.cpp src/cmd.cpp src/frame_source.cpp src/frame_sink.cpp src/detect.cpp )
target_link_libraries( face ${OpenCV_LIBS} )
