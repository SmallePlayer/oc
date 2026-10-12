add_executable(server ser.cpp)
target_link_libraries(server core)

add_executable(client cli.cpp)
target_link_libraries(client core)

find_package(OpenCV REQUIRED)
add_executable(cv_test cv_test.cpp)
target_include_directories(cv_test PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(cv_test ${OpenCV_LIBS})

add_executable(udp_frame_sender udp_frame_sender.cpp)
target_link_libraries(udp_frame_sender cv_net)

add_executable(udp_frame_receiver udp_frame_receiver.cpp)
target_link_libraries(udp_frame_receiver cv_net)
