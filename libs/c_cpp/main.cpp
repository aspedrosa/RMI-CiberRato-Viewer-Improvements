//
// Created by aspedrosa on 1/7/21.
//

#include "DrawSock.h"

int main(int argc, char **argv) {
    InitDraw3();

    //drawEllipse(char *id, double diam_vertical, double diam_horizontal, double x, double y, Color *color, int time_to_live);
    //drawCircle(char *id, double diam, double x, double y, Color *color, int time_to_live);
    //drawText(char *id, char *text, Color *color, int time_to_live);
    //drawRectangle(char *id, double width, double height, double x, double y, Color *color, int time_to_live);
    //drawSquare(char *id, double width, double x, double y, Color *color, int time_to_live);
    //drawPolygon(char *id, int num_of_points, Point *point_list, Color *color, int time_to_live);
    //drawLine(char *id, double x0, double y0, double x1, double y1, Color *color, int time_to_live);
    //drawLine2(char *id, Point *x0, Point *x1, Color *color, int time_to_live);

    setColor(255, 0, 0);
    bufferOnOff(false);

    drawEllipse("5.loc.4", 50, 25, 100, 200, nullptr, -1);
    drawCircle("5.loc.3", 50, 100, 300, nullptr, -1);
    drawText("5.loc.2", "ola", 25, 250, 20, nullptr, -1);
    Color c = {0, 255, 0};
    drawRectangle("5.loc.1", 50, 10, 400, 200, &c, -1);
    drawSquare("5.loc.5", 10, 300, 200, nullptr, -1);
    Point points[3];
    points[0] = {0, 0};
    points[1] = {0, 50};
    points[2] = {50, 0};
    drawPolygon("6.loc.1", 3, points, nullptr, -1);
    drawLine("7.loc.1", 0, 20, 20, 200, nullptr, -1);
    drawLine2("7.loc.2", points, points+1, nullptr, 10000);
    //drawAll();
}
