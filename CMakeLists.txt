cmake_minimum_required(VERSION 3.10)
project(VisualPhysicsEngine)

set(CMAKE_CXX_STANDARD 17)

# Set default build type to Debug for validation layers
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Find Vulkan
find_package(Vulkan REQUIRED)

# Use local GLFW
set(GLFW_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/glfw-3.3.8.bin.WIN64")
set(GLFW_INCLUDE_DIRS "${GLFW_DIR}/include")
set(GLFW_LIBRARIES "${GLFW_DIR}/lib-vc2022/glfw3.lib")

# Create executable
add_executable(${PROJECT_NAME} main.cpp)

# Include GLFW headers
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRS})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    Vulkan::Vulkan 
    ${GLFW_LIBRARIES}
)

message(STATUS "Using local GLFW - surface support enabled")
