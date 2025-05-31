cmake_minimum_required(VERSION 3.22)
project(LAP LANGUAGES CXX)

# Set C++ standard and build type
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)

# Find the Eigen3 package
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(Boost REQUIRED COMPONENTS system)

# 1. Tell CMake where the source CSV lives
set(SCENE_CSV_IN  ${CMAKE_SOURCE_DIR}/SCENE_DATA/matricies.csv)

# 2. Tell CMake to copy it into your build directory at configure time
configure_file(
  ${SCENE_CSV_IN}
  ${CMAKE_BINARY_DIR}/SCENE_DATA/matricies.csv
  COPYONLY
)

# List your sources
set(SOURCES
    src/main.cpp
    src/SceneParser.cpp
)

# 1) Define the executable target
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(LAP Eigen3::Eigen)
target_link_libraries(LAP ${Boost_LIBRARIES})

# 2) Now that the target exists, set its include dirs
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/inc
)
