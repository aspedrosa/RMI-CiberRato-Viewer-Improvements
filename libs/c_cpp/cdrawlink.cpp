//
// Created by aspedrosa on 1/7/21.
//

#include "cdrawlink.h"

#include <iostream>

CDrawLink::CDrawLink(char *host, unsigned short port) : port(port, host,0) {
    this->color = {0, 0, 0};
    msg_index = sprintf(msg, "<Shapes>");
    initial_msg_index = msg_index;
    buffered = true;
    Status = 0;

    if(!this->port.init())
    {
        // cerr << "Failed socket init" << endl;
        Status=-1;
        return;
    }
}

CDrawLink::~CDrawLink() = default;

void CDrawLink::bufferOnOff(bool on_off){
    buffered = on_off;
}

void CDrawLink::setColor(int r, int g, int b){
    this->color.r = r;
    this->color.g = g;
    this->color.b = b;
}

void CDrawLink::sendMessage(){
    msg_index += sprintf(msg + msg_index, "</Shapes>");
    msg[msg_index+ 1] = '\0';
    if(!port.send_info(msg, msg_index+1)) {
        // cerr << "Failed Send Init" << endl;
        Status=-1;
        return;
    }
    msg_index = initial_msg_index;
}

void CDrawLink::drawAll(){
    if (buffered) {
        sendMessage();
    }
}

void CDrawLink::drawEllipse(char *id, double diam_vertical, double diam_horizontal, double x, double y, Color *color, int time_to_live){
    int r,g,b;
    if(color == nullptr){
        r = this->color.r;
        g = this->color.g;
        b = this->color.b;
    }
    else{
        r = color->r;
        g = color->g;
        b = color->b;
    }
    int ttl = time_to_live == -1 ? ttl_default : time_to_live;

    msg_index += sprintf(msg+msg_index, R"(<Ellipse ttl="%d" id="%s" red="%d" green="%d" blue="%d" Diam_vertical="%f" Diam_horizontal="%f" x="%f" y="%f"></Ellipse>)",
                    ttl,id,r,g,b,diam_vertical,diam_horizontal,x,y);
    if(!buffered) {
        sendMessage();
    }
}

void CDrawLink::drawText(char *id, int font_size, char *text, double x, double y, Color *color, int time_to_live){
    int r,g,b;
    if(color == nullptr){
        r = this->color.r;
        g = this->color.g;
        b = this->color.b;
    }
    else{
        r = color->r;
        g = color->g;
        b = color->b;
    }
    int ttl = time_to_live == -1 ? ttl_default : time_to_live;

    msg_index += sprintf(msg+msg_index, R"(<Text ttl="%d" id="%s" red="%d" green="%d" blue="%d" text="%s" x="%f" y="%f" font_size="%d"></Text>)",
                         ttl,id,r,g,b,text,x,y,font_size);
    if(!buffered) {
        sendMessage();
    }
}

void CDrawLink::drawCircle(char *id, double diam, double x, double y, Color *color, int time_to_live) {
    drawEllipse(id, diam, diam, x, y, color, time_to_live);
}

void CDrawLink::drawRectangle(char *id, double width, double height, double x, double y, Color *color, int time_to_live) {
    int r, g, b;
    if (color == nullptr) {
        r = this->color.r;
        g = this->color.g;
        b = this->color.b;
    } else {
        r = color->r;
        g = color->g;
        b = color->b;
    }
    int ttl = time_to_live == -1 ? ttl_default : time_to_live;

    msg_index += sprintf(msg + msg_index,
                         R"(<Rectangle ttl="%d" id="%s" red="%d" green="%d" blue="%d" Width="%f" Height="%f" x="%f" y="%f"></Rectangle>)",
                         ttl, id, r, g, b, width, height, x, y);
    if (!buffered) {
        sendMessage();
    }
}

void CDrawLink::drawSquare(char *id, double width, double x, double y, Color *color, int time_to_live) {
    drawRectangle(id,width,width,x,y,color,time_to_live);
}

void CDrawLink::drawPolygon(char *id, int num_of_points, Point *point_list, Color *color, int time_to_live){
    int r,g,b;
    if(color == nullptr){
        r = this->color.r;
        g = this->color.g;
        b = this->color.b;
    }
    else{
        r = color->r;
        g = color->g;
        b = color->b;
    }
    int ttl = time_to_live == -1 ? ttl_default : time_to_live;

    msg_index += sprintf(msg+msg_index, R"(<Polygon ttl="%d" id="%s" red="%d" green="%d" blue="%d">)",
                         ttl,id,r,g,b);


    for(int i = 0; i < num_of_points; i++){
        msg_index += sprintf(msg+msg_index, R"(<Point x="%f" y="%f"></Point>)", point_list[i].x, point_list[i].y);
    }

    msg_index += sprintf(msg+msg_index, "</Polygon>");
    if(!buffered) {
        sendMessage();
    }
}

void CDrawLink::drawLine(char *id, double x0, double y0, double x1, double y1, Color *color, int time_to_live) {
    int r, g, b;
    if (color == nullptr) {
        r = this->color.r;
        g = this->color.g;
        b = this->color.b;
    } else {
        r = color->r;
        g = color->g;
        b = color->b;
    }
    int ttl = time_to_live == -1 ? ttl_default : time_to_live;

    msg_index += sprintf(msg + msg_index,
                         R"(<Line ttl="%d" id="%s" red="%d" green="%d" blue="%d" x0="%f" y0="%f" x1="%f" y1="%f"></Line>)",
                         ttl, id, r, g, b, x0, y0, x1, y1);
    if (!buffered) {
        sendMessage();
    }
}

void CDrawLink::drawLine2(char *id, Point &p0, Point &p1, Color *color, int time_to_live) {
    drawLine(id, p0.x, p0.y, p1.x, p1.y, color, time_to_live);
}
