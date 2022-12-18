#include "bits.h"
#include "treenode.h"
#include "huffman.h"
#include "map.h"
#include "vector.h"
#include "priorityqueue.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
using namespace std;

/**
 * Given a Queue<Bit> containing the compressed message bits and the encoding tree
 * used to encode those bits, decode the bits back to the original message text.
 *
 * You can assume that tree is a well-formed non-empty encoding tree and
 * messageBits queue contains a valid sequence of encoded bits.
 *
 * Your implementation may change the messageBits queue however you like. There
 * are no requirements about what it should look like after this function
 * returns. The encoding tree should be unchanged.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits) {
    /* TODO: Implement this function. */
    EncodingTreeNode* temp = tree;
    int QueueSize = messageBits.size();
    string Text = "";
    Queue<Bit> tempBits = messageBits;
    //遍历解码队列
    for (int i = 0; i < QueueSize; i++ ) {
        Bit tempbit = tempBits.dequeue();
        if (!temp->isLeaf()) {
            if (tempbit == 0) {
                temp = temp->zero;
            } else if (tempbit == 1) {
                temp = temp->one;
            }

        }
        //每次找到一个叶节点，就回到树顶端
        if (temp->isLeaf()) {
            Text.append(charToString(temp->getChar()));
            temp = tree;
        }
    }
    return Text;
}

/**
 * Reconstruct encoding tree from flattened form Queue<Bit> and Queue<char>.
 *
 * You can assume that the queues are well-formed and represent
 * a valid encoding tree.
 *
 * Your implementation may change the queue parameters however you like. There
 * are no requirements about what they should look like after this function
 * returns.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodingTreeNode* unflattenTree(Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    /* TODO: Implement this function. */
    EncodingTreeNode* Tree = nullptr;
    while (!treeShape.isEmpty()) {
        //treeshape中0表示叶节点，1为根节点
        Bit treenodeBit = treeShape.dequeue();
        if (treenodeBit == 0) {
            //base case 节点为叶节点时，直接添加
            char ch = treeLeaves.dequeue();
            Tree = new EncodingTreeNode(ch);
        } else if (treenodeBit == 1) {
            //节点为内部节点时先创建两个分支
            EncodingTreeNode* zero  = nullptr;
            EncodingTreeNode* one = nullptr;
            Tree = new EncodingTreeNode(zero, one);
            //先递归左分支
            Tree->zero = unflattenTree(treeShape, treeLeaves);
            //再递归右分支
            Tree->one = unflattenTree(treeShape, treeLeaves);

        }
        //没有break会卡进某个递归分支
        break;

    }

    return Tree;
}

/**
 * Decompress the given EncodedData and return the original text.
 *
 * You can assume the input data is well-formed and was created by a correct
 * implementation of compress.
 *
 * Your implementation may change the data parameter however you like. There
 * are no requirements about what it should look like after this function
 * returns.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
string decompress(EncodedData& data) {
    /* TODO: Implement this function. */
    EncodingTreeNode* Tree = nullptr;
    //创建树
    Tree = unflattenTree(data.treeShape, data.treeLeaves);
    //解码得到文本
    string Text = decodeText(Tree, data.messageBits);
    //
    deallocateTree(Tree);
    return Text;
}

/**
 * Constructs an optimal Huffman coding tree for the given text, using
 * the algorithm described in lecture.
 *
 * Reports an error if the input text does not contain at least
 * two distinct characters.
 *
 * When assembling larger trees out of smaller ones, make sure to set the first
 * tree dequeued from the queue to be the zero subtree of the new tree and the
 * second tree as the one subtree.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodingTreeNode* buildHuffmanTree(string text) {
    /* TODO: Implement this function. */

    // 给字符串中的字符按照出现频次做映射
        Map<char, int> frequency;
        for(char ch: text){
            /*get and operator[] have a small but significant difference when
            used to retrieve the value for a key not contained in the map.
            Both expressions return the default value, but accessing
            map[key] adds this new entry to the map while map.get(key) does not.*/
            frequency[ch]++;
        }

        /* 将每个字符按照树节点的形式放入优先队列中（频率顺序） */
           PriorityQueue<EncodingTreeNode*> pqTreeNode;
           for (char ch : frequency) {
               EncodingTreeNode* tree = new EncodingTreeNode(ch);
               pqTreeNode.enqueue(tree, frequency.get(ch));
           }

           while (pqTreeNode.size() > 1) {
               //当队列中只剩一个成员是，树已创建完成，跳出（一定要注意边界）
               //左子节点出队列
               double zeroPiority = pqTreeNode.peekPriority();
               EncodingTreeNode* zero = pqTreeNode.dequeue();
               //右子节点出队列
               double onePiority = pqTreeNode.peekPriority();
               EncodingTreeNode* one = pqTreeNode.dequeue();
               //合并为根节点
               EncodingTreeNode* root = new EncodingTreeNode(zero, one);
               //合并后的节点与其有现金再进入优先队列
               pqTreeNode.enqueue(root, zeroPiority + onePiority);
           }


    return pqTreeNode.dequeue();
}

/**
 * Given a string and an encoding tree, encode the text using the tree
 * and return a Queue<Bit> of the encoded bit sequence.
 *
 * You can assume tree is a valid non-empty encoding tree and contains an
 * encoding for every character in the text.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */

//建立字符与比特位的对照表
void getCBitTable (EncodingTreeNode*& tree, Vector<Bit>& charBit, Map<char, Vector<Bit>>& CharBitTable) {

    //base case
    if (tree->isLeaf()) {
        CharBitTable.put(tree->getChar(), charBit);
        charBit.remove(charBit.size()-1);
        return;
    } else {
        //左递归➕0
        charBit.add(0);
        getCBitTable(tree->zero, charBit, CharBitTable);
        //右递归➕1
        charBit.add(1);
        getCBitTable(tree->one, charBit, CharBitTable);
        if (!charBit.isEmpty()) {
            charBit.remove(charBit.size()-1);
        }

    }

    //

}


Queue<Bit> encodeText(EncodingTreeNode* tree, string text) {
    /* TODO: Implement this function. */
    Queue<Bit> messageBits;
    Vector<Bit> charBit;
    Map<char, Vector<Bit>> CharBitTable;
    //字符与比特位的对照表
    getCBitTable (tree, charBit, CharBitTable);

    for (char ch : text) {
        if (CharBitTable.containsKey(ch)) {
            charBit = CharBitTable.get(ch);
            for (Bit singleBit : charBit) {
                messageBits.enqueue(singleBit);
            }

        }
    }

    return messageBits;
}



/**
 * Flatten the given tree into a Queue<Bit> and Queue<char> in the manner
 * specified in the assignment writeup.
 *
 * You can assume the input queues are empty on entry to this function.
 *
 * You can assume tree is a valid well-formed encoding tree.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
//先序遍历
void PreOrderTraversal (EncodingTreeNode* tree, Queue<Bit>& treeShape) {
    //base case
    if (tree->isLeaf()) {
        treeShape.enqueue(0);
        return;
    } else {
        treeShape.enqueue(1);
    }
    //left tree rec
    PreOrderTraversal(tree->zero, treeShape);
    //right tree rec
    PreOrderTraversal(tree->one, treeShape);

}

//中序遍历
void InOrderTraversal (EncodingTreeNode* tree, Queue<char>& treeLeaves) {
    //base case
    if (tree->isLeaf()) {
        treeLeaves.enqueue(tree->getChar());
        return;
    } else {
        //left tree rec
        InOrderTraversal(tree->zero, treeLeaves);
        //right tree rec
        InOrderTraversal(tree->one, treeLeaves);
    }

}



void flattenTree(EncodingTreeNode* tree, Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    /* TODO: Implement this function. */
    /* pre-order traversal to get Bit sequences */
    PreOrderTraversal(tree, treeShape);
    /* in-order traversal to get Char sequences */
    InOrderTraversal(tree, treeLeaves);
}

/**
 * Compress the input text using Huffman coding, producing as output
 * an EncodedData containing the encoded message and flattened
 * encoding tree used.
 *
 * Reports an error if the message text does not contain at least
 * two distinct characters.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodedData compress(string messageText) {
    /* TODO: Implement this function. */

    EncodingTreeNode* huffmanTree = buildHuffmanTree(messageText);

    EncodedData textData;

    flattenTree(huffmanTree, textData.treeShape, textData.treeLeaves);

    textData.messageBits = encodeText(huffmanTree, messageText);
    //删除树
    deallocateTree(huffmanTree);

    return textData;
}

/* * * * * * Testing Helper Functions Below This Point * * * * * */

EncodingTreeNode* createExampleTree() {
    /* Example encoding tree used in multiple test cases:
     *                *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */
    /* TODO: Implement this utility function needed for testing. */
    EncodingTreeNode * R = new EncodingTreeNode('R');
    EncodingTreeNode * S = new EncodingTreeNode('S');
    EncodingTreeNode * RS = new EncodingTreeNode(R, S);
    EncodingTreeNode * E = new EncodingTreeNode('E');
    EncodingTreeNode * RSE = new EncodingTreeNode(RS, E);
    EncodingTreeNode * T = new EncodingTreeNode('T');
    EncodingTreeNode * TRSE = new EncodingTreeNode(T, RSE);


    return TRSE;
}

void deallocateTree(EncodingTreeNode* t) {
    /* TODO: Implement this utility function needed for testing. */
    //base case
    while (t == nullptr) {
        return;
    }
    //递归左子树
    deallocateTree(t->zero);
    //递归右子树
    deallocateTree(t->one);
    delete t;

}

bool areEqual(EncodingTreeNode* a, EncodingTreeNode* b) {
    /* Huffman树的每个节点要么没有子节点，
     * 要么有两个子节点，不会出现只有一个子节点的情况 */
    //当一棵树无子节点，而另一棵树的节点有子节点时，二者不相等
    if ((a == nullptr && b != nullptr) ||
             (a!= nullptr && b== nullptr )  ) {
            return false;
        } else if (a == nullptr && b == nullptr ) {
            return true;
        }
    //base case
    if (a->isLeaf() && b->isLeaf()) {
        if (a->ch != b->ch) {
            return false;
        }
    }
    //右递归
    if (!areEqual(a->one, b->one)) {
        return false;
    }
    //左递归
    if (!areEqual(a->zero, b->zero)) {
        return false;
    }

    return true;
}

/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Write your own student tests. */


STUDENT_TEST("Test of deallocateTree/areEqual") {
    EncodingTreeNode* a = nullptr;
    EncodingTreeNode* b = nullptr;
    EXPECT(areEqual(a, b));
    EXPECT(areEqual(a, b));


    EncodingTreeNode* c = new EncodingTreeNode('A');
    EXPECT(!areEqual(a, c));
    EXPECT(!areEqual(c, a));


    EncodingTreeNode* d = new EncodingTreeNode('B');
    EXPECT(!areEqual(d, c));

    EncodingTreeNode* Tree = createExampleTree();
    EncodingTreeNode* Tree2 = createExampleTree();

    EXPECT(areEqual(Tree, Tree2 ));

    EncodingTreeNode* childTree = Tree->one;
    EXPECT(!areEqual(childTree, Tree2));

    deallocateTree(a);
    deallocateTree(b);
    deallocateTree(d);
    deallocateTree(c);
    deallocateTree(Tree);
    deallocateTree(Tree2);

}






/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("decodeText, small example encoding tree") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    deallocateTree(tree);
}

PROVIDED_TEST("unflattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  treeShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeLeaves = { 'T', 'R', 'S', 'E' };
    EncodingTreeNode* tree = unflattenTree(treeShape, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

PROVIDED_TEST("decompress, small example input") {
    EncodedData data = {
        { 1, 0, 1, 1, 0, 0, 0 }, // treeShape
        { 'T', 'R', 'S', 'E' },  // treeLeaves
        { 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1 } // messageBits
    };

    EXPECT_EQUAL(decompress(data), "TRESS");
}


PROVIDED_TEST("encodeText, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(encodeText(reference, "E"), messageBits);

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(encodeText(reference, "SET"), messageBits);

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1 }; // STREETS
    EXPECT_EQUAL(encodeText(reference, "STREETS"), messageBits);

    deallocateTree(reference);
}


PROVIDED_TEST("buildHuffmanTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    EncodingTreeNode* tree = buildHuffmanTree("STREETTEST");
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);
}

PROVIDED_TEST("encodeText, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(encodeText(reference, "E"), messageBits);

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(encodeText(reference, "SET"), messageBits);

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1 }; // STREETS
    EXPECT_EQUAL(encodeText(reference, "STREETS"), messageBits);

    deallocateTree(reference);
}

PROVIDED_TEST("flattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  expectedShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'T', 'R', 'S', 'E' };

    Queue<Bit>  treeShape;
    Queue<char> treeLeaves;
    flattenTree(reference, treeShape, treeLeaves);

    EXPECT_EQUAL(treeShape,  expectedShape);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);

    deallocateTree(reference);
}

PROVIDED_TEST("compress, small example input") {
    EncodedData data = compress("STREETTEST");
    Queue<Bit>  treeShape   = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeChars   = { 'T', 'R', 'S', 'E' };
    Queue<Bit>  messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0 };

    EXPECT_EQUAL(data.treeShape, treeShape);
    EXPECT_EQUAL(data.treeLeaves, treeChars);
    EXPECT_EQUAL(data.messageBits, messageBits);
}

PROVIDED_TEST("Test end-to-end compress -> decompress") {
    Vector<string> inputs = {
        "HAPPY HIP HOP",
        "Nana Nana Nana Nana Nana Nana Nana Nana Batman"
        "Research is formalized curiosity. It is poking and prying with a purpose. – Zora Neale Hurston",
    };

    for (string input: inputs) {
        EncodedData data = compress(input);
        string output = decompress(data);

        EXPECT_EQUAL(input, output);
    }
}
