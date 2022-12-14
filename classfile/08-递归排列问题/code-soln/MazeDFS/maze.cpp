// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "set.h"
#include "queue.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;

#define dfs

#ifdef dfs
bool solveMazeRec(Grid<bool> &maze, Stack<GridLocation> &path, Set<GridLocation> &visited) {
    GridLocation mazeExit = {maze.numRows() - 1, maze.numCols() - 1};
    if (path.peek() == mazeExit) {
        return true;
    }
    Set<GridLocation> neighbors = generateValidMoves(maze, path.peek()).difference(visited);
    for (const auto &item : neighbors) {
        path.push(item);
        visited.add(item);
        MazeGraphics::highlightPath(path, "GREEN", 1);
        if (solveMazeRec(maze, path, visited))
            return true;
        path.pop();
        MazeGraphics::highlightPath(path, "GREEN", 1);
    }

    return false;
}

Stack<GridLocation>  solveMaze(Grid<bool> &maze) {
    MazeGraphics::drawGrid(maze);
    Stack<GridLocation> initPath = {{0, 0}};
    Set<GridLocation> visited = {{0, 0}};
    MazeGraphics::highlightPath(initPath, "GREEN", 1);

    if (solveMazeRec(maze, initPath, visited))
        return initPath;

    return {};
}
#endif

#ifdef bfs
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    GridLocation mazeExit = {maze.numRows() - 1, maze.numCols() - 1};
    Stack<GridLocation> initPath = {{0, 0}};
    Queue<Stack<GridLocation>> todo = {initPath};

    while (!todo.isEmpty()) {
        Stack<GridLocation> path = todo.dequeue();
        MazeGraphics::highlightPath(path, "GREEN", 100);
        if (path.peek() == mazeExit)
            return path;
        auto neighbers = generateValidMoves(maze, path.peek());
        for (const auto &pos : neighbers) {
            if (!gatherVisited(path).contains(pos)) {
                auto copyPath = path;
                copyPath.push(pos);
                todo.enqueue(copyPath);
            }
        }
    }

    return {};
}
#endif

PROVIDED_TEST("solveMaze testing") {
    Grid<bool> maze;
    readMazeFile("res/21x25.maze", maze);
    auto path = solveMaze(maze);
    cout << path << endl;
}

// ********************** ???????????????????????? ***************************

/**
 * @brief generateValidMoves - ??????????????????????????????????????????
 * @param maze
 * @param cur
 * @return ??????????????????
 */
Set<GridLocation> generateValidMoves(Grid<bool> &maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    /* TODO: Fill in the remainder of this function. */
    GridLocation next;

    next = GridLocation(cur.row - 1, cur.col);
    if (maze.inBounds(next) && maze[next]) // ???????????????????????????????????????
        neighbors.add(next);
    next = GridLocation(cur.row, cur.col - 1);
    if (maze.inBounds(next) && maze[next])
        neighbors.add(next);
    next = GridLocation(cur.row + 1, cur.col);
    if (maze.inBounds(next) && maze[next])
        neighbors.add(next);
    next = GridLocation(cur.row, cur.col + 1);
    if (maze.inBounds(next) && maze[next])
        neighbors.add(next);

    return neighbors;
}

/**
 * @brief gatherVisited - ????????????????????????????????????
 * @param path
 * @return ?????????????????????
 */
Set<GridLocation> gatherVisited(Stack<GridLocation> path) {
    Set<GridLocation> visited;
    while (!path.isEmpty()) {
        visited.add(path.pop());
    }
    return visited;
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}
