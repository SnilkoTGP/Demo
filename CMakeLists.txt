cmake_minimum_required(VERSION 3.5)

project(TGC_Client)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(TGC_Client main.cpp main.h config.xml)

# Add TinyXML library
find_library(TINYXML_LIBRARIES NAMES tinyxml2 HINTS ${CMAKE_SOURCE_DIR}/libs/tinyxml2/lib)

# Add include directories for TinyXML
target_include_directories(TGC_Client PRIVATE ${CMAKE_SOURCE_DIR}/libs/tinyxml2/include)

# Link TinyXML library
target_link_libraries(TGC_Client ${TINYXML_LIBRARIES})
