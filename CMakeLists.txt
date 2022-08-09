cmake_minimum_required(VERSION 3.9)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set a project name.
project(Voxel-Physics)

# Find the packages we need.
find_package(OpenGL REQUIRED)
if (UNIX)
  find_package(glfw3 REQUIRED)
endif (UNIX)

# Add source files
file(GLOB_RECURSE SOURCE_FILES voxel-physics/*.cpp )
# Add resource files
#file(GLOB_RECURSE RESOURCE_FILES voxel-physics/assets/*)
# Copy resource files to the current binary directory (relative paths)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/voxel-physics/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(WIN32)
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/voxel-physics/libraries/lib/glfw3.lib
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif(WIN32)

# Add executable (Voxel-Engine)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

# Include directories
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/voxel-physics/libraries 
  ${CMAKE_CURRENT_SOURCE_DIR}/voxel-physics/libraries/include)

# Link link_libraries to target
if (UNIX)
  target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC ${OPENGL_LIBRARIES} glfw)
endif (UNIX)
if(WIN32)
  target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC ${OPENGL_LIBRARIES} glfw3)
endif(WIN32)

