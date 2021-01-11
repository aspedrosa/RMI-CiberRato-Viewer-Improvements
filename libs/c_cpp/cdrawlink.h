/**
 * Actual implementation of the library
 */
#ifndef C_CPP_CDRAWLINK_H
#define C_CPP_CDRAWLINK_H

#include "netif.h"
#include "structs.h"

#define ttl_default 5000
#define MSGMAXSIZE (4096)

/**
 * Class with the actual implementation to handle
 *  the process to send messages to the viewer.
 */
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

    /**
     * Method to activate and deactivate the buffer mode
     *
     * @param on_off if the buffer is now on or off
     */
    void bufferOnOff(bool on_off);

    /**
     * Method to define the default color for shapes if no color is defined
     *
     * @param r red value
     * @param g green value
     * @param b blue value
     */
    void setColor(int r, int g, int b);

    /**
     * Sends a message to the Viewer with all shapes that are in the buffer.
     */
    void drawAll();

    /**
     * Sends a message to draw a Ellipse shape
     *
     * @param id shape id, ex: rob1.Motors.left
     * @param diam_vertical vertical diameter
     * @param diam_horizontal horizontal diameter
     * @param x coordinate of the center point
     * @param y coordinate of the center point
     * @param color color for this specific shape
     * @param time_to_live time to after which this shapes is removed
     */
    void drawEllipse(char *id, double diam_vertical, double diam_horizontal, double x, double y, Color *color=nullptr, int time_to_live=-1);
    /**
     * Sends a message to draw a Circle shape
     *
     * @param id shape id, ex: rob1.Motors.left
     * @param diam diameter
     * @param x coordinate of the center point
     * @param y coordinate of the center point
     * @param color color for this specific shape
     * @param time_to_live time to after which this shapes is removed
     */
    void drawCircle(char *id, double diam, double x, double y, Color *color=nullptr, int time_to_live=-1);
    /**
     * Sends a message to draw a Text shape
     *
     * @param id shape id, ex: rob1.Motors.left
     * @param text text to display
     * @param x coordinate of the center point
     * @param y coordinate of the center point
     * @param font_size font size for the text
     * @param color color for this specific shape
     * @param time_to_live time to after which this shapes is removed
     */
    void drawText(char *id, char *text, double x, double y, int font_size=14, Color *color=nullptr, int time_to_live=-1);
    /**
     * Sends a message to draw a Rectangle shape
     *
     * @param id shape id, ex: rob1.Motors.left
     * @param width rectangle's width
     * @param height rectangle's height
     * @param x coordinate of the initial point
     * @param y coordinate of the initial point
     * @param color color for this specific shape
     * @param time_to_live time to after which this shapes is removed
     */
    void drawRectangle(char *id, double width, double height, double x, double y, Color *color=nullptr, int time_to_live=-1);
    /**
     * Sends a message to draw a Square shape
     *
     * @param id shape id, ex: rob1.Motors.left
     * @param width rectangle's width
     * @param x coordinate of the initial point
     * @param y coordinate of the initial point
     * @param color color for this specific shape
     * @param time_to_live time to after which this shapes is removed
     */
    void drawSquare(char *id, double width, double x, double y, Color *color=nullptr, int time_to_live=-1);
    /**
     * Sends a message to draw a Polygon shape
     *
     * @param id shape id, ex: rob1.Motors.left
     * @param num_of_points size of the point_list array
     * @param point_list array of points to build the polygon
     * @param color color for this specific shape
     * @param time_to_live time to after which this shapes is removed
     */
    void drawPolygon(char *id, int num_of_points, Point *point_list, Color *color=nullptr, int time_to_live=-1);
    /**
     * Sends a message to draw a Line shape
     *
     * @param id shape id, ex: rob1.Motors.left
     * @param x0 coordinate of the point 0, initial of the line
     * @param y0 coordinate of the point 0, initial of the line
     * @param x1 coordinate of the point 1, end of the line
     * @param y1 coordinate of the point 1, end of the line
     * @param color color for this specific shape
     * @param time_to_live time to after which this shapes is removed
     */
    void drawLine(char *id, double x0, double y0, double x1, double y1, Color *color=nullptr, int time_to_live=-1);
    /**
     * Sends a message to draw a Line shape
     *
     * @param id shape id, ex: rob1.Motors.left
     * @param x0 initial of the line
     * @param x1 end of the line
     * @param color color for this specific shape
     * @param time_to_live time to after which this shapes is removed
     */
    void drawLine2(char *id, Point &x0, Point &x1, Color *color=nullptr, int time_to_live=-1);
    /**
     * Getter of the initialization status code variable
     *
     * @return status variable
     */
    [[nodiscard]] inline int status() const { return Status; };

private:
    /**
     * Auxiliary method to send messages to draw shapes.
     */
    void sendMessage();

    /**
     * Default color
     */
    Color color{};
    /**
     * Buffered mode
     */
    bool buffered;
    /**
     * buffer to write messages
     */
    char msg[MSGMAXSIZE]{};
    /**
     * current index on the buffer
     */
    int msg_index;
    /**
     * index where new messages will start to be written.
     * Skips initial "<Shapes>"
     */
    int initial_msg_index;

    /**
     * Viewer drawing port
     */
    Port port;
    /**
     * Initialization status code
     */
    int Status;
};

#endif //C_CPP_CDRAWLINK_H
