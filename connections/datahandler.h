#ifndef CONNECTIONS_DATAHANDLER_H
#define CONNECTIONS_DATAHANDLER_H
#include <QObject>
#include "constantsandfunctions.h"

enum HandlerState {Normal, ReadFirmware, Flash };

class DataHandler: public QObject
{
    Q_OBJECT
public:
    explicit DataHandler(QObject *parent);
    ~DataHandler();
    virtual void ClearBuffer();
private:
    virtual void CreateHander()=0;
    virtual void ConnectObjects()=0;
Q_SIGNALS:
    void ToButtonsEnabledChanging(bool state);
    void ToConsoleLog(QString message);
    void ToStateWidgetConsoleLog(QString message);
    void ToFirmwareWidgetConsoleLog(QString message);
    void ToStateGettingFromMessage(const QByteArray &message);
    void ToRegisterStateChanging(quint8 state);
    void ToReadFirmwareAgain();
    void ToFirmWareFormDeviceLoaded(QByteArray *firmwareFromDevice);
public:
    void SetHandlerState(HandlerState state);
    void SendMessageDevice(const QByteArray &array);
    virtual void SetConnectionState(quint8 state)=0;
    virtual void WriteMessageToBuffer(const QByteArray &bytes)=0;
    virtual void FlushBuffer()=0;
    virtual void DisconnectByUser() = 0;
protected:
    void NormalStateMessageAnalyze(const QByteArray &incomingByteArray);
    void ReadFirmwareMessageAnalyze(const QByteArray &incomingByteArray);
    void FlashFirmwareMessageAnalyze(const QByteArray &incomingMessage);
private:
    bool isFullFArray() const;
protected:
    const quint8 m_maxNumOfLinesToStop=6;
    HandlerState m_gettingMessageType;
    QByteArray *m_readyReadBuffer;
    quint8 m_currentStopLineNumber;
private:
    const QByteArray *m_stopHexArray;
    QByteArray m_firmwareFromDevice;
};

#endif // CONNECTIONS_DATAHANDLER_H
