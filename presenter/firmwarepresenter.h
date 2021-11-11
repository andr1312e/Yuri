#ifndef PRESENTER_FIRMWAREPRESENTER_H
#define PRESENTER_FIRMWAREPRESENTER_H

#include <QMap>
#include <QDebug>
#include <QQueue>
#include <QString>
#include <QTimer>
#include <QByteArray>


#ifdef Q_OS_WIN
#include <windows.h>
#endif
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QLinkedList>
#else
#include <QList>
#endif

#include "connections/datahandler.h"

#include "messageCreators/firmwarecommansmaker.h"

class FirmwarePresenter :public QObject
{
    Q_OBJECT
public:
    FirmwarePresenter(const QMap<QString, quint8> *actions, QObject *parent);
    ~FirmwarePresenter();

private:
    void CreateObjects();
    void InitObjects();
    void ConnectObjects();

Q_SIGNALS:
    void ToSetButtonsEnabled(int state);
    void ToConsoleLog(QString errorMessage);
    void ToSetMaximumCountOfPages(int pageNum);
    void ToPageUpdated(int pageNum);
    void ToWidgetsEnable(bool state);


public Q_SLOTS:
    void WhenFlash(QByteArray *firmwareFromFile);
    void WhenStartReadingFirmWareFromDevice();
private Q_SLOTS:
    void WhenTimerTimeout();
    void WhenReadFirmwareAgain();
    void WhenWritingTimerTimeOut();
    void WhenErasingTimerTimeOut();
    void WhenPageUpdated(QByteArray &firmware);
    void WhenTakedHardwareState(quint8 state);

public:
    void DisconnectOldHandler();
    void ConnectHander(DataHandler *dataHandler);
    void GetDataFromWidget(const QString &id, const QString& adress, const QString &lenght);
    void SendMessageToQueue(quint8 command, quint32 adress, quint8 lenght);
    void SetFirmwareSourceFromFile(QByteArray *firmwareFromFile);

private:
    void PrepareCommandsToFlash(QLinkedList<QByteArray> &pagesOfFirmware);
    void FillFullPageIntoBuffer(QByteArray *partOfFirmware);
    void FillLastPageIntoBuffer(QByteArray *partOfFirmware);
    const QByteArray GetPartOfFirmwareFromArray(quint32 currentIndex, QByteArray *firmwareFromFile) const;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QLinkedList<QByteArray> GenerateFirmwarePages (QByteArray *firmwareFromFile);
#else
    QList<QByteArray> GenerateFirmwarePages ();
#endif
    void SleepMiliseconds(int ms);
private:
    DataHandler *m_dataHandler;
    const quint16 m_maxMessageSize;
    const quint8 m_maxBufferBytesCount;
    const quint8 m_bufferCounts;

    quint32 m_totalCountOfPages;
    quint32 m_currentPage;
    quint32 m_currentAdress;

    const QMap<QString, quint8> *m_actions;
private:
    QTimer *m_timer;
    QTimer *m_writingFirmwareTimer;
    QTimer *m_erasingTimer;
private:
    FirmwareMessageMaker *m_firmwareMessageMaker;

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QLinkedList<QByteArray> m_pagesOfFirmware;
    QLinkedList<QByteArray> *m_writinFirmwareCommandsList;
    QLinkedList<QByteArray>::iterator m_writingFirmwareCommandsListIterator;
    QLinkedList<QByteArray>::iterator m_endOfWritingFimrwareCommandsList;
#else
    QList<QByteArray> m_pagesOfFirmware;
    QList<QByteArray> *m_writinFirmwareCommandsList;
    QList<QByteArray>::iterator m_writingFirmwareCommandsListIterator;
    QList<QByteArray>::iterator m_endOfWritingFimrwareCommandsList;
#endif
    QByteArray  *m_firmwareFromFile;
    quint32 m_firmwareSize;

    QQueue<QByteArray> *m_commandsQueue;

    bool m_isPcbBisy;

    QByteArray m_firmwareOnDevice;

};

#endif // PRESENTER_FIRMWAREPRESENTER_H
