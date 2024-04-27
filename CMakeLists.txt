cmake_minimum_required(VERSION 3.0)

project(GENESIS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

# Find the libraries
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(spdlog REQUIRED)

set(SOURCES_DIR 
    src
)

#NOTE: Must refresh with 'cmake .. ' every time a new cpp file is added
file(GLOB_RECURSE SOURCES ${SOURCES_DIR}/*.cpp)

#Build logger
add_library(Logger SHARED
    src/genlog/log.cpp
    src/genlog/log.hpp
    src/genlog/logger.hpp
  )


# Add the executable
add_executable(Genesis main.cpp ${SOURCES})

# Link the libraries
target_link_libraries(Genesis spdlog::spdlog Logger glfw Vulkan::Vulkan ) 
