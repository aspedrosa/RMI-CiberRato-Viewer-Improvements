#include "DrawSock.h"
#include "cdrawlink.h"

#include <clocale>

static CDrawLink *link=nullptr;

int InitDraw(char *host, unsigned short port) {
    link = new CDrawLink(host, port);
    setlocale(LC_ALL, "C");
    return link->status();
}

void bufferOnOff(bool on_off){
    link->bufferOnOff(on_off);
}

void setColor(int r, int g, int b) {
    link->setColor(r, g, b);
}

void drawAll() {
    link->drawAll();
}

void drawEllipse(char *id, double diam_vertical, double diam_horizontal, double x, double y, Color *color=nullptr, int time_to_live=-1){
    link->drawEllipse(id, diam_vertical, diam_horizontal, x, y, color, time_to_live);
}

void drawCircle(char *id, double diam, double x, double y, Color *color=nullptr, int time_to_live=-1) {
    link->drawCircle(id, diam, x, y, color, time_to_live);
}

void drawText(char *id, char *text, Color *color=nullptr, int time_to_live=-1){
    link->drawText(id, text, color,time_to_live);
}

void drawRectangle(char *id, double width, double height, double x, double y, Color *color=nullptr, int time_to_live=-1){
    link->drawRectangle(id,width,height,x,y,color,time_to_live);
}

void drawSquare(char *id, double width, double x, double y, Color *color=nullptr, int time_to_live=-1){
    link->drawSquare(id,width,x,y,color,time_to_live);
}

void drawPolygon(char *id, int num_of_points, Point *point_list, Color *color=nullptr, int time_to_live=-1){
    link->drawPolygon(id, num_of_points, point_list, color, time_to_live);
}

void drawLine(char *id, double x0, double y0, double x1, double y1, Color *color=nullptr, int time_to_live=-1){
    link->drawLine(id,x0,y0,x1,y1,color,time_to_live);
}

void drawLine(char *id, Point &x0, Point &x1, Color *color=nullptr, int time_to_live=-1){
    link->drawLine(id, x0, x1, color, time_to_live);
}
