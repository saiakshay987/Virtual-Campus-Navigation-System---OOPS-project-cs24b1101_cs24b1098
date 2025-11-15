cmake_minimum_required(VERSION 3.10)
project(VirtualCampusNavigator VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Source files
set(SOURCES
    src/main.cpp
    src/Location.cpp
    src/Path.cpp
    src/Navigator.cpp
    src/GUIHandler.cpp
)

# Headers
set(HEADERS
    src/CampusData.h
    src/Location.h
    src/AcademicBuilding.h
    src/HostelBuilding.h
    src/Path.h
    src/Graph.h
    src/NavigationMode.h
    src/WalkingMode.h
    src/CyclingMode.h
    src/Navigator.h
    src/GUIHandler.h
)

# Create executable
add_executable(VirtualCampusNavigator ${SOURCES} ${HEADERS})

# Link SFML libraries
target_link_libraries(VirtualCampusNavigator 
    sfml-graphics 
    sfml-window 
    sfml-system
)

# Include directories
target_include_directories(VirtualCampusNavigator PRIVATE src)

# Compiler warnings
if(MSVC)
    target_compile_options(VirtualCampusNavigator PRIVATE /W4)
else()
    target_compile_options(VirtualCampusNavigator PRIVATE -Wall -Wextra -pedantic)
endif()
