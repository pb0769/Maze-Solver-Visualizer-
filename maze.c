#include "maze.h"
#include <stdlib.h>

int maze[ROWS][COLS];

// This function clears the maze completely
// Every cell is set to 0 (which means no wall)
void initMaze()
{

    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {

            // setting cell as empty
            maze[r][c] = 0;
        }
    }
}

// This function generates a random maze
// Each cell has some probability of becoming a wall
void randomMaze()
{

    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {

            // roughly 25% chance to become a wall
            // rand() % 4 gives 0–3 → only 0 becomes wall
            maze[r][c] = (rand() % 4 == 0);
        }
    }
}