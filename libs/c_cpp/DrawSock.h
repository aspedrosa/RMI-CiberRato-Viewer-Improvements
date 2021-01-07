#ifndef C_CPP_DRAWSOCK_H
#define C_CPP_DRAWSOCK_H

#include "structs.h"

#ifdef __cplusplus
extern "C"
{
#else
typedef unsigned char bool;
#endif

int InitDraw(char *host, unsigned short port);
void bufferOnOff(bool on_off);
void setColor(int r, int g, int b);
void drawAll();
void drawEllipse(char *id, double diam_vertical, double diam_horizontal, double x, double y, Color *color, int time_to_live);
void drawCircle(char *id, double diam, double x, double y, Color *color, int time_to_live);
void drawText(char *id, char *text, Color *color, int time_to_live);
void drawRectangle(char *id, double width, double height, double x, double y, Color *color, int time_to_live);
void drawSquare(char *id, double width, double x, double y, Color *color, int time_to_live);
void drawPolygon(char *id, int num_of_points, Point *point_list, Color *color, int time_to_live);
void drawLine(char *id, double x0, double y0, double x1, double y1, Color *color, int time_to_live);
void drawLine(char *id, Point &x0, Point &x1, Color *color, int time_to_live);

#ifdef __cplusplus
}
#endif

#endif //C_CPP_DRAWSOCK_H
