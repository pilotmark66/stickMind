#include "labyrinth.h"
#include "demo/MazeGenerator.h"
#include "testing/SimpleTest.h"
#include "error.h"
#include "set.h"
using namespace std;

/* Change kYourName constant to your full name.
 *
 * WARNING: Once you've set set this constant and started exploring your labyrinth,
 * do NOT edit the value of kYourName. Changing kYourName will change which
 * maze you get back, which might invalidate all your hard work!
 */
const string kYourName = "Jzx";

/* Change this constant to the path out of your labyrinth. */
const string kPathOutOfNormalMaze = "EESSSWENNWWWSSEWNEE";

/**
 * Given a location in a maze, returns whether the given sequence of
 * steps will let you escape the maze.
 *
 * To escape the maze, you need to collect all items in the `needs` set.
 * You can only take steps in the four cardinal directions,
 * and you can't move in directions that don't exist in the maze.
 */
bool isPathToFreedom(MazeCell* start, string moves, Set<string> needs) {
    Set<string> getTool = { };
    //检验起点位置是否有道具，并获取道具
    if (start->contents != ""){
        getTool.add(start->contents);
        if (needs.equals(getTool)) {
            return true;
        }
    }
    string direction = "NSEW";
    for (char  move : moves) {
        /* 判断路径是否包含无效字符 */
      if (!stringContains(direction, move)) {
        error("The input path is invalid.");
      }


      if (move == 'N') {
          //判断下一步访问路径是否合法
          if (start->north == nullptr) {
              return false;
          } else {
              start = start->north;
          }
          if (start->contents != ""){
              getTool.add(start->contents);
          }
      }
      if (move == 'S') {

          if (start->south == nullptr) {
              return false;
          } else {
              start = start->south;
          }
          if (start->contents != ""){
              getTool.add(start->contents);
          }

      }
      if (move == 'E') {

          if (start->east == nullptr) {
              return false;
          } else {
              start = start->east;
          }
          if (start->contents != ""){
              getTool.add(start->contents);
          }

      }
      if (move == 'W') {

          if (start->west == nullptr) {
              return false;
          } else {
              start = start->west;
          }
          if (start->contents != ""){
              getTool.add(start->contents);
          }
      }
    }



    if (needs.equals(getTool)) {
        return true;
    } else {
        return false;
    }

}


/* * * * * * Test Cases Below This Point * * * * * */

//PROVIDED_TEST("Check paths in the sample from writeup") {
//    Set<string> allThree = {"Spellbook", "Potion", "Wand"};
//    auto maze = toMaze({"* *-W *",
//                        "| |   |",
//                        "*-* * *",
//                        "  | | |",
//                        "S *-*-*",
//                        "|   | |",
//                        "*-*-* P"});

//    /* These paths are the ones in the handout. They all work. */
//    EXPECT(isPathToFreedom(maze[2][2], "ESNWWNNEWSSESWWN", allThree));
//    EXPECT(isPathToFreedom(maze[2][2], "SWWNSEENWNNEWSSEES", allThree));
//    EXPECT(isPathToFreedom(maze[2][2], "WNNEWSSESWWNSEENES", allThree));

//    /* This path works to pick up just potion */
//    EXPECT(isPathToFreedom(maze[2][2], "ESNW", {"Potion"}));

//    /* This path doesn't work, since doesn't pick up all items. */
//    EXPECT(!isPathToFreedom(maze[2][2], "ESNW", allThree));

//    /* This path doesn't work, cannot walk through walls. */
//    EXPECT(!isPathToFreedom(maze[2][2], "NNWWSSSEEE", allThree));
//}

PROVIDED_TEST("Works when start on an item") {
    auto maze = toMaze({"P-S-W"});

    EXPECT(isPathToFreedom(maze[0][0], "E", {"Potion"}));
    EXPECT(isPathToFreedom(maze[0][1], "W", {"Potion", "Spellbook"}));
}

PROVIDED_TEST("Reports errors if given illegal characters") {
    auto maze = toMaze({"P-S-W"});

    EXPECT_ERROR(isPathToFreedom(maze[0][0], "Q", {"Wand"}));
    EXPECT_ERROR(isPathToFreedom(maze[0][0], "Ee", {"Wand"}));
}

PROVIDED_TEST("Escape from your personal labyrinth!") {
    Set<string> allThree = {"Spellbook", "Potion", "Wand"};
    /* A maze for you to escape from. This maze will be personalized
     * based on the constant kYourName.
     */
    EXPECT(kYourName == "Jzx");
    MazeCell* startLocation = mazeFor(kYourName);

    /* Set a breakpoint here. As a reminder, the labyrinth you
     * get will be personalized to you, so don't start exploring the labyrinth
     * unless you've edited the constant kYourName to include your name(s)!
     * Otherwise, you'll be escaping the wrong labyrinth.
     */
    EXPECT(kPathOutOfNormalMaze != "");
    EXPECT(isPathToFreedom(startLocation, kPathOutOfNormalMaze, allThree));
}

