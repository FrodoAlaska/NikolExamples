cmake_minimum_required(VERSION 3.27)
project(NikolExamples)

### Project Variables ###
############################################################
set(EXAMPLES_SRC_DIR examples)
set(EXAMPLES_LIBS_DIR ${CMAKE_SOURCE_DIR}/libs)
set(EXAMPLES_INCLUDE_DIR ${EXAMPLES_SRC_DIR} ${EXAMPLES_LIBS_DIR} ${EXAMPLES_LIBS_DIR}/nikol/include ${EXAMPLES_LIBS_DIR}/glm)
set(EXAMPLES_LIBRARIES nikol)

# set(EXAMPLES_BUILD_FLAGS -w)
set(EXAMPLES_BUILD_TYPE DEBUG)
############################################################

### CMake Variables ###
############################################################
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE ${EXAMPLES_BUILD_TYPE})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS ${EXAMPLES_BUILD_FLAGS})
############################################################

### Library Sources ###
############################################################
set(LIBS_SOURCES 
  ${EXAMPLES_LIBS_DIR}/stb/stb_image.cpp
)
############################################################

### Libraries ###
############################################################
add_subdirectory(libs/nikol)
############################################################

### Final Build ###
############################################################
# Copy the assets for all the examples to use
file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# List of all current examples
add_subdirectory(${EXAMPLES_SRC_DIR}/hello_nikol)
add_subdirectory(${EXAMPLES_SRC_DIR}/batch_renderer)
############################################################
