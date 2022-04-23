#include <QtWidgets>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#include <QPrintDialog>
#endif

#include "scribblearea.h"

#include <QDebug>

#define function(args...) [=](args)
#include <echoclient.h>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


EchoClient *echoClient2;
//! [0]
ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    modified = false;
    scribbling = false;
    myPenWidth = 2;
    myPenColor = Qt::blue;
    myPenColorStr = "blue";


    echoClient2 = new EchoClient;

    //echoClient2->open(QUrl("ws://192.168.1.96:8000"));
    echoClient2->open(QUrl("ws://fixblueperu.com"));
    //echoClient2->open(QUrl("ws://192.168.1.96:3001"));

    echoClient2->m_client.on("news", function(QJsonArray data) {
        //qDebug() << "event orecibido en interfaz" << data;
    });

    echoClient2->m_client.on("drawing", function(QJsonArray data) {

        QColor color;
        if(data.at(1).toObject().value("color").toString()=="green")
        {
            color.setRgb(0, 128, 0);
        }
        else if(data.at(1).toObject().value("color").toString()=="black")
        {
            color.setRgb(0, 0, 0);
        }
        else if(data.at(1).toObject().value("color").toString()=="blue")
        {
            color.setRgb(0, 0, 255);
        }
        else if(data.at(1).toObject().value("color").toString()=="red")
        {
            color.setRgb(255, 0, 0);
        }
        else if(data.at(1).toObject().value("color").toString()=="yellow")
        {
            color.setRgb(255, 255, 0);
        }
        onDrawingEvent(QPoint(data.at(1).toObject().value("x0").toVariant().toFloat()*this->width(),data.at(1).toObject().value("y0").toVariant().toFloat()*this->height()),QPoint(data.at(1).toObject().value("x1").toVariant().toFloat()*this->width(),data.at(1).toObject().value("y1").toVariant().toFloat()*this->height()),color);
    });


}
//! [0]


//! [5]
void ScribbleArea::setPenColor(const QColor &newColor, QString newColorStr)
//! [5] //! [6]
{
    myPenColor = newColor;
    myPenColorStr = newColorStr;
}
//! [6]

//! [7]
void ScribbleArea::setPenWidth(int newWidth)
//! [7] //! [8]
{
    myPenWidth = newWidth;
}
//! [8]

//! [9]
void ScribbleArea::clearImage()
//! [9] //! [10]
{
    image.fill(qRgb(255, 255, 255));
    modified = true;
    update();
}
//! [10]

//! [11]

void ScribbleArea::mousePressEvent(QMouseEvent *event)
//! [11] //! [12]
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        scribbling = true;
    }
}


void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling)
    {
        drawLineTo(event->pos());
    }
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling) {
        scribbling = false;
        drawLineTo(event->pos());
    }
}

//! [12] //! [13]
void ScribbleArea::paintEvent(QPaintEvent *event)
//! [13] //! [14]
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}
//! [14]

//! [15]
void ScribbleArea::resizeEvent(QResizeEvent *event)
//! [15] //! [16]
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}
//! [16]

//! [17]
void ScribbleArea::drawLineTo(const QPoint &endPoint)
//! [17] //! [18]
{
    QJsonObject body1;

    body1.insert("x0",QVariant(lastPoint.x()).toFloat()/QVariant(this->width()).toFloat());
    body1.insert("y0",QVariant(lastPoint.y()).toFloat()/QVariant(this->height()).toFloat());
    body1.insert("x1",QVariant(endPoint.x()).toFloat()/QVariant(this->width()).toFloat());
    body1.insert("y1",QVariant(endPoint.y()).toFloat()/QVariant(this->height()).toFloat());
    body1.insert("color", myPenColorStr);

    echoClient2->m_client.emitMessage("drawing",body1);

    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);




    modified = true;

    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void ScribbleArea::onDrawingEvent(const QPoint &lastPoint1, const QPoint &endPoint, const QColor color)
{
    QPainter painter(&image);

    painter.setPen(QPen(color, myPenWidth,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));

    /*
        NoBrush,
        SolidPattern,
        Dense1Pattern,
        Dense2Pattern,
        Dense3Pattern,
        Dense4Pattern,
        Dense5Pattern,
        Dense6Pattern,
        Dense7Pattern,
        HorPattern,
        VerPattern,
        CrossPattern,
        BDiagPattern,
        FDiagPattern,
        DiagCrossPattern,
        LinearGradientPattern,
        RadialGradientPattern,
        ConicalGradientPattern,
        TexturePattern = 24
    */
    painter.setBrush(Qt::BrushStyle::Dense7Pattern);

    painter.drawLine(lastPoint1, endPoint);
    modified = true;

    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint1, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));

}
//! [18]

//! [19]
void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
//! [19] //! [20]
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}
