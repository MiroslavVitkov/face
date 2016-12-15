project( identity )

# Dependencies.
cmake_minimum_required(VERSION 2.8)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wfatal-errors -Wall -Wextra -Wpedantic")
find_package( OpenCV REQUIRED )

# Executable, watches the camera and saves faces to a folder.
add_executable( face_detect_and_save src/face_detect_and_save.cpp )
target_link_libraries( face_detect_and_save ${OpenCV_LIBS} )

# Executable, trains and algorithm over all face pictures in subfolders.
add_executable( train src/train.cpp )
target_link_libraries( train ${OpenCV_LIBS} )

# Executable, wathces the camera and recognises faces, needs training.
add_executable( recognise src/recognise.cpp )
target_link_libraries( recognise ${OpenCV_LIBS} )
