//
// Created by aspedrosa on 12/10/20.
//

#ifndef VIEWER_CRQROBOTCOMM_H
#define VIEWER_CRQROBOTCOMM_H

/*! \class CRQSimulatorComm
 * 	\brief Implementation of the comunication class.
 *
 * 	This class is responsible for all conections in the viewer.
 * 	It creates one socket and waits for simulator messages.
 * 	When a new message is received, it calls the respective parser.
 * 	After that, the respective functions of scene, lab and dataview
 * 	are called,	according to the type of information received.
 */

#include <map>
#include <unordered_map>
#include <chrono>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <QUdpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QHash>
#include "../Lab/crlab.h"
#include "../Lab/crrobot.h"
#include "../Lab/crgrid.h"
#include "../crqscene.h"
#include "../crqdataview.h"
#include "../crqlabview.h"
#include "crreply.h"
#include "crqcommhandler.h"

#include <QtGui>

namespace std {
    // define hash function for the QString class
    template<> struct hash<QString> {
        std::size_t operator()(const QString& s) const noexcept {
            return (size_t) qHash(s);
        }
    };
}


class CRQLabView;

typedef struct {
    QGraphicsItem* item;
    QString id;
} shape_info;

class CRQRobotComm : public QUdpSocket
{
    Q_OBJECT
public:
    /*!	This is the constructor.
     *	\param commScene the scene this class will work with.
     *	\param port the port to listen to drawing messages.
     *	\param data_view View holding the tree widget that displays the shapes filtering tree
     */
    CRQRobotComm(CRQScene *commScene, unsigned short port, CRQDataView *data_view);
    /*! This is the destructor.
     */
    ~CRQRobotComm();

public slots:
    /*! Function called by the notifier to process the received information.
     */
    void dataControler();

    /**
     * Slot to execute when a item is checked on the TreeWidget that filters shapes
     *
     * @param item the item checked
     * @param column parameter ignored
     */
    void filterItems(QTreeWidgetItem* item, int column);

private:
    /**
     * To ensure mutual exclusion
     */
    std::mutex m;
    std::condition_variable shapes_ttl;
    CRQScene *scene;			//Scene
    /**
     * \var Map for delete shapes after the time to live.
     */
    std::map<long, shape_info> ttd;
    /**
     * \var Map to save all the shapes that are currently drawn.
     */
    std::unordered_map<QString, QGraphicsItem*> ShapesDrawn;
    CRQDataView *data_view; //Data view
    /**
     * \var Thread for deleting shapes after the corresponding time to live.
     */
    std::thread *ttl_checker_thread;
    bool stop;
};

#endif //VIEWER_CRQAGENTCOMM_H
