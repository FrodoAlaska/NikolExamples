cmake_minimum_required(VERSION 3.27)
project(Nikol3DExample)

### CMake Variables ###
############################################################
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
############################################################

### Project Sources ###
############################################################
set(EXAMPLE_SOURCES 
  main.cpp
  mesh.cpp
  material.cpp
  transform.cpp
  camera.cpp
  renderer.cpp
)
############################################################

### Final Build ###
############################################################
add_executable(${PROJECT_NAME} ${EXAMPLE_SOURCES} ${LIBS_SOURCES})
############################################################

### Linking ###
############################################################
target_include_directories(${PROJECT_NAME} PUBLIC BEFORE ${EXAMPLES_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${EXAMPLES_LIBRARIES})
############################################################
