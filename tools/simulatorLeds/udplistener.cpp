#include "udplistener.h"
#include <QtEndian>

udplistener::udplistener(QObject *parent):
    QObject(parent)
{

    socket = new QUdpSocket(this);

    if( socket->bind(QHostAddress::LocalHost, 1200) == false) {
        qDebug("ERROR UDP cannot bind() to port 1200 !!!!");
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

udplistener::~udplistener() {
    socket->abort();
    socket->deleteLater();
}

void udplistener::readyRead()
{
    // when data comes in
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;

    // qint64 QUdpSocket::readDatagram(char * data, qint64 maxSize,
    //                 QHostAddress * address = 0, quint16 * port = 0)
    // Receives a datagram no larger than maxSize bytes and stores it in data.
    // The sender's host address and port is stored in *address and *port
    // (unless the pointers are 0).

    socket->readDatagram(buffer.data(), buffer.size(),
                         &sender, &senderPort);

//    qDebug() << "Message from: " << sender.toString();
//    qDebug() << "Message port: " << senderPort;
//    qDebug() << "Message size: " << buffer.size();
//    qDebug() << "Message: " << buffer << "\n";

    int idx,count;
    uint8_t r,g,b;

    //header "AdA" non traité et count supposé de 1 pixel toujours :
    //TODO FIXME: amélioration : optimisation échanges ...
    // on ne gere qu'un seul pixel par message

    //check basic header start 'Ad'
    if ( buffer.mid(0,2).compare(QByteArray("Ad")) == 0 ){
        if(buffer.at(2) == 'A') {
            // AdA
            count= qFromLittleEndian<qint16>(buffer.mid(3,2).data());
            idx= qFromLittleEndian<qint16>(buffer.mid(5,2).data());
            r=buffer.at(8);
            g=buffer.at(9);
            b=buffer.at(10);
    //    qDebug("Pixel cnt%d idx%d (0x%02x,0x%02x,0x%02x)",count, idx,r,g,b);
            emit newPixel(idx, QColor(r,g,b));
        }else if(buffer.at(2) == 'B') {
            int cmd= qFromLittleEndian<qint16>(buffer.mid(3,2).data());
            switch(cmd) {
            case 1 : /* solid color */
                r=buffer.at(5);
                g=buffer.at(6);
                b=buffer.at(7);
                //qDebug("AdB cmd %d solid (0x%02x,0x%02x,0x%02x)",cmd,r,g,b);
                emit newPixels( QColor(r,g,b) );
                break;
            default:
                qDebug("ERROR not managed AdB command : %d", cmd );
                break;
            }
        } else {
            qDebug("ERROR not managed UDP header : %s", buffer.mid(0,3).data() );
        }
    } else {
        qDebug("ERROR wrong UDP header : %s", buffer.mid(0,2).data() );
    }
}
