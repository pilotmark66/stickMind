/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "recursion.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}

/*
 * TODO: 输入点坐标，三角形阶数，返回绘制的三角形个数
 * 0阶时，一个三角形，
 * 大的外三角形的每一边的中点是较小的内三角形的一个角。中点是 {(x1 + x2) / 2, (y1 + y2) / 2}
 */
int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    /* TODO: Implement this function. */
    GPoint oneAndtwo = GPoint((one.x + two.x) / 2, (one.y + two.y) / 2);        //小三角形端点
    GPoint oneAndthree = GPoint((one.x + three.x) / 2, (one.y + three.y) / 2);  //小三角形端点
    GPoint twoAndthree = GPoint((two.x + three.x) / 2, (two.y + three.y) / 2);  //小三角形端点
    if(order == 0){
            fillBlackTriangle(window,one,two,three);
            return 1;
        }else {
            int left;
            int right;
            int middle;
            left = drawSierpinskiTriangle(window, one, oneAndtwo, oneAndthree, order - 1);
            right = drawSierpinskiTriangle(window, oneAndtwo, two, twoAndthree, order - 1);
            middle = drawSierpinskiTriangle(window, oneAndthree, twoAndthree, three, order - 1);
            return left+ middle + right;
        }


}


/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runInteractiveDemo();
}

