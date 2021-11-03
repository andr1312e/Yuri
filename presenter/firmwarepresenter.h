#ifndef PRESENTER_FIRMWAREPRESENTER_H
#define PRESENTER_FIRMWAREPRESENTER_H

#include <QMap>
#include <chrono>
#include <QQueue>
#include <QString>
#include <forward_list>
#include <QElapsedTimer>


#include "messageCreators/firmwarecommansmaker.h"
#include "tcpsocket.h"

class FirmwarePresenter:public QObject
{
    Q_OBJECT
public:
    FirmwarePresenter(const int &blockSize, const QMap<QString, quint8> *commands, TcpSocket *socket);
    ~FirmwarePresenter();
private:
    void CreateObjects();
    void InitTimer();
    void CreateConnections();

    ///////////////////////////////////////////////////////////////////////////////////////

public Q_SLOTS:
    void Flash();
    void StartReadingFirmWareFromDevice();
Q_SIGNALS:
    void SetButtonsEnabled(int state);
    void ConsoleLog(QString errorMessage);
    void SendFirmwareSourcePartFromDevice(QByteArray &firmware);
private Q_SLOTS:
    void CheckCommandsQueue();
    void GetFirmwareSourcePartFromDevice(QByteArray &firmwarePartSource);
    void GetPartOfFirmWareAgain();
    void WritePartOfFirmWareAgain();
    void UpdateHardwareState(quint8 state);
    ////////////////////////////////////////////////////////////////////////////////////////

public:
    void GetDataFromWidget(const QString &id, const QString& adress, const QString &lenght);
    void SendMessageToQueue(quint8 command, quint32 adress, quint8 lenght);
    void SetFirmwareSource(QByteArray *firmwareSource);

private:
    void EraseDevice();
    const QByteArray GetPartOfFirmwareFromDevice(const quint32 &currentIndex) const;
    void GetFirmwarePagesIntoList( std::list<QByteArray> &pagesOfFirmware);
    void SleepThread(int ms);
private:
    quint32 m_numOfPages;
    quint64 iteration=0;
    quint16 statusRead=0;
    quint16 pages=0;
    QElapsedTimer qelapsedTimer;

    const quint8 m_blockSize;
    const int m_writeBlockSize;
    const QMap<QString, quint8> *m_commands;

    FirmwareMessageMaker *m_firmwareMessageMaker;
    TcpSocket *m_socket;

    QTimer *m_timer;
    QTimer *m_wirtingTimer;
    QQueue<QByteArray> *m_commandsQueue;

    std::list<QByteArray> *m_writinFirmwareCommandssList;
    std::list<QByteArray>::iterator m_writingFirmwareCommandsListIterator;
    std::list<QByteArray>::iterator m_endOfFimrwareCommandsList;

    bool m_PcbIsBisy;
    bool m_noDataSended=true;

    QByteArray m_firmwareOnDevice;
    QByteArray  *m_firmwareFromFile;
    quint32 m_currentAdress;

    quint32 m_firmwareSize;

};

#endif // PRESENTER_FIRMWAREPRESENTER_H
