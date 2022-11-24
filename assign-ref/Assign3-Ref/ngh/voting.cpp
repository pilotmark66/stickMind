/**
 * File: voting.cpp
 * ----------------
 * 此文件为 CS106B 作业3.2的第三题。
 * 解决了计算大选中区块的Banzhaf影响力指数的问题。
 */

#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "map.h"
#include "set.h"
#include "voting.h"
#include "testing/SimpleTest.h"
using namespace std;



// 将提供向量中的元素值求总和 int
int sumOf(const Vector<int>& vec) {
    int total = 0;
    for (int v : vec) {
        total += v;
    }
    return total;
}

// 将提供向量中的元素值求总和 unsigned long long int
unsigned long long int sumOf(const Vector<unsigned long long int>& vec) {
    unsigned long long int total = 0;
    for (unsigned long long int v : vec) {
        total += v;
    }
    return total;
}

// 将提供向量中的元素值相对于所有元素值之和进行百分比转换
Vector<int> percentConversion(const Vector<unsigned long long int>& keyBlocks) {
    Vector<int> result;

    for (int i = 0; i < keyBlocks.size(); i++) {
        int n = (long double)(keyBlocks[i]) / sumOf(keyBlocks) * 100;
        result.add(n);
    }

    return result;
}



// computePowerIndexes的辅助函数
// 找出不包括当前地区的所有组合（子集）并判断该地区是否为该组合的关键票。
// 最终返回当前地区的关键票总数。

unsigned long long int computePowerIndexesHelper(
                              const Vector<int>& blocks,
                              const int& winVotes,
                              const int& targetBlock,
                              unsigned long long int soFarVotes,
                              unsigned long long int index,
                              const int& blocksSize,
                              Map<Vector<unsigned long long int>, unsigned long long int>& calculatedSubsets
                              ) {


    // 不加当前地区的票数已经胜选，则当前地区肯定不是关键区，该二叉树节点向下所有节点均不是关键区，不需要再继续了
    if (soFarVotes >= winVotes)
        return 0;
    if(calculatedSubsets.containsKey({soFarVotes, index}))
        return calculatedSubsets.get({soFarVotes, index});


    if (index == blocks.size()) {

        // 当组合总票数未超过总票数的一半 且 加上当前地区的票就能超一半，说明当前地区是关键区，返回 1。
        if (soFarVotes < winVotes && soFarVotes + targetBlock >= winVotes)
            return 1;

    } else {

        unsigned long long int n1 = computePowerIndexesHelper(blocks, winVotes, targetBlock, soFarVotes + blocks[index], index + 1, blocksSize, calculatedSubsets); // 包含的情况

        unsigned long long int n2 = computePowerIndexesHelper(blocks, winVotes, targetBlock, soFarVotes, index + 1, blocksSize, calculatedSubsets); // 不包含的情况

        unsigned long long int total = n1 + n2;

        calculatedSubsets.put({soFarVotes, index}, total);

        //if (index == 0)
        //    cout << "Map Size: " << calculatedSubsets.size() << endl;

        return total;
    }
    return 0;
}

/*
 * 此函数计算区块的Banzhaf影响力指数。
 * 接收包括区块票数的向量作为参数，
 * 返回包含各个区块Banzhaf影响力指数的向量。
*/
Vector<int> computePowerIndexes(Vector<int>& blocks){
    // 计算胜选票数
    int winVotes = 0;
    int totalVotesBlocks = sumOf(blocks);
    winVotes = (totalVotesBlocks / 2) + 1;


    Vector<unsigned long long int> keyBlocks;

    Map<unsigned long long int, unsigned long long int> alreadyCountedBlocks;

    int blocksSize = blocks.size();
    for (int i = 0; i < blocksSize; i++) {
        Map<Vector<unsigned long long int>, unsigned long long int> calculatedSubsets;
        int block = blocks[i];
        if (!alreadyCountedBlocks.containsKey(block)) {
            Vector<int> newBlocks = blocks;
            newBlocks.remove(i);
            unsigned long long int keyBlock = computePowerIndexesHelper(newBlocks, winVotes, block, 0, 0, blocksSize - 1, calculatedSubsets);
            keyBlocks.add(keyBlock);
            alreadyCountedBlocks.put(block, keyBlock);
        } else {
            keyBlocks.add(alreadyCountedBlocks.get(block));
        }
    }

    //cout << endl << keyBlocks << endl;
    Vector<int> result = percentConversion(keyBlocks);
    cout << endl << result << endl;
    //return percentConversion(keyBlocks);
    return result;
}


/* * * * * * Test Cases * * * * * */
/*
 * */

PROVIDED_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int i = 0; i < 85; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}


STUDENT_TEST("Test percentConversion, keyBlocks 3-1-1") {
    Vector<unsigned long long int> keyBlocks = {3, 1, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(percentConversion(keyBlocks), expected);
}

STUDENT_TEST("Test percentConversion, keyBlocks 2-3-1-5-4") {
    Vector<unsigned long long int> keyBlocks = {2, 3, 1, 5, 4};
    Vector<int> expected = {13, 20, 6, 33, 26};
    EXPECT_EQUAL(percentConversion(keyBlocks), expected);
}

// int长度版本
/*
STUDENT_TEST("Test percentConversion, keyBlocks 3-1-1") {
    Vector<int> keyBlocks = {3, 1, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(percentConversion(keyBlocks), expected);
}

STUDENT_TEST("Test percentConversion, keyBlocks 2-3-1-5-4") {
    Vector<int> keyBlocks = {2, 3, 1, 5, 4};
    Vector<int> expected = {13, 20, 6, 33, 26};
    EXPECT_EQUAL(percentConversion(keyBlocks), expected);
}
*/

STUDENT_TEST("Test power index, blocks 50-49-1") {
    Vector<int> blocks = {33, 29, 29, 27};
    Vector<int> expected = {50, 16, 16, 16};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

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

STUDENT_TEST("Time power index operation, blocks EU post-Nice") {
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}
PROVIDED_TEST("Test power index, blocks EU post-Nice") {
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

STUDENT_TEST("Time power index operation, United States Electoral College Votes by State") {
    Vector<int> blocks = { 9, 3,11, 6,55, 9, 7, 3, 3,29,16,
                           4, 4,20,11, 6, 6, 8, 8, 4,10,
                          11,16,10, 6,10, 3, 5, 6, 4,14,
                           5,29,15, 3,18, 7, 7,20, 4, 9,
                           3,11,38, 6, 3,13,12, 5,10, 3};
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}

STUDENT_TEST("Time power index operation, United States Electoral College Votes by State, blocks.sort()") {
    Vector<int> blocks = { 9, 3,11, 6,55, 9, 7, 3, 3,29,16,
                           4, 4,20,11, 6, 6, 8, 8, 4,10,
                          11,16,10, 6,10, 3, 5, 6, 4,14,
                           5,29,15, 3,18, 7, 7,20, 4, 9,
                           3,11,38, 6, 3,13,12, 5,10, 3};
    blocks.sort();
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}

STUDENT_TEST("Time power index operation, Same number of votes per block") {
    Vector<int> blocks;
    for (int i = 0; i < 60; i++) {
        blocks.add(1);
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}

STUDENT_TEST("Time power index operation, different number of votes per block") {
    Vector<int> blocks;
    for (int i = 0; i < 60; i++) {
        blocks.add(i + 1);
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}



PROVIDED_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int i = 0; i < 85; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}

STUDENT_TEST("Time power index operation") {
    for (int k = 50; k <= 100; k+=5) {
        Vector<int> blocks;
        for (int i = 0; i < k; i++) {
            blocks.add(randomInteger(1, 10));
        }
        TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
    }

}

