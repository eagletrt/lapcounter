cmake_minimum_required(VERSION 3.10)

project(lapcounter_playground)

add_subdirectory(lapcounter)

find_package(raylib 3.0 REQUIRED)

include_directories(${PROJECT_SOURCE_DIR})

add_executable(lapcounter_stats lapcounter_stats.cpp)
target_compile_features(lapcounter_stats PRIVATE cxx_std_17)
target_link_libraries(lapcounter_stats PUBLIC lapcounter)

add_executable(lapcounter_playground lapcounter_playground.cpp)
target_compile_features(lapcounter_playground PRIVATE cxx_std_17)
target_link_libraries(lapcounter_playground PUBLIC lapcounter raylib dl pthread)

if (APPLE)
    target_link_libraries(lapcounter_playground PUBLIC "-framework IOKit")
    target_link_libraries(lapcounter_playground PUBLIC "-framework Cocoa")
    target_link_libraries(lapcounter_playground PUBLIC "-framework OpenGL")
endif()