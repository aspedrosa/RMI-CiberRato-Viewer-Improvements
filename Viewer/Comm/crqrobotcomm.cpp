//
// Created by aspedrosa on 12/10/20.
//

#ifdef MicWindows

#include <WinSock2.h>

#else

#include <sys/socket.h>
#include <stdlib.h>

#endif

#include <typeinfo>
#include <iostream>
#include <QString>
#include <qxml.h>
#include <QObject>
#include "crqrobotcomm.h"
#include "crqdrawhandler.h"

#include <QtGui>

using namespace std;

CRQRobotComm::CRQRobotComm()
{

}

/*============================================================================*/

CRQRobotComm::CRQRobotComm(CRQScene *commScene, unsigned short port): QUdpSocket() {
    scene = commScene;	// Scene passed by main

    QObject::connect (this, SIGNAL(readyRead()), SLOT(dataControler()));

    QHostAddress localAddress;
    localAddress.setAddress(QString( "0.0.0.0" ));

    if(bind( localAddress, port ) == FALSE ) {  //Bind for local address
        cerr << "Failed to assign address" << endl;
        exit (-1);
    }
}

/*============================================================================*/

CRQRobotComm::~CRQRobotComm()
{

}

/*============================================================================*/

void CRQRobotComm::dataControler() //Called when the socket receive something
{
    while (hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(pendingDatagramSize());
        if (readDatagram( datagram.data(), datagram.size()) == -1 ) //Read from socket
        {
            cerr << "Failure to read socket " << endl;
        }

        QXmlInputSource source;
        source.setData( QString( datagram.data() ) );
        // set commHandler for LAB, GRID and ROBOT
        CRQDrawHandler drawHandler;

        // parse received message with commHandler
        QXmlSimpleReader reader;
        reader.setContentHandler(&drawHandler);
        if(reader.parse(source)) {
            CRQDrawHandler::Type objectReceived = drawHandler.objectType(); //Object type received

            switch (objectReceived) {
                case CRQDrawHandler::SHAPES:
                    for (auto *shape: drawHandler.get_shapes()) {
                        QGraphicsItem *item;
                        std::cerr << dynamic_cast<Polygon*>(shape);
                        if (dynamic_cast<Ellipse*>(shape)) {
                            auto *circle = (Ellipse*) shape;
                            item = new QGraphicsEllipseItem(circle->get_p().get_x(), circle->get_p().get_y(), circle->get_diam_horizontal(), circle->get_diam_vertical(), 0, scene);
                        } else if (dynamic_cast<Rectangle*>(shape)) {
                            auto *rectangle = (Rectangle*) shape;
                            item = new QGraphicsRectItem(rectangle->get_p().get_x(), rectangle->get_p().get_y(), rectangle->get_width(), rectangle->get_height(), 0, scene);
                        } else if (dynamic_cast<Line*>(shape)) {
                            auto *line = (Line*) shape;
                            QGraphicsLineItem a;
                            item = new QGraphicsLineItem(line->get_p_begin().get_x(), line->get_p_begin().get_y(), line->get_p_end().get_x(), line->get_p_end().get_y(), 0, scene);
                        } else if (dynamic_cast<Quote*>(shape)) {
                            auto *text = (Quote*) shape;
                            item = new QGraphicsTextItem(text->get_text(), 0, scene);
                        } else if (dynamic_cast<Polygon*>(shape)){
                            auto *polygon = (Polygon*) shape;
                            for(auto &point: polygon->get_points()){
                                std::cerr << point.x();
                                std::cerr << point.y();
                                std::cerr << endl;
                            }
                            item = new QGraphicsPolygonItem(polygon->get_points(), 0, scene);
                        }

                        if (dynamic_cast<QAbstractGraphicsShapeItem*>(item)) {
                            auto *graphics_shape = (QAbstractGraphicsShapeItem*) item;
                            graphics_shape->setBrush( QBrush( shape->get_color()));
                        }
                        else if (dynamic_cast<QGraphicsLineItem*>(item)){
                            auto *line_item = (QGraphicsLineItem*) item;
                            line_item->setPen(QPen(shape->get_color()));
                        }
                        item->setVisible( true );
                        item->setZValue(7);
                        item->setOpacity(0.5);
                    }
                    break;
                case CRQDrawHandler::INTERNAL_KNOWLEDGE:
                    break;
            } // End of switch (selecciona o objecto recebido)

        } // End of if (caso o parser tenha funcionado)

        else
        {
            cerr << "Invalid message\n";
        }
    }
}
