//eu
#include "crqdrawhandler.h"
#include <iostream>
#include <QString>

bool CRQDrawHandler::startDocument()
{
    // Initialize all elements with null
    type = UNKNOWN;

    return TRUE;
}

bool CRQDrawHandler::endDocument()
{
    return TRUE;
}



bool CRQDrawHandler::startElement( const QString&, const QString&,
                                   const QString& qName,
                                   const QXmlAttributes& attr)
{
    const QString &tag = qName;

    switch (type) 	//Type defined in .h as enum.
    {
        case UNKNOWN:
            if (tag == "Shapes") {
                type = SHAPES;
            }
            break;
        case SHAPES: { //If the message contains shapes
            //Get shape color from the xml message
            const QString red_color = attr.value(QString("red"));
            const QString green_color = attr.value(QString("green"));
            const QString blue_color = attr.value(QString("blue"));
            if (red_color.isNull() || green_color.isNull() || blue_color.isNull()) {
                // TODO missing value
                return TRUE;
            } else {
                color = new QColor(red_color.toInt(), green_color.toInt(), blue_color.toInt());
            }

            //Get shape identifier from the xml message
            shape_id = attr.value(QString("id"));

            //Get shape time to live from the xml message
            QString ttl_string = attr.value(QString("ttl"));
            if (ttl_string.isNull()) {
                ttl = DEFAULT_SHAPE_TTL;
            }
            else {
                ttl = ttl_string.toUInt();
            }
            //Get all the fields to draw an ellipse and add it to shapes vector.
            if (tag == "Ellipse") {
                const QString diam_vertical = attr.value(QString("Diam_vertical"));
                const QString diam_horizontal = attr.value(QString("Diam_horizontal"));
                const QString x = attr.value(QString("x"));
                const QString y = attr.value(QString("y"));
                if (!diam_vertical.isNull() && !diam_horizontal.isNull() && !x.isNull() && !y.isNull()) {
                    auto *p = new Point(x.toDouble(), y.toDouble());
                    auto *ellipse = new Ellipse(*color, shape_id, *p, diam_vertical.toDouble(), diam_horizontal.toDouble(), ttl);
                    shapes.push_back(ellipse);
                }
            //Get all the fields to draw text and add it to shapes vector.
            } else if (tag == "Text") {
                QString text = attr.value(QString("text"));
                int font_size = attr.value(QString("font_size")).toInt();
                const QString x = attr.value(QString("x"));
                const QString y = attr.value(QString("y"));
                if (!text.isNull()) {
                    auto *p = new Point(x.toDouble(), y.toDouble());
                    auto *quote = new Quote(*color, shape_id, text, font_size, *p, ttl);
                    shapes.push_back(quote);
                }
            //Get all the fields to draw a circle and add it to shapes vector.
            } else if (tag == "Circle") {
                const QString diam = attr.value(QString("Diam"));
                const QString x = attr.value(QString("x"));
                const QString y = attr.value(QString("y"));
                if (!diam.isNull() && !x.isNull() && !y.isNull()) {
                    auto *p = new Point(x.toDouble(), y.toDouble());
                    auto *circle = new Circle(*color, shape_id, *p, diam.toDouble(), ttl);
                    shapes.push_back(circle);
                }
            //Get all the fields to draw a rectangle and add it to shapes vector.
            } else if (tag == "Rectangle") {
                const QString width = attr.value(QString("Width"));
                const QString height = attr.value(QString("Height"));
                const QString x = attr.value(QString("x"));
                const QString y = attr.value(QString("y"));
                if (!width.isNull() && !height.isNull() && !x.isNull() && !y.isNull()) {
                    auto *p = new Point(x.toDouble(), y.toDouble());
                    auto *rectangle = new Rectangle(*color, shape_id, *p, width.toDouble(), height.toDouble(), ttl);
                    shapes.push_back(rectangle);
                }
            //Get all the fields to draw a square and add it to shapes vector.
            } else if (tag == "Square") {
                const QString width = attr.value(QString("Width"));
                const QString x = attr.value(QString("x"));
                const QString y = attr.value(QString("y"));
                if (!width.isNull() && !x.isNull() && !y.isNull()) {
                    auto *p = new Point(x.toDouble(), y.toDouble());
                    auto *square = new Square(*color, shape_id, *p, width.toDouble(), ttl);
                    shapes.push_back(square);
                }
            //Move the type to get all the polygon points
            } else if (tag == "Polygon") {
                type = POINT;
                polygon_points = new QVector<QPointF>();
            //Get all the fields to draw a line and add it to shapes vector.
            } else if (tag == "Line") {
                const QString x0 = attr.value(QString("x0"));
                const QString y0 = attr.value(QString("y0"));
                const QString x1 = attr.value(QString("x1"));
                const QString y1 = attr.value(QString("y1"));
                if (!x0.isNull() && !y0.isNull() && !x1.isNull() && !y1.isNull()) {
                    auto *p0 = new Point(x0.toDouble(), y0.toDouble());
                    auto *p1 = new Point(x1.toDouble(), y1.toDouble());
                    auto *line = new Line(*color, shape_id, *p0, *p1, ttl);
                    shapes.push_back(line);
                }
            } }
            break;
        //Get all the polygon points
        case POINT: {
            const QString x = attr.value(QString("x"));
            const QString y = attr.value(QString("y"));
            QPointF *point;
            if (!x.isNull() && !y.isNull()) {
                point = new QPointF(x.toDouble(), y.toDouble());
            }
            polygon_points->push_back(*point);
        }
            break;
    }

    return TRUE;

}

bool CRQDrawHandler::endElement( const QString&, const QString&, const
QString& qName)
{
    /* process end tag */

    const QString &tag = qName;

    switch (type)
    {
        case UNKNOWN:
            break;
        case POINT:
            //Add the polygon to the shapes vector.
            if(tag == "Polygon") {
                auto *polygon = new Polygon(*color, shape_id, *polygon_points, ttl);
                shapes.push_back(polygon);
                type = SHAPES;
            }
            break;
    }
    return TRUE;
}

void CRQDrawHandler::setDocumentLocator(QXmlLocator *)
{

}

CRQDrawHandler::Type CRQDrawHandler::objectType()
{
    return type;
}
