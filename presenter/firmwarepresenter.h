#ifndef PRESENTER_FIRMWAREPRESENTER_H
#define PRESENTER_FIRMWAREPRESENTER_H

#include <QDebug>
#include <QQueue>
#include <QString>
#include <QTimer>
#include <QByteArray>
#include <QDateTime>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include "services/settingfileservice.h"

#include "connections/datahandler.h"

#include "messageCreators/firmwarecommansmaker.h"

class FirmwarePresenter :public QObject
{
    Q_OBJECT
public:
    FirmwarePresenter(QSharedPointer<SettingFileService> &settingFileService, QObject *parent);
    ~FirmwarePresenter();
private:
    void CreateObjects();
    void InitObjects();
    void ConnectObjects();

Q_SIGNALS:
    void ToSetButtonsEnabled(bool state);
    void ToConsoleLog(QString errorMessage);
    void ToSetMaximumProgressBar(int maxPageNum);
    void ToProgressBarSetValue(int pageNum);
    void ToResultLabelSetText(const QString &text);
public Q_SLOTS:
    void OnSetFirmwareFromFileToPresenter(QByteArray &firmwareFromFile);
    void OnFlash(bool isNeedToCheck, bool isNeedToRestartAfterSuccess);
    void OnStartReadingFirmWareFromDevice();
private Q_SLOTS:
    void OnTimerTimeout();
    void OnReadFirmwareAgain();
    void OnWritingTimerTimeOut();
    void OnErasingTimerTimeOut();
    void OnTakedHardwareState(quint8 state);
    void OnFirmwareFromDeviceLoaded(QByteArray *firmwareFromDevice);

public:
    void DisconnectOldHandler();
    void ConnectHander(DataHandler *dataHandler);
    void SendMessageToQueue(quint8 command, quint32 adress, quint8 lenght);
private:
    const QByteArray MidPartOfFirmware(quint32 currentIndex, QByteArray &firmwareFromFile) const;
    const std::list<QByteArray> GenerateFirmwarePages(QByteArray &firmwareFromFile) const;

    void FillFullPageIntoBuffer(const QByteArray *partOfFirmware);
    void FillLastPageIntoBuffer(const QByteArray *partOfFirmware);

    void PrepareCommandsToFlash(const std::list<QByteArray> &pagesOfFirmware);
    void SleepMiliseconds(int ms);
private:
    DataHandler *m_dataHandler;
    const quint16 m_maxMessageSize;
    const quint8 m_maxBufferBytesCount;
    const quint8 m_bufferCounts;
    bool m_needToCheck;
    bool m_isNeedToRestartAfterSuccess;

    quint32 m_totalCountOfPages;
    quint32 m_currentPage;
    quint32 m_currentAdress;

private:
    QTimer *m_timer;
    QTimer *m_writingFirmwareTimer;
    QTimer *m_erasingTimer;
private:
    QSharedPointer<SettingFileService> m_settingFileService;
    const QString m_lastFirmwareAttribute="lastFirmware";
    FirmwareMessageMaker *m_firmwareMessageMaker;

    std::list<QByteArray> *m_writinFirmwareCommandsList;
    std::list<QByteArray>::iterator m_writingFirmwareCommandsListIterator;
    std::list<QByteArray>::iterator m_endOfWritingFimrwareCommandsList;

    QByteArray m_firmwareFromFile;
    QByteArray *m_firmwareFromDevice;
    quint32 m_firmwareSize;

    QQueue<QByteArray> *m_commandsQueue;

    bool m_isPcbBisy;
};

#endif // PRESENTER_FIRMWAREPRESENTER_H
