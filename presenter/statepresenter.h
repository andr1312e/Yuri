#ifndef PRESENTER_STATEPRESENTER_H
#define PRESENTER_STATEPRESENTER_H

#include <QObject>
#include <QtGlobal>
#include <QScopedPointer>
#include <QSharedPointer>
#include "connections/datahandler.h"

#include "services/settingfileservice.h"

#include <messageCreators/statemessagegetter.h>
#include <messageCreators/statemessagesender.h>

class StatePresenter : public QObject
{
    Q_OBJECT
public:
    explicit StatePresenter(QSharedPointer<SettingFileService> &settingFileService, QObject *parent);
    ~StatePresenter();
private:
    void CreateObjects();

Q_SIGNALS:
    void ToSetButtonsEnabled(int state);
    void ToConsoleLog(QString message);
    void ToUpdateHistoryFile();
private Q_SLOTS:
    void OnGetMessageWithState(QByteArray &messageFromDevice);

public:
    void DisconnectOldHandler();
    void ConnectHander(DataHandler *dataHandler);
    void SetStateToDevice(quint8 messageId, double firstParam = 0.0, double SecondParam = 0.0);
    void ToSendMessageToDeivce(const QByteArray &arr);
    void GetStateFromDevice(quint8 messageIdWantToGet);

private:
    QSharedPointer<SettingFileService> m_settingFileService;
    DataHandler *m_dataHandler;

    QSharedPointer <StateMessageSender> m_messageSetter;
    QSharedPointer <StateMessageGetter> m_messageGetter;
};

#endif // PRESENTER_STATEPRESENTER_H
