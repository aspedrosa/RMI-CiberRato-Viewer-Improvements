
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
        // <Ellipse diam="3" x="3"></Ellipse>
        case UNKNOWN:
            if (tag == "Shapes") {
                type = SHAPES;
            } else if (tag == "InternalKnowledge") {
                type = INTERNAL_KNOWLEDGE;
            }
            break;
        case SHAPES: {
            const QString red_color = attr.value(QString("red"));
            const QString green_color = attr.value(QString("green"));
            const QString blue_color = attr.value(QString("blue"));
            if (red_color.isNull() || green_color.isNull() || blue_color.isNull()) {
                // TODO missing value
                return TRUE;
            } else {
                color = new QColor(red_color.toInt(), green_color.toInt(), blue_color.toInt());
            }

            if (tag == "Ellipse") {
                const QString diam_vertical = attr.value(QString("Diam_vertical"));
                const QString diam_horizontal = attr.value(QString("Diam_horizontal"));
                const QString x = attr.value(QString("x"));
                const QString y = attr.value(QString("y"));
                if (!diam_vertical.isNull() && !diam_horizontal.isNull() && !x.isNull() && !y.isNull()) {
                    auto *p = new Point(x.toDouble(), y.toDouble());
                    shapes.push_back(Ellipse(*color, *p, diam_vertical.toDouble(), diam_horizontal.toDouble()));
                }
            } else if (tag == "Text") {
                QString text = attr.value(QString("text"));
                if (!text.isNull()) {
                    shapes.push_back(Text(*color, text));
                }
            } else if (tag == "Circle") {
                const QString diam = attr.value(QString("Diam"));
                const QString x = attr.value(QString("x"));
                const QString y = attr.value(QString("y"));
                if (!diam.isNull() && !x.isNull() && !y.isNull()) {
                    auto *p = new Point(x.toDouble(), y.toDouble());
                    shapes.push_back(Circle(*color, *p, diam.toDouble()));
                }
            } else if (tag == "Rectangle") {
                const QString width = attr.value(QString("Width"));
                const QString height = attr.value(QString("Width"));
                const QString x = attr.value(QString("x"));
                const QString y = attr.value(QString("y"));
                if (!width.isNull() && !height.isNull() && !x.isNull() && !y.isNull()) {
                    auto *p = new Point(x.toDouble(), y.toDouble());
                    shapes.push_back(Rectangle(*color, *p, width.toDouble(), height.toDouble()));
                }
            } else if (tag == "Square") {
                const QString width = attr.value(QString("Width"));
                const QString x = attr.value(QString("x"));
                const QString y = attr.value(QString("y"));
                if (!width.isNull() && !x.isNull() && !y.isNull()) {
                    auto *p = new Point(x.toDouble(), y.toDouble());
                    shapes.push_back(Square(*color, *p, width.toDouble()));
                }
            } else if (tag == "Polygon") {
                type = POINT;
                polygon_points = new QVector<QPointF>();
            } else if (tag == "Line") {
                const QString x0 = attr.value(QString("x0"));
                const QString y0 = attr.value(QString("y0"));
                const QString x1 = attr.value(QString("x1"));
                const QString y1 = attr.value(QString("y1"));
                if (!x0.isNull() && !y0.isNull() && !x1.isNull() && !y1.isNull()) {
                    auto *p0 = new Point(x0.toDouble(), y0.toDouble());
                    auto *p1 = new Point(x1.toDouble(), y1.toDouble());
                    shapes.push_back(Line(*color, *p0, *p1));
                }
            } }
            break;
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
        case INTERNAL_KNOWLEDGE:
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
            if(tag == "Polygon") {
                shapes.push_back(Polygon(*color, *polygon_points));
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
