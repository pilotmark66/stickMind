#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;
const int NONE = -1; // used as sentinel index

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
PQHeap::PQHeap() {
    _numAllocated = INITIAL_CAPACITY;
    _elements = new DataPoint[_numAllocated](); // allocated zero'd memory
    _numFilled = 0;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
PQHeap::~PQHeap() {
    delete[] _elements;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
void PQHeap::enqueue(DataPoint elem) {
    //没有足够空间
    if(_numFilled == _numAllocated){
        //扩容
        _numAllocated *= 2;
        //备份数据
        DataPoint* temp = _elements;
        //转移数据
        _elements = new DataPoint[_numAllocated]();
        for (int i = 0; i < _numFilled; i++) {
            _elements[i] = temp[i];
        }
        //删除临时数组
        delete[] temp;
        }

        //插入元素
        int index = _numFilled;
        _elements[_numFilled] = elem;
        //元素排序
        BubbleUp(_numFilled);
        //数组大小计数增加
        _numFilled++;



}

//对入堆的元素进行排序
void PQHeap::BubbleUp(int index){
    int parentIndex = getParentIndex(index);
    if(parentIndex != NONE) {
        //base case parentindex为NONE
        if(_elements[parentIndex].priority > _elements[index].priority) {
            swap(parentIndex, index);
            BubbleUp(parentIndex);
        }
    }

}


/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
DataPoint PQHeap::peek() const {
    if(isEmpty()){
            error("PQHeap is empty!");
        }
    //返回第一个元素，并不出堆
        return _elements[0];
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
DataPoint PQHeap::dequeue() {
    DataPoint front = peek();
    swap(0, _numFilled - 1);
    _numFilled--;
    BubbleDown(0);

    return front;
}

//对出堆后堆元素重新排序
void PQHeap::BubbleDown(int index) {

    int left = getLeftChildIndex(index);
    int right = getRightChildIndex(index);
    if(left != NONE) {
        // base case没有子节点
        int smallChild = left;
        //当有两个子节点时，找出最小子节点
        if (right != NONE && _elements[left].priority > _elements[right].priority) {
                    smallChild = right;
                }
        if (_elements[index].priority > _elements[smallChild].priority) {
            swap(index, smallChild);
            BubbleDown(smallChild);
        }

    }

}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
bool PQHeap::isEmpty() const {

    return size() == 0;

}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
int PQHeap::size() const {
    return _numFilled;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
void PQHeap::clear() {
    _numFilled = 0;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
void PQHeap::printDebugInfo(string msg) const {
    cout << msg << endl;
    for (int i = 0; i < _numFilled; i++) {
        cout << "[" << i << "]=" <<  _elements[i] << endl;
    }
}

/*
 * Private member function. This helper exchanges the element at
 * indexA with the element at indexB.
 */
void PQHeap::swap(int indexA, int indexB) {
    DataPoint temp = _elements[indexA];
    _elements[indexA] = _elements[indexB];
    _elements[indexB] = temp;
}

/*
 * We strongly recommend implementing this helper function, which
 * should traverse the heap array and ensure that the heap property
 * holds for all elements in the array. If elements are found that
 * violate the heap property, an error should be thrown.
 */
void PQHeap::validateInternalState() const {
    /* TODO: Implement this function. */
    if (_numFilled > _numAllocated) {
        error ("the elements are too much");
    }
    for (int i = 0; i < _numFilled; i++) {
        int RightChild = getRightChildIndex(i);
        int LeftChild = getLeftChildIndex(i);
        //没有子节点
        if (LeftChild == NONE) {
            return;
        }
        //仅有左子节点
        if (RightChild == NONE) {
            if (_elements[LeftChild].priority < _elements[i].priority) {
                error("The left child's priority is less(higher) than parent's");
            }
        } else {
            //有两个子节点
            if (_elements[RightChild].priority < _elements[i].priority) {
                error("The right child's priority is less(higher) than parent's");
            } else if (_elements[LeftChild].priority < _elements[i].priority) {
                error("The left child's priority is less(higher) than parent's");
            }

        }

    }
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the parent of the
 * specified child index. If this child has no parent, return
 * the sentinel value NONE.
 */
int PQHeap::getParentIndex(int child) const {
    if (child == 0) {
            return NONE;
        }
        return (child - 1) / 2;
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the left child of the
 * specified parent index. If this parent has no left child, return
 * the sentinel value NONE.
 */
int PQHeap::getLeftChildIndex(int parent) const {
    int LeftChildIndex = 2 * parent + 1;
    if (LeftChildIndex >= _numFilled) {
        return NONE;
    }
    return LeftChildIndex;

}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the right child of the
 * specified parent index. If this parent has no right child, return
 * the sentinel value NONE.
 */
int PQHeap::getRightChildIndex(int parent) const {
    int RightChildIndex = 2 * parent + 2;
    if (RightChildIndex >= _numFilled) {
        return NONE;
    }
    return RightChildIndex;
}

/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Add your own custom tests here! */

STUDENT_TEST("PQHeap: operations size/isEmpty/clear") {
    PQHeap pq;

    EXPECT(pq.isEmpty());
    pq.clear();
    EXPECT_EQUAL(pq.isEmpty(), pq.size() == 0);
    pq.enqueue({ "", 7 });
    EXPECT_EQUAL(pq.size(), 1);
    pq.enqueue({ "", 5 });
    EXPECT_EQUAL(pq.size(), 2);
    pq.enqueue({ "", 5 });
    EXPECT_EQUAL(pq.size(), 3);
    pq.validateInternalState();
    pq.clear();
    pq.validateInternalState();
    EXPECT(pq.isEmpty());
    EXPECT_EQUAL(pq.size(), 0);
}


STUDENT_TEST("PQHeap: test enlarge array memory") {
    for (int size = 5; size <= 500; size *= 5) {
        PQHeap pq;

        for (int i = 1; i <= size; i++) {
            pq.enqueue({"", double(i) });
        }
        pq.validateInternalState();

        for (int i = 1; i <= size; i++) {
            DataPoint expected = {"", double(i) };
            EXPECT_EQUAL(pq.dequeue(), expected);
        }
    }

}


STUDENT_TEST("PQHeap stress test, cycle many random elements in and out") {
    PQHeap pq;
    int n = 0, maxEnqueues = 1000;
    double sumEnqueued = 0, sumDequeued = 0;

    setRandomSeed(42); // make test behavior deterministic

    DataPoint mostUrgent = { "", 0 };
    pq.enqueue(mostUrgent);
    while (true) {
        if (++n < maxEnqueues && randomChance(0.9)) {
            DataPoint elem = { "", randomInteger(-10, 10) + 0.5 };
            if (elem.priority < mostUrgent.priority) {
                mostUrgent = elem;
            }
            sumEnqueued += elem.priority;
            pq.enqueue(elem);
        } else {
            DataPoint elem = pq.dequeue();
            sumDequeued += elem.priority;
            EXPECT_EQUAL(elem, mostUrgent);
            if (pq.isEmpty()) break;
            mostUrgent = pq.peek();
        }
    }
    EXPECT_EQUAL(sumEnqueued, sumDequeued);
}


void fillQueue(PQHeap& pq, int n) {
    pq.clear(); // start with empty queue
    for (int i = 0; i < n; i++) {
        pq.enqueue({ "", randomReal(0, 10) });
    }
}

void emptyQueue(PQHeap& pq, int n) {
    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
}

STUDENT_TEST("PQArray timing test, fillQueue and emptyQueue") {
    PQHeap pq;
    int n = 2000000;

    TIME_OPERATION(n, fillQueue(pq, n));
    TIME_OPERATION(n, emptyQueue(pq, n));
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQHeap example from writeup, validate each step") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();
    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }
    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
}
