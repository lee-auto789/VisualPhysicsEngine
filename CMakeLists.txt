cmake_minimum_required(VERSION 3.10)
project(VisualPhysicsEngine)

set(CMAKE_CXX_STANDARD 17)

# Set default build type to Debug for validation layers
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Find Vulkan
find_package(Vulkan REQUIRED)

# Create executable
add_executable(${PROJECT_NAME} main.cpp)

# Link Vulkan
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)
