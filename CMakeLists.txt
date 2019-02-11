project( face )

# Dependencies.
cmake_minimum_required(VERSION 3.13)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -lstdc++ -Wfatal-errors -Wall -Wextra -Wpedantic")
find_package( OpenCV REQUIRED )

add_executable( detect src/detect.cpp )
target_link_libraries( detect ${OpenCV_LIBS} )

add_executable( train src/train.cpp )
target_link_libraries( train ${OpenCV_LIBS} )

add_executable( recognise src/recognise.cpp )
target_link_libraries( recognise ${OpenCV_LIBS} )
