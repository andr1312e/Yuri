#include "statepresenter.h"

#include <QByteArray>

StatePresenter::StatePresenter(TcpSocket *socket)
    : m_socket(socket)
{
    CreateObjects();
    CreateConnections();
}

StatePresenter::~StatePresenter()
{
    delete m_messageGetter;
    delete m_messageSetter;
}

void StatePresenter::SetStateToDevice(quint8 messageId, double firstParam, double SecondParam)
{
    m_socket->SetSocketState(0);
    QByteArray message;
    switch (messageId) {
    case 1:
    {
        message=m_messageSetter->createFirstCommand(firstParam);
        break;
    }
    case 2:
    {
        message=m_messageSetter->createSecondCommand(firstParam, SecondParam);
        break;
    }
    case 3:
    {
        message=m_messageSetter->createThirdCommand(firstParam);
        break;
    }
    case 4:
    {
        message=m_messageSetter->createFourthCommand(firstParam, SecondParam);
        break;
    }
    case 5:
    {
        message=m_messageSetter->createFiveCommand(firstParam);
        break;
    }
    case 6:
    {
        message=m_messageSetter->createSixCommand(firstParam);
        break;
    }
    case 7:
    {
        message=m_messageSetter->createSevenCommand(firstParam);
        break;
    }
    default:
    {
        message=m_messageSetter->createZeroCommand();
        break;
    }

    }
    Q_EMIT UpdateHistoryFile();
    Q_EMIT ConsoleLog("Отправляемое сообщeние " +message.toHex() + " его размер: " + QString::number(message.size()) + " байт сформировано");
    m_socket->send(message);
}

void StatePresenter::GetStateFromDevice(quint8 messageWantToGetId)
{
    m_socket->SetSocketState(0);
    quint8 messageIdWantToGet=quint8(messageWantToGetId);
    QByteArray message;
    message=m_messageSetter->createSevenCommand(messageIdWantToGet);
    m_socket->send(message);
}

void StatePresenter::CreateObjects()
{
    m_messageSetter=new StateMessageSender();
    m_messageGetter=new StateMessageGetter();
}

void StatePresenter::CreateConnections()
{
    connect(m_socket, &TcpSocket::ConsoleLog, this, &StatePresenter::ConsoleLog);
    connect(m_socket, &TcpSocket::StateConsoleLog, this, &StatePresenter::ConsoleLog);
    connect(m_socket, &TcpSocket::SetButtonsEnabled, this, &StatePresenter::SetButtonsEnabled);
    connect(m_socket, &TcpSocket::GetMessageWithState, this, &StatePresenter::GetMessageWithState);
}

void StatePresenter::GetMessageWithState(QByteArray &byteArray)
{
    QString currentValue=m_messageGetter->getDataFromMessage(byteArray);
    ConsoleLog(currentValue);
}
