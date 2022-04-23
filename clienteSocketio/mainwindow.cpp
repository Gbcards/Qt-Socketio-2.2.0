#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <echoclient.h>

EchoClient *echoClient;


#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QDebug>

#define function(args...) [=](args)


#include <QtMath>
#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    echoClient = new EchoClient;
    //echoClient->open(QUrl("ws://192.168.1.96:8000"));

    echoClient->open(QUrl("ws://192.168.1.96:3060"));

    echoClient->m_client.on("news", function(QJsonArray data) {
        qDebug() << "event orecibido en interfaz" << data;
    });

    echoClient->m_client.on("prueba", function(QJsonArray data) {
        qDebug() << "event orecibido en interfaz" << data;
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

/*
            echoClient->m_client.emitMessage("MSG",
                                 QVariantMap({ {"order", "REG"},
                                               {QStringLiteral("host"), QStringLiteral("123456")},
                                               {QStringLiteral("device"),"desktop"},
                                               {QStringLiteral("deviceinfo"),"hola mundo xd"}}),
                                 function(QJsonArray returnValue) {
                qDebug() << "Got reply from event with a json object:" << returnValue;
            });
*/



    echoClient->m_client.emitMessage("REG",QString("hola mundo JAAAAAA desde QT c++ xd :V"));

    /*

    QJsonObject body;

    body.insert("x0", 0.4216691068814056);
    body.insert("y0",0.716323296354992);
    body.insert("x1",0.4216691068814056);
    body.insert("y1", 0.716323296354992);
    body.insert("color", "blue");

    echoClient->m_client.emitMessage("drawing",body);


    QJsonObject body1;

    body1.insert("x0", 0.4216691068814056);
    body1.insert("y0",0.716323296354992);
    body1.insert("x1",0.4224011713030747);
    body1.insert("y1", 0.716323296354992);
    body1.insert("color", "blue");

    echoClient->m_client.emitMessage("drawing",body1);

    QJsonObject body2;

    body2.insert("x0", 0.4224011713030747);
    body2.insert("y0",0.716323296354992);
    body2.insert("x1",0.4253294289897511);
    body2.insert("y1", 0.7099841521394612);
    body2.insert("color", "blue");

    echoClient->m_client.emitMessage("drawing",body2);


    QJsonObject body3;

    body3.insert("x0", 0.4253294289897511);
    body3.insert("y0",0.7099841521394612);
    body3.insert("x1",0.42752562225475843);
    body3.insert("y1", 0.7068145800316957);
    body3.insert("color", "blue");

    echoClient->m_client.emitMessage("drawing",body3);


    echoClient->m_client.on("drawing",function(QJsonArray returnValue) {
        qDebug() << "Got reply from event with a json object:" << returnValue;
    });
*/

}
