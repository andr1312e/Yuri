#include "datahandler.h"

DataHandler::DataHandler(QObject *parent)
    : QObject(parent)
    , m_maxNumOfLinesToStop(6)
    , pcbStateMessageBytesCount(1)
    , m_maxMessageBytesCount(255)
    , m_endOfFirmwareSymbol(static_cast<char>(0xff))
    , m_gettingMessageType(HandlerState::Normal)
    , m_currentStopLineNumber(0)
    , m_stopReadingFirmwareArray(256, m_endOfFirmwareSymbol)
    , m_firmwareFromDevice(new QByteArray())
{

}

DataHandler::~DataHandler()
{
    delete m_firmwareFromDevice;
}

void DataHandler::ClearBuffer()
{
    m_readyReadBuffer.clear();
    m_currentStopLineNumber = 0;
    m_firmwareFromDevice->clear();
}

void DataHandler::SetHandlerState(HandlerState state)
{

    m_gettingMessageType = state;
    ClearBuffer();
}


void DataHandler::SendMessageToDevice(const QByteArray &array)
{
    WriteMessageToBuffer(array);
    Q_EMIT ToConsoleLog("Размер " + QString::number(array.size()) + " выслали "  + QString::fromLatin1(array.toHex()));
    FlushBuffer();
}

void DataHandler::SetConnectionState(quint8 state)
{
    m_gettingMessageType = static_cast<HandlerState>(state);
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
    m_readyReadBuffer.append(incomingByteArray);
    ToConsoleLog(m_readyReadBuffer.toHex());
    if (m_readyReadBuffer.count() > 1)
    {
        if (m_readyReadBuffer.count() == 2  && static_cast<char>(0x4f) == m_readyReadBuffer.front() && static_cast<char>(0x6b) == m_readyReadBuffer.at(1))
        {
            qDebug() << "Ok";
            m_readyReadBuffer.clear();
            Q_EMIT ToStateWidgetConsoleLog("Получили сообщение - OK");
        }
        else
        {
            if (m_readyReadBuffer.front() == 0x07)
            {
                qDebug() << "start getting get message";
                const int messageId = m_readyReadBuffer.at(1);
                if (0x06 == messageId)
                {
                    if (m_readyReadBuffer.count() > 2)
                    {
                        const int subMessageId = m_readyReadBuffer.at(2);
                        switch (subMessageId)
                        {
                        case 0:
                        case 1:
                        case 2:
                        case 3:
                        case 4:
                            Q_EMIT ToStateGettingFromMessage(m_readyReadBuffer);
                            break;
                        case 5:
                            if (m_readyReadBuffer.count() == 8)
                            {
                                Q_EMIT ToStateGettingFromMessage(m_readyReadBuffer);
                                m_readyReadBuffer.clear();
                                break;
                            }
                        default:
                            break;
                        }
                    }
                    return;
                }
                else
                {
                    if ((messageId > 0x00 && messageId < 0x06) || messageId == 0x09)
                    {
                        if (m_readyReadBuffer.count() == m_messagesSizes.at(messageId))
                        {
                            qDebug() << "parsing";
//                        Q_EMIT ToStateWidgetConsoleLog("Получили сообщение c состоянием: " + QString::fromLatin1(incomingByteArray));
                            Q_EMIT ToStateGettingFromMessage(m_readyReadBuffer);
                            m_readyReadBuffer.clear();
                            return;
                        }
                        else
                        {
                            qDebug() << " wrong size";
                            if (m_readyReadBuffer.count() > m_messagesSizes.at(messageId))
                            {
                                qDebug() << "  size bigger";
                                m_readyReadBuffer.clear();
                            }
                            return;
                        }
                    }
                }
            }
            m_readyReadBuffer.clear();
            Q_EMIT ToGetKoordinatesMessage(incomingByteArray);
        }
    }
}

void DataHandler::ReadFirmwareMessageAnalyze(const QByteArray &incomingByteArray)
{
    m_readyReadBuffer.append(incomingByteArray);
    if (m_readyReadBuffer.count() < m_maxMessageBytesCount)
    {
        return;
    }
    else
    {
        if (IsArrayEndOfFirmware())
        {
            m_readyReadBuffer.clear();
            RemoveLastFBytesInFimware();
            Q_EMIT ToFirmWareFormDeviceLoaded(m_firmwareFromDevice);
        }
        else
        {
            m_firmwareFromDevice->append(m_readyReadBuffer);
            m_readyReadBuffer.clear();
            Q_EMIT ToReadFirmwareAgain();
        }
    }
}

void DataHandler::FlashFirmwareMessageAnalyze(const QByteArray &incomingMessage)
{
    if (incomingMessage.count() == pcbStateMessageBytesCount)
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
        Q_EMIT ToFirmwareWidgetConsoleLog("Получили сообщение: " + QString::fromLatin1(incomingMessage.toHex()));
    }
}

bool DataHandler::IsArrayEndOfFirmware() const noexcept
{
    return m_stopReadingFirmwareArray == m_readyReadBuffer;
}

void DataHandler::RemoveLastFBytesInFimware()
{
    int fBytesCount = 0;
    for (int i = m_firmwareFromDevice->count() - 1; i > 0; i--)
    {
        if (m_endOfFirmwareSymbol == m_firmwareFromDevice->at(i))
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
