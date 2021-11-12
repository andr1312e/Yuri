#include "datahandler.h"

DataHandler::DataHandler(QObject *parent)
    : QObject(parent)
    , m_maxNumOfLinesToStop(6)
    , pcbStateMessageBytesCount(1)
    , m_maxMessageBytesCount(255)
    , m_endOfFirmwareSymbol(static_cast<char>(0xff))
    , m_gettingMessageType(Normal)
    , m_currentStopLineNumber(0)
    , m_readyReadBuffer(new QByteArray())
    , m_stopReadingFirmwareArray(new QByteArray(256, m_endOfFirmwareSymbol))
    , m_firmwareFromDevice(new QByteArray())
{

}

DataHandler::~DataHandler()
{
    delete m_readyReadBuffer;
    delete m_stopReadingFirmwareArray;
    delete m_firmwareFromDevice;
}

void DataHandler::ClearBuffer()
{
    m_readyReadBuffer->clear();
    m_currentStopLineNumber=0;
}

void DataHandler::SetHandlerState(HandlerState state)
{
    m_gettingMessageType=state;
    ClearBuffer();
}


void DataHandler::SendMessageToDevice(const QByteArray &array)
{
    WriteMessageToBuffer(array);
    Q_EMIT ToConsoleLog(QString::number(array.size())+" выслали "  + QString::fromLatin1(array.toHex()));
    FlushBuffer();
}

void DataHandler::SetConnectionState(quint8 state)
{
    m_gettingMessageType=static_cast<HandlerState>(state);
}

void DataHandler::FromHostDisconnected()
{
    Q_EMIT ToConsoleLog(QStringLiteral("Отключено..."));
    Q_EMIT ToButtonsEnabledChanging(false);
}

void DataHandler::ToHostConnected()
{
    Q_EMIT ToConsoleLog(QStringLiteral("Подключились"));
    Q_EMIT ToButtonsEnabledChanging(true);
}

void DataHandler::NormalStateMessageAnalyze(const QByteArray &incomingByteArray)
{
    m_readyReadBuffer->append(incomingByteArray);
    if (m_readyReadBuffer->count()<3)
    {
#if QT_VERSION > QT_VERSION_CHECK(5, 10, 0)
        if (static_cast<char>(0x07)!=m_readyReadBuffer->front())
#else
        if (static_cast<char>(0x07)!=m_readyReadBuffer->at(0))
#endif
        {
            Q_EMIT ToStateWidgetConsoleLog("Получили сообщение ("+ QString::fromLatin1(incomingByteArray.toHex())+") - OK");
            m_readyReadBuffer->clear();
        }
    }
    else
    {
        Q_EMIT ToStateWidgetConsoleLog("Получили сообщение c состоянием: "+ QString::fromLatin1(incomingByteArray.toHex()));
        Q_EMIT ToStateGettingFromMessage(*m_readyReadBuffer);
    }
}

void DataHandler::ReadFirmwareMessageAnalyze(const QByteArray &incomingByteArray)
{
    m_readyReadBuffer->append(incomingByteArray);
    Q_EMIT ToConsoleLog(QString::number(m_readyReadBuffer->size())+" cчитали"  + QString::fromLatin1(m_readyReadBuffer->toHex()));
    if (m_readyReadBuffer->count()<m_maxMessageBytesCount)
    {
        return;
    }
    else
    {
        if (IsArrayEndOfFirmware())
        {
            m_readyReadBuffer->clear();
            RemoveLastFBytesInFimware();
            Q_EMIT ToFirmWareFormDeviceLoaded(m_firmwareFromDevice);
        }
        else
        {
            Q_EMIT ToConsoleLog("cчитали" + QString::fromLatin1(m_readyReadBuffer->toHex()));
            m_firmwareFromDevice->append(*m_readyReadBuffer);
            m_readyReadBuffer->clear();
            Q_EMIT ToReadFirmwareAgain();
        }
    }
}

void DataHandler::FlashFirmwareMessageAnalyze(const QByteArray &incomingMessage)
{
    if (incomingMessage.count()==pcbStateMessageBytesCount)
    {
        Q_EMIT ToFirmwareWidgetConsoleLog(QStringLiteral("Получили состояние "));
#if QT_VERSION > QT_VERSION_CHECK(5, 10, 0)
        Q_EMIT ToRegisterStateChanging(incomingMessage.front());
#else
        Q_EMIT ToRegisterStateChanging(incomingMessage.at(0));
#endif
    }
    else
    {
        Q_EMIT ToFirmwareWidgetConsoleLog("Получили сообщение: "+ QString::fromLatin1(incomingMessage.toHex()));
    }
}

bool DataHandler::IsArrayEndOfFirmware() const
{
    if ((*m_stopReadingFirmwareArray)==(*m_readyReadBuffer))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void DataHandler::RemoveLastFBytesInFimware()
{
    int fBytesCount=0;
    for (int i=m_firmwareFromDevice->count()-1; i>0; i--)
    {
        if(m_endOfFirmwareSymbol==m_firmwareFromDevice->at(i))
        {
            fBytesCount++;
        }
        else
        {
            break;
        }
    }
    m_firmwareFromDevice->chop(fBytesCount);
}
