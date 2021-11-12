#ifndef CONNECTIONS_TCPHANDLER_H
#define CONNECTIONS_TCPHANDLER_H

#include <QTcpSocket>

#include "connections/datahandler.h"

class TcpHandler : public DataHandler
{
    Q_OBJECT
public:
    explicit TcpHandler(QObject *parent);
    ~TcpHandler();
private:
    virtual void CreateHander() override;
    virtual void ConnectObjects() override;

private Q_SLOTS:
    void OnReadyRead();
    void OnErrorOccurred(QAbstractSocket::SocketError socketError);

public:
    void TryToConnectToHost(const QString &adress, const QString &port);
public:
    virtual void WriteMessageToBuffer(const QByteArray &part) override;
    virtual void FlushBuffer() override;
    virtual void FromHostDisconnect() override;
private:
    QTcpSocket *m_connectionPort;

};

#endif // CONNECTIONS_TCPHANDLER_H
