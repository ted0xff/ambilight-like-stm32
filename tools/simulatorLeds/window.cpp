/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
**
****************************************************************************/

#include "renderarea.h"
#include "window.h"

#include <QtWidgets>

/** Idealemenet devrait etre configurable et changeable dynamiquement **/
/**  TODO FIXME : le resize initial de renderaea pose pb si ca trop petit !!  */
#define D_WIDTH_COUNT_BOX  (100)
#define D_HEIGHT_COUNT_BOX (80)

Window::Window() : counterAkey( 8 /* 1er test en 8*/), currentColor(QColor(0xAA,00,0xFF)){
    renderArea = new RenderArea(0, D_WIDTH_COUNT_BOX, D_HEIGHT_COUNT_BOX);

    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->setColumnStretch(0, 1);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(renderArea, 0, 0, 1, 1);
    setLayout(mainLayout);

#ifdef M_RELEASE
    setWindowFlags(Qt::FramelessWindowHint);
#else
    setWindowTitle(tr("Color boxes"));
#endif

    qDebug("Press Key Q to exit");

    // TODO : dans destructeur faudrait del !
    socketUDPListener = new udplistener();

    connect(socketUDPListener, SIGNAL(newPixel(int,QColor)), this, SLOT(setPixel(int,QColor)));
    connect(socketUDPListener, SIGNAL(newPixels(QColor)), this, SLOT(setAllPixels(QColor)));
}

// Cette fonction/Slot à appeler pour MaJ d'un pixel !
void Window::setPixel(int idx, QColor color) {
    renderArea->setBrush(idx, color);
}

void Window::setAllPixels(QColor color) {
    for(int idx=0; idx< ((m_w*2)+(m_h*2)-3) ; idx++) {
        renderArea->setBrush(idx, color);
    }
}

void Window::keyPressEvent(QKeyEvent *event) {

    switch ( event->key() ) {
    case Qt::Key_Q:
        // close application
        close();
        break;

    // test changements  event brush
    case Qt::Key_A:
        setPixel(counterAkey, currentColor);
        counterAkey++;
        break;

    case Qt::Key_R: // reset + RED
        counterAkey = 0;
        currentColor = QColor(0xFF,00,0x00);
        break;
    case Qt::Key_G: // reset + Green
        counterAkey = 0;
        currentColor = QColor(0x00,0xFF,0x00);
        break;
    case Qt::Key_B: // reset + Blue
        counterAkey = 0;
        currentColor = QColor(0x00,0x00,0xFF);
        break;
    case Qt::Key_Y: // reset + Yellow
        counterAkey = 0;
        currentColor = QColor(0xff,0xff,0x00);/*Qt::yellow*/
        break;
    case Qt::Key_W: // reset + White
        counterAkey = 0;
        currentColor = QColor(0xff,0xff,0xFF);
        break;

    case Qt::Key_PageDown: // lower tone (pas de gestion underflow !)
        counterAkey = 0;
        currentColor = QColor(currentColor.red()/2,currentColor.green()/2,currentColor.blue()/2);
        break;

    case Qt::Key_PageUp: // upper tone (pas de gestion overflow !)
        counterAkey = 0;
        currentColor = QColor(currentColor.red()*2,currentColor.green()*2,currentColor.blue()*2);
        break;

    default:
        qDebug("You Pressed Other Key");
        break;
    }
}

void Window::resizeEvent(QResizeEvent * /*event*/) {
    //quand resize, fait en sorte d'etre multiple qui va bien
    int w,h;
    int wprev,hprev;
    wprev = width();
    hprev = height();

    // ca fait arrondi et align
    w = wprev / m_w;
    w = w * m_w;
    // ca fait arrondi et align
    h = hprev / m_h;
    h = h * m_h;

    // Evite le risque la recursion !
    if( (h != hprev) || (w != wprev)) {
        //qDebug("resize %dx%d", w,h);
        // on resize au multiple qui va bien
        resize(w,h);
    }
}

// découpage en box
//TODO FIXME : Cette focntion n'a pas de sens : faudrait un meilleur constructeur
void Window::setWH(int w, int h) {
      renderArea->widthUpdate(w);
      renderArea->heightUpdate(h);
      m_w = w;
      m_h = h;
}
