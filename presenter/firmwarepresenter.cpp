#include "firmwarepresenter.h"

#include <QByteArray>
#include <QElapsedTimer>
#include <QList>
#include <QThread>
#ifdef Q_OS_WIN
#include <windows.h> // for Sleep
#endif

FirmwarePresenter::FirmwarePresenter(const int &blockSize, const QMap<QString, quint8> *commands, TcpSocket *socket)
    : m_blockSize(blockSize)
    , m_writeBlockSize(256)
    , m_commands(commands)
    , m_socket(socket)
    , m_currentAdress(0)
{
    CreateObjects();
    InitTimer();
    CreateConnections();
}

FirmwarePresenter::~FirmwarePresenter()
{
    delete m_firmwareMessageMaker;
    delete m_timer;
    delete m_commandsQueue;
}

void FirmwarePresenter::CreateObjects()
{
    m_firmwareMessageMaker=new FirmwareMessageMaker();
    m_timer=new QTimer(this);
    m_wirtingTimer=new QTimer(this);
    m_commandsQueue=new QQueue<QByteArray>();
    m_writinFirmwareCommandssList=new std::list<QByteArray>();
}

void FirmwarePresenter::InitTimer()
{
    m_timer->setInterval(3);
    m_timer->setTimerType(Qt::PreciseTimer);
    m_wirtingTimer->setInterval(10);
    m_wirtingTimer->setTimerType(Qt::PreciseTimer);
    m_timer->start();
}

void FirmwarePresenter::CreateConnections()
{
    connect(m_timer, &QTimer::timeout, this, &FirmwarePresenter::CheckCommandsQueue);
    connect(m_wirtingTimer, &QTimer::timeout, this, &FirmwarePresenter::WritePartOfFirmWareAgain);
    connect(m_socket, &TcpSocket::FirmwareConsoleLog, this, &FirmwarePresenter::ConsoleLog);
    connect(m_socket, &TcpSocket::ConsoleLog, this, &FirmwarePresenter::ConsoleLog);
    connect(m_socket, &TcpSocket::SendFirmwareSourcePartFromDevice, this, &FirmwarePresenter::GetFirmwareSourcePartFromDevice);
    connect(m_socket, &TcpSocket::GetPartOfFirmWareAgain, this, &FirmwarePresenter::GetPartOfFirmWareAgain);
    connect(m_socket, &TcpSocket::SetButtonsEnabled, this, &FirmwarePresenter::SetButtonsEnabled);
    connect(m_socket, &TcpSocket::UpdateHardwareState, this, &FirmwarePresenter::UpdateHardwareState);
}

void FirmwarePresenter::SendMessageToQueue(quint8 command, quint32 adress, quint8 lenght)
{
    switch (command) {
    case 0:
    {
        m_commandsQueue->enqueue(m_firmwareMessageMaker->RestartFlashCommand());
        break;
    }
    case 2:
    {
        QByteArray by="!!!!!!!";
        m_commandsQueue->enqueue(m_firmwareMessageMaker->WriteBufferToFlashCommand(adress, by.length()-1));
        m_commandsQueue->enqueue(m_firmwareMessageMaker->WriteDataToBufferCommand(true, by));
        break;
    }
    case 3:
    {
        m_commandsQueue->enqueue(m_firmwareMessageMaker->ReadToBufferCommand(adress, lenght));
        m_commandsQueue->enqueue(*m_firmwareMessageMaker->m_ReadFromBufferCommand);
        break;
    }
    case 4:
    {
        m_commandsQueue->enqueue(m_firmwareMessageMaker->MakeReadOnlyBufferCommand());
        break;
    }
    case 5:
    {
        m_commandsQueue->enqueue(*m_firmwareMessageMaker->m_ReadRegisterStatusCommand);
        m_commandsQueue->enqueue(*m_firmwareMessageMaker->m_ReadFromBufferCommand);
        break;
    }
    case 6:
    {
        m_commandsQueue->enqueue(m_firmwareMessageMaker->MakeWriteOnlyBufferCommand());
        break;
    }
    case 9:
    {
        m_commandsQueue->enqueue(m_firmwareMessageMaker->m_flashIdCommad);
        m_commandsQueue->enqueue(*m_firmwareMessageMaker->m_ReadFromBufferCommand);
        break;
    }
    case 199:
    {
        m_commandsQueue->enqueue(m_firmwareMessageMaker->EraseCommand());
        break;
    }
    default:
    {
        qDebug()<< "Нет команды";
    }

    }
}

void FirmwarePresenter::SetFirmwareSource(QByteArray *firmwareSource)
{
    m_firmwareFromFile=firmwareSource;
}

void FirmwarePresenter::Flash()
{
    Q_EMIT ConsoleLog("начали формировать страницы");
    m_writinFirmwareCommandssList->clear();
    quint32 currentAdress=0;
    std::list<QByteArray> pagesOfFirmware;
    m_timer->stop();
    GetFirmwarePagesIntoList(pagesOfFirmware);
    m_socket->SetSocketState(2);
    //    EraseDevice();
    qDebug()<< pagesOfFirmware.begin()->toHex();
    for(std::list<QByteArray>::iterator it=pagesOfFirmware.begin(); it!=pagesOfFirmware.cend(); ++it)
    {
        m_writinFirmwareCommandssList->push_back(m_firmwareMessageMaker->MakeWriteOnlyBufferCommand());
        if(it->count()==m_writeBlockSize)
        {

            QByteArray by=it->mid(0, m_blockSize);
            m_writinFirmwareCommandssList->push_back(m_firmwareMessageMaker->WriteDataToBufferCommand(true, by));
            int currentSubByteArrayIndex=m_blockSize;
            for (int i=1; i<m_blockSize; i++)
            {
                by=it->mid(currentSubByteArrayIndex, m_blockSize);
                m_writinFirmwareCommandssList->push_back(m_firmwareMessageMaker->WriteDataToBufferCommand(false, by));
                currentSubByteArrayIndex+=m_blockSize;
            }

        }
        else
        {
            qDebug()<< pagesOfFirmware.back().toHex();
            if (it->count()<16)
            {
                QByteArray by=*it;
                m_writinFirmwareCommandssList->push_back(m_firmwareMessageMaker->WriteDataToBufferCommand(true, by));
            }
            else
            {
                QByteArray by=it->mid(0, m_blockSize);
                m_writinFirmwareCommandssList->push_back(m_firmwareMessageMaker->WriteDataToBufferCommand(true, by));
                int currentSubByteArrayIndex=m_blockSize;
                while (true) {
                    if (currentSubByteArrayIndex+m_blockSize<it->count())
                    {
                        by=it->mid(currentSubByteArrayIndex, m_blockSize);
                        m_writinFirmwareCommandssList->push_back(m_firmwareMessageMaker->WriteDataToBufferCommand(false, by));
                        currentSubByteArrayIndex=currentSubByteArrayIndex+m_blockSize;
                    }
                    else
                    {
                        by= it->mid(currentSubByteArrayIndex);
                        m_writinFirmwareCommandssList->push_back(m_firmwareMessageMaker->WriteDataToBufferCommand(false, by));
                        break;
                    }
                }
            }
        }
        m_writinFirmwareCommandssList->push_back(m_firmwareMessageMaker->WriteBufferToFlashCommand(currentAdress, it->length()));
        currentAdress+=m_writeBlockSize;
    }
    m_writingFirmwareCommandsListIterator=m_writinFirmwareCommandssList->begin();
//    for (int i=0; i<30; i++)//12
//    {
//        m_writingFirmwareCommandsListIterator.operator--();
//    }
//    qDebug()<< *m_writingFirmwareCommandsListIterator;
//    for (m_writingFirmwareCommandsListIterator; m_writingFirmwareCommandsListIterator!=m_writinFirmwareCommandssList->end(); ++m_writingFirmwareCommandsListIterator)
//    {
//        qDebug()<< m_writingFirmwareCommandsListIterator->toHex();
//    }
//    m_endOfFimrwareCommandsList=m_writinFirmwareCommandssList->end();
    m_numOfPages=pagesOfFirmware.size();
    Q_EMIT ConsoleLog("готово отсылаем");
    qelapsedTimer.start();
    m_wirtingTimer->start(10);
}

void FirmwarePresenter::StartReadingFirmWareFromDevice()
{
    m_currentAdress=0;
    m_socket->SetSocketState(1);
    GetPartOfFirmWareAgain();
}

void FirmwarePresenter::CheckCommandsQueue()
{
    if(!m_commandsQueue->isEmpty())
    {
        QByteArray ba(m_commandsQueue->dequeue());
        Q_EMIT ConsoleLog("SEND  "+ba.toHex());
        m_socket->send(ba);
    }
}

void FirmwarePresenter::GetFirmwareSourcePartFromDevice(QByteArray &firmwarePartSource)
{
    m_firmwareOnDevice.append(firmwarePartSource);
    QByteArray hex=firmwarePartSource.toHex();
    Q_EMIT SendFirmwareSourcePartFromDevice(firmwarePartSource);
}

void FirmwarePresenter::GetPartOfFirmWareAgain()
{
    //    qDebug()<< "m_currentAdress="<< m_currentAdress;
    SendMessageToQueue(3, m_currentAdress, m_blockSize);
    m_currentAdress=m_currentAdress+m_blockSize;
}

void FirmwarePresenter::WritePartOfFirmWareAgain()
{
    if(pages>=m_numOfPages)//list
    {
        Q_EMIT ConsoleLog(QString::number(qelapsedTimer.elapsed()));
        m_wirtingTimer->stop();
        m_timer->start();
    }
    else
    {
        {
            {
                qDebug()<< "page" << pages;
                for (int i=0; i<18; i++)
                {
//                    qDebug()<< *m_writingFirmwareCommandsListIterator;
                    m_socket->SendPartOfFirmware(*m_writingFirmwareCommandsListIterator);
                    ++m_writingFirmwareCommandsListIterator;
                }
                m_socket->Flush();
                m_PcbIsBisy=true;
                ++pages;

                Q_EMIT ConsoleLog("page"+ QString::number(pages));
                SleepThread(32);
            }
        }
    }
}

void FirmwarePresenter::UpdateHardwareState(quint8 state)
{
    qDebug()<< "state " << state /*<< " iter " << iteration*/;
    m_PcbIsBisy=false;
    //    if(state==0)
    //    {
    //        m_PcbIsBisy=false;
    //    }
    //    else
    //    {
    //        m_PcbIsBisy=true;
    //    }

}

void FirmwarePresenter::GetDataFromWidget(const QString &id, const QString &adress, const QString &lenght)
{
    m_socket->SetSocketState(2);
    quint8 idVal=m_commands->value(id);
    quint32 adressVal=adress.toUInt();
    quint8 lenghtVal=lenght.toUInt();
    SendMessageToQueue(idVal, adressVal, lenghtVal);
}

void FirmwarePresenter::EraseDevice()
{
    QByteArray by=m_firmwareMessageMaker->MakeWriteOnlyBufferCommand();
    m_socket->send(by);
    by=m_firmwareMessageMaker->EraseCommand();
    m_socket->send(by);
    m_PcbIsBisy=true;
}

const QByteArray FirmwarePresenter::GetPartOfFirmwareFromDevice(const quint32 &currentIndex) const
{
    if (currentIndex+m_writeBlockSize<m_firmwareSize)
    {
        return m_firmwareFromFile->mid(currentIndex, m_writeBlockSize);
    }
    else
    {
        return m_firmwareFromFile->mid(currentIndex);
    }
}


void FirmwarePresenter::GetFirmwarePagesIntoList(std::list<QByteArray> &pagesOfFirmware)
{
    quint32 currentAdress=0;
    while(true)
    {
        const QByteArray partOfPage=GetPartOfFirmwareFromDevice(currentAdress);
        if(partOfPage.isEmpty())
        {
            break;
        }
        else
        {
            currentAdress+=partOfPage.length();
            pagesOfFirmware.push_back(partOfPage);
        }
    }
}

void FirmwarePresenter::SleepThread(int ms)
{
#ifdef Q_OS_WIN
    Sleep(uint(ms));
#else
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
#endif
}
