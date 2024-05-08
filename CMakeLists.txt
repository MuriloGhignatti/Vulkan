cmake_minimum_required(VERSION 3.22.0)

project(VulkanApp)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Generate the include paths
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Find Vulkan package
find_package(Vulkan REQUIRED)
find_package(GLFW3 REQUIRED)

# Add include directories
include_directories(${Vulkan_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIR})

# Define the executable
add_executable(${PROJECT_NAME} main.cpp)

# Link with Vulkan library
target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARIES} ${GLFW3_LIBRARY})

# If using Visual Studio, set the debugger working directory
if(MSVC)
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endif()
