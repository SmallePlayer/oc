find_package(OpenCV REQUIRED)

add_library(cv_net STATIC
    cv_net.cpp
)

target_include_directories(cv_net PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(cv_net PUBLIC
    core
    ${OpenCV_LIBS}
)
