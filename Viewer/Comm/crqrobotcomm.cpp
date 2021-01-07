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
#include <chrono>
#include <thread>
#include <QString>
#include <qxml.h>
#include <QObject>
#include "crqrobotcomm.h"
#include "crqdrawhandler.h"

#include <QtGui>

#include <unistd.h>

using namespace std;

long getCurrentTime(){
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::microseconds>(now);
    auto epoch = now_ms.time_since_epoch();
    auto value = std::chrono::duration_cast<std::chrono::microseconds>(epoch);
    long duration = value.count();
    return duration;
}

[[noreturn]] void ttl_checker(
        std::map<long, shape_info> *ttd,
        std::unordered_map<QString, QGraphicsItem*> *ShapesDrawn,
        QGraphicsScene *scene,
        std::mutex *m,
        std::condition_variable *shapes_ttl,
        CRQDataView **data_view
        ) {

    std::unique_lock<std::mutex> lock(*m);
    while (1) {
        cerr << "working" << endl;
        if (ttd->empty()) {
            cerr << "waiting" << endl;
            shapes_ttl->wait(lock, [ttd]{return !ttd->empty();});
        }
        else {
            long now = getCurrentTime();
            auto first = ttd->begin();
            if (now > first->first) {
                QString id = first->second.id;
                QGraphicsItem* item = first->second.item;
                if (ShapesDrawn->count(id) > 0 && ShapesDrawn->at(id) == item) {  // TODO find()
                    scene->removeItem(item);
                    (*data_view)->removeItem(id);
                    ShapesDrawn->erase(id);
                }
                ttd->erase(first->first);
            }

            if (!ttd->empty()) {
                now = getCurrentTime();
                cerr << "sleeping " << ttd->begin()->first- now << endl;
                shapes_ttl->wait_for(lock, chrono::microseconds(ttd->begin()->first- now));
            }
        }
    }
}

/*============================================================================*/

CRQRobotComm::CRQRobotComm(CRQScene *commScene, unsigned short port, CRQDataView **data_view): QUdpSocket() {
    scene = commScene;	// Scene passed by main
    this->data_view = data_view;

    QObject::connect (this, SIGNAL(readyRead()), SLOT(dataControler()));

    QHostAddress localAddress;
    localAddress.setAddress(QString( "0.0.0.0" ));

    if(bind( localAddress, port ) == FALSE ) {  //Bind for local address
        cerr << "Failed to assign address" << endl;
        exit (-1);
    }

    new std::thread(ttl_checker, &ttd, &ShapesDrawn, scene, &m, &shapes_ttl, data_view);
}

/*============================================================================*/

CRQRobotComm::~CRQRobotComm()
{

}

/*
void delete_item(CRQScene *scene, std::unordered_map<int, QGraphicsItem*> *ShapesDrawn, unsigned int ttl, int id, QGraphicsItem *item) {
    cerr << "going to zzzzzzzz" << endl;

    this_thread::sleep_until(
        chrono::system_clock::now() + chrono::milliseconds(ttl)
    );

    cerr << "going to delete" << endl;

    if(ShapesDrawn->find(id)->second == item){
        scene->removeItem(item);
    }
}
 */

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
                        item->setZValue(8);
                        item->setOpacity(1);

                        {
                            std::lock_guard<std::mutex> lock(m);
                            bool notify = false;
                            if (ttd.empty() || ttd.begin()->first > getCurrentTime() + shape->getTTL()) {
                                notify = true;
                            }
                            if (ShapesDrawn.count(shape->getId()) == 1) {
                                scene->removeItem(ShapesDrawn[shape->getId()]);
                            }
                            ShapesDrawn[shape->getId()] = item;
                            ttd[getCurrentTime() + shape->getTTL() * 1000] = {item, shape->getId()};
                            if (*data_view != NULL) {
                                (*data_view)->addItem(shape->getId());
                            }
                            if (notify) {
                                shapes_ttl.notify_all();
                            }
                        }
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
