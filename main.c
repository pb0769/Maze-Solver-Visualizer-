#include "raylib.h"
#include "maze.h"
#include "solver.h"
#include <time.h>   
#include <stdio.h>  
#include <stdlib.h> 

#define CELL_SIZE 30
#define TOP_MARGIN 60

// This structure represents a clickable button on the screen with a rectangle and label
typedef struct
{
    Rectangle box;
    const char *label;
} Button;

// These variables store the coordinates of start and end positions in the maze grid
int start_r = -1, start_c = -1;
int end_r = -1, end_c = -1;

// These flags control the execution state of the algorithm and drawing logic
int isRunning = 0;
int pathFound = 0;
int pathDrawn = 0;

// Control which algorithm is used and how fast animation runs
int useDFS = 0;
int delay = 5;

// Stores length of final path and status message shown on screen
int pathLen = 0;
char statusMsg[100] = "";

/*
This function is responsible for saving the computed path into a text file.
It creates a folder named "output" and stores each result with a timestamp
so that previous results are not overwritten.
*/
void saveSolutionToFile()
{
    system("mkdir output > nul 2>&1");

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    char filename[100];

    strftime(filename, sizeof(filename),
             "output\\solution_%Y%m%d_%H%M%S.txt",
             tm_info);

    FILE *fp = fopen(filename, "w");

    if (fp == NULL)
    {
        printf("Error creating output file\n");
        return;
    }

    fprintf(fp, "Maze Solver Output\n");
    fprintf(fp, "-------------------------\n");
    fprintf(fp, "Algorithm: %s\n", useDFS ? "DFS" : "BFS");
    fprintf(fp, "Start: (%d, %d)\n", start_r, start_c);
    fprintf(fp, "End: (%d, %d)\n", end_r, end_c);
    fprintf(fp, "Path Length: %d\n\n", pathLen);
    fprintf(fp, "Path (from end to start):\n");

    Node cur = {end_r, end_c};

    while (!(cur.row == start_r && cur.col == start_c))
    {
        fprintf(fp, "(%d, %d)\n", cur.row, cur.col);
        cur = parent[cur.row][cur.col];
    }

    fprintf(fp, "(%d, %d)\n", start_r, start_c);

    fclose(fp);

    printf("Solution saved successfully: %s\n", filename);
    sprintf(statusMsg, "Solution saved!");
}

int main()
{
    InitWindow(COLS * CELL_SIZE, ROWS * CELL_SIZE + TOP_MARGIN, "Maze Solver");
    SetTargetFPS(60);

    srand(time(NULL));
    initMaze();

    // Define all buttons that appear at the top UI panel
    Button startBtn = {{10, 10, 80, 30}, "Start"};
    Button resetBtn = {{100, 10, 80, 30}, "Reset"};
    Button randomBtn = {{190, 10, 90, 30}, "Random"};
    Button modeBtn = {{290, 10, 80, 30}, "BFS"};
    Button plusBtn = {{380, 10, 40, 30}, "+"};
    Button minusBtn = {{430, 10, 40, 30}, "-"};

    int frame = 0;

    while (!WindowShouldClose())
    {
        frame++;

        Vector2 mouse = GetMousePosition();

        int col = mouse.x / CELL_SIZE;
        int row = (mouse.y - TOP_MARGIN) / CELL_SIZE;

        // This block allows user to draw or erase walls using mouse clicks inside the grid
        if (!isRunning && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouse.y > TOP_MARGIN)
        {
            if (row >= 0 && row < ROWS && col >= 0 && col < COLS)
            {
                maze[row][col] = !maze[row][col];
            }
            statusMsg[0] = '\0';
        }

        // Handle all button clicks in one place for better control
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            // Start button logic handles all validation before running algorithm
            if (CheckCollisionPointRec(mouse, startBtn.box) && !isRunning)
            {
                if (start_r == -1 || end_r == -1)
                {
                    sprintf(statusMsg, "Set start & end first!");
                }
                else if (start_r == end_r && start_c == end_c)
                {
                    sprintf(statusMsg, "Start = End not allowed!");
                }
                else if (maze[start_r][start_c] == 1 || maze[end_r][end_c] == 1)
                {
                    sprintf(statusMsg, "Start/End blocked!");
                }
                else
                {
                    initSearch(start_r, start_c);

                    // Clear previous visited states so old paths don’t remain visible
                    for (int i = 0; i < ROWS; i++)
                        for (int j = 0; j < COLS; j++)
                            visited[i][j] = 0;

                    // Reset parent tracking to avoid incorrect backtracking
                    for (int i = 0; i < ROWS; i++)
                        for (int j = 0; j < COLS; j++)
                        {
                            parent[i][j].row = -1;
                            parent[i][j].col = -1;
                        }

                    isRunning = 1;
                    pathFound = 0;
                    pathDrawn = 0;
                    pathLen = 0;

                    statusMsg[0] = '\0';
                }
            }

            // Reset button clears everything and brings maze back to initial state
            if (CheckCollisionPointRec(mouse, resetBtn.box))
            {
                initMaze();

                start_r = start_c = -1;
                end_r = end_c = -1;

                isRunning = pathFound = pathDrawn = 0;
                pathLen = 0;

                for (int i = 0; i < ROWS; i++)
                    for (int j = 0; j < COLS; j++)
                        visited[i][j] = 0;

                for (int i = 0; i < ROWS; i++)
                    for (int j = 0; j < COLS; j++)
                    {
                        parent[i][j].row = -1;
                        parent[i][j].col = -1;
                    }

                statusMsg[0] = '\0';
            }

            // Random button generates a fresh maze and resets everything
            if (CheckCollisionPointRec(mouse, randomBtn.box))
            {
                randomMaze();

                start_r = start_c = -1;
                end_r = end_c = -1;

                isRunning = pathFound = pathDrawn = 0;
                pathLen = 0;

                for (int i = 0; i < ROWS; i++)
                    for (int j = 0; j < COLS; j++)
                        visited[i][j] = 0;

                statusMsg[0] = '\0';
            }

            // Toggle between BFS and DFS
            if (CheckCollisionPointRec(mouse, modeBtn.box))
            {
                useDFS = !useDFS;
                statusMsg[0] = '\0';
            }

            // Adjust animation speed
            if (CheckCollisionPointRec(mouse, plusBtn.box) && delay > 1)
                delay--;

            if (CheckCollisionPointRec(mouse, minusBtn.box) && delay < 50)
                delay++;
        }

        // Allow user to set start and end points using keyboard keys
        if (row >= 0 && row < ROWS && col >= 0 && col < COLS)
        {
            if (!isRunning && IsKeyPressed(KEY_S) && maze[row][col] == 0)
            {
                start_r = row;
                start_c = col;
            }

            if (!isRunning && IsKeyPressed(KEY_E) && maze[row][col] == 0)
            {
                end_r = row;
                end_c = col;
            }
        }

        // Execute algorithm step-by-step based on frame delay
        if (isRunning && frame % delay == 0)
        {
            int state = stepSearch(end_r, end_c, maze, useDFS);

            if (state == 2)
            {
                isRunning = 0;
                pathFound = 1;
            }
            else if (state == 0)
            {
                isRunning = 0;
                sprintf(statusMsg, "No path found!");
            }
        }

        // Once path is found, reconstruct it safely
        if (pathFound && !pathDrawn)
        {
            Node cur = {end_r, end_c};
            pathLen = 0;

            while (!(cur.row == start_r && cur.col == start_c))
            {
                if (cur.row == -1 || cur.col == -1)
                {
                    sprintf(statusMsg, "Path error!");
                    break;
                }

                visited[cur.row][cur.col] = 2;
                cur = parent[cur.row][cur.col];
                pathLen++;
            }

            visited[start_r][start_c] = 2;

            pathDrawn = 1;
            saveSolutionToFile();
        }

        // ---------------- DRAWING ----------------
        BeginDrawing();
        ClearBackground(RAYWHITE);

        modeBtn.label = useDFS ? "DFS" : "BFS";

        Button buttons[] = {startBtn, resetBtn, randomBtn, modeBtn, plusBtn, minusBtn};

        for (int i = 0; i < 6; i++)
        {
            Color color = CheckCollisionPointRec(mouse, buttons[i].box) ? GRAY : LIGHTGRAY;

            DrawRectangleRec(buttons[i].box, color);
            DrawRectangleLinesEx(buttons[i].box, 1, BLACK);
            DrawText(buttons[i].label,
                     buttons[i].box.x + 10,
                     buttons[i].box.y + 8,
                     12,
                     BLACK);
        }

        DrawText("Click: Wall | S: Start | E: End", 10, 45, 15, DARKGRAY);
        DrawText(TextFormat("Speed: %d", delay), 500, 10, 15, DARKGRAY);
        DrawText(TextFormat("Path: %d", pathLen), 500, 30, 15, DARKGRAY);
        DrawText(statusMsg, 200, 20, 20, GREEN);

        // Draw maze grid with different colors for walls, path, visited nodes
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                int y = i * CELL_SIZE + TOP_MARGIN;

                if (maze[i][j] == 1)
                    DrawRectangle(j * CELL_SIZE, y, CELL_SIZE, CELL_SIZE, BLACK);
                else if (i == start_r && j == start_c)
                    DrawRectangle(j * CELL_SIZE, y, CELL_SIZE, CELL_SIZE, ORANGE);
                else if (i == end_r && j == end_c)
                    DrawRectangle(j * CELL_SIZE, y, CELL_SIZE, CELL_SIZE, RED);
                else if (visited[i][j] == 2)
                    DrawRectangle(j * CELL_SIZE, y, CELL_SIZE, CELL_SIZE, GREEN);
                else if (visited[i][j] == 1)
                    DrawRectangle(j * CELL_SIZE, y, CELL_SIZE, CELL_SIZE, BLUE);
                else
                    DrawRectangleLines(j * CELL_SIZE, y, CELL_SIZE, CELL_SIZE, GRAY);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}