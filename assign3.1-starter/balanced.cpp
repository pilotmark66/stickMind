/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;

/*
 * TODO: 将字符串中[]{}()提取出来
 * 输入字符串中只包括括号符号
 * 当字符串第一个字符为括号运算符时，返回该字符并递归调用字符串的子字符串重复判断
 * 直到字符串为空.
 */
string operatorsFrom(string str) {
    /* TODO: Implement this function. */
    if (str.empty()) {
        return "";
    } else {
        if (ispunct(str[0])) {                             //非括号类符号怎么处理？
            return  str[0] + operatorsFrom(str.substr(1));
        } else {
            return operatorsFrom(str.substr(1));
        }
    }
}

/*
 * TODO: 判断括号字符是否对称
 * 输入为括号字符串
 */
bool operatorsAreMatched(string ops) {
    /* TODO: Implement this function. */
    if (ops.empty()) {
        return true;
    } else {
        if (ops.find("()") != string::npos) {                          //find,返回找到第一个匹配的位置，如果没有返回npos
            return operatorsAreMatched(ops.erase(ops.find("()"), 2));  //erase掉对称的括号
        }
        if (ops.find("{}") != string::npos) {
            return operatorsAreMatched(ops.erase(ops.find("{}"), 2));
        }
        if (ops.find("[]") != string::npos) {
            return operatorsAreMatched(ops.erase(ops.find("[]"), 2));
        }
    }
    return false;
}

/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}


STUDENT_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
    EXPECT_EQUAL(operatorsFrom(")v(c[3{}]()"), ")([{}]()");
    EXPECT_EQUAL(operatorsFrom("([])"), "([])");
    EXPECT_EQUAL(operatorsFrom("123"), "");
}

STUDENT_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

STUDENT_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}()[]"));
    EXPECT(!operatorsAreMatched("{[({)}}"));
    EXPECT(!operatorsAreMatched("[])("));
    EXPECT(!operatorsAreMatched("{[]({)}}"));
}
