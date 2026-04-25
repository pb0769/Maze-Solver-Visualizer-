#include "solver.h"

int visited[ROWS][COLS];
Node parent[ROWS][COLS];

// This array is used for both BFS and DFS
// It behaves like:
// queue → in BFS
// stack → in DFS
Node container[ROWS * COLS];

int front, rear;

// movement directions → down, up, right, left
int dRow[] = {1, -1, 0, 0};
int dCol[] = {0, 0, 1, -1};

// This function prepares everything before starting search
void initSearch(int startRow, int startCol)
{

    front = rear = 0;

    // clear visited and parent arrays
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {

            visited[r][c] = 0;

            // (-1, -1) means no parent assigned yet
            parent[r][c] = (Node){-1, -1};
        }
    }

    // push starting cell into container
    container[rear++] = (Node){startRow, startCol};

    // mark start as visited
    visited[startRow][startCol] = 1;
}

// This function performs ONE step of search
// It is called repeatedly to create animation
int stepSearch(int endRow, int endCol, int maze[ROWS][COLS], int useDFS)
{

    // if nothing left to explore → stop
    if (front >= rear)
        return 0;

    Node curr;

    // choose behavior based on algorithm
    if (useDFS)
    {
        // DFS → remove from end (stack behavior)
        curr = container[--rear];
    }
    else
    {
        // BFS → remove from front (queue behavior)
        curr = container[front++];
    }

    // check if we reached destination
    if (curr.row == endRow && curr.col == endCol)
    {
        return 2;
    }

    // explore all 4 neighboring cells
    for (int i = 0; i < 4; i++)
    {

        int nr = curr.row + dRow[i];
        int nc = curr.col + dCol[i];

        // check if neighbor is inside grid
        if (nr >= 0 && nc >= 0 && nr < ROWS && nc < COLS)
        {

            // visit only if:
            // - not already visited
            // - not a wall
            if (!visited[nr][nc] && maze[nr][nc] == 0)
            {

                visited[nr][nc] = 1;

                // store where we came from
                parent[nr][nc] = curr;

                // add to container for future exploration
                container[rear++] = (Node){nr, nc};
            }
        }
    }

    // search still ongoing
    return 1;
}