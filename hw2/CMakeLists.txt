cmake_minimum_required(VERSION 3.10)

project(PhahTitu)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable target
add_executable(${PROJECT_NAME} main2.cpp)

# Add include directories
target_include_directories(${PROJECT_NAME} PRIVATE phah_titu)

# Add source files
target_sources(${PROJECT_NAME} PRIVATE
    phah_titu/base.cpp
    phah_titu/entity.cpp
    phah_titu/environment.cpp
)
