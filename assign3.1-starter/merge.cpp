/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * TODO: 将两个从小到大的队列合并为一个新的从小到大的队列
 * header comment.
 */
Queue<int> binaryMerge(Queue<int> a, Queue<int> b) {
    Queue<int> result;
    /* TODO: Implement this function. */
    int min_a = INT_MIN;
    int min_b = INT_MIN;
    while (!a.isEmpty() && !b.isEmpty()) {
        if(a.peek() <= b.peek()) {            //比较a b第一个元素的大小，并将最小的取出放到新的队列
            if (min_a > a.peek()) {
                error("unsorted queue!");
            }
            min_a = a.peek();
            result.enqueue(a.dequeue());
        } else {
            if (min_b > b.peek()) {
                error("unsorted queue!");
            }
            min_b = b.peek();
            result.enqueue(b.dequeue());
        }
    }
    while (!a.isEmpty()) {
        if (min_a > a.peek()) {
            error("unsorted queue!");
        }
        min_a = a.peek();
        result.enqueue(a.dequeue());
    }
    while (!b.isEmpty()) {
        if (min_b > b.peek()) {
            error("unsorted queue!");
        }
        min_b = b.peek();
        result.enqueue(b.dequeue());
    }


    return result;
}

/*
 * This function assumes correct functionality of the previously
 * defined binaryMerge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented –
 * it does not need to modified at all.
 */
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = binaryMerge(q, result);
    }
    return result;
}

/*
 * TODO: 将序列递归分半，直到最简
 * header comment.
 */
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;
    /* TODO: Implement this function. */
    int k = all.size();
    if (k == 1) {
        return all.get(0);     //k==1时 序列化为最简
    } else if (k == 0){       //当序列本来就为空时
        return {};
    } else {
        Vector<Queue<int>> sublist1 = all.subList(0, k / 2);
        Vector<Queue<int>> sublist2 = all.subList(k / 2);
        return binaryMerge(recMultiMerge(sublist1), recMultiMerge(sublist2)); //将序列递归分半

    }

    return result;
}


/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

PROVIDED_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 1000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}


/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}


STUDENT_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}


STUDENT_TEST("binaryMerge, divide 0 ~ 9999 into two sorted sequences") {
    Queue<int> a, b, expected;
    for (int i = 0; i < 10000; i++) {
        if (i % 3 == 0) {
            a.enqueue(i);
        } else {
            b.enqueue(i);
        }
        expected.enqueue(i);
    }
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}


STUDENT_TEST("binaryMerge, two empty sequences") {
    Queue<int> a = {};
    Queue<int> b = {};
    Queue<int> expected = {};

    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

STUDENT_TEST("binaryMerge, unsorted sequences") {
    Queue<int> a = {3, 2, 1};
    Queue<int> b = {1, 2, 3};
    EXPECT_ERROR(binaryMerge(a, b));
    EXPECT_ERROR(binaryMerge(b, a));
}

STUDENT_TEST("Time naiveMultiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}


STUDENT_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}


STUDENT_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

STUDENT_TEST("recMultiMerge, empty vector and vector of empty queues") {
    Vector<Queue<int>> emptyQueue = {};
    Vector<Queue<int>> EmptyQueues(20);
    Queue<int> expected = {};
    EXPECT_EQUAL(recMultiMerge(emptyQueue), expected);
    EXPECT_EQUAL(recMultiMerge(EmptyQueues), expected);
}
