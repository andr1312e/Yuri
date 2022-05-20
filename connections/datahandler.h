#ifndef CONNECTIONS_DATAHANDLER_H
#define CONNECTIONS_DATAHANDLER_H
#include <QObject>
#include <QByteArray>

enum HandlerState {Normal, ReadFirmware, Flash };

class DataHandler: public QObject
{
    Q_OBJECT
public:
    explicit DataHandler(QObject *parent);
    virtual ~DataHandler();
    virtual void ClearBuffer();
private:
    virtual void CreateHander() = 0;
    virtual void ConnectObjects() = 0;
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
    void SendMessageToDevice(const QByteArray &array);
    void SetConnectionState(quint8 state);
    virtual void WriteMessageToBuffer(const QByteArray &bytes) = 0;
    virtual void FlushBuffer() = 0;
    virtual void FromHostDisconnect() = 0;
    virtual void FromHostDisconnected();
protected:
    virtual void ToHostConnected();
    void NormalStateMessageAnalyze(const QByteArray &incomingByteArray);
    void ReadFirmwareMessageAnalyze(const QByteArray &incomingByteArray);
    void FlashFirmwareMessageAnalyze(const QByteArray &incomingMessage);
private:
    bool IsArrayEndOfFirmware() const;
    void RemoveLastFBytesInFimware();
protected:
    const quint8 m_maxNumOfLinesToStop;
    const quint8 pcbStateMessageBytesCount;
    const quint8 m_maxMessageBytesCount;
    const char m_endOfFirmwareSymbol;
    HandlerState m_gettingMessageType;
    quint8 m_currentStopLineNumber;
    QByteArray m_readyReadBuffer;
private:
    const QByteArray m_stopReadingFirmwareArray;
    QByteArray *m_firmwareFromDevice;
};

#endif // CONNECTIONS_DATAHANDLER_H
