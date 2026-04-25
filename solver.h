#ifndef SOLVER_H
#define SOLVER_H

#define ROWS 20
#define COLS 20

// structure to represent a cell in the grid
// row and col store its position
typedef struct
{
    int row;
    int col;
} Node;

// visited array keeps track of exploration
// 0 → not visited
// 1 → visited during search
// 2 → part of final shortest path
extern int visited[ROWS][COLS];

// parent array is used to reconstruct path
// it stores from which cell we reached a given cell
extern Node parent[ROWS][COLS];

// initializes search from starting cell
void initSearch(int startRow, int startCol);

// performs one step of BFS or DFS
// returns:
// 0 → no nodes left (search ended)
// 1 → still searching
// 2 → destination found
int stepSearch(int endRow, int endCol, int maze[ROWS][COLS], int useDFS);

#endif