//eu
#ifndef VIEWER_CRQDRAWHANDLER_H
#define VIEWER_CRQDRAWHANDLER_H

/*! \class CRQCommHandler
 *  \brief This class implements one simple parser for information in XML
 *  format.
 *  This is one parser prepared to receive information that represents one
 *  robot, one lab or one grid.
 */

#include <vector>
#include <qxml.h>
#include "../Lab/crrobot.h"
#include "../Lab/crgrid.h"
#include "../Lab/crlab.h"
#include "../Lab/crwall.h"
#include "../Lab/crtarget.h"
#include "../Lab/crvertice.h"
#include "../Lab/crbeacon.h"
#include "../Lab/crshape.h"

#define DEFAULT_SHAPE_TTL 5000;

class QString;
class QXmlLocator;

class CRQDrawHandler : public QXmlDefaultHandler
{
public:
    /*! \enum Type
     *  This enum type represent the object received.
     */
    enum Type {UNKNOWN, SHAPES, POINT};
    /*! This function will be called in the begining of the XML document.
     */
    bool startDocument();
    /*! This function is called in the end of the XML document.
     */
    bool endDocument();
    /*! This function is called when new tag arrive. It is inside this function,
     * that all parsing is done. According to the tag received, a new object
     * will be created.
     */
    bool startElement( const QString&, const QString&, const QString& ,
                       const QXmlAttributes& );
    /*! This function is called at the end of one tag. Inside, if is necessary,
     * some elements are added to anothers.
     */
    bool endElement( const QString&, const QString&, const QString& );
    /*! For internal use only.
     */
    void setDocumentLocator(QXmlLocator *);
    /*! This function returns one Lab.
     */
    Type objectType();

    /*!
     * Get the shapes that need to be drawn.
     * @return The vector of objects that represent shapes.
     */
    vector<Shape*>& get_shapes() {
        return this->shapes;
    }

private:
    /*!
     * \var Type of message.
     */
    Type type;
    /*!
     * \var Vector of shapes.
     */
    vector<Shape*> shapes;
    /*!
     * \var Shape color.
     */
    QColor *color;
    /*!
     * \var Identifier for the shape.
     */
    QString shape_id;
    /*!
     * \var Time to live for the shape.
     */
    unsigned int ttl;
    /*!
     * Vector of points for the polygon.
     */
    QVector<QPointF> *polygon_points;
};

#endif //VIEWER_CRQDRAWHANDLER_H
