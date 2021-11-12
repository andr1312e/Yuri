#ifndef PRESENTER_STATEPRESENTER_H
#define PRESENTER_STATEPRESENTER_H
#include <QObject>
#include <QtGlobal>
#include "connections/datahandler.h"
#include <messageCreators/statemessagegetter.h>
#include <messageCreators/statemessagesender.h>

class StatePresenter :public QObject
{
    Q_OBJECT
public:
    explicit StatePresenter();
    ~StatePresenter();
private:
    void CreateObjects();

private Q_SLOTS:
    void WhenGetMessageWithState(QByteArray &messageFromDevice);
Q_SIGNALS:
    void ToSetButtonsEnabled(int state);
    void ToConsoleLog(QString message);
    void ToUpdateHistoryFile();

public:
    void DisconnectOldHandler();
    void ConnectHander(DataHandler *dataHandler);
    void SetStateToDevice(quint8 messageId, double firstParam=0.0, double SecondParam=0.0);
    void GetStateFromDevice(quint8 messageIdWantToGet);

private:
    DataHandler *m_dataHandler;
    StateMessageSender *m_messageSetter;
    StateMessageGetter *m_messageGetter;
};

#endif // PRESENTER_STATEPRESENTER_H
