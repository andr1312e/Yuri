#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QTimer>

#include "messageCreators/statemessagegetter.h"

#include "messageCreators/statemessagesender.h"

enum SocketActionState {SetAndReadData, ReadFirmWare, WriteFirmWare };

class TcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit TcpSocket(QObject *parent);
    ~TcpSocket();
    void connectTo(const QString &adress, const QString &port);
    void SetSocketState(quint8 state);

Q_SIGNALS:
    void SetButtonsEnabled(int state);
    void updateFile();
    void ConsoleLog(QString message);
    void StateConsoleLog(QString errorMessage);
    void GetMessageWithState(QByteArray &message);
    void FirmwareConsoleLog(QString errorMessage);
    void SendFirmwareSourcePartFromDevice(QByteArray &firmware);
    void UpdateHardwareState(quint8 state);
    void GetPartOfFirmWareAgain();
public Q_SLOTS:
    void connected();
    void readyRead();
    void disconnected();
    void disconnect();
    void getErrorMessqage(QAbstractSocket::SocketError socketError );
    void send(const QByteArray &array);

    void SendGetPCBState(const QByteArray *state);
    void SendGetPCBStateReadFromBuffer(const QByteArray *state);
    void SendPartOfFirmware(QByteArray &part);
    void Flush();
private:
    void createConnections();
    QString getStringSocketState(QAbstractSocket::SocketState state);
    bool IsEndOfFirmware(QByteArray &firmwarePart);
public:
    QTcpSocket *m_tcpSocket;
private:
    SocketActionState m_state=SetAndReadData;
    QByteArray gettingMessageArray;

    const QByteArray m_stopByteArray="00ffffffffffffffffffffffffffffffffff00";
    quint8 m_stopLines=0;
    const quint8 m_maxNumOfLinesToStop=255;

};

#endif // TCPSOCKET_H
