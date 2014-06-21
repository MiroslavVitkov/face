cmake_minimum_required(VERSION 2.8)
project( lapcam )
find_package( OpenCV REQUIRED )

add_executable( face_detect_and_save src/face_detect_and_save.cpp )
target_link_libraries( face_detect_and_save ${OpenCV_LIBS} )

add_executable( face_rec_train src/face_rec_train.cpp )
target_link_libraries( face_rec_train ${OpenCV_LIBS} )
