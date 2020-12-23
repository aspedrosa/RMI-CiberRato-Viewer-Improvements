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
#include <QUdpSocket>
#include <QHostAddress>
#include <QTimer>
#include "../Lab/crlab.h"
#include "../Lab/crrobot.h"
#include "../Lab/crgrid.h"
#include "../crqscene.h"
#include "../crqdataview.h"
#include "../crqlabview.h"
#include "crreply.h"
#include "crqcommhandler.h"

#include <QtGui>


class CRQLabView;

typedef struct {
    QGraphicsItem* item;
    int id;
} shape_info;

class CRQRobotComm : public QUdpSocket
{
    Q_OBJECT
public:
    /*!	This is the constructor.
     *	\param commScene the scene this class will work with.
     *	\param commLab the lab this class will work with.
     *	\param host the address of simulator.
     *	\param port the port of simulator.
     */
    CRQRobotComm(CRQScene *commScene, unsigned short port);
    /*! This is the destructor.
     */
    ~CRQRobotComm();

    /*! One constructor without parameters.
     */
    CRQRobotComm();

public slots:
    /*! Function called by the notifier to process the received information.
     */
    void dataControler();

private:
    CRQScene *scene;			//Scene
    std::map<time_t, shape_info> ttd;
    std::unordered_map<int, QGraphicsItem*> ShapesDrawn;
    // TODO concurrent maps
};

#endif //VIEWER_CRQAGENTCOMM_H
