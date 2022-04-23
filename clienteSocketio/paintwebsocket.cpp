#include "paintwebsocket.h"
#include "ui_paintwebsocket.h"

#include <scribblearea.h>

#include <QHBoxLayout>

ScribbleArea *scribbleArea;
PaintWebSocket::PaintWebSocket(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PaintWebSocket)
{
    ui->setupUi(this);
    scribbleArea = new ScribbleArea;


    QHBoxLayout *hb = new QHBoxLayout();
    hb->setMargin(0);
    hb->addWidget(scribbleArea);

    ui->parapinta->setLayout(hb);
    //setCentralWidget(scribbleArea);

    //scribbleArea->drawLineTo(QPoint(350,227));

}

PaintWebSocket::~PaintWebSocket()
{
    delete ui;
}

void PaintWebSocket::on_btnNegro_clicked()
{
    scribbleArea->setPenColor(QColor(0,0,0),"black");

}

void PaintWebSocket::on_btnRojo_clicked()
{
    scribbleArea->setPenColor(QColor(255, 0, 0),"red");
}

void PaintWebSocket::on_btnVerde_clicked()
{
   scribbleArea->setPenColor(QColor(0, 128, 0),"green");
}

void PaintWebSocket::on_btnAzul_clicked()
{
   scribbleArea->setPenColor(QColor(0, 0, 255),"blue");
}

void PaintWebSocket::on_btnAmarillo_clicked()
{
    scribbleArea->setPenColor(QColor(255, 255, 0),"yellow");
}
