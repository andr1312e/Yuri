#ifndef PRESENTER_STATEPRESENTER_H
#define PRESENTER_STATEPRESENTER_H
#include <QObject>
#include <QtGlobal>

#include <tcpsocket.h>

#include <messageCreators/statemessagegetter.h>
#include <messageCreators/statemessagesender.h>

class StatePresenter :public QObject
{
    Q_OBJECT
public:
    StatePresenter(TcpSocket *socket);
    ~StatePresenter();
private:
    void CreateObjects();
    void CreateConnections();

private Q_SLOTS:

    void GetMessageWithState(QByteArray &byteArray);

    ///////////////////////////////////////////////////////////////////////////////////////
Q_SIGNALS:
    void SetButtonsEnabled(int state);
    void ConsoleLog(QString message);
    void UpdateHistoryFile();

    ///////////////////////////////////////////////////////////////////////////////////////

public:
    void SetStateToDevice(quint8 messageId, double firstParam=0.0, double SecondParam=0.0);
    void GetStateFromDevice(quint8 messageWantToGetId);



private:
    StateMessageSender *m_messageSetter;
    StateMessageGetter *m_messageGetter;
    TcpSocket *m_socket;
};

#endif // PRESENTER_STATEPRESENTER_H
