cmake_minimum_required(VERSION 3.10)

project(lapcounter_playground)

add_subdirectory(lapcounter)

find_package(raylib 3.0 REQUIRED)

add_executable(lapcounter_playground lapcounter_playground.cpp)

target_compile_features(lapcounter_playground PRIVATE cxx_std_17)
target_link_libraries(lapcounter_playground PUBLIC lapcounter raylib)
target_include_directories(lapcounter_playground PUBLIC ${PROJECT_SOURCE_DIR})

if (APPLE)
    target_link_libraries(lapcounter_playground PUBLIC "-framework IOKit")
    target_link_libraries(lapcounter_playground PUBLIC "-framework Cocoa")
    target_link_libraries(lapcounter_playground PUBLIC "-framework OpenGL")
endif()