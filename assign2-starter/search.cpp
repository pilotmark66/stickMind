// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "simpio.h"
#include "testing/SimpleTest.h"
using namespace std;


// TODO: 删除字符串中头尾的符号，中间的不删除，字符串中没有字母返回空
// 分别考虑头和尾，利用substr方法
string cleanToken(string s)
{
    string originToken = toLowerCase(s);
    string cleToken;
    int numNumber = 0;

    int index = 0;
    while (ispunct(originToken[index])) {    //删除头部符号
        index++;
    }
    cleToken = originToken.substr(index);

    int backindex = cleToken.size();
    while (ispunct(cleToken[backindex - 1])) {   //删除尾部符号
        backindex--;
    }
    cleToken = cleToken.substr(0, backindex);

    for (int i = 0; i < cleToken.size(); i++) {  //检查是否全部为字母
        if (isalpha(cleToken[i])) {
            numNumber++;
        }

    }
    if (numNumber == 0) {
        return "";
    }

    s = cleToken;
    return s;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> gatherTokens(string text)
{
    Set<string> tokens;
    Vector<string> bodyText = stringSplit(text, " ");
    for (string token : bodyText) {
        string cleToken = cleanToken(token);
        if (!tokens.contains(cleToken)) {
            if (cleToken != "") {
            tokens.add(cleToken);
            }
        }
    }

    return tokens;
}

// TODO: 函数 buildIndex 从数据库文件读取内容，并将其处理为反向索引的形式。
// behavior of the function and how you implemented this behavior
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    ifstream in;

    if (!openFile(in, dbfile))
        error("Cannot open file named " + dbfile);

    Vector<string> lines;
    Set<string> webTextToken;
    Set<string> webSites;
    readEntireFile(in, lines);
    for (int i = 0; i < lines.size(); i++) {
        string url = lines.get(i);
        i++;
        string webText = lines.get(i);            //间隔一行，为网页文本内容
        webTextToken = gatherTokens(webText);

        for (auto token : webTextToken) {
            webSites = index.get(token);            //将map里面相同key的网址url取出
            webSites.add(url);                      //在此值上附加新的url
            index.put(token, webSites);
        }

    }

    return lines.size() / 2;
}

// TODO:查询时提供的 query 字符串，要么是单个搜索关键词，要么是组合搜索关键词。
//一个搜索关键词，必须是一个单词，或是使用 + 或 - 连接的单词组合序列。查询不区分大小写
// 可以使用 stringSplit 函数，拆分复合搜索关键词。
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;
    // TODO: your code here
    Set<string> webSites;
    string keyWords = toLowerCase(query);
    Vector<string> keys = stringSplit(keyWords, " ");
    webSites = index.get(query);
    for (string key : keys) {
        string cleKey = cleanToken(key);
        if (index.containsKey(cleKey)) {
            if (!ispunct(key[0])) {
                webSites = webSites.unionWith(index.get(cleKey));  //  没有使用修饰符连接关键词，搜索结果是每个关键词匹配项的并集。（可以使用 Set 中的 unionWith 接口）
            }

            if (key[0] == '+') {
                webSites = webSites.intersect(index.get(cleKey));  //   + 号连接，结果将是每个关键词匹配项的交集。（可以使用 Set 中的 intersect 接口）
            }
            if (key[0] == '-') {
                webSites = webSites.difference(index.get(cleKey));  //   - 号连接，结果将会从前面关键词的匹配项中，去除 - 号后面关键词的匹配项。（可以使用 Set 中的 difference接口）
            }
        }
    }

    result = webSites;

    return result;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
void searchEngine(string dbfile)
{
    // TODO: your code here
    Map<string, Set<string>> index;            //构建反向索引

    string query;
    Set<string> matchPages;
    int pages = buildIndex(dbfile, index);
    int keyTerms = index.size();
    cout << "Stand by while building index..." << endl;
    cout << "Indexed " << pages <<" pages containing " << keyTerms << " unique terms" << endl;
    while (1) {
        query = getLine("Enter query sentence (RETURN/ENTER to quit):");

        if (!query.size()) {                       //接收空字符串时，结束
            break ;
        }
        matchPages = findQueryMatches(index, query);
        cout << "Found " << matchPages.size() << " matching pages" << endl;
        cout << matchPages << endl;
    }


}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


// TODO: add your test cases here


STUDENT_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("I"), "i");
    EXPECT_EQUAL(cleanToken("[][][]CS*106B"), "cs*106b");
    EXPECT_EQUAL(cleanToken("heLlo"), "hello");
    EXPECT_EQUAL(cleanToken("WOrLD"), "world");

    EXPECT_EQUAL(cleanToken("/hel\\\\\\lo/"), "hel\\\\\\lo");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
    EXPECT_EQUAL(cleanToken("10610--6106106"), "");
    EXPECT_EQUAL(cleanToken("~!,.[]!!!"), "");
}

STUDENT_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    Set<string> expected2 = {"one", "fish", "two", "red", "blue",};
    EXPECT_EQUAL(gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?"), expected2);
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}


STUDENT_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

STUDENT_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

STUDENT_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}


STUDENT_TEST("buildIndex from tiny.txt, websites containing bread") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> expectedWebsites = {"www.shoppinglist.com"};
    EXPECT_EQUAL(index.get("bread"), expectedWebsites);
}



STUDENT_TEST("buildIndex from tiny.txt, websites containing red") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> expectedWebsites = {"www.shoppinglist.com", "www.dr.seuss.net", "www.bigbadwolf.com"};
    EXPECT_EQUAL(index.get("fish"), expectedWebsites);
}

STUDENT_TEST("Time trials of building inverted index for res/tiny.txt") {
    Map<string, Set<string>> index;
    TIME_OPERATION("search", buildIndex("res/tiny.txt", index));
}

STUDENT_TEST("Time trials of building inverted index for res/website.txt") {
    Map<string, Set<string>> index;
    TIME_OPERATION("search", buildIndex("res/website.txt", index));
}

STUDENT_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

STUDENT_TEST("findQueryMatches from website.txt, compound queries interleaved with invalid terms") {
    Map<string, Set<string>> index;
    buildIndex("res/website.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "cs107 stanford -qt");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);

}
