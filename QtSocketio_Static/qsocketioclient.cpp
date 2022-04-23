#include "qsocketioclient.h"

#include <QtWebSockets/QWebSocket>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QtCore/QRegExp>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QDebug>
#include <functional>

#include <QLoggingCategory>
#include <yeast.h>

QSocketIoClient::QSocketIoClient(QObject *parent) :
    QObject(parent),
    m_pWebSocket(new QWebSocket()),
    m_pNetworkAccessManager(new QNetworkAccessManager()),
    m_pNetworkAccessManager2(new QNetworkAccessManager()),
    m_pNetworkAccessManager3(new QNetworkAccessManager()),
    m_requestUrl(),
    m_connectionTimeout(30000),
    m_heartBeatTimeout(20000),
    m_pHeartBeatTimer(new QTimer()),
    m_sessionId()
{
    QLoggingCategory::setFilterRules("qt.network.ssl.warning=false");

    m_pHeartBeatTimer->setInterval(m_heartBeatTimeout);

   connect(m_pWebSocket, SIGNAL(connected()),
            this, SLOT(onconnected()));

    connect(m_pWebSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(onError(QAbstractSocket::SocketError)));

    connect(m_pWebSocket, SIGNAL(textMessageReceived(QString)),
            this, SLOT(onMessage(QString)));

    connect(m_pNetworkAccessManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));



    connect(m_pHeartBeatTimer, SIGNAL(timeout()), this, SLOT(sendHeartBeat()));
}

QSocketIoClient::~QSocketIoClient()
{
    m_pHeartBeatTimer->stop();
    delete m_pHeartBeatTimer;
    delete m_pWebSocket;
    delete m_pNetworkAccessManager;
    delete m_pNetworkAccessManager2;
    delete m_pNetworkAccessManager3;
}

bool QSocketIoClient::open(const QUrl &url)
{
    m_requestUrl = url;

    QUrl requestUrl(QStringLiteral("http://%1:%2/socket.io/?EIO=3&transport=polling&t=%3")
                    .arg(url.host())
                    .arg(QString::number(url.port(80)))
                    .arg("Me"+Yeast::encode(QDateTime::currentMSecsSinceEpoch())));

    QNetworkRequest request(requestUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("text/html"));
    request.setRawHeader(QByteArrayLiteral("Accept"), QByteArrayLiteral("*/*"));
    request.setRawHeader(QByteArrayLiteral("Connection"), QByteArrayLiteral("keep-alive"));
    m_pNetworkAccessManager->get(request);



    return true;
}

void QSocketIoClient::onError(QAbstractSocket::SocketError error)
{
    qDebug() << "Error occurred: " << error;
    open(m_requestUrl);
}

void QSocketIoClient::onMessage(QString textMessage)
{
    Q_UNUSED(textMessage);
    parseMessage(textMessage);
}

void QSocketIoClient::sendHeartBeat()
{
   //qDebug() << "Enviando latido";
    (void)m_pWebSocket->sendTextMessage(QStringLiteral("2"));
}

void QSocketIoClient::replyFinished(QNetworkReply *reply)
{
    //with socket.io actual
    //97:0{\"sid\":\"LVpTnCUbq4DGhRukAAAE\",\"upgrades\":[\"websocket\"],\"pingInterval\":25000,\"pingTimeout\":60000}2:40
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    //QString statusReason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    switch (status)
    {
        case 200:
        {
            //everything allright
            QString payload = QString::fromUtf8(reply->readAll());

            QString messageType = "";
            int particion = payload.indexOf(":");
            for(int i=0;i<particion;i++)
            {
                messageType+= payload.at(i);
            }

            QJsonDocument document = QJsonDocument::fromJson(payload.remove(messageType+":0").remove("2:40").toUtf8());

            QString sid =  document.object().value("sid").toVariant().toString();
            QString upgrades =  document.object().value("upgrades").toArray().at(0).toVariant().toString();
            int pingInterval =  document.object().value("pingInterval").toVariant().toInt();
            int pingTimeout =  document.object().value("pingTimeout").toVariant().toInt();

            QString sessionId = sid;
            m_heartBeatTimeout = pingInterval;
            m_connectionTimeout = pingTimeout;

            m_pHeartBeatTimer->setInterval(m_heartBeatTimeout);

            QString protocols = upgrades;
            if (!protocols.contains("websocket"))
            {
                qDebug() << "websockets not supported; so cannot continue";
                return;
            }
            m_sessionId = sessionId;
            handshakeSucceeded();

            break;
        }

        case 401:	//unauthorized
        {
            //the server refuses to authorize the client to connect,
            //based on the supplied information (eg: Cookie header or custom query components).
            qDebug() << "Error:" << reply->readAll();
            break;
        }

        case 500:	//internal server error
        {
            qDebug() << "Error:" << reply->readAll();
            break;
        }

        case 404:	//Not Found
        {
            qDebug() << "Error: " << reply->readAll();
        }

        case 503:	//service unavailable
        {
            //the server refuses the connection for any reason (e.g. overload)
            qDebug() << "Error:" << reply->readAll();
            break;
        }
        case 0: //cuando el servidor esta apagado
        {
            open(m_requestUrl);
            qDebug() << "Error:" << reply->readAll();
            break;
        }
        default:
        {
        }
    }
}

void QSocketIoClient::handshakeSucceeded()
{
    //socket io actual
    //ws://192.168.1.96:3060/socket.io/?EIO=3&transport=websocket&sid=mG2GGJ8HVMfrCU_XAAAB
    QUrl url(m_requestUrl.toString() + QStringLiteral("/socket.io/?EIO=3&transport=websocket&sid=") + m_sessionId);
    m_pWebSocket->open(url);
}

void QSocketIoClient::ackReceived(int messageId, QJsonArray arguments)
{
    QAbstractCallback *callback = m_callbacks.value(messageId, Q_NULLPTR);
    if (callback) {
        (*callback)(arguments);
        delete callback;
    }
}

#include <functional>

void QSocketIoClient::eventReceived(QString message, QJsonArray arguments,
                                    bool mustAck, int messageId)
{
    if (m_subscriptions.contains(message)) {

        QJsonValue retVal;
        QAbstractCallback *callback = m_subscriptions[message];
        /*if (callback->hasReturnValue()) {
            retVal = (*callback)(arguments);
        } else {*/
            (*callback)(arguments);
        //}
        if (mustAck) {
            acknowledge(messageId, retVal);
        }
    }
}

void QSocketIoClient::parseMessage(const QString &message)
{
    //ejemplo de mensaje 5:::{\"name\":\"news\",\"args\":[{\"hello\":\"world\"}]}
    /*
  var packets = parser.packets = [
      'disconnect'
    , 'connect'
    , 'heartbeat'
    , 'message'
    , 'json'
    , 'event'
    , 'ack'
    , 'error'
    , 'noop'
  ];
    */

    if(message=="3probe")
    {
        (void)m_pWebSocket->sendTextMessage(QStringLiteral("5"));
        m_pHeartBeatTimer->start();
        Q_EMIT(connected("endpoint"));
    }
    else
    {
        if(message=="3")
        {
            //Q_EMIT(heartbeatReceived());
        }
        else
        {
            qDebug() << message;
            QString messageType = "";
            QString data = message;
            int particion = message.indexOf("[");
            data = data.remove(0,particion);
            for(int i=0;i<particion;i++)
            {
                messageType+= message.at(i);
            }

            if(messageType=="42")
            {
                QJsonDocument document = QJsonDocument::fromJson(data.toUtf8());

                qDebug() << document.array().at(0);
                qDebug() << document.array().at(1);
                int messageId = 0;
                bool mustAck = false;
                bool autoAck = false;

                eventReceived(document.array().at(0).toString(), document.array(), mustAck && !autoAck, messageId);
            }
            else {
                qDebug() << message;
            }


        }


    }

    /*
    QRegExp regExp("^([^:]+):([0-9]+)?(\\+)?:([^:]+)?:?([\\s\\S]*)?$", Qt::CaseInsensitive,
                   QRegExp::RegExp2);
    if (regExp.indexIn(message) != -1)
    {
        QStringList captured = regExp.capturedTexts();
        int messageType = captured.at(1).toInt();
        int messageId = captured.at(2).toInt();
        bool mustAck = (messageId != 0);
        bool autoAck = mustAck && captured.at(3).isEmpty();
        QString endpoint = captured.at(4);
        QString data = captured.at(5);

        if (autoAck)
        {
            acknowledge(messageId);
        }

        switch(messageType)
        {
            case 0:	//disconnect
            {
                Q_EMIT(disconnected(endpoint));
                break;
            }
            case 1: //connect
            {
                m_pHeartBeatTimer->start();
                Q_EMIT(connected(endpoint));
                break;
            }
            case 2:	//heartbeat
            {
                Q_EMIT(heartbeatReceived());
                break;
            }
            case 3:	//message
            {
                Q_EMIT(messageReceived(data));
                break;
            }
            case 4:	//json message
            {
                qDebug() << "JSON message received:" << data;
                break;
            }
            case 5: //event
            {
                QJsonParseError parseError;
                QJsonDocument document = QJsonDocument::fromJson(QByteArray(data.toLatin1()),
                                                                 &parseError);
                if (parseError.error != QJsonParseError::NoError)
                {
                    qDebug() << parseError.errorString();
                }
                else
                {
                    if (document.isObject())
                    {
                        QJsonObject object = document.object();
                        QJsonValue value = object["name"];
                        if (!value.isUndefined())
                        {
                            QString message = value.toString();
                            QJsonArray arguments;
                            QJsonValue argsValue = object["args"];
                            if (!argsValue.isUndefined() && !argsValue.isNull())
                            {
                                if (argsValue.isArray())
                                {
                                    arguments = argsValue.toArray();
                                }
                                else
                                {
                                    qWarning() << "Args argument is not an array";
                                    return;
                                }
                            }
                            eventReceived(message, arguments, mustAck && !autoAck, messageId);
                        }
                        else
                        {
                            qWarning() << "Invalid event received: no name";
                        }
                    }
                }
                break;
            }
            case 6:	//ack
            {
                QRegExp regExp("^([0-9]+)(\\+)?(.*)$", Qt::CaseInsensitive, QRegExp::RegExp2);
                if (regExp.indexIn(data) != -1)
                {
                    QJsonParseError parseError;
                    QJsonArray arguments;
                    int messageId = regExp.cap(1).toInt();
                    QString argumentsValue = regExp.cap(3);
                    if (!argumentsValue.isEmpty())
                    {
                        QJsonDocument doc = QJsonDocument::fromJson(argumentsValue.toLatin1(),
                                                                    &parseError);
                        if (parseError.error != QJsonParseError::NoError)
                        {
                            qWarning() << "JSONParseError:" << parseError.errorString();
                            return;
                        }
                        else
                        {
                            if (doc.isArray())
                            {
                                arguments = doc.array();
                            }
                            else
                            {
                                qWarning() << "Error: data of event is not an array";
                                return;
                            }
                        }
                    }
                    ackReceived(messageId, arguments);
                }
                break;
            }
            case 7:	//error
            {
                QStringList pieces = data.split("+");
                QString reason = pieces[0];
                QString advice;
                if (pieces.length() == 2)
                {
                    advice = pieces[1];
                }
                Q_EMIT(errorReceived(reason, advice));
                break;
            }
            case 8:	//noop
            {
                qDebug() << "Noop received" << data;
                break;
            }
        }
    }
    */
}

QJsonDocument QSocketIoClient::package(const QVariant &value)
{
    if (value.canConvert<QVariantMap>()) {
        return QJsonDocument(QJsonObject::fromVariantMap(value.toMap()));
    } else if (value.canConvert<QVariantList>()) {
        return QJsonDocument(QJsonArray::fromVariantList(value.toList()));
    } else {
        QJsonArray ar;
        ar.append(QJsonValue::fromVariant(value));
        return QJsonDocument(ar);
    }
}

void QSocketIoClient::emitMessage(const QString &message, bool value)
{
    const QString m_endPoint;
    doEmitMessage(message, package(value), m_endPoint, true);
}

void QSocketIoClient::emitMessage(const QString &message, int value)
{
    const QString m_endPoint;
    doEmitMessage(message, package(value), m_endPoint, true);
}

void QSocketIoClient::emitMessage(const QString &message, double value)
{
    const QString m_endPoint;
    doEmitMessage(message, package(value), m_endPoint, true);
}

void QSocketIoClient::emitMessage(const QString &message, const QString &value)
{
    const QString m_endPoint;
    //doEmitMessage(message, package(value), m_endPoint, true);
    doEmitMessage(message,value,m_endPoint,true);
}

void QSocketIoClient::emitMessage(const QString &message, const QJsonObject &value)
{
    const QString m_endPoint;
    doEmitMessage(message,value,m_endPoint,true);
}

void QSocketIoClient::emitMessage(const QString &message, const QVariantList &arguments)
{
    const QString m_endPoint;
    doEmitMessage(message, package(arguments), m_endPoint, true);

}

void QSocketIoClient::emitMessage(const QString &message, const QVariantMap &arguments)
{
    const QString m_endPoint;
    doEmitMessage(message, package(arguments), m_endPoint, true);
}

QString QSocketIoClient::sessionId() const
{
    return m_sessionId;
}

void QSocketIoClient::onconnected()
{
    //qDebug() << "Conexion con socket";
    (void)m_pWebSocket->sendTextMessage(QStringLiteral("2probe"));
}

void QSocketIoClient::acknowledge(int messageId, const QJsonValue &retVal)
{
    QString msg = QStringLiteral("6:::") + QString::number(messageId);
    if (!retVal.isUndefined() && !retVal.isNull()) {
        QJsonDocument doc;
        if (retVal.isArray()) {
            doc.setArray(retVal.toArray());
        } else if (retVal.isObject()) {
            doc.setObject(retVal.toObject());
        } else {
            QJsonArray ar;
            ar.append(retVal);
            doc.setArray(ar);
        }
        msg.append(QStringLiteral("+") + QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
    }
    (void)m_pWebSocket->sendTextMessage(msg);
}

int QSocketIoClient::doEmitMessage(const QString &message, const QJsonDocument &document,
                                    const QString &endpoint, bool callbackExpected)
{

    static int id = 0;
    const QString msg = QStringLiteral("42[\"%1\",\"%2\"]")
            .arg(message)
            .arg(QString::fromUtf8(document.toJson(QJsonDocument::Compact)));

    //"42[\"REG\",\"hola mundo JAAAAAA desde QT c++\"]"

    (void)m_pWebSocket->sendTextMessage(msg);
    return id;

    /*
    static int id = 0;
        const QString msg = QStringLiteral("5:%1%2:%3:{\"name\":\"%4\",\"args\":%5}")
                .arg(++id)
                .arg(callbackExpected ? QStringLiteral("+") : QStringLiteral(""))
                .arg(endpoint)
                .arg(message)
                .arg(QString::fromUtf8(document.toJson(QJsonDocument::Compact)));
        (void)m_pWebSocket->sendTextMessage(msg);
        return id;
    */
}

int QSocketIoClient::doEmitMessage(const QString &message, const QString &data, const QString &endpoint, bool callbackExpected)
{
    static int id = 0;
    const QString msg = QStringLiteral("42[\"%1\",\"%2\"]").arg(message).arg(data);

    (void)m_pWebSocket->sendTextMessage(msg);
    return id;
}

int QSocketIoClient::doEmitMessage(const QString &message, const QJsonObject &data, const QString &endpoint, bool callbackExpected)
{
    static int id = 0;

    QJsonDocument doc(data);
    QString strJson(doc.toJson(QJsonDocument::Compact));

    const QString msg = QStringLiteral("42[\"%1\",%2]").arg(message).arg(strJson);

    (void)m_pWebSocket->sendTextMessage(msg);
    return id;
}
