#ifndef PRESENTER_STATEPRESENTER_H
#define PRESENTER_STATEPRESENTER_H

#include <QObject>
#include <QtGlobal>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QDebug>
#include <QByteArray>
#include <QWeakPointer>
#include <QQueue>

#include "connections/datahandler.h"

#include "services/settingfileservice.h"

#include "messageCreators/statemessagegetter.h"
#include "messageCreators/statemessagesender.h"

class StatePresenter : public QObject
{
    Q_OBJECT
public:
    explicit StatePresenter(SettingFileService *settingFileService, QObject *parent);
    ~StatePresenter();
private:
    void CreateObjects();

Q_SIGNALS:
    void ToSetButtonsEnabled(int state);
    void ToConsoleLog(QString message);
    void ToUpdateHistoryFile();
private Q_SLOTS:
    void OnGetMessageWithState(const QByteArray &messageFromDevice);
public:
    void DisconnectOldHandler();
    void ConnectHander(DataHandler *dataHandler);
    void SetMessageToQueue(quint8 messageId, double firstParam = 0.0, double SecondParam = 0.0);
    void SendBparMessage(quint8 foButtonId, bool isLcm, quint8 tksIndex, bool hasThreshold, quint16 currentThreshold, int distance);
    void ToSendMessageToDeivce(const QByteArray &arr);
    void GetStateFromDevice(quint8 messageIdWantToGet);
protected:
    void virtual timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
private:
    QQueue<QByteArray> m_messagesQueue;
    SettingFileService *m_settingFileService;
    DataHandler *m_dataHandler;

    StateMessageSender *m_messageSetter;
    StateMessageGetter *m_messageGetter;
};

#endif // PRESENTER_STATEPRESENTER_H
