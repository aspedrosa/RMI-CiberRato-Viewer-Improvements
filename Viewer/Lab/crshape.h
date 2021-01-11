//
// Created by aspedrosa on 12/16/20.
//

#ifndef VIEWER_CRSHAPE_H
#define VIEWER_CRSHAPE_H

#include <QString>
#include <QColor>
#include <QPointF>
#include <QVector>

/**
 * Used to translate robot coordinates to scene coordinates
 */
#define ZOOM 34

/**
 * Point class used by other shape classes
 *  to define point on the scene
 */
class Point {
public:
    Point() {
        this->x = this->y = 0;
    }

    Point(double x, double y) {
        this->x = x * ZOOM;
        this->y = y * ZOOM;
    }

    /**
     * Gets the x coordinate on the scene coordinate space
     *
     * @return x coordinate converted to the scene coordinates space
     */
    double get_x() const {
        return x;
    }

    /**
     * Gets the y coordinate on the scene coordinate space.
     * This extra calculation is needed because the robot coordinate
     *  space and scene coordinate space have a different 0,0.
     *
     * @param labSize_y size of the scene used to convert to the scene coordinates space
     * @return y coordinate converted to the scene coordinates space
     */
    double get_y(double labSize_y) const {
        return ZOOM * labSize_y - y;
    }

private:
    double x, y;
};

/**
 * Base Shape class
 */
class Shape {
public:
    Shape(QColor &color, QString id, unsigned int ttl) {
        this->color = color;
        this->id = id;
        this->ttl = ttl;
    }
    virtual ~Shape() {
        delete &color;
    }

    const QColor &get_color() const {
        return color;
    }

    QString& getId(){
        return id;
    }

    unsigned int getTTL() {
        return ttl;
    }

private:
    QColor color;
    QString id;
    unsigned int ttl;
};

/**
 * Class to represent a text in the scene
 */
class Quote : public Shape {
public:
    Quote(QColor &color, QString id, QString &text, int font_size, Point &p, unsigned int time) : Shape(color, id, time){
        this->text = text;
        this->p = p;
        this->font_size = font_size;

    }
    ~Quote() {
        delete &text, &p;
    }

    const QString &get_text() const {
        return text;
    }

    const Point &get_p() const {
        return p;
    }

    const int get_font_size() const {
        return font_size;
    }


private:
    QString text;
    int font_size;
    Point p;
};

/**
 * Class to represent a line in the scene
 */
class Line : public Shape {
public:
    Line(QColor &color, QString id, Point &p_begin, Point &p_end, unsigned int time) : Shape (color, id, time) {
        this->p_begin = p_begin;
        this->p_end = p_end;
    }
    ~Line(){
        delete &p_begin, &p_end;
    }

    const Point &get_p_begin() const {
        return p_begin;
    }

    const Point &get_p_end() const {
        return p_end;
    }

private:
    Point p_begin, p_end;
};

/**
 * Class to represent a ellipse in the scene
 */
class Ellipse : public Shape {
public:
    Ellipse(QColor &color, QString id, Point &p, double diam_vertical, double diam_horizontal, unsigned int time) : Shape(color, id, time) {
        this->p = p;
        this->diam_vertical = diam_vertical;
        this->diam_horizontal = diam_horizontal;
    }
    ~Ellipse() {
        delete &p;
    }

    const Point &get_p() const {
        return p;
    }

    double get_diam_vertical() const {
        return diam_vertical;
    }

    double get_diam_horizontal() const {
        return diam_horizontal;
    }

private:
    Point p;
    double diam_vertical, diam_horizontal;
};

/**
 * Class to represent a circle in the scene
 */
class Circle : public Ellipse {
public:
    Circle(QColor &color, QString id, Point &p, double diam, unsigned int time) : Ellipse(color, id, p, diam, diam, time) {}
};

class Polygon : public Shape {
public:
    Polygon(QColor &color, QString id, QVector<QPointF> &points, unsigned int time) : Shape(color, id, time){
        this->points = points;

    }
    ~Polygon(){
        delete &points;
    }

    const QVector<QPointF> &get_points() const {
        return points;
    }

private:
    QVector<QPointF> points;
};

/**
 * Class to represent a rectangle in the scene
 */
class Rectangle : public Shape {
public:
    Rectangle(QColor &color, QString id, Point &p, double width, double height, unsigned int time) : Shape(color, id, time){
        this-> p = p;
        this->width = width;
        this-> height = height;
    }
    ~Rectangle(){
        delete &p;
    }

    const Point &get_p() const {
        return p;
    }

    double get_width() const {
        return width;
    }

    double get_height() const {
        return height;
    }

private:
    Point p;
    double width;
    double height;
};

/**
 * Class to represent a Square in the scene
 */
class Square : public Rectangle {
public:
    Square(QColor &color, QString id, Point &p, double width, unsigned int time) : Rectangle(color, id, p, width, width, time){}
};

#endif //VIEWER_CRSHAPE_H