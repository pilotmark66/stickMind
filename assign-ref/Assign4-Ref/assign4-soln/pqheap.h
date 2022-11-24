#pragma once
#include "testing/MemoryUtils.h"
#include "datapoint.h"

/**
 * Priority queue of DataPoints implemented using a binary heap.
 */
class PQHeap {
public:
    /**
     * Creates a new, empty priority queue.
     */
    PQHeap();

    /**
     * Cleans up all memory allocated by this priority queue.
     */
    ~PQHeap();

    /**
     * Adds a new element into the queue. This operation runs in time O(log n),
     * where n is the number of elements in the queue.
     *
     * @param element The element to add.
     */
    void enqueue(DataPoint element);

    /**
     * Removes and returns the element that is frontmost in this priority queue.
     * The frontmost element is the one with the most urgent priority. A priority
     * of 1 is more urgent than priority 2 which is more urgent than priority 7
     * and so on. If the priority queue contains two or more elements of equal
     * priority, the order those elements are dequeued is arbitrary, i.e. there
     * is no required tie-break handling.
     *
     * If the priority queue is empty, this function calls error().
     *
     * This operation must run in time O(log n).
     *
     * @return The frontmost element, which is removed from queue.
     */
    DataPoint dequeue();

    /**
     * Returns, but does not remove, the element that is frontmost.
     *
     * If the priority queue is empty, this function calls error().
     *
     * This operation must run in time O(1).
     *
     * @return frontmost element
     */
    DataPoint peek() const;

    /**
     * Returns whether this priority queue is empty.
     *
     * This operation must run in time O(1).
     *
     * @return true if contains no elements, false otherwise.
     */
    bool isEmpty() const;

    /**
     * Returns the count of elements in this priority queue.
     *
     * This operation must run in time O(1).
     *
     * @return The count of elements in the priority queue.
     */
    int size() const;

    /**
     * Removes all elements from the priority queue.
     *
     * This operation must run in time O(1).
     */
    void clear();

    void printDebugInfo(std::string msg) const;

    void validateInternalState() const;
    void validateHelperTest();

private:
    DataPoint *_elements; // dynamic array
    int _numAllocated;    // number of slots allocated in array
    int _numFilled;       // number of slots filled in array

    void swap(int indexA, int indexB);
    void bubleDownItr(int parent);
    void bubleDownRec(int parent);
    void validateHelper(int parent) const;

    int getParentIndex(int child) const;
    int getLeftChildIndex(int parent) const;
    int getRightChildIndex(int parent) const;

    DISALLOW_COPYING_OF(PQHeap);
};
