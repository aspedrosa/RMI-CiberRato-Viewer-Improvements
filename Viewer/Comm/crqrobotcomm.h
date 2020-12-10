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
    CRQRobotComm(CRQScene *commScene, QString host, unsigned short port, const char autoC, const char autoS);
    /*! This is the destructor.
     */
    ~CRQRobotComm();

    /*! One constructor without parameters.
     */
    CRQRobotComm( );

    /*! This function will send information to simulator. It will be used by
    *  the control panel.
    * \param msg the message to send.
    */
    void sendMessage( const char * msg );

    /*! Send the registration message to simulator and and wait for response.
     * If OK, the aplication continue. Also connect the signal of notifier
     * to slot replyControler.
     * \sa replyControler
     */
    void connect(void);

public slots:
    /*! Function called by the notifier to process the received information.
     */
    void dataControler();

    /*! Function called by notifier to process the received information.
     * If the reply is OK, this function connect the signal of notifier
     * to the slot dataControler.
     * \sa dataControler
     */
    void replyControler();

private:
    char autoConnect;
    char autoStart;
    CRReply *reply;
    CRQScene *scene;			//Scene
    QHostAddress serverAddress; //Server Address
    unsigned short port;
    CRQCommHandler::Type objectReceived;
    QString host;
    QTimer timer;
    bool isConnected;

};

#endif //VIEWER_CRQAGENTCOMM_H
