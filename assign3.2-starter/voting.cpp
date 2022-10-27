// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "voting.h"
#include "testing/SimpleTest.h"
using namespace std;


// TODO: 采用 "exclude/include" 模式求解子集问题
// behavior of the function and how you implemented this behavior

int computePowerIndexesHelper(Vector<int>& blocks, int totalVotes, int index, int curBlock, int coalitionSum) {
    //base case
    if (index == curBlock) {         //联盟对象为自己时，直接跳过
        index++;
    }
    if(coalitionSum > totalVotes / 2) {  //当前联盟的总票数(没有curBlock的联盟)>半数票数时，说明curBlock选区必不是关键票
        return 0;
    }
    if (index == blocks.size()) {       
        if (coalitionSum <= totalVotes / 2 && coalitionSum + blocks[curBlock] > totalVotes / 2) {    //当前联盟的总票数>半数票数时，并且缺少curBlock就会少于半数票数
            return 1;                               //可以认为curBlock为关键票
        } else {
            return 0;
        }
    }
        //include case
    int inCase = computePowerIndexesHelper(blocks, totalVotes, index + 1, curBlock, coalitionSum + blocks[index]);
        //exclude case
    int exCase = computePowerIndexesHelper(blocks, totalVotes, index + 1, curBlock, coalitionSum);


    return inCase + exCase;
}




// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior

Vector<int> computePowerIndexes(Vector<int>& blocks)
{
    Vector<int> result(blocks.size(), 0);
    int totalVotes = 0;
    for (auto value : blocks) {
        totalVotes += value;
    }
    for (int i = 0; i < blocks.size(); i++) {
        int powerIndex = computePowerIndexesHelper(blocks, totalVotes, 0, i, 0);  //遍历每一个区块，计算每一个区块的关键指数
        result[i] = powerIndex;
    }

    // 关键指数百分比化
    int sumOfIndex = 0;
    for (auto index : result) {
        sumOfIndex += index;
    }
    for (int i = 0; i < result.size(); i++) {
        result[i] = 100 * result[i] / sumOfIndex;
    }
    return result;
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Test power index, blocks 50-49-1") {
    Vector<int> blocks = {50, 49, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks Hempshead 1-1-3-7-9-9") {
    Vector<int> blocks = {1, 1, 3, 7, 9, 9};
    Vector<int> expected = {0, 0, 0, 33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-NY 55-38-39") {
    Vector<int> blocks = {55, 38, 29};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-GA 55-38-16") {
    Vector<int> blocks = {55, 38, 16};
    Vector<int> expected = {100, 0, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks EU post-Nice") {
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int i = 0; i < 15; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}

STUDENT_TEST("Test power index, blocks 49-48-2-1") {
    Vector<int> blocks = {49, 48, 2, 1};
    Vector<int> expected = {41, 25, 25, 8};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}
