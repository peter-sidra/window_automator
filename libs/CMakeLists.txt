find_package(OpenCV CONFIG REQUIRED)

add_library(${PROJECT_NAME}_libs INTERFACE)
target_link_libraries(${PROJECT_NAME}_libs INTERFACE ${OpenCV_LIBS} Dxgi Dwmapi)
target_include_directories(${PROJECT_NAME}_libs INTERFACE ${OpenCV_INCLUDE_DIRS})
