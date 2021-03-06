#include "echoclient.h"

//#include <QtSocketIo/QSocketIOClient>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QDebug>

#define function(args...) [=](args)

EchoClient::EchoClient(QObject *parent) :
    QObject(parent),
    m_client()
{
    QObject::connect(&m_client, SIGNAL(connected(QString)), this, SLOT(connected(QString)));
    QObject::connect(&m_client, SIGNAL(disconnected(QString)), this, SLOT(disconnected(QString)));
    QObject::connect(&m_client, SIGNAL(errorReceived(QString,QString)), this, SLOT(errorReceived(QString,QString)));
    QObject::connect(&m_client, SIGNAL(heartbeatReceived()), this, SLOT(heartbeatReceived()));
    QObject::connect(&m_client, SIGNAL(messageReceived(QString)), this, SLOT(messageReceived(QString)));
}

EchoClient::~EchoClient()
{
}

void EchoClient::open(QUrl url)
{
    m_client.open(url);
}

void EchoClient::messageReceived(QString message)
{
    qDebug() << "Message Received:" << message;
}

void EchoClient::errorReceived(QString reason, QString advice)
{
    qDebug() << "Error received:" << reason << "(advice" << advice << ")";
}

void EchoClient::ackReceived(int messageId, QJsonArray arguments)
{
    qDebug() << "Ack received for message with id" << messageId << "arguments:" << arguments;
}

void EchoClient::connected(QString endpoint)
{
    qDebug() << "Connected to endpoint" << endpoint;
    /*
    m_client.emitMessage("REG",
                         QVariantMap({ {"order", "REG"},
                                       {QStringLiteral("host"), QStringLiteral("123456")},
                                       {QStringLiteral("device"),"desktop"},
                                       {QStringLiteral("deviceinfo"),"Toby's Mac Book Pro 13"}}),
                         function(QJsonArray returnValue) {
        qDebug() << "Got reply from event with a json object:" << returnValue;
    });
    */
    /*
    m_client.on("news", function(QJsonArray data) {
        qDebug() << "Got event from server with data" << data;
    });

    m_client.on("prueba", function(QJsonArray data) {
        qDebug() << "Got event from server with data" << data;
    });
    */
}

void EchoClient::disconnected(QString endpoint)
{
    qDebug() << "Disconnected from endpoint" << endpoint;
}

void EchoClient::heartbeatReceived()
{
    qDebug() << "Received heartbeat";
    /*
    m_client.emitMessage("MSG",
                         QVariantMap({ {"order", "MSG"},
                                       {QStringLiteral("id"), QStringLiteral("1234561")},
                                       {QStringLiteral("poster"),"22222"},
                                       {QStringLiteral("deviceinfo"),"Toby's Mac Book Pro 13"}}),
                         function(QJsonArray returnValue) {
        qDebug() << "Got reply from event with a json object:" << returnValue;
    });
    */
}
