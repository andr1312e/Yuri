#ifndef CONNECTIONS_TCPHANDLER_H
#define CONNECTIONS_TCPHANDLER_H

#include <QObject>
#include <QTcpSocket>

#include "datahandler.h"
#include "constantsandfunctions.h"

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
    void WhenReadyRead();
    void WhenErrorOccurred(QAbstractSocket::SocketError socketError);
    void WhenSocketConnected();
    void WhenDisconnectedFromHost();
public Q_SLOTS:
    void WhenDisconnectByUserFromHost();
public:
    void ConnectToMoxa(const QString &adress, const QString &port);

public:
    virtual void ClearBuffer() override;
    virtual void SetConnectionState(quint8 state) override;
    virtual void WriteMessageToBuffer(const QByteArray &part) override;
    virtual void FlushBuffer() override;
    virtual void DisconnectByUser() override;
private:
    QString getStringSocketState(QAbstractSocket::SocketState state);
private:
    QTcpSocket *m_connectionPort;

};

#endif // CONNECTIONS_TCPHANDLER_H
