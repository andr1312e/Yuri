#include "presenter/statepresenter.h"

StatePresenter::StatePresenter(SettingFileService *settingFileService, QObject *parent)
    :  QObject(parent)
    ,  m_settingFileService(settingFileService)
    ,  m_dataHandler(Q_NULLPTR)
{
    CreateObjects();
    ConnectObjects();
    startTimer(std::chrono::seconds(1), Qt::VeryCoarseTimer);
}

StatePresenter::~StatePresenter()
{
    delete m_messageSetter;
    delete m_messageGetter;
}

void StatePresenter::CreateObjects()
{
    bool ok;
    double f = m_settingFileService->GetAttribute("const", "f", "30625000").toDouble(&ok);
    if (!ok || f <= 0)
    {
        f = 30625000;
    }
    double fref = m_settingFileService->GetAttribute("const", "fref", "40000000").toDouble(&ok);
    if (!ok || fref <= 0)
    {
        fref = 40000000;
    }
    m_messageSetter = new StateMessageSender(f, fref);
    m_messageGetter = new StateMessageGetter(f, fref, this);
}

void StatePresenter::ConnectObjects()
{
    connect(m_messageGetter, &StateMessageGetter::ToUpdateLatLong, this, &StatePresenter::ToUpdateLatLong);
    connect(m_messageGetter, &StateMessageGetter::ToConsoleLog, this, &StatePresenter::ToConsoleLog);
    connect(this, &StatePresenter::ToChangeRangeToUkit, m_messageGetter, &StateMessageGetter::OnChangeRangeToUkit);
}

void StatePresenter::OnGetMessageWithState(const QByteArray &messageFromDevice)
{
    const QString currentValue = m_messageGetter->GetDataFromMessage(messageFromDevice);
    Q_EMIT ToConsoleLog(currentValue);
}

void StatePresenter::OnGetKoordinatesMessage(const QByteArray &messageWithKoordinates)
{
    m_messageGetter->GetKoordinatesFromSevenMessage(messageWithKoordinates);
}

void StatePresenter::DisconnectOldHandler()
{
    disconnect(m_dataHandler, &DataHandler::ToConsoleLog, this, &StatePresenter::ToConsoleLog);
    disconnect(m_dataHandler, &DataHandler::ToStateWidgetConsoleLog, this, &StatePresenter::ToConsoleLog);
    disconnect(m_dataHandler, &DataHandler::ToButtonsEnabledChanging, this, &StatePresenter::ToSetButtonsEnabled);
    disconnect(m_dataHandler, &DataHandler::ToStateGettingFromMessage, this, &StatePresenter::OnGetMessageWithState); \
    disconnect(m_dataHandler, &DataHandler::ToGetKoordinatesMessage, this, &StatePresenter::OnGetKoordinatesMessage);

}

void StatePresenter::ConnectHander(DataHandler *dataHandler)
{
    m_dataHandler = dataHandler;
    connect(m_dataHandler, &DataHandler::ToConsoleLog, this, &StatePresenter::ToConsoleLog);
    connect(m_dataHandler, &DataHandler::ToStateWidgetConsoleLog, this, &StatePresenter::ToConsoleLog);
    connect(m_dataHandler, &DataHandler::ToButtonsEnabledChanging, this, &StatePresenter::ToSetButtonsEnabled);
    connect(m_dataHandler, &DataHandler::ToStateGettingFromMessage, this, &StatePresenter::OnGetMessageWithState);
    connect(m_dataHandler, &DataHandler::ToGetKoordinatesMessage, this, &StatePresenter::OnGetKoordinatesMessage);
}

void StatePresenter::SetMessageToQueue(quint8 messageId, double firstParam, double SecondParam)
{
    m_dataHandler->SetHandlerState(HandlerState::Normal);
    switch (messageId)
    {
    case 1:
    {
        m_messagesQueue.enqueue(m_messageSetter->CreateFirstCommand(firstParam));
        break;
    }
    case 2:
    {
        m_messagesQueue.enqueue(m_messageSetter->CreateSecondCommand(firstParam));
        break;
    }
    case 3:
    {
        m_messagesQueue.enqueue(m_messageSetter->CreateThirdCommand(firstParam, SecondParam));
        break;
    }
    case 4:
    {
        m_messagesQueue.enqueue(m_messageSetter->CreateFourthCommand(firstParam, SecondParam));
        break;
    }
    case 5:
    {
        m_messagesQueue.enqueue(m_messageSetter->CreateFiveCommand(firstParam));
        break;
    }
    case 6:
    {
        m_messagesQueue.enqueue(m_messageSetter->CreateSixCommand(firstParam, SecondParam));
        break;
    }
    case 7:
    {
        m_messagesQueue.enqueue(m_messageSetter->CreateSevenCommand(firstParam));
        break;
    }
    case 9:
    {
        m_messagesQueue.enqueue(m_messageSetter->CreateNineCommand(firstParam));
        break;
    }
    default:
    {
        m_messagesQueue.enqueue(m_messageSetter->CreateZeroCommand());
        break;
    }
    }
}

void StatePresenter::SendBparMessage(quint8 foButtonId, bool isLcm, quint8 tksIndex, bool hasThreshold, quint16 currentThreshold, int distance, double distanceToUkit)
{
    const QByteArray messageBpar = m_messageSetter->CreateBparCommand(foButtonId, isLcm, tksIndex, hasThreshold, currentThreshold, distance, distanceToUkit);
    ToSendMessageToDeivce(messageBpar);
}

void StatePresenter::ToSendMessageToDeivce(const QByteArray &message)
{
    Q_EMIT ToConsoleLog("???????????? " + QString::number(message.size()) + " ???????????????? ???????????????? "  + QString::fromLatin1(message.toHex()));
    m_messagesQueue.enqueue(message);
}

void StatePresenter::GetStateFromDevice(quint8 messageIdWantToGet, quint8 secondParam)
{
    m_dataHandler->SetHandlerState(HandlerState::Normal);
    const QByteArray message(m_messageSetter->CreateSevenCommand(messageIdWantToGet, secondParam));
    m_messagesQueue.enqueue(message);
}

void StatePresenter::timerEvent(QTimerEvent *event)
{
    if (!m_messagesQueue.isEmpty())
    {
        Q_EMIT ToUpdateHistoryFile();
//        auto array = m_messagesQueue.dequeue();
//        Q_EMIT ToConsoleLog("???????????? " + QString::number(array.size()) + " ???????????????? ???????????????? "  + QString::fromLatin1(array.toHex()));
        m_dataHandler->SendMessageToDevice(m_messagesQueue.dequeue());
    }
}
