#ifndef CONNECTIONS_SERIALHANDLER_H
#define CONNECTIONS_SERIALHANDLER_H

#include <QSerialPort>
#include <QDebug>

#include "connections/datahandler.h"

class SerialHandler : public DataHandler
{
    Q_OBJECT
public:
    explicit SerialHandler(QObject *parent);
    ~SerialHandler();
private:
    virtual void CreateHander() Q_DECL_OVERRIDE;
    virtual void ConnectObjects() Q_DECL_OVERRIDE;

private Q_SLOTS:
    void OnReadyRead();
    void OnErrorOccurred(QSerialPort::SerialPortError error);

public:
    void TryToConnectToHost(const QString &comPortName);
public:
    virtual void WriteMessageToBuffer(const QByteArray &array) Q_DECL_OVERRIDE;
    virtual void FlushBuffer() Q_DECL_OVERRIDE;
    virtual void FromHostDisconnect() Q_DECL_OVERRIDE;
private:
    QSerialPort *m_connectionPort;
};

#endif // CONNECTIONS_SERIALHANDLER_H
