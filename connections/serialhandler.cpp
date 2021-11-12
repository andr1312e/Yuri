#include "serialhandler.h"

SerialHandler::SerialHandler(QObject *parent)
    : DataHandler(parent)
{
    CreateHander();
    ConnectObjects();
}

SerialHandler::~SerialHandler()
{
    delete m_connectionPort;
}

void SerialHandler::CreateHander()
{
    m_connectionPort=new QSerialPort(this);
    m_connectionPort->setBaudRate(QSerialPort::Baud115200);
    m_connectionPort->setDataBits(QSerialPort::Data8);
    m_connectionPort->setParity(QSerialPort::EvenParity);
    m_connectionPort->setStopBits(QSerialPort::OneStop);
}

void SerialHandler::ConnectObjects()
{
    connect(m_connectionPort, &QSerialPort::readyRead, this, &SerialHandler::OnReadyRead);
    connect(m_connectionPort, &QSerialPort::errorOccurred, this, &SerialHandler::OnErrorOccurred);
}

void SerialHandler::OnReadyRead()
{
    switch (m_gettingMessageType) {
    case Normal:
    {
        NormalStateMessageAnalyze(m_connectionPort->readAll());
        break;
    }
    case ReadFirmware:
    {
        ReadFirmwareMessageAnalyze(m_connectionPort->readAll());
        break;
    }
    case Flash:
    {
        FlashFirmwareMessageAnalyze(m_connectionPort->readAll());
        break;
    }
    }
}

void SerialHandler::OnErrorOccurred(QSerialPort::SerialPortError error)
{
    FromHostDisconnect();
    switch (error) {
    case QSerialPort::NoError:
        break;
    case QSerialPort::DeviceNotFoundError:
        Q_EMIT ToConsoleLog(QStringLiteral("Произошла ошибка при попытке открыть несуществующее устройство. "));
        break;
    case QSerialPort::PermissionError:
        Q_EMIT ToConsoleLog(QStringLiteral("Произошла ошибка при попытке открыть уже открытое устройство другим процессом или пользователем, не имеющим достаточных разрешений и учетных данных для открытия. "));
        break;
    case QSerialPort::OpenError:
        Q_EMIT ToConsoleLog(QStringLiteral("Произошла ошибка при попытке открыть уже открытое устройство в этом объекте. "));
        break;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    case QSerialPort::ParityError:
        Q_EMIT ToConsoleLog(QStringLiteral("Ошибка четности обнаружена оборудованием при чтении данных. "));
        break;
    case QSerialPort::FramingError:
        Q_EMIT ToConsoleLog(QStringLiteral("Ошибка кадрирования обнаружена оборудованием при чтении данных."));
        break;
    case QSerialPort::BreakConditionError:
        Q_EMIT ToConsoleLog(QStringLiteral("Обрыв, обнаруженный оборудованием на входной линии. "));
        break;
#endif
    case QSerialPort::WriteError:
        Q_EMIT ToConsoleLog(QStringLiteral("Ошибка ввода-вывода при записи данных."));
        break;
    case QSerialPort::ReadError:
        Q_EMIT ToConsoleLog(QStringLiteral("Ошибка ввода-вывода при чтении данных. "));
        break;
    case QSerialPort::ResourceError:
        Q_EMIT ToConsoleLog(QStringLiteral("Ошибка ввода-вывода произошла, когда ресурс стал недоступен, например при неожиданном удалении устройства из системы. "));
        break;
    case QSerialPort::UnsupportedOperationError:
        Q_EMIT ToConsoleLog(QStringLiteral("Запрошенная операция устройства не поддерживается или запрещена работающей операционной системой. "));
        break;
    case QSerialPort::UnknownError:
        Q_EMIT ToConsoleLog(QStringLiteral("Неизвестная ошибка"));
        break;
    case QSerialPort::TimeoutError:
        Q_EMIT ToConsoleLog(QStringLiteral("Произошла ошибка тайм-аута."));
        break;
    case QSerialPort::NotOpenError:
        Q_EMIT ToConsoleLog(QStringLiteral("Не подключено. Эта ошибка возникает при выполнении операции, которая может быть успешно выполнена только в том случае, если устройство доступно. Пытаемся соединится..."));
        break;

    }
}

void SerialHandler::TryToConnectToHost(const QString &comPortName)
{
    m_connectionPort->setPortName(comPortName);
    if(m_connectionPort->open(QIODevice::ReadWrite))
    {
        DataHandler::ToHostConnected();
    }
}

void SerialHandler::WriteMessageToBuffer(const QByteArray &array)
{
    m_connectionPort->write(array);
}

void SerialHandler::FlushBuffer()
{
    m_connectionPort->flush();
}

void SerialHandler::FromHostDisconnect()
{
    if(m_connectionPort->isOpen())
    {
        m_connectionPort->close();
    }
}
