# Virtual Campus Navigator - IIITDM Kancheepuram

## Overview
GUI-based C++ application for interactive campus navigation using Dijkstra's algorithm.

## Features
- Interactive campus map with 13 key buildings
- Shortest path calculation using Dijkstra's algorithm
- Multiple navigation modes (Walking/Cycling)
- Real GPS coordinates from IIITDM Kancheepuram
- Visual route highlighting
- Distance and time estimation

## OOP Concepts Implemented

### 1. Encapsulation
- `Location` class with private members and public getters/setters

### 2. Inheritance
- `AcademicBuilding` and `HostelBuilding` inherit from `Location`

### 3. Polymorphism
- `NavigationMode` abstract base class
- `WalkingMode` and `CyclingMode` override virtual methods

### 4. Abstraction
- `Navigator` class hides Dijkstra implementation

### 5. Operator Overloading
- `Path` class overloads +, ==, <, []

### 6. Templates
- `Graph<T>` template class for generic graph operations

### 7. Exception Handling
- Custom exceptions: `PathNotFoundException`, `InvalidLocationException`

## Building the Project

### Prerequisites
- C++ compiler (g++ 7.0+, clang++ 5.0+, MSVC 2017+)
- CMake 3.10+
- SFML 2.5+

### Compilation
```bash
mkdir build
cd build
cmake ..
make
./VirtualCampusNavigator
