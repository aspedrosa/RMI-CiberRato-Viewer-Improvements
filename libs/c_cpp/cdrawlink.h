//
// Created by aspedrosa on 1/7/21.
//

#ifndef C_CPP_CDRAWLINK_H
#define C_CPP_CDRAWLINK_H

#include "netif.h"
#include "structs.h"

#define ttl_default 5000
#define MSGMAXSIZE (4096)

class CDrawLink {
//#ifdef CIBERQTAPP
//: public QObject
//#endif
//{
//#ifdef CIBERQTAPP
//    Q_OBJECT
//#endif
public:
    CDrawLink(char *host, unsigned short port);
    virtual ~CDrawLink();

    void bufferOnOff(bool on_off);
    void setColor(int r, int g, int b);
    void drawAll();
    void drawEllipse(char *id, double diam_vertical, double diam_horizontal, double x, double y, Color *color=nullptr, int time_to_live=-1);
    void drawCircle(char *id, double diam, double x, double y, Color *color=nullptr, int time_to_live=-1);
    void drawText(char *id, int font_size=14, char *text, double x, double y, Color *color=nullptr, int time_to_live=-1);
    void drawRectangle(char *id, double width, double height, double x, double y, Color *color=nullptr, int time_to_live=-1);
    void drawSquare(char *id, double width, double x, double y, Color *color=nullptr, int time_to_live=-1);
    void drawPolygon(char *id, int num_of_points, Point *point_list, Color *color=nullptr, int time_to_live=-1);
    void drawLine(char *id, double x0, double y0, double x1, double y1, Color *color=nullptr, int time_to_live=-1);
    void drawLine2(char *id, Point &x0, Point &x1, Color *color=nullptr, int time_to_live=-1);
    inline int status() { return Status; };

private:
    void sendMessage();

    Color color{};
    bool buffered;
    char msg[MSGMAXSIZE]{};
    int msg_index;
    int initial_msg_index;

    Port port;
    int Status;
};

#endif //C_CPP_CDRAWLINK_H
