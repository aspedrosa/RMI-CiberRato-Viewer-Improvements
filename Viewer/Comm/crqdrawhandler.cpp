
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
            }
            else if (tag == "InternalKnowledge") {
                type = INTERNAL_KNOWLEDGE;
            }
            break;
        case SHAPES:
            if (tag == "Ellipse") {
                // attr.value(QString(""))  TODO
            }
            // else  TODO
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
        case ELLIPSE:
            break;
        case RECT:
            break;
        case LINE:
            break;
        case TEXT:
            break;
        case POLYGON:
            break;
    }
    return TRUE;
}

void CRQDrawHandler::setDocumentLocator(QXmlLocator *)
{

}

CRQDrawHandler::Type CRQCommHandler::objectType()
{
    return type;
}
