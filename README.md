# Maze Solver using BFS and DFS (C + Raylib)

## Overview
This project is an interactive maze solver implemented in C using the Raylib graphics library. 
It demonstrates how Breadth-First Search (BFS) and Depth-First Search (DFS) work through real-time visualization.

---

## Features
- Interactive grid-based maze editor
- Supports BFS and DFS algorithms
- Step-by-step visualization of search
- Adjustable animation speed
- Random maze generation
- Saves solution to a text file

---

## Controls
- Mouse Click → Draw / Remove walls
- S → Set Start point
- E → Set End point
- Start Button → Begin solving
- Reset Button → Clear maze
- Random Button → Generate maze
- Mode Button → Toggle BFS / DFS
- + / - → Adjust speed

---

## Project Structure
```
maze-solver/
│
├── main.c
├── maze.c
├── maze.h
├── solver.c
├── solver.h
├── build.bat
└── README.md
```

---

## Code Structure

---
## Design Approach

The project follows a clear separation of concerns:

- `main.c` → Handles interaction and visualization  
- `maze.c` → Manages data  
- `solver.c` → Handles logic  
---

### 1. main.c (Visualization and Control Layer)

This file acts as the entry point of the program and controls the overall execution.

It is responsible for handling user interaction, rendering the interface, and coordinating the algorithm execution.

#### Key Responsibilities:
- Initializes the window using Raylib
- Handles mouse input for drawing and removing walls
- Handles keyboard input for setting start and end points
- Manages UI buttons:
  - Start
  - Reset
  - Random Maze
  - BFS / DFS Toggle
  - Speed Control
- Executes the algorithm step-by-step for smooth animation
- Draws the maze grid and updates colors dynamically
- Reconstructs and displays the final path after solving

---

### 2. maze.c / maze.h (Data Layer)

These files are responsible for managing the maze structure and its data.

They strictly deal with storing and modifying the maze, without any involvement in solving it.

#### Key Responsibilities:
- Stores the maze as a 2D grid
- Initializes the maze (clears all cells)
- Generates random maze configurations
- Provides basic utility functions for maze handling

#### Cell Representation:
- `0` → Empty cell (walkable)
- `1` → Wall (blocked)

---

### 3. solver.c / solver.h (Algorithm Layer)

These files contain the core logic of the maze-solving algorithms.

They are completely independent of any graphical interface and focus purely on computation.

#### Key Responsibilities:
- Implements both BFS (Breadth-First Search) and DFS (Depth-First Search)
- Maintains a `visited` array to track explored cells
- Maintains a `parent` array to reconstruct the final path
- Uses a shared container:
  - Acts as a **queue** for BFS
  - Acts as a **stack** for DFS
- Supports step-by-step execution for animation purposes

#### Working Logic:
1. The start node is added to the container
2. At each step:
   - A node is removed (front for BFS, top for DFS)
   - Its neighbors are explored
3. Valid neighbors are:
   - Marked as visited
   - Stored with parent reference
   - Added to the container
4. Once the end node is reached:
   - The path is reconstructed using the parent array

---

## How to Run

1. Make sure Raylib is installed
2. Run:
```
build.bat
```

---

## Output

After solving, a file is generated in:

```
output/solution_YYYYMMDD_HHMMSS.txt
```

This file contains:
- Algorithm used (DFS or BFS)
- Start and End points coordinates
- Path length
- Travelled Path coordinates 

---

## Key Highlights
- BFS guarantees the shortest path and does not ensure the fastest results.
- DFS may find a path faster but not necessarily the shortest.
- Some randomly generated mazes may not have a valid path.
- If Start or End point, either of them is in place from where path could not be found then, maze indicates **NO PATH FOUND**.
- After solving the whole path by using either algorithm, it also calculates **Path length**.
- After finding the path, it stores the whole result in a **OUTPUT** folder, with the name of file according to the time stamp.
- The Output folder consists of the following information - Algorithm used, Path length, Start point, End point, Coordinates of the path taken.
