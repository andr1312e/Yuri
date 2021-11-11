#include "datahandler.h"


DataHandler::DataHandler(QObject *parent)
    : QObject(parent)
    , m_gettingMessageType(Normal)
    , m_readyReadBuffer(new QByteArray())
    , m_currentStopLineNumber(0)
    , m_stopHexArray(new QByteArray(std::begin<char>({static_cast<char>(0xff), static_cast<char>(0xff), static_cast<char>(0xff), static_cast<char>(0xff),
                                                      static_cast<char>(0xff), static_cast<char>(0xff), static_cast<char>(0xff), static_cast<char>(0xff),
                                                      static_cast<char>(0xff), static_cast<char>(0xff), static_cast<char>(0xff), static_cast<char>(0xff),
                                                      static_cast<char>(0xff), static_cast<char>(0xff), static_cast<char>(0xff), static_cast<char>(0xff)}), 16))
{

}

DataHandler::~DataHandler()
{
    delete m_readyReadBuffer;
    delete m_stopHexArray;
}

void DataHandler::SetHandlerState(HandlerState state)
{
    m_gettingMessageType=state;
}


void DataHandler::SendMessageDevice(const QByteArray &array)
{
    WriteMessageToBuffer(array);
    FlushBuffer();
}

bool DataHandler::isEndOfFirmware(QByteArray &firmwarePart)
{
    QByteArray hex=firmwarePart.toHex();
    if(isFullFArray(hex))
    {
        ++m_currentStopLineNumber;
        if (m_currentStopLineNumber>m_maxNumOfLinesToStop)
        {
            return true;
        }
    }
    else
    {
        m_currentStopLineNumber=0;
        return false;
    }
}

void DataHandler::NormalStateMessageAnalyze(const QByteArray &incomingByteArray)
{
    if (m_readyReadBuffer->count()<3)
    {
        Q_EMIT ToStateWidgetConsoleLog("Получили сообщение ("+ QString(incomingByteArray.toHex())+") - OK");
    }
    else
    {
        Q_EMIT ToStateWidgetConsoleLog("Получили сообщение c состоянием: "+ QString(incomingByteArray.toHex()));
        Q_EMIT ToStateGettingFromMessage(incomingByteArray);
    }
}

void DataHandler::ReadFirmwareMessageAnalyze(const QByteArray incomingByteArray)
{
    m_readyReadBuffer->append(incomingByteArray);
    if (m_readyReadBuffer->count()<maxMessageBytesCount)
    {
        return;
    }
    else
    {
        m_readyReadBuffer->clear();
        Q_EMIT ToReadFirmwareAgain();
    }
}

void DataHandler::FlashFirmwareMessageAnalyze(const QByteArray incomingMessage)
{
    if (incomingMessage.count()==pcbStateMessageBytesCount)
    {
        Q_EMIT ToFirmwareWidgetConsoleLog("Получили состояние ");
        Q_EMIT ToRegisterStateChanging(incomingMessage.front());
    }
    else
    {
        Q_EMIT ToFirmwareWidgetConsoleLog("Получили сообщение: "+ QString(incomingMessage.toHex()));
    }
}

bool DataHandler::isFullFArray(QByteArray &firmwarePart) const
{
    if ((*m_stopHexArray)==firmwarePart)
    {
        return true;
    }
    else
    {
        return false;
    }
}
