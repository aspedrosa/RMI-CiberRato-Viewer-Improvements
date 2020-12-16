//
// Created by aspedrosa on 12/10/20.
//

#ifdef MicWindows

#include <WinSock2.h>

#else

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

#endif

#include <iostream>
#include <QString>
#include <qxml.h>
#include <QObject>
#include "crqrobotcomm.h"
#include "crqcommhandler.h"
#include "crqreplyhandler.h"

#include <QtGui>

using namespace std;

CRQRobotComm::CRQRobotComm()
{

}

/*============================================================================*/

CRQRobotComm::CRQRobotComm(CRQScene *commScene, QString h, unsigned short port_ , const char autoC, const char autoS)
        : QUdpSocket(), timer(this)
{
    autoConnect = autoC;
    autoStart = autoS;
    port = port_;
    scene = commScene;	// Scene passed by main
    host = h;
    isConnected = false;

    QObject::connect (this, SIGNAL(readyRead()), SLOT(dataControler()));

    struct hostent *host_ent ;
    struct in_addr *addr_ptr ;

    QByteArray hostLatin = host.toLatin1();
    if ((host_ent = (struct hostent *)gethostbyname(hostLatin.constData())) == NULL)
    {
        /* Check if a numeric address */
        if (inet_addr(hostLatin.constData()) == INADDR_NONE)
        {
            cerr << "Invalid server address\n";
            exit(-1);
        }
    }
    else
    {
        addr_ptr = (struct in_addr *) *host_ent->h_addr_list ;
        host = QString(QLatin1String( inet_ntoa(*addr_ptr) )) ;
    }

    serverAddress.setAddress( host ); //Server address

    QHostAddress localAddress;			//Local Address
    localAddress.setAddress( QString( "127.0.0.1" ));

    if( bind( localAddress, 0 ) == FALSE )	//Bind for local address
    {
        cerr << "Failed to assign address" << endl;
        exit (-1);
    }

    if(autoConnect == 'y')
        this->connect();

    // Connect Alarm event
    QObject::connect(&timer, SIGNAL(timeout()), SLOT(SendRequests()));
}

/*============================================================================*/

void CRQRobotComm::connect(void)
{
    if (isConnected)
        return;

#ifdef DEBUG
    cout << "CRQSimulatorComm::connect\n";
#endif

    //Registration <VIEW/>
    QObject::disconnect(this, SIGNAL(readyRead()), this, SLOT(dataControler()));
    QObject::connect(this, SIGNAL(readyRead()), SLOT(replyControler()));

    port=6000; // CORRIGIR!!!
    if( writeDatagram("<View/>\n", 9, serverAddress, port ) == -1 )
    {
        cerr << "Failure when writting <View/>" << endl;
        exit (-1);
    }

}

/*============================================================================*/

void CRQRobotComm::replyControler()
{
#ifdef DEBUG
    cout << "CRQSimulatorComm::replyControler\n";
#endif

    //char data[16384];
    //Read confirmation <REPLY STATUS="ok/refuse".../>

    //cerr << "reply controller \n";

    while (hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(pendingDatagramSize());
        if( readDatagram( datagram.data(), datagram.size(), &serverAddress, &port ) == -1 )
        {
            cerr << "Failure to read confirmation from the socket " << endl;
            exit (-1);
        }
        QXmlInputSource source;
        source.setData( QString( datagram.data() ) );

        /* set replyHandler */
        CRQReplyHandler replyHandler;

        /* parse reply message with replyHandler */
        QXmlSimpleReader reader;
        reader.setContentHandler(&replyHandler);
        if(reader.parse(source))
        {
            reply = replyHandler.reply(); // The status of request
            if( reply->status )   // REPLY = "ok"
            {
                QObject::disconnect(this, SIGNAL(readyRead()), this, SLOT(replyControler()));
                QObject::connect(this, SIGNAL(readyRead()), SLOT(dataControler()));

                // Initialize timer
                timer.setSingleShot(true);
                if(autoConnect == 'y')
                    timer.start(500);
                else
                    timer.start(3000);

                isConnected = true;
                // emit signal
                //emit viewerConnected(true);  // TODO
                return;
            }
            else
            {
                cerr << "Connection refused.\n";
                exit(1);
            }
        }
        else
        {
            cerr << "Invalid Reply message\n";
        }
    }


    isConnected = false;
    // emit viewerConnected(false);  // TODO
}

/*============================================================================*/

CRQRobotComm::~CRQRobotComm()
{

}

/*============================================================================*/

void CRQRobotComm::dataControler() //Called when the socket receive something
{
    //char data[16384];

    while (hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(pendingDatagramSize());
        if (readDatagram( datagram.data(), datagram.size(), &serverAddress, &port) == -1 ) //Read from socket
        {
            cerr << "Failure to read socket " << endl;
        }

        QXmlInputSource source;
        source.setData( QString( datagram.data() ) );
        // set commHandler for LAB, GRID and ROBOT
        CRQCommHandler commHandler;

        // parse received message with commHandler
        QXmlSimpleReader reader;
        reader.setContentHandler(&commHandler);
        if(reader.parse(source))
        {
            objectReceived = commHandler.objectType(); //Object type received

            switch (objectReceived)
            {
                // TODO
            } // End of switch (selecciona o objecto recebido)

        } // End of if (caso o parser tenha funcionado)

        else
        {
            cerr << "Invalid message\n";
        }
    }
}

//=============================================================================
void CRQRobotComm::sendMessage(const char *mensagem )
{
    if( writeDatagram( mensagem, strlen(mensagem) + 1, serverAddress, port ) == -1 )
    {
        cerr << "Failure writting msg" << endl;
        exit (-1);
    }
}