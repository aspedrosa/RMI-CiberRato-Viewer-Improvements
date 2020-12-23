//
// Created by aspedrosa on 12/16/20.
//

#ifndef VIEWER_CRSHAPE_H
#define VIEWER_CRSHAPE_H

#include <QString>
#include <QColor>
#include <QPointF>
#include <QVector>

class Point {
public:
    Point() {
        this->x = this->y = 0;
    }

    Point(double x, double y) {
        this->x = x;
        this->y = y;
    }

    double get_x() const {
        return x;
    }

    double get_y() const {
        return y;
    }

private:
    double x, y;
};

class Shape {
public:
    Shape(QColor &color, int id) {
        this->color = color;
        this->id = id;
    }
    virtual ~Shape() {
        delete &color;
    }

    const QColor &get_color() const {
        return color;
    }

    int getId(){
        return id;
    }

private:
    QColor color;
    int id;
};

class Quote : public Shape {
public:
    Quote(QColor &color, int id, QString &text) : Shape(color, id){
        this->text;
    }
    ~Quote() {
        delete &text;
    }

    const QString &get_text() const {
        return text;
    }

private:
    QString text;
};

class Line : public Shape {
public:
    Line(QColor &color, int id, Point &p_begin, Point &p_end) : Shape (color, id) {
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

class Ellipse : public Shape {
public:
    Ellipse(QColor &color, int id, Point &p, double diam_vertical, double diam_horizontal) : Shape(color, id) {
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

class Circle : public Ellipse {
public:
    Circle(QColor &color, int id, Point &p, double diam) : Ellipse(color, id, p, diam, diam) {}
};

class Polygon : public Shape {
public:
    Polygon(QColor &color, int id, QVector<QPointF> &points) : Shape(color, id){
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

class Rectangle : public Shape {
public:
    Rectangle(QColor &color, int id, Point &p, double width, double height) : Shape(color, id){
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

class Square : public Rectangle {
public:
    Square(QColor &color, int id, Point &p, double width) : Rectangle(color, id, p, width, width){}
};

#endif //VIEWER_CRSHAPE_H