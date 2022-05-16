cmake_minimum_required(VERSION 3.19.0)
project(window_automator)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion -pedantic")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

add_subdirectory(libs)

set(SRCS
    src/keyboard_utils.cpp
)
set(HEADERS
    include/window.hpp
    include/keyboard_utils.hpp
    include/keyboard.hpp
)

add_library(${PROJECT_NAME} STATIC ${SRCS} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC include/)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_libs)

add_subdirectory(examples)
