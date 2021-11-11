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
private:
    virtual void CreateHander()=0;
    virtual void ConnectObjects()=0;
Q_SIGNALS:
    void ToButtonsEnabledChanging(int state);
    void ToConsoleLog(QString message);
    void ToStateWidgetConsoleLog(QString message);
    void ToFirmwareWidgetConsoleLog(QString message);
    void ToStateGettingFromMessage(const QByteArray &message);
    void ToRegisterStateChanging(quint8 state);
    void ToReadFirmwareAgain();
public:
    void SetHandlerState(HandlerState state);
    void SendMessageDevice(const QByteArray &array);
    virtual void SetConnectionState(quint8 state)=0;
    virtual void WriteMessageToBuffer(const QByteArray &bytes)=0;
    virtual void FlushBuffer()=0;
protected:
    bool isEndOfFirmware(QByteArray &firmwarePart);
    void NormalStateMessageAnalyze(const QByteArray &incomingByteArray);
    void ReadFirmwareMessageAnalyze(const QByteArray incomingByteArray);
    void FlashFirmwareMessageAnalyze(const QByteArray incomingMessage);
private:
    bool isFullFArray(QByteArray &firmwarePart) const;
protected:
    const quint8 m_maxNumOfLinesToStop=6;
    HandlerState m_gettingMessageType;
    QByteArray *m_readyReadBuffer;
    quint8 m_currentStopLineNumber;
private:
    const QByteArray *m_stopHexArray;
};

#endif // CONNECTIONS_DATAHANDLER_H
