add_executable(server ser.cpp)
target_link_libraries(server core)

add_executable(client cli.cpp)
target_link_libraries(client core)
