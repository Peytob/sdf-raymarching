cmake_minimum_required(VERSION 3.12)
set (PROJECT_NAME SDF-RayMarching)
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 14)

# - - - CMake configuration - - -

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# - - - Libraries - - -

# - Nlohmann JSON

find_package(nlohmann_json REQUIRED)
link_libraries(nlohmann_json::nlohmann_json)

# - OGL

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
include_directories(${OPENGL_INCLUDE_DIRS})
link_libraries(${OPENGL_LIBRARIES})

# - GLFW

find_package(glfw3 REQUIRED)
link_libraries(glfw)

# - GLEW

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

# - GLM

find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})
link_directories(${GLM_LIBRARY_DIRS})
add_definitions(${GLM_DEFINITIONS})

# - - - Resources - - -

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

# - - - Project - - -

file(GLOB_RECURSE PROJECT_SRC_FILES CONFIGURE_DEPENDS
	"src/*.cpp",
	"src/*.c"
)

include_directories("src/")

add_executable(${PROJECT_NAME} ${PROJECT_SRC_FILES})

