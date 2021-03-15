/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "renderarea.h"
#include <QPainter>


RenderArea::RenderArea(QWidget *parent, int wbox, int hbox)
    : QWidget(parent), wbox(wbox), hbox(hbox) {
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    //this->pen=QPen(Qt::NoPen);
    this->pen= QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);

    updateBrushes();
}

void RenderArea::updateBrushes(void) {
    brushList.clear();

    //rempli la liste de brush, retire les doublons des coins
    for(int i=0; i< ((wbox*2) + (hbox*2) - 3) ; i++) {
        brushList << QBrush(Qt::magenta ,   Qt::SolidPattern);
    }
    qDebug("wh %dx%d", wbox, hbox );
    qDebug("Brushes count: %d", brushList.size() );

    // on réécrase des poitn d'intérets
    // first
    brushList[0] = QBrush(Qt::yellow, Qt::SolidPattern);
    //topleft (dernier 1ere ligne)
    brushList[wbox-1] = QBrush(Qt::green, Qt::SolidPattern);
    //bottomleft
    brushList[wbox-1+hbox-1] = QBrush(Qt::red, Qt::SolidPattern);
    //bottomright
    brushList[(2*(wbox-1))+hbox-1] = QBrush(Qt::blue, Qt::SolidPattern);
    //topright -1 (dernier pixel)
    brushList[ ((wbox*2) + (hbox*2) - 3) -2 ] = QBrush(Qt::cyan, Qt::SolidPattern);
}


QSize RenderArea::minimumSizeHint() const {
    return QSize(wbox*4, hbox*4);
}

QSize RenderArea::sizeHint() const {
    // offre en hint 4 pixels par box
    return QSize(wbox*4, hbox*4);
}


/* TODO FIXME  ces 2 fonctions ne devrait pas exister : ré-init les brush car pb de constructeur limité */
void RenderArea::widthUpdate(int w) {
    wbox = w==0 ? 1: w;
    updateBrushes();
}
void RenderArea::heightUpdate(int h) {
    hbox = h==0 ? 1: h;
    updateBrushes();
}

void RenderArea::setBrush(int idx, QColor color) {
    // avoid overflow
    if( (idx > (brushList.size()-1) ) || (idx < 0) ) {
        qDebug("ERROR RenderArea::setBrush Erreur idx %d > %d", idx, brushList.size() );
        return;
    }

    brushList[ idx ] = QBrush( color );
    // lance repaint() event
    update();
}

void RenderArea::paintEvent(QPaintEvent * /* event */) {

    QPainter painter(this);
    QRect rect;
    int incrW,incrH;
    int brushCounter = 0;

    // TODO FIXME BUG : CELA APPORT DES ERREUR D'ARRONDI !!!!!!!!!!!!!!!
    // et foire donc le compteur pour les brush !
    // et du coup en fonction de la taille et arrondi, ca plante !
    // 25-20 ok
    // 13-7 ok
    // 35-30 : KO Qlist OutOfRange
    incrW = width() / wbox;
    incrH = height() / hbox;

    //sécurité
    if(incrW==0) incrW=1;
    if(incrH==0) incrH=1;

//    qDebug("w %d incr %d - %d", width(), incrW, wbox);
//    qDebug("h %d incr %d - %d", height(), incrH, hbox);

    rect.setX(0);
    rect.setY(0);
    rect.setHeight(incrH);
    rect.setWidth(incrW);

    painter.setPen(pen);

    //! top (right to left)
    int yy = 0;
//    qDebug("Debut paint, idx: %d", brushCounter);
    for (int x = width()-incrW; x >= 0 ; x -= incrW) {
            painter.save();
            painter.translate(x, yy);

            painter.setBrush( brushList.at(brushCounter++) );
            painter.drawRect(rect);

            painter.restore();
    }
//    qDebug("end h1, idx: %d", brushCounter);
    //! left (top to bottom)

    int xx =0;
//    qDebug("stt v1, idx: %d", brushCounter);
    for (int y = incrH; y <= height()-(incrH*2); y += incrH) {
        painter.save();
        painter.translate(xx, y);

        painter.setBrush( brushList.at(brushCounter++) );
        painter.drawRect(rect);

        painter.restore();
    }
//    qDebug("end v1, idx: %d", brushCounter);

    //! bottom (left to righ)
    yy = height()-incrH;
//    qDebug("stt h2, idx: %d", brushCounter);
    for (int x = 0; x <= width()-incrW; x += incrW) {
        painter.save();
        painter.translate(x, yy);

        painter.setBrush( brushList.at(brushCounter++) );
        painter.drawRect(rect);

        painter.restore();
    }
//    qDebug("end h2, idx: %d", brushCounter);


    //! right (bottom to top)
    xx = width()-incrW;
//    qDebug("stt v2, idx: %d", brushCounter);
    for (int y = height()-(incrH*2); y >= incrH ; y -= incrH) {
        painter.save();
        painter.translate(xx, y);
        //Debug("y %d - brush idx %d", y, brushCounter);
        painter.setBrush( brushList.at(brushCounter++) );
        painter.drawRect(rect);

        painter.restore();
    }
    brushCounter--;
//    qDebug("end v2, idx: %d", brushCounter);

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}
