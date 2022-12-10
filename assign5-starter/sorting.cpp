#include "listnode.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment about your implementation of the function.
 */

void addNode(ListNode*& list, ListNode*& node) {
    if (list == nullptr) {
        list = node;
        list->next = nullptr;
    } else {
        //每次在链表头部插入节点
        ListNode * temp = list;
        list = node;
        list->next = temp;
    }

}


void partition(ListNode*& list, ListNode*& little, ListNode *& pivot, ListNode*& bigger) {

    if (list == nullptr) {
        return;
    }
    //将list链表里的节点分配到三个链表中
    int pivotNum = list->data;
    while (list != nullptr) {
        ListNode * temp = list;
        list = list->next;
        if (pivotNum == temp->data) {
            addNode(pivot, temp);
        }
        if (pivotNum < temp->data) {
            addNode(bigger, temp);
        }
        if (pivotNum > temp->data) {
            addNode(little, temp);
        }

    }

}

void addList(ListNode*& list1, ListNode*& list2) {
    //当目标链表为空
    if (list1 == nullptr) {
        list1 = list2;
    } else {
        //遍历目标链表，并在尾部接上值更大的链表
        auto temp = list1;
        while (temp != nullptr) {
            if (temp->next == nullptr) {
               temp->next = list2;
               return;
            } else {
                temp = temp->next;
            }
        }

    }

}

void concatenate(ListNode*& front, ListNode*& little, ListNode *& pivot, ListNode*& bigger) {
    //链接链表，按从小到大的顺序
    addList(front, little);

    addList(front, pivot);

    addList(front, bigger);
}


void quickSort(ListNode*& front) {
    if (front == nullptr) {
        return;
    }
    /* 将链表节点分为3类 */
    //初始化3条子链表
    ListNode* littleNodes = nullptr;
    ListNode* pivotNodes = nullptr;
    ListNode* biggerNodes = nullptr;
    partition(front, littleNodes, pivotNodes, biggerNodes);
    //递归分类链表节点
    quickSort(littleNodes);
    quickSort(biggerNodes);
    //链接链表
    concatenate(front, littleNodes, pivotNodes, biggerNodes);

}


/* * * * * * Test Code Below This Point * * * * * */

/* TODO: Write your own student tests below to test your sorting helper
 * functions and overall sorting algorithm correctness. We have provided
 * the skeleton of some tests below to help you get started with this
 * process.
 */

/*
 * We have provided this utility function as a convenience. This
 * function traverses the linked list represented by the provided
 * front pointer and prints out the data stored in the list along
 * the way in a convenient format. You may find it to be helpful to
 * use this function as a debugging aid.
 */
void printList(ListNode* front) {
   cout << "{";
   for (ListNode *cur = front; cur != nullptr; cur = cur->next) {
       cout << cur->data;
       if (cur->next != nullptr){
           cout << ", ";
       }
   }
   cout << "}" << endl;
}

/*
 * This utility function deallocates the memory for all the nodes in a
 * given linked list. It can be used to recycle the memory allocated
 * during a test case and avoid memory leaks.
 */
void deallocateList(ListNode* front) {
    /* TODO: Implement this function. */
    while (front != nullptr) {
        ListNode* temp = front;
        front = front->next;
        delete temp;
    }

}

/*
 * This utility function is given a vector of values and constructs
 * an equivalent linked list containing the same values in the same order.
 * It can be used to construct a linked list to use as an input to
 * a function being tested.
 */
ListNode* createList(Vector<int> values){
    //创建头节点
    ListNode* node = nullptr;

    for (int value : values) {
        //创建第一个点
        if (node == nullptr) {
            node = new ListNode(value, nullptr);
        } else {
            //后续节点需要通过一个一直指向末位的游离指针来创建
            ListNode* temp = node;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = new ListNode(value, nullptr);
        }

    }
    return node;
}

/*
 * This utility function compares a linked list and a vector for
 * equivalence and returns true if both contain the same values in the
 * same order. It can be used to confirm a linked list's contents match
 * the expected.
 */
bool areEquivalent(ListNode* front, Vector<int> v){
    /* TODO: Implement this function. */
    ListNode* temp = front;
    for (int value : v) {
        //当输入链表为空,或者有值不相等
        if (temp == nullptr || temp->data != value) {
            return false;
        }
        temp = temp->next;
    }
    //当链表太长
    if (temp == nullptr) {
            return true;
        }
    return false;

}

STUDENT_TEST("This is a skeleton to demonstrate an end-to-end student test."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {1, 2, 3, 4};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

PROVIDED_TEST("This tests some of the testing helper functions you are recommended to implement."){
    /* Creates a small test list containing the values 1->2->3. */
    ListNode* testList = nullptr;
    testList = new ListNode(3, testList);
    testList = new ListNode(2, testList);
    testList = new ListNode(1, testList);

    /* Test equality comparison function. */
    EXPECT(areEquivalent(testList, {1, 2, 3}));
    EXPECT(!areEquivalent(testList, {1, 2}));
    EXPECT(!areEquivalent(testList, {1, 2, 3, 4}));
    EXPECT(!areEquivalent(testList, {1, 3, 2}));

    /* Test create list function. */
    ListNode* studentList = createList({1, 2, 3, 4});
    printList(studentList);
    ListNode* cur = studentList;
    for (int i = 1; i <= 4; i++){
        EXPECT(cur != nullptr);
        EXPECT_EQUAL(cur->data, i);
        cur = cur->next;
    }

    /* Test deallocate list functions. There should be no memory leaks from this test. */
    deallocateList(studentList);
    deallocateList(testList);
}

/*
 * The provided test case demonstrates a comparsion between the same
 * algorithm (quicksort) operating on a linked list versus a vector.
 */
PROVIDED_TEST("Time linked list quicksort vs vector quicksort") {
    int startSize = 50000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort());        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */

        deallocateList(list);
    }
}

