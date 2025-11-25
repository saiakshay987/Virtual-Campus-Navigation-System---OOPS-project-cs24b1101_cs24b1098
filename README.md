# Virtual Campus Navigator - IIITDM Kancheepuram

A sophisticated C++ application demonstrating **Object-Oriented Programming (OOP) principles** combined with **interactive geospatial navigation** using SFML 3.0. Navigate the IIITDM Kancheepuram campus with real GPS coordinates, pathfinding algorithms, and multiple navigation modes.

---

##  Table of Contents

1. [Overview](#overview)
2. [Features](#features)
3. [OOP Concepts Demonstrated](#oop-concepts-demonstrated)
4. [Project Structure](#project-structure)
5. [Build Instructions](#build-instructions)
6. [Usage Guide](#usage-guide)
7. [Technical Details](#technical-details)
8. [File Descriptions](#file-descriptions)

---

##  Overview

The **Virtual Campus Navigator** is an educational C++ project that showcases:
- **OOP Design Patterns**: Encapsulation, inheritance, polymorphism, and abstraction.
- **Graph Algorithms**: Dijkstra's shortest path algorithm for campus navigation.
- **GUI Development**: SFML 3.0-based interactive map with pan/zoom controls.
- **Geospatial Computing**: GPS coordinate mapping, Haversine distance calculations.
- **Real Campus Data**: Integrated with IIITDM Kancheepuram coordinates and infrastructure.

Users can explore campus buildings, calculate optimal routes, and experience walking/cycling time estimates.

---

##  Features

### Core Functionality
- **Two UI Modes**:
  - **Explore Mode**: Click buildings to view detailed information (name, GPS coordinates, description).
  - **Navigation Mode**: Select start/end points, add waypoint detours (vias), and compute optimal paths.

- **Smart Pathfinding**:
  - Dijkstra's algorithm finds shortest paths through campus roads.
  - Support for intermediate waypoints (via locations).
  - Real-time distance and time estimates.

- **Multiple Navigation Modes**:
  - **Walking Mode**: Calculates walking time (5 km/h default).
  - **Cycling Mode**: Calculates cycling time (15 km/h default).

- **Interactive Map**:
  - Pan: Click and drag to move around the campus.
  - Zoom: Mouse wheel to zoom in/out (0.5x to 3.0x).
  - Color-coded markers:
    - Green: Selected start location.
    - Red: Selected end location.
    - Magenta: Via waypoints (with numeric badges).
    - Cyan: Regular buildings.
  - Hidden turn nodes (intermediate navigation points) displayed as small circles without labels.

- **Info Panel** (side panel):
  - Displays current mode, selected locations, and estimated time/distance.
  - Lists via waypoints with up/down/delete controls.
  - Real-time error messages if route not found.

- **Keyboard Shortcuts**:
  - **E**: Switch to Explore mode.
  - **N**: Switch to Navigation mode.
  - **W**: Switch to Walking navigation.
  - **C**: Switch to Cycling navigation.
  - **Esc**: Clear path selection.
  - **Right-Click** on a building: Toggle it as a via waypoint.

---

##  OOP Concepts Demonstrated

### 1. **Encapsulation**
- `Location` class hides internal data (latitude, longitude, ID).
- Public getter/setter methods provide controlled access with validation.
- Example: `Location::setLatitude()` validates coordinate ranges.

### 2. **Inheritance & Polymorphism**
- Derived classes: `AcademicBuilding`, `HostelBuilding` inherit from `Location`.
- Virtual method `displayInfo()` allows polymorphic behavior.
- Each building type provides specialized properties (departments, capacity, floors).

### 3. **Abstraction**
- `Navigator` class abstracts Dijkstra's algorithm.
- Users call simple `findPath(start, end)` without knowing implementation details.
- `Graph<T>` template abstracts graph operations.

### 4. **Operator Overloading**
- `Path` class overloads:
  - `operator+` to concatenate paths (combine legs for via waypoints).
  - `operator<` to compare paths by distance.

### 5. **Templates & Generics**
- `Graph<T>` is a template class supporting any node type (not just `Location*`).
- Enables reusable graph algorithms.

### 6. **Exception Handling**
- Custom exceptions: `InvalidLocationException`, `PathNotFoundException`, `ViaSelectionException`.
- Graceful error handling in pathfinding and GUI interactions.

### 7. **Navigation Modes (Strategy Pattern)**
- `NavigationMode` interface with implementations:
  - `WalkingMode`: 5 km/h speed.
  - `CyclingMode`: 15 km/h speed.
- Modes swapped at runtime via `Navigator::setNavigationMode()`.

---

##  Project Structure

```
Project/
├── src/
│   ├── main.cpp                 # Entry point, initialization, OOP demo
│   ├── Location.h / Location.cpp # Base location class (encapsulation)
│   ├── AcademicBuilding.h / .cpp # Derived class (inheritance)
│   ├── HostelBuilding.h / .cpp   # Derived class (inheritance)
│   ├── Navigator.h / Navigator.cpp # Pathfinding engine (Dijkstra, abstraction)
│   ├── Graph.h                   # Template graph class (templates, generics)
│   ├── Path.h / Path.cpp         # Path class (operator overloading)
│   ├── CampusData.h              # GPS coordinates & paths (data layer)
│   ├── GUIHandler.h / GUIHandler.cpp # SFML GUI (events, rendering, pan/zoom)
│   ├── NavigationMode.h          # Interface for navigation modes
│   ├── WalkingMode.h / CyclingMode.h # Concrete modes (strategy pattern)
│   └── CMakeLists.txt            # Build configuration
├── README.md                     # This file
└── VirtualCampusNavigator.exe    # Compiled executable
```

---

##  Build Instructions

### Option 1: Using VS Code Build Task
1. Open the project in VS Code.
2. Press `Ctrl+Shift+B` to run the default build task (`build SFML project`).
3. Executable `VirtualCampusNavigator.exe` is created in the project root.

### Option 2: Manual Compilation (PowerShell)
```powershell
cd C:\Users\HP\OneDrive\Desktop\Project

g++ src/main.cpp src/Location.cpp src/GUIHandler.cpp `
    src/Navigator.cpp src/Path.cpp src/AcademicBuilding.cpp `
    src/HostelBuilding.cpp -o VirtualCampusNavigator.exe `
    -IC:/Users/HP/OneDrive/Desktop/SFML-3.0.2/include `
    -LC:/Users/HP/OneDrive/Desktop/SFML-3.0.2/lib `
    -lsfml-graphics -lsfml-window -lsfml-system `
    -lfreetype -lvorbisfile -lvorbisenc -lvorbis -lFLAC -logg
```

### Build Output
- Success: `VirtualCampusNavigator.exe` created (✓ Exit Code 0).
- Failure: Compile/link errors displayed. Check SFML paths and MinGW installation.

---

## 🎮 Usage Guide

### Launching the Application
```powershell
.\VirtualCampusNavigator.exe
```

### Startup Sequence
1. Console displays OOP concept demonstrations.
2. Pathfinding test runs (Walking & Cycling modes).
3. SFML GUI window opens with the campus map.

### Navigation Workflow

#### **Explore Mode (Learning)**
1. Press **E** or click "Switch to Explore (E)".
2. Click any building marker on the map.
3. Right panel displays:
   - Name, GPS coordinates, unique ID.
   - Detailed description (e.g., "Mtech and Mdes hostel for boys").

#### **Navigation Mode (Route Planning)**
1. Press **N** or click "Switch to Navigation (N)".
2. **Select Start**: Click a building (turns green).
3. **Select End**: Click another building (turns red).
4. Path is calculated and displayed in red on the map.
5. Right panel shows:
   - Start/End location names.
   - **Distance** in meters and **Time** in minutes.

#### **Adding Waypoints (Vias)**
1. In Navigation mode, **right-click** a building to add it as a via.
2. Building marker turns magenta with a numbered badge.
3. Path recalculates to pass through vias in order.
4. Right panel shows via list with controls:
   - **u** button: Move via up in the list.
   - **d** button: Move via down in the list.
   - **Del** button: Remove via from route.

#### **Changing Navigation Speed**
1. Press **W** to switch to Walking (slower, realistic walking speed).
2. Press **C** to switch to Cycling (faster, biking speed).
3. Time estimate updates instantly.

#### **Map Controls**
- **Drag (Left-Click + Move)**: Pan around the map.
- **Mouse Wheel Up**: Zoom in.
- **Mouse Wheel Down**: Zoom out.
- **Zoom Range**: 0.5x (zoomed out) to 3.0x (zoomed in).

#### **Clearing Selection**
- Press **Esc** to reset Start/End/Via selections.

---

##  Technical Details

### Pathfinding Algorithm
**Dijkstra's Shortest Path**
- **Time Complexity**: O((V + E) log V) using min-heap priority queue.
- **Space Complexity**: O(V) for distance and visited maps.
- **Inputs**: Start location, end location, optional via waypoints.
- **Outputs**: Ordered path with total distance.

### Distance Calculation
**Haversine Formula** (great-circle distance on Earth)
```
d = 2R * arcsin(sqrt(sin²(Δlat/2) + cos(lat₁)cos(lat₂)sin²(Δlon/2)))
```
- **R** = Earth radius ≈ 6,371 km
- **Δlat**, **Δlon** = latitude/longitude differences in radians
- Returns distance in meters.

### Time Estimation
```
Walking:  time (min) = distance (m) / (3 km/h) / 1000 * 60
Cycling:  time (min) = distance (m) / (10 km/h) / 1000 * 60
```

### Campus Data
- **27 visible buildings** (academic blocks, hostels, amenities, sports facilities).
- **9 hidden turn nodes** (intermediate navigation waypoints without labels).
- **45+ path connections** (bidirectional edges linking buildings and turns).
- **GPS Bounds**: 
  - Latitude: 12.83508° to 12.84066°N
  - Longitude: 80.13529° to 80.13933°E
  - (Campus area: ~51 acres)

### SFML 3.0 API Highlights
- **Event Polling**: `window_.pollEvent()` returns `std::optional<sf::Event>`.
- **Event Checking**: `event.getIf<sf::Event::MouseButtonPressed>()` for type-safe checks.
- **Text Rendering**: `sf::Text` requires font; fallback to system fonts if not found.
- **View Transforms**: Pan/zoom via `sf::View` with `setSize()` and `move()`.
- **Cursor Shapes**: `sf::Cursor::createFromSystem()` for hand/arrow cursors.

---

##  File Descriptions

### Core Classes

| File | Purpose | Key Methods |
|------|---------|-------------|
| `Location.h/cpp` | Base class for campus locations | `getName()`, `getLatitude()`, `getLongitude()`, `getDescription()`, `setLatitude(val)`, `setLongitude(val)` |
| `AcademicBuilding.h/cpp` | Academic facility (inherits Location) | `addDepartment()`, `setNumberOfClassrooms()`, `setNumberOfLabs()` |
| `HostelBuilding.h/cpp` | Student hostel (inherits Location) | `setCapacity()`, `setCurrentOccupancy()`, `setGenderType()`, `setNumberOfFloors()` |
| `Navigator.h/cpp` | Pathfinding engine | `findPath(start, end)`, `findPath(start, end, vias)`, `setNavigationMode()`, `getEstimatedTime()` |
| `Graph.h` | Template graph data structure | `addNode()`, `addUndirectedEdge()`, `getNeighbors()` |
| `Path.h/cpp` | Represents a route | `addLocation()`, `getTotalDistance()`, `getLocations()`, `operator+()` |
| `GUIHandler.h/cpp` | SFML GUI and rendering | `initialize()`, `run()`, `handleEvents()`, `render()`, `drawBuildings()`, `drawPaths()` |
| `CampusData.h` | Static GPS data | `BUILDINGS[]`, `PATHS[]`, `gpsToScreen()` |
| `NavigationMode.h` | Interface for speed modes | `calculateTime(distance)`, `getModeName()` |
| `WalkingMode.h`, `CyclingMode.h` | Concrete modes | Walking: 3 km/h; Cycling: 10 km/h |

### Data Files

| File | Purpose |
|------|---------|
| `CampusData.h` | GPS coordinates (27 buildings + 9 turn nodes), path connections, bounding box |

---

## Troubleshooting

### Exe Closes Immediately
- **Cause**: Compilation error or missing SFML libs.
- **Fix**: Run from PowerShell console to see error messages:
  ```powershell
  .\VirtualCampusNavigator.exe
  ```

### SFML Include/Linker Errors
- **Cause**: SFML path incorrect.
- **Fix**: Update paths in build command:
  ```
  -IC:/Users/HP/OneDrive/Desktop/SFML-3.0.2/include
  -LC:/Users/HP/OneDrive/Desktop/SFML-3.0.2/lib
  ```

### Text Not Rendering
- **Cause**: Font file not found.
- **Fix**: App tries Windows system fonts first (`C:\Windows\Fonts\arial.ttf`); falls back gracefully. Check console output for warnings.

### Pathfinding Returns "No Path Exists"
- **Cause**: Start/end nodes not connected in the graph.
- **Fix**: Ensure both buildings are listed in `BUILDINGS[]` and a path edge connects them (directly or via turns) in `PATHS[]`.

---

##  Sample Outputs

### Console (Startup)
```
========================================
Virtual Campus Navigator
IIITDM Kancheepuram
========================================

Initializing campus data...
Loaded 36 campus buildings
Loaded 45 path connections
Graph initialized successfully

[OOP CONCEPTS DEMONSTRATION - omitted]

========================================
PATHFINDING TEST
========================================

Finding path from Main gate to SBI ATM

Walking Mode:
Path (256.442m): Main gate -> turn_01 -> turn_02 -> SBI ATM
Distance: 256.442m
Time: 3.0773 minutes

Cycling Mode:
Path (256.442m): Main gate -> turn_01 -> turn_02 -> SBI ATM
Distance: 256.442m
Time: 1.02577 minutes

Launching GUI...
```

### GUI Display
- **Left**: Interactive campus map with color-coded markers, red path line.
- **Right**: Info panel showing current mode, selected locations, distance, time, and via list.

---

## Learning Outcomes

By making this project, we understood:
1.  How to design classes with encapsulation and inheritance.
2.  How to implement polymorphism and virtual methods.
3.  How to apply Dijkstra's algorithm to real-world problems.
4.  How to build interactive GUIs with SFML.
5.  How to handle geospatial data (GPS coordinates, distances).
6.  How to organize a mid-sized C++ project.
7.  How to use templates for generic programming.
8.  How to implement strategy and factory patterns.

---

##  Authors
Sai akshay potnuru - CS24B1101
Harshith Ch - CS24B1098



## 🔗 References

- **SFML Documentation**: https://www.sfml-dev.org/documentation/3.0/
- **Dijkstra's Algorithm**: https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
- **Haversine Formula**: https://en.wikipedia.org/wiki/Haversine_formula
- **OOP in C++**: https://en.cppreference.com/

---
