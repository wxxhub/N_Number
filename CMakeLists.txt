cmake_minimum_required(VERSION 3.5)
project(n_digital)

include_directories(
    include
)

aux_source_directory(src DIR_LIB_SRCS)
add_library(${PROJECT_NAME}
    ${DIR_LIB_SRCS}
)
add_executable(demo example/demo.cpp)
target_link_libraries(demo
    ${PROJECT_NAME}
)