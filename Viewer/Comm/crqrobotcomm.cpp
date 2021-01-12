//
// Created by aspedrosa on 12/10/20.
//

#ifdef MicWindows

#include <WinSock2.h>

#else

#include <sys/socket.h>
#include <stdlib.h>

#endif

#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <QString>
#include <qxml.h>
#include <QObject>
#include "crqrobotcomm.h"
#include "crqdrawhandler.h"

#include <QtGui>

#include <unistd.h>

using namespace std;

/**
 * Auxiliary function to return the current time since epoch in microseconds
 *
 * @return current timestamp in microseconds
 */
long getCurrentTime(){
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::microseconds>(now);
    auto epoch = now_ms.time_since_epoch();
    auto value = std::chrono::duration_cast<std::chrono::microseconds>(epoch);
    long duration = value.count();
    return duration;
}

/**
 * Thread in charge of deleting shapes after their time-to-live (ttl) time.
 * Also, when the ttl reaches the end this thread will remove the respective
 *  item from the TreeWidget
 *
 * @param m mutex to access shared structured with the main thread
 * @param stop if this thread should exit
 * @param ttd shared map of shapes ordered by their ttl
 * @param ShapesDrawn shared map to get the shape's QGraphicsItem by its id
 * @param shapes_ttl Condition where this thread sleeps:
 *  -the smallest ttl of all shapes;
 *  or
 *  -until a new shape is drawn;
 * @param data_view View holding the tree widget that displays the shapes filtering tree
 */
[[noreturn]] void ttl_checker(
        std::mutex *m,
        const bool *stop,
        std::map<long, shape_info> *ttd,
        std::unordered_map<QString, QGraphicsItem*> *ShapesDrawn,
        QGraphicsScene *scene,
        std::condition_variable *shapes_ttl,
        CRQDataView *data_view
        ) {

    std::unique_lock<std::mutex> lock(*m);
    while (!*stop) {
        if (ttd->empty()) {
            // wait until a new shape is added
            shapes_ttl->wait(lock, [stop, ttd]{return *stop || !ttd->empty();});
        }
        else {
            long now = getCurrentTime();
            auto smallest_ttl = ttd->begin();
            if (now > smallest_ttl->first) {
                QString id = smallest_ttl->second.id;
                QGraphicsItem* item = smallest_ttl->second.item;
                if (ShapesDrawn->count(id) > 0 && ShapesDrawn->at(id) == item) {
                    scene->removeItem(item);
                    data_view->removeItem(id);
                    ShapesDrawn->erase(id);
                }
                ttd->erase(smallest_ttl->first);
            }

            if (!ttd->empty()) {  // if there is still shapes still for the smallest ttl
                now = getCurrentTime();
                long sleep_time = ttd->begin()->first - now;
                if (sleep_time <= 0) {  // skip if the smallest ttl expired
                    continue;
                }
                shapes_ttl->wait_for(lock, chrono::microseconds(sleep_time));
            }
        }
    }
    lock.unlock();
}

/*============================================================================*/

CRQRobotComm::CRQRobotComm(CRQScene *commScene, unsigned short port, CRQDataView *data_view): QUdpSocket() {
    scene = commScene;	// Scene passed by main
    this->data_view = data_view;
    stop = false;

    QObject::connect (this, SIGNAL(readyRead()), SLOT(dataControler()));

    QObject::connect (
        data_view->getFilterTreeWidget(),
        SIGNAL(itemChanged(QTreeWidgetItem*, int)),
        SLOT(filterItems(QTreeWidgetItem*, int))
    );

    QHostAddress localAddress;
    localAddress.setAddress(QString( "0.0.0.0" ));

    if(bind( localAddress, port ) == FALSE ) {  //Bind for local address
        cerr << "Failed to assign address" << endl;
        exit (-1);
    }

    //Creates the thread
    ttl_checker_thread = new std::thread(ttl_checker, &m, &stop, &ttd, &ShapesDrawn, scene, &shapes_ttl, data_view);
}

/*============================================================================*/

CRQRobotComm::~CRQRobotComm()
{
    // tell the ttl_checker thread to exit
    stop = true;
    shapes_ttl.notify_all();

    ttl_checker_thread->join();

    QObject::disconnect(
            data_view->getFilterTreeWidget(),
            SIGNAL(itemChanged(QTreeWidgetItem*, int)),
            this,
            SLOT(filterItems(QTreeWidgetItem*, int))
    );
}

void CRQRobotComm::filterItems(QTreeWidgetItem *item, int _) {
    if(item->parent() != NULL && !item->parent()->checkState(0) && item->checkState(0)){
        // if the parent of the current item is unchecked then this item should also be unchecked
        item->setCheckState(0, Qt::Unchecked);
        return;
    }

    QString item_id = item->text(0);
    QTreeWidgetItem* item_tmp = item->parent();
    while(item_tmp != NULL) {  // go up in the filtering tree to build the shape's id
        item_id = item_tmp->text(0) + "." + item_id;
        item_tmp = item_tmp->parent();
    }

    for (int child_index = 0; child_index < item->childCount(); child_index++) {
        // check this item's children if itself is checked
        item->child(child_index)->setCheckState(0, item->checkState(0) ? Qt::Checked : Qt::Unchecked);
        // disable this item's children if itself is unchecked
        item->child(child_index)->setDisabled(!item->checkState(0));
    }

    auto graphics_item = ShapesDrawn.find(item_id);
    if (item->childCount() == 0 && graphics_item != ShapesDrawn.end()) {
        // apply the check state to the shape
        graphics_item->second->setVisible(item->checkState(0));
    }
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
                        //Draw the corresponding shape depending on his subclass
                        QGraphicsItem *item;
                        if (dynamic_cast<Ellipse*>(shape)) {
                            auto *circle = (Ellipse*) shape;
                            item = new QGraphicsEllipseItem(circle->get_p().get_x(), circle->get_p().get_y(scene->lab->labSize().y()), circle->get_diam_horizontal(), circle->get_diam_vertical(), 0);
                        } else if (dynamic_cast<Rectangle*>(shape)) {
                            auto *rectangle = (Rectangle*) shape;
                            item = new QGraphicsRectItem(rectangle->get_p().get_x(), rectangle->get_p().get_y(scene->lab->labSize().y()), rectangle->get_width(), rectangle->get_height(), 0);
                        } else if (dynamic_cast<Line*>(shape)) {
                            auto *line = (Line*) shape;
                            QGraphicsLineItem a;
                            item = new QGraphicsLineItem(line->get_p_begin().get_x(), line->get_p_begin().get_y(scene->lab->labSize().y()), line->get_p_end().get_x(), line->get_p_end().get_y(scene->lab->labSize().y()), 0);
                        } else if (dynamic_cast<Quote*>(shape)) {
                            auto *quote = (Quote*) shape;
                            item = new QGraphicsTextItem(quote->get_text(), 0);
                            QFont f;
                            f.setPixelSize(quote->get_font_size());
                            auto *text = (QGraphicsTextItem*) item;
                            text->setPos(quote->get_p().get_x(),quote->get_p().get_y(scene->lab->labSize().y()));
                            text->setFont(f);
                            text->setDefaultTextColor(shape->get_color());
                        } else if (dynamic_cast<Polygon*>(shape)){
                            auto *polygon = (Polygon*) shape;
                            item = new QGraphicsPolygonItem(polygon->get_points(scene->lab->labSize().y()), 0);
                        }
                        //Paint regualar shapes & text
                        if (dynamic_cast<QAbstractGraphicsShapeItem*>(item)) {
                            auto *graphics_shape = (QAbstractGraphicsShapeItem*) item;
                            graphics_shape->setBrush( QBrush( shape->get_color()));
                        }
                        //Paint if shape is a line
                        else if (dynamic_cast<QGraphicsLineItem*>(item)){
                            auto *line_item = (QGraphicsLineItem*) item;
                            line_item->setPen(QPen(shape->get_color()));
                        }
                        //Ensures shape visibility
                        item->setVisible(data_view->isItemChecked(shape->getId()));
                        item->setZValue(8);
                        item->setOpacity(1);

                        {
                            std::lock_guard<std::mutex> lock(m);
                            bool notify = ttd.empty() || ttd.begin()->first > getCurrentTime() + shape->getTTL() * 1000;
                            //Verify if already exists a shape with this identifier
                            if (ShapesDrawn.count(shape->getId()) == 1) {
                                scene->removeItem(ShapesDrawn[shape->getId()]);
                            }
                            //Add to the shapes draw
                            ShapesDrawn[shape->getId()] = item;
                            ttd[getCurrentTime() + shape->getTTL() * 1000] = {item, shape->getId()};
                            data_view->addItem(shape->getId());
                            if (notify) {
                                shapes_ttl.notify_all();
                            }
                            scene->addItem(item);
                        }
                    }
                    break;
            } // End of switch (selecciona o objecto recebido)

        } // End of if (caso o parser tenha funcionado)

        else
        {
            cerr << "Invalid message\n";
        }
    }
}
