#ifndef MAZE_H
#define MAZE_H

#define ROWS 20
#define COLS 20

// This 2D array represents the maze grid
// 0 → free cell
// 1 → wall
extern int maze[ROWS][COLS];

// initializes maze with all cells empty (no walls)
void initMaze();

// fills maze randomly with walls
void randomMaze();

#endif