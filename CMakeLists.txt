cmake_minimum_required(VERSION 3.25)
project(erlivideo-task)

find_package( OpenCV REQUIRED )

add_executable(${PROJECT_NAME})
add_subdirectory(src)

target_link_libraries(${PROJECT_NAME}
        tvm
        ${OpenCV_LIBS})
