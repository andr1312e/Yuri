#include "statepresenter.h"
#include <QDebug>
#include <QByteArray>
#include <QWeakPointer>

StatePresenter::StatePresenter(QObject *parent)
    :  QObject(parent)
    ,  m_dataHandler(Q_NULLPTR)
{
    CreateObjects();
}

StatePresenter::~StatePresenter()
{
}

void StatePresenter::CreateObjects()
{
    m_messageSetter.operator=(QSharedPointer<StateMessageSender>(new StateMessageSender()));
    m_messageGetter.operator=(QSharedPointer<StateMessageGetter>(new StateMessageGetter()));
}

void StatePresenter::OnGetMessageWithState(QByteArray &messageFromDevice)
{
    QString currentValue=m_messageGetter->getDataFromMessage(messageFromDevice);
    Q_EMIT ToConsoleLog(currentValue);
}

void StatePresenter::DisconnectOldHandler()
{
    disconnect(m_dataHandler, &DataHandler::ToConsoleLog, this, &StatePresenter::ToConsoleLog);
    disconnect(m_dataHandler, &DataHandler::ToStateWidgetConsoleLog, this, &StatePresenter::ToConsoleLog);
    disconnect(m_dataHandler, &DataHandler::ToButtonsEnabledChanging, this, &StatePresenter::ToSetButtonsEnabled);
    disconnect(m_dataHandler, &DataHandler::ToStateGettingFromMessage, this, &StatePresenter::OnGetMessageWithState);
}

void StatePresenter::ConnectHander(DataHandler *dataHandler)
{
    m_dataHandler=dataHandler;
    connect(m_dataHandler, &DataHandler::ToConsoleLog, this, &StatePresenter::ToConsoleLog);
    connect(m_dataHandler, &DataHandler::ToStateWidgetConsoleLog, this, &StatePresenter::ToConsoleLog);
    connect(m_dataHandler, &DataHandler::ToButtonsEnabledChanging, this, &StatePresenter::ToSetButtonsEnabled);
    connect(m_dataHandler, &DataHandler::ToStateGettingFromMessage, this, &StatePresenter::OnGetMessageWithState);
}

void StatePresenter::SetStateToDevice(quint8 messageId, double firstParam, double SecondParam)
{
    m_dataHandler->SetHandlerState(HandlerState::Normal);
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
        message=m_messageSetter->createSixCommand(firstParam, SecondParam);
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
    Q_EMIT ToUpdateHistoryFile();
    Q_EMIT ToConsoleLog("Отправляемое сообщeние " +message.toHex() + " его размер: " + QString::number(message.size()) + " байт сформировано");
    m_dataHandler->SendMessageToDevice(message);
}

void StatePresenter::GetStateFromDevice(quint8 messageIdWantToGet)
{
    m_dataHandler->SetHandlerState(HandlerState::Normal);
    QByteArray message(m_messageSetter->createSevenCommand(messageIdWantToGet));
    m_dataHandler->SendMessageToDevice(message);
}
