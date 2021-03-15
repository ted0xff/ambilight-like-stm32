#ifndef UDPLISTENER_H
#define UDPLISTENER_H

#include <QObject>
#include <QUdpSocket>
#include <QColor>

class udplistener: public QObject
{
    Q_OBJECT
public:
    explicit udplistener(QObject *parent = 0);
    ~udplistener();

signals:
    void newPixel(int idx, QColor color);
    void newPixels(QColor color);

public slots:
    void readyRead();

private:
    QUdpSocket *socket;

};

#endif // UDPLISTENER_H
