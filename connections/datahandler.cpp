#include "datahandler.h"
#include <QDebug>
#include <QFile>

DataHandler::DataHandler(QObject *parent)
    : QObject(parent)
    , m_gettingMessageType(Normal)
    , m_readyReadBuffer(new QByteArray())
    , m_currentStopLineNumber(0)
    , m_stopHexArray(new QByteArray(256, static_cast<char>(0xff)))
{

}

DataHandler::~DataHandler()
{
    delete m_readyReadBuffer;
    delete m_stopHexArray;
}

void DataHandler::ClearBuffer()
{
    m_readyReadBuffer->clear();
    m_currentStopLineNumber=0;
}

void DataHandler::SetHandlerState(HandlerState state)
{
    m_gettingMessageType=state;
    m_readyReadBuffer->clear();
}


void DataHandler::SendMessageDevice(const QByteArray &array)
{
    WriteMessageToBuffer(array);
    FlushBuffer();
}

void DataHandler::NormalStateMessageAnalyze(const QByteArray &incomingByteArray)
{
    if (m_readyReadBuffer->count()<3)
    {
        if (m_readyReadBuffer->front()==static_cast<char>(0x07))
        {
            m_readyReadBuffer->append(incomingByteArray);
        }
        else
        {
            Q_EMIT ToStateWidgetConsoleLog("Получили сообщение ("+ QString(incomingByteArray.toHex())+") - OK");
        }
    }
    else
    {
        Q_EMIT ToStateWidgetConsoleLog("Получили сообщение c состоянием: "+ QString::fromLatin1(incomingByteArray.toHex()));
        Q_EMIT ToStateGettingFromMessage(incomingByteArray);
    }
}

void DataHandler::ReadFirmwareMessageAnalyze(const QByteArray &incomingByteArray)
{
    m_readyReadBuffer->append(incomingByteArray);
    if (m_readyReadBuffer->count()<maxMessageBytesCount)
    {
        return;
    }
    else
    {
        if (isFullFArray())
        {
            m_readyReadBuffer->clear();
//            QFile file("firmware.bin");
//            if(file.open(QIODevice::WriteOnly))
//            {
//                file.write(m_firmwareFromDevice);
//                file.close();
//            }
            int fBytesCount=0;
            for (int i=m_firmwareFromDevice.count()-1; i>0; i--)
            {
                if(m_firmwareFromDevice.at(i)==static_cast<char>(0xff))
                {
                    fBytesCount++;
                }
                else
                {
                    break;
                }
            }
            m_firmwareFromDevice.chop(fBytesCount);
            Q_EMIT ToFirmWareFormDeviceLoaded(&m_firmwareFromDevice);
        }
        else
        {
            m_firmwareFromDevice.append(*m_readyReadBuffer);
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

bool DataHandler::isFullFArray() const
{
    if ((*m_stopHexArray)==(*m_readyReadBuffer))
    {
        return true;
    }
    else
    {
        return false;
    }
}
