/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h"
#include <algorithm>
using namespace std;

/* This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 * WARNING: The provided code is buggy!
 *
 * Use unit tests to identify which inputs to this function
 * are incorrectly handled. Then, remove this comment and
 * replace it with a description of the bug you fixed.
 */
string removeNonLetters(string s) {
    string result;
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}



/* 将不同字母替换为不同数字
 * 输入为一个字母字符，返回数字字符串.
 */
string change(char x) {
    if (x == 'A' || x == 'E' || x == 'I' || x == 'O' || x == 'U' || x == 'H' || x == 'W' || x == 'Y') return "0";
    else if (x == 'B' || x == 'F' || x == 'P' || x == 'V') return "1";
    else if (x == 'C' || x == 'G' || x == 'J' || x == 'K' || x == 'Q' || x == 'S' || x == 'X' || x == 'Z') return "2";
    else if (x == 'D' || x == 'T') return "3";
    else if (x == 'L') return "4";
    else if (x == 'M' || x == 'N') return "5";
    else if (x == 'R') return "6";
    else return "0";
}


/* 将字符串替换为法则对应下的数字组合
 * 输入为字母字符串，返回数字字符串.
 */

string nameToNum(string s) {
    string Num;
    for (int i = 0; i < s.length(); i++) {
        Num+= change(s[i]);
    }
    return Num;
}

/* 将数字组合的0去掉
 * 输入为数字字符串，返回不带0的字符串
 */
string removeZero(string s) {
    string noZero;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] != '0'){
            noZero+= s[i];
        }

    }
    return noZero;
}

/* 将数字组合统一为4位，多余的截掉，缺少的补0
 * 输入为数字字符串，4位字符串
 */

string addZero(string s) {
    string soundNum = "0000";
    if (s.length() >= 4) {
        for (int i = 0; i < 4; i++) {

            soundNum[i] = s[i];

        }
    } else {
        for (int j = 0; j < s.length(); j++) {
            soundNum[j] = s[j];
        }
    }
    return soundNum;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
string soundex(string s) {
    /* TODO: Fill in this function. */
    string result = removeNonLetters(s);
    string upName = toUpperCase(result);
    string nameNum = nameToNum(upName);
    string noRepeat;
    unique_copy(nameNum.begin(), nameNum.end(), back_inserter(noRepeat));  //使用unique copy合并相邻重复字符
    noRepeat[0] = upName[0];                                               //替换第一个数字为名字的第一个大写字母
    string noZero = removeZero(noRepeat);
    return addZero(noZero);
}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    // The names in the database are now stored in the provided
    // vector named databaseNames

    /* TODO: Fill in the remainder of this function. */
    Vector<string> dataMatch;
    string enterName = getLine("Enter a surname (RETURN to quit):");
    string enterNameNum = soundex(enterName);
    int matchNum = 0;
    for (int i = 0; i < databaseNames.size(); i++) {
        if (enterNameNum == soundex(databaseNames[i])) {
            dataMatch.insert(matchNum, databaseNames[i]);
            matchNum++;
        }
    }
    dataMatch.sort();

    cout << "soundex code is " << enterNameNum << endl;
    cout << dataMatch << endl;
}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
    s = "2l-dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "ldr");
    s = "";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");
    s = "a2346";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "a");
    s = "2346";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here



STUDENT_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
    s = "2l-dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "ldr");
    s = "";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");
    s = "a2346";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "a");
    s = "2346";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");
}

STUDENT_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = soundex(s);
    EXPECT_EQUAL(result, "O600");
    s = "Planet9";
    result = soundex(s);
    EXPECT_EQUAL(result, "P453");

}
