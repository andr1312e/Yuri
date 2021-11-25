#include "firmwarepresenter.h"

#include <QElapsedTimer>


FirmwarePresenter::FirmwarePresenter(QObject *parent)
    : QObject(parent)
    , m_dataHandler(Q_NULLPTR)
    , m_maxMessageSize(256)
    , m_maxBufferBytesCount(16)
    , m_bufferCounts(m_maxMessageSize/m_maxBufferBytesCount)
    , m_needToCheck(false)
    , m_isNeedToRestartAfterSuccess(false)
    , m_totalCountOfPages(0)
    , m_currentPage(0)
    , m_currentAdress(0)
{
    CreateObjects();
    InitObjects();
    ConnectObjects();
}

FirmwarePresenter::~FirmwarePresenter()
{
    delete m_timer;
    delete m_writingFirmwareTimer;
    delete m_erasingTimer;

    delete m_firmwareMessageMaker;
    delete m_commandsQueue;
    delete m_writinFirmwareCommandsList;
}

void FirmwarePresenter::CreateObjects()
{
    m_timer=new QTimer(this);
    m_writingFirmwareTimer=new QTimer(this);
    m_erasingTimer=new QTimer(this);
    m_firmwareMessageMaker=new FirmwareMessageMaker();
    m_commandsQueue=new QQueue<QByteArray>();
    m_writinFirmwareCommandsList=new std::list<QByteArray>();
    m_firmwareFromFile=Q_NULLPTR;
}

void FirmwarePresenter::InitObjects()
{
    m_timer->setInterval(3);
    m_timer->setTimerType(Qt::PreciseTimer);
    m_writingFirmwareTimer->setInterval(10);
    m_writingFirmwareTimer->setTimerType(Qt::PreciseTimer);
    m_erasingTimer->setInterval(1000);
    m_timer->start();
}

void FirmwarePresenter::ConnectObjects()
{
    connect(m_timer, &QTimer::timeout, this, &FirmwarePresenter::OnTimerTimeout);
    connect(m_writingFirmwareTimer, &QTimer::timeout, this, &FirmwarePresenter::OnWritingTimerTimeOut);
    connect(m_erasingTimer, &QTimer::timeout, this, &FirmwarePresenter::OnErasingTimerTimeOut);
}

void FirmwarePresenter::OnFlash(bool isNeedToCheck, bool isNeedToRestartAfterSuccess)
{
    m_timer->stop();
    Q_EMIT ToSetButtonsEnabled(false);
    m_dataHandler->SendMessageToDevice(*m_firmwareMessageMaker->m_makeWriteOnlyBufferCommand);

    m_dataHandler->SetHandlerState(HandlerState::Flash);
    m_dataHandler->SendMessageToDevice(*m_firmwareMessageMaker->m_eraseCommand);
    m_needToCheck=isNeedToCheck;

    m_isNeedToRestartAfterSuccess=isNeedToRestartAfterSuccess;
    m_writinFirmwareCommandsList->clear();
    m_commandsQueue->clear();
    m_currentPage=0;
    m_currentAdress=0;
    m_isPcbBisy=true;
    m_firmwareSize=m_firmwareFromFile.size();
    Q_EMIT ToConsoleLog(QStringLiteral("Начали формировать страницы"));
    const std::list<QByteArray> pagesOfFirmware(GenerateFirmwarePages(m_firmwareFromFile));
    Q_EMIT ToConsoleLog(QStringLiteral("Сформировали страницы. Формируем команды"));
    PrepareCommandsToFlash(pagesOfFirmware);
    Q_EMIT ToConsoleLog(QStringLiteral("Формирование команды готово. Форматируемся..."));
    m_erasingTimer->start();

}

void FirmwarePresenter::SendMessageToQueue(quint8 command, quint32 adress, quint8 lenght)
{
    switch (command) {
    case 0:
    {
        m_commandsQueue->enqueue(*m_firmwareMessageMaker->m_restartHardwareCommand);
        break;
    }
    case 3:
    {
        m_commandsQueue->enqueue(*m_firmwareMessageMaker->ReadToBufferCommand(adress, lenght));
        m_commandsQueue->enqueue(*m_firmwareMessageMaker->m_readFromBufferCommand);
        break;
    }
    case 4:
    {
        m_commandsQueue->enqueue(*m_firmwareMessageMaker->m_restartHardwareCommand);
        break;
    }
    case 5:
    {
        m_commandsQueue->enqueue(*m_firmwareMessageMaker->m_readRegisterStatusCommand);
        m_commandsQueue->enqueue(*m_firmwareMessageMaker->m_readFromBufferCommand);
        break;
    }
    case 6:
    {
        m_commandsQueue->enqueue(*m_firmwareMessageMaker->m_makeWriteOnlyBufferCommand);
        break;
    }
    case 9:
    {
        m_commandsQueue->enqueue(*m_firmwareMessageMaker->m_flashIdCommad);
        m_commandsQueue->enqueue(*m_firmwareMessageMaker->m_readFromBufferCommand);
        break;
    }
    case 199:
    {
        m_commandsQueue->enqueue(*m_firmwareMessageMaker->m_makeWriteOnlyBufferCommand);
        m_commandsQueue->enqueue(*m_firmwareMessageMaker->m_eraseCommand);
        break;
    }
    default:
    {
        qFatal("Нет команды");
    }

    }
}



void FirmwarePresenter::FillFullPageIntoBuffer(const QByteArray *partOfFirmware)
{
    QByteArray by=partOfFirmware->mid(0, m_maxBufferBytesCount);
    m_writinFirmwareCommandsList->push_back(m_firmwareMessageMaker->WriteDataToBufferCommand(true, by));
    int currentSubByteArrayIndex=m_maxBufferBytesCount;
    for (int i=1; i<m_bufferCounts; i++)
    {
        by=partOfFirmware->mid(currentSubByteArrayIndex, m_maxBufferBytesCount);
        m_writinFirmwareCommandsList->push_back(m_firmwareMessageMaker->WriteDataToBufferCommand(false, by));
        currentSubByteArrayIndex+=m_maxBufferBytesCount;
    }
}

void FirmwarePresenter::FillLastPageIntoBuffer(const QByteArray *partOfFirmware)
{
    if (partOfFirmware->count()<m_maxBufferBytesCount)
    {
        m_writinFirmwareCommandsList->push_back(m_firmwareMessageMaker->WriteDataToBufferCommand(true, *partOfFirmware));
    }
    else
    {
        QByteArray by=partOfFirmware->mid(0, m_maxBufferBytesCount);
        m_writinFirmwareCommandsList->push_back(m_firmwareMessageMaker->WriteDataToBufferCommand(true, by));
        int currentSubByteArrayIndex=m_maxBufferBytesCount;
        while (true) {
            if (currentSubByteArrayIndex+m_maxBufferBytesCount<partOfFirmware->count())
            {
                by=partOfFirmware->mid(currentSubByteArrayIndex, m_maxBufferBytesCount);
                m_writinFirmwareCommandsList->push_back(m_firmwareMessageMaker->WriteDataToBufferCommand(false, by));
                currentSubByteArrayIndex=currentSubByteArrayIndex+m_maxBufferBytesCount;
            }
            else
            {
                by= partOfFirmware->mid(currentSubByteArrayIndex);
                m_writinFirmwareCommandsList->push_back(m_firmwareMessageMaker->WriteDataToBufferCommand(false, by));
                break;
            }
        }
    }
}



void FirmwarePresenter::OnStartReadingFirmWareFromDevice()
{
    Q_EMIT ToConsoleLog(QStringLiteral("Начали читать страницы с устройства"));
    m_currentPage=0;
    m_currentAdress=0;
    m_dataHandler->ClearBuffer();
    m_dataHandler->SetHandlerState(HandlerState::ReadFirmware);
    OnReadFirmwareAgain();
}

void FirmwarePresenter::OnFirmwareFromDeviceLoaded(QByteArray *firmwareFromDevice)
{
    if ((m_firmwareFromFile)==(*firmwareFromDevice))
    {
        Q_EMIT ToConsoleLog(QStringLiteral("Верификация файла прошла успешно, прошивки совпадают"));

        if (m_isNeedToRestartAfterSuccess)
        {
            m_isNeedToRestartAfterSuccess=false;
            SendMessageToQueue(0, 0, 0);
        }
    }
    else
    {
        Q_EMIT ToConsoleLog(QStringLiteral("Верификация файла прошла не успешно. Надо перешится."));
    }
    m_timer->start();
    Q_EMIT ToSetButtonsEnabled(true);
}

void FirmwarePresenter::OnTimerTimeout()
{
    if(!m_commandsQueue->isEmpty())
    {
        QByteArray commandToExecute(m_commandsQueue->dequeue());
        Q_EMIT ToConsoleLog("Выслали команду  "+QString::fromLatin1(commandToExecute.toHex()));
        m_dataHandler->SendMessageToDevice(commandToExecute);
    }
}

void FirmwarePresenter::OnSetFirmwareFromFileToPresenter(QByteArray &firmwareFromFile)
{
    m_firmwareFromFile=firmwareFromFile;
}

void FirmwarePresenter::OnReadFirmwareAgain()
{
    Q_EMIT ToConsoleLog("Считываем страницу: " + QString::number(m_currentPage));
    Q_EMIT ToProgressBarSetValue(m_currentPage);
    m_currentPage++;
    m_dataHandler->SendMessageToDevice(*m_firmwareMessageMaker->ReadToBufferCommand(m_currentAdress, m_maxMessageSize-1));
    SleepMiliseconds(3);
    m_dataHandler->SendMessageToDevice(*m_firmwareMessageMaker->m_readFromBufferCommand);
    m_currentAdress=m_currentAdress+m_maxMessageSize;
}

void FirmwarePresenter::OnWritingTimerTimeOut()
{
    if(m_currentPage==m_totalCountOfPages)//list
    {
        m_writingFirmwareTimer->stop();
        if(m_needToCheck)
        {
            Q_EMIT ToConsoleLog(QStringLiteral("Записаны все страницы. Проверяемся"));
            m_needToCheck=false;
            OnStartReadingFirmWareFromDevice();
        }
        else
        {
            Q_EMIT ToSetButtonsEnabled(true);
            m_timer->start();
        }
    }
    else
    {
        if (m_currentPage<m_totalCountOfPages-1)
        {
            for (int i=0; i<18; ++i)
            {
                m_dataHandler->WriteMessageToBuffer(*m_writingFirmwareCommandsListIterator);
                ++m_writingFirmwareCommandsListIterator;
            }
            m_dataHandler->FlushBuffer();
            SleepMiliseconds(35);
        }
        else
        {
            while( m_writingFirmwareCommandsListIterator!=m_writinFirmwareCommandsList->end())
            {
                m_dataHandler->WriteMessageToBuffer(*m_writingFirmwareCommandsListIterator);
                ++m_writingFirmwareCommandsListIterator;
            }
            m_dataHandler->FlushBuffer();
        }
        Q_EMIT ToConsoleLog("Записана страница "+ QString::number(m_currentPage));
        ++m_currentPage;
        Q_EMIT ToProgressBarSetValue(m_currentPage);
    }
}

void FirmwarePresenter::OnErasingTimerTimeOut()
{
    Q_EMIT ToConsoleLog(QStringLiteral("Проверяем состояние регистров после форматирования:"));
    if (m_isPcbBisy)
    {
        Q_EMIT ToConsoleLog(QStringLiteral("Занят. Форматируется еще..."));
        Q_EMIT ToConsoleLog(QStringLiteral("Высылаем еще раз запрос на проверку состояния"));
        m_dataHandler->SendMessageToDevice(*m_firmwareMessageMaker->m_readRegisterStatusCommand);
        m_dataHandler->SendMessageToDevice(*m_firmwareMessageMaker->m_readFromBufferCommand);
    }
    else
    {
        Q_EMIT ToConsoleLog(QStringLiteral("Свободен. Шьемся"));
        m_erasingTimer->stop();
        m_writingFirmwareTimer->start();
    }
}

void FirmwarePresenter::OnTakedHardwareState(quint8 state)
{
    Q_EMIT ToConsoleLog("Состояние= "+ QString::number(state));
    if(state==0)
    {
        m_isPcbBisy=false;
    }
    else
    {
        m_isPcbBisy=true;
    }

}

void FirmwarePresenter::DisconnectOldHandler()
{
    m_commandsQueue->clear();
    disconnect(m_dataHandler, &DataHandler::ToFirmwareWidgetConsoleLog, this, &FirmwarePresenter::ToConsoleLog);
    disconnect(m_dataHandler, &DataHandler::ToConsoleLog, this, &FirmwarePresenter::ToConsoleLog);
    disconnect(m_dataHandler, &DataHandler::ToReadFirmwareAgain, this, &FirmwarePresenter::OnReadFirmwareAgain);
    disconnect(m_dataHandler, &DataHandler::ToButtonsEnabledChanging, this, &FirmwarePresenter::ToSetButtonsEnabled);
    disconnect(m_dataHandler, &DataHandler::ToRegisterStateChanging, this, &FirmwarePresenter::OnTakedHardwareState);
    disconnect(m_dataHandler, &DataHandler::ToFirmWareFormDeviceLoaded, this, &FirmwarePresenter::OnFirmwareFromDeviceLoaded);
}

void FirmwarePresenter::ConnectHander(DataHandler *dataHandler)
{
    m_dataHandler=dataHandler;
    connect(m_dataHandler, &DataHandler::ToFirmwareWidgetConsoleLog, this, &FirmwarePresenter::ToConsoleLog);
    connect(m_dataHandler, &DataHandler::ToConsoleLog, this, &FirmwarePresenter::ToConsoleLog);
    connect(m_dataHandler, &DataHandler::ToReadFirmwareAgain, this, &FirmwarePresenter::OnReadFirmwareAgain);
    connect(m_dataHandler, &DataHandler::ToButtonsEnabledChanging, this, &FirmwarePresenter::ToSetButtonsEnabled);
    connect(m_dataHandler, &DataHandler::ToRegisterStateChanging, this, &FirmwarePresenter::OnTakedHardwareState);
    connect(m_dataHandler, &DataHandler::ToFirmWareFormDeviceLoaded, this, &FirmwarePresenter::OnFirmwareFromDeviceLoaded);
}

const QByteArray FirmwarePresenter::MidPartOfFirmware(quint32 currentIndex, QByteArray &firmwareFromFile) const
{
    if (currentIndex+m_maxMessageSize<m_firmwareSize)
    {
        return firmwareFromFile.mid(currentIndex, m_maxMessageSize);
    }
    else
    {
        return firmwareFromFile.mid(currentIndex);
    }
}

const std::list<QByteArray> FirmwarePresenter::GenerateFirmwarePages(QByteArray &firmwareFromFile) const
{
    std::list<QByteArray> pagesOfFirmware;
    quint32 currentAdress=0;
    while(true)
    {
        const QByteArray partOfPage=MidPartOfFirmware(currentAdress, firmwareFromFile);
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
    return pagesOfFirmware;
}

void FirmwarePresenter::PrepareCommandsToFlash(const std::list<QByteArray> &pagesOfFirmware)
{
    Q_EMIT ToConsoleLog(QStringLiteral("Делаем команды"));
    quint32 currentAdress=0;
    for(std::list<QByteArray>::const_iterator it=pagesOfFirmware.begin(); it!=pagesOfFirmware.end(); ++it)
    {
        m_writinFirmwareCommandsList->push_back(*m_firmwareMessageMaker->m_makeWriteOnlyBufferCommand);
        if(it->count()==m_maxMessageSize)
        {
            FillFullPageIntoBuffer(&*it);
        }
        else
        {
            FillLastPageIntoBuffer(&*it);
        }
        m_writinFirmwareCommandsList->push_back(m_firmwareMessageMaker->WriteBufferToFlashCommand(currentAdress, it->length()));
        currentAdress+=m_maxMessageSize;
    }
    m_writingFirmwareCommandsListIterator=m_writinFirmwareCommandsList->begin();
    Q_EMIT ToConsoleLog(QStringLiteral("Вычисляем колличество страниц в прошивке"));
    m_totalCountOfPages=pagesOfFirmware.size();
    Q_EMIT ToSetMaximumProgressBar(m_totalCountOfPages);
    Q_EMIT ToConsoleLog("Страниц в прошивке: "+ QString::number(m_totalCountOfPages));
}

void FirmwarePresenter::SleepMiliseconds(int ms)
{
#ifdef Q_OS_WIN
    Sleep(uint(ms));
#else
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
#endif
}
