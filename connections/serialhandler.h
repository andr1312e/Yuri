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
    virtual void CreateHander() override;
    virtual void ConnectObjects() override;

private Q_SLOTS:
    void OnReadyRead();
    void OnErrorOccurred(QSerialPort::SerialPortError error);

public:
    void TryToConnectToHost(const QString &comPortName);
public:
    virtual void WriteMessageToBuffer(const QByteArray &array) override;
    virtual void FlushBuffer() override;
    virtual void FromHostDisconnect() override;
private:
    QSerialPort *m_connectionPort;
};

#endif // CONNECTIONS_SERIALHANDLER_H
