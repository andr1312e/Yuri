#ifndef CONNECTIONS_SERIALHANDLER_H
#define CONNECTIONS_SERIALHANDLER_H

#include <QByteArray>
#include <QObject>
#include <QSerialPort>

#include "connections/datahandler.h"
#include "constantsandfunctions.h"

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
    void WhenReadyRead();
    void WhenErrorOccurred(QSerialPort::SerialPortError error);
public Q_SLOTS:
    void WhenDisconnectByUserFromHost();

public:
    void ConnectToHost(const QString &comPortName);
public:
    virtual void SetConnectionState(quint8 state) override;
    virtual void WriteMessageToBuffer(const QByteArray &array) override;
    virtual void FlushBuffer() override;
private:
    QSerialPort *m_connectionPort;
};

#endif // CONNECTIONS_SERIALHANDLER_H
