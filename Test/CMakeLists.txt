project(clipboard_lite_tests)


add_executable(${PROJECT_NAME} main.cpp lodepng.cpp)
target_link_libraries(${PROJECT_NAME} clipboard_lite)

add_test (NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
