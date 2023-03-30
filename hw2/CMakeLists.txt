cmake_minimum_required(VERSION 3.10)

project(SpiderAttack)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable target
add_executable(${PROJECT_NAME} main.cpp)

# Add include directories
target_include_directories(${PROJECT_NAME} PRIVATE spider_attack)

# Add source files
target_sources(${PROJECT_NAME} PRIVATE
    spider_attack/base.cpp
    spider_attack/entity.cpp
    spider_attack/environment.cpp
)
