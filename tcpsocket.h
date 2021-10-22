#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QTimer>

#include "messagesetter.h"
#include "messagesgetter.h"

class TcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit TcpSocket(QObject *parent);
    ~TcpSocket();
    void connectTo(QString &ip, QString &port);
    void createMessages(quint8 messageId, double firstParam, double SecondParam);

Q_SIGNALS:
    void setButtonsEnabled(bool state);
    void setState(QString state);
    void updateFile();
    void sendMessageToLog(QString errorMessage);
public Q_SLOTS:
    void connected();
    void readyRead();
    void disconnected();
    void disconnect();
    void getErrorMessqage(QAbstractSocket::SocketError socketError );
    void send(QByteArray &array);
private:
    void initCheckingConnectionTimer();
    void createConnections();
    QString getStringSocketState(QAbstractSocket::SocketState state);
private Q_SLOTS:
    void checkConnection();
private:
    QTcpSocket *m_tcpSocket;
    QTimer *m_timer;
    MessagesSetter *m_messageSetter;
    MessagesGetter *m_messageGetter;
};

#endif // TCPSOCKET_H
