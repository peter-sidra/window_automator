add_executable(${PROJECT_NAME}_example example1.cpp)
target_link_libraries(${PROJECT_NAME}_example PRIVATE ${PROJECT_NAME})