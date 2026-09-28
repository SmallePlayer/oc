add_executable(example_server ser.cpp)
target_link_libraries(example_server core)

add_executable(example_client cli.cpp)
target_link_libraries(example_client core)
