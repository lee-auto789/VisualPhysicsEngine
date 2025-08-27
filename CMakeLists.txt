cmake_minimum_required(VERSION 3.10)
project(VisualPhysicsEngine)

set(CMAKE_CXX_STANDARD 17)

# Set default build type to Debug for validation layers
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Find Vulkan
find_package(Vulkan REQUIRED)

# Find GLFW
find_package(glfw3 3.3 REQUIRED)

# Add GLM (OpenGL Mathematics)
# You can download GLM from https://github.com/g-truc/glm
# For now, we'll assume GLM is in a local directory
set(GLM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/glm")
if(EXISTS "${GLM_DIR}")
    set(GLM_INCLUDE_DIRS "${GLM_DIR}")
else()
    # If GLM is not found locally, you can use vcpkg or download it
    message(WARNING "GLM not found in external/glm. Please download GLM or install via vcpkg.")
    # For vcpkg: find_package(glm CONFIG REQUIRED)
endif()

# Create physics library
add_library(Physics STATIC
    PhysicsEngine.cpp
    PhysicsObject.cpp
)

# Create executable
add_executable(${PROJECT_NAME} 
    main.cpp 
    Renderer.cpp
)

# Create test executable
add_executable(PhysicsTest
    main_test.cpp
)

# Include directories
target_include_directories(Physics PRIVATE
    ${GLM_INCLUDE_DIRS}
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${GLM_INCLUDE_DIRS}
)

target_include_directories(PhysicsTest PRIVATE
    ${GLM_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    Vulkan::Vulkan 
    glfw
    Physics
)

target_link_libraries(PhysicsTest
    Physics
)

message(STATUS "Using local GLFW - surface support enabled")
