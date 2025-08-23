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

# Create executable
add_executable(${PROJECT_NAME} 
    main.cpp 
    PhysicsEngine.cpp
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${GLFW_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    Vulkan::Vulkan 
    ${GLFW_LIBRARIES}
)

message(STATUS "Using local GLFW - surface support enabled")
