#include "DrawSock.h"

int main(int _, char **__) {
    InitDraw3();

    setColor(255, 0, 0);
    //bufferOnOff(false);

    drawEllipse("5.loc.4", 50, 25, 7, 7, nullptr, -1);
    drawCircle("5.loc.3", 50, 7, 8, nullptr, -1);
    drawText("5.loc.2", "ola", 1, 2, 20, nullptr, -1);
    Color c = {0, 255, 0};
    drawRectangle("5.loc.1", 50, 10, 10, 10, &c, -1);
    drawSquare("5.loc.5", 50, 10, 13, nullptr, -1);
    Point points[3];
    points[0] = {0, 0};
    points[1] = {0, 7};
    points[2] = {15, 0};
    drawPolygon("6.loc.1", 3, points, nullptr, -1);
    drawLine("7.loc.1", 0, 0, 28, 14, nullptr, -1);
    drawLine2("7.loc.2", points, points+1, nullptr, 10000);
    drawAll();
}
