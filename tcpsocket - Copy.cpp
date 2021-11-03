#include "tcpsocket.h"

TcpSocket::TcpSocket(QObject *parent)
    : QObject(parent)
    , m_tcpSocket(new QTcpSocket(this))
{
    createConnections();
}

TcpSocket::~TcpSocket()
{
    delete m_tcpSocket;
}

void TcpSocket::createConnections()
{
    connect(m_tcpSocket, &QTcpSocket::connected, this, &TcpSocket::connected);
    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &TcpSocket::readyRead);
    connect(m_tcpSocket, &QTcpSocket::disconnected, this, &TcpSocket::disconnected);
    connect(m_tcpSocket, &QTcpSocket::errorOccurred, this, &TcpSocket::getErrorMessqage);
}

void TcpSocket::connected()
{
    Q_EMIT ConsoleLog("Подключились");
    Q_EMIT SetButtonsEnabled(true);
}

void TcpSocket::readyRead()
{
    gettingMessageArray.append(m_tcpSocket->readAll());
    if (message.length()<2)
    {
        message+=gettingMessage;
    }
    else
    {
        qDebug()<< "HEX lenght" << message.length();
        m_tcpSocket->flush();
        switch (m_state) {
        case WriteFirmWare:
            qDebug()<< " hexs" << message.toHex();
            if (message.length()==3 && message.at(0)==0 && message.at(2)==0)
            {
                qDebug()<< "UpdateHardwareState hex" << message.toHex();
                Q_EMIT UpdateHardwareState(message.at(1));
            }
            else
            {
                Q_EMIT FirmwareConsoleLog(message.toHex());
            }
            break;
        case SetAndReadData:
            if (message.length()<5)
            {
                Q_EMIT StateConsoleLog("Получили сообщение ("+ QString(message.toHex())+") - OK");
            }
            else
            {
                Q_EMIT StateConsoleLog("Получили сообщение c состоянием: ("+ QString(message.toHex())+") - OK");
                Q_EMIT GetMessageWithState(message);
            }
            break;
        case ReadFirmWare:
            if (message.length()<5)
            {
                Q_EMIT FirmwareConsoleLog("Данные из в буфера ("+ QString(message.toHex())+") - OK");
            }
            else
            {
                if (!IsEndOfFirmware(message))
                {
                    Q_EMIT SendFirmwareSourcePartFromDevice(message);
                    Q_EMIT GetPartOfFirmWareAgain();
                }
            }
            break;
        }
        message.clear();
    }
}

void TcpSocket::connectTo(const QString &adress,const QString &port)
{
    m_tcpSocket->connectToHost(adress, port.toInt(),  QIODevice::ReadWrite);
}

void TcpSocket::SetSocketState(quint8 state)
{
    m_state=static_cast<SocketActionState>(state);
}

void TcpSocket::disconnected()
{
    Q_EMIT SetButtonsEnabled(false);
    Q_EMIT ConsoleLog("Не подключено");
    Q_EMIT ConsoleLog("Отключено от сокета юзером");
}

void TcpSocket::disconnect()
{
    m_tcpSocket->disconnectFromHost();
}

void TcpSocket::getErrorMessqage(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::ConnectionRefusedError:
        Q_EMIT ConsoleLog(("Истекло время ожидания"));
        break;
    case QAbstractSocket::RemoteHostClosedError:
        Q_EMIT ConsoleLog(("Удаленный хост закрыл соединение"));
        break;
    case QAbstractSocket::SocketAccessError:
        Q_EMIT ConsoleLog(("Адрес хоста не найден"));
        break;
    case QAbstractSocket::SocketResourceError:
        Q_EMIT ConsoleLog(("Приложению не хватало необходимых прав"));
        break;
    case QAbstractSocket::SocketTimeoutError:
        Q_EMIT ConsoleLog(("Слишком много сокетов"));
        break;
    case QAbstractSocket::DatagramTooLargeError:
        Q_EMIT ConsoleLog(("Размер дейтаграммы превышал предел операционной системы"));
        break;
    case QAbstractSocket::NetworkError:
        Q_EMIT ConsoleLog(("Произошла ошибка сети (например, сетевой кабель был случайно отключен)"));
        break;
    case QAbstractSocket::AddressInUseError:
        Q_EMIT ConsoleLog(("Слишком много сокетов"));
        break;
    case QAbstractSocket::SocketAddressNotAvailableError:
        Q_EMIT ConsoleLog(("Адрес, уже используется в каком то соединении"));
        break;
    case QAbstractSocket::UnsupportedSocketOperationError:
        Q_EMIT ConsoleLog(("Адрес не принадлежит хосту"));
        break;
    case QAbstractSocket::UnfinishedSocketOperationError:
        Q_EMIT ConsoleLog(("Запрошенная операция сокета не поддерживается локальной операционной системой"));
        break;
    case QAbstractSocket::ProxyAuthenticationRequiredError:
        Q_EMIT ConsoleLog(("Подтверждение связи SSL / TLS не удалось, поэтому соединение было закрыто "));
        break;
    case QAbstractSocket::SslHandshakeFailedError:
        Q_EMIT ConsoleLog(("Последняя попытка операции еще не завершена"));
        break;
    case QAbstractSocket::ProxyConnectionRefusedError:
        Q_EMIT ConsoleLog(("Не удалось связаться с прокси-сервером, потому что соединение с этим сервером было отказано"));
        break;
    case QAbstractSocket::ProxyConnectionClosedError:
        Q_EMIT ConsoleLog(("Соединение с прокси-сервером было неожиданно закрыто"));
        break;
    case QAbstractSocket::ProxyConnectionTimeoutError:
        Q_EMIT ConsoleLog(("Время ожидания подключения к прокси-серверу истекло или прокси-сервер перестал отвечать на этапе проверки подлинности."));
        break;
    case QAbstractSocket::ProxyNotFoundError:
        Q_EMIT ConsoleLog(("Адрес прокси, заданный с помощью setProxy () (или прокси приложения), не найден."));
        break;
    case QAbstractSocket::ProxyProtocolError:
        Q_EMIT ConsoleLog(("Согласование соединения с прокси-сервером не удалось, потому что ответ прокси-сервера не был понят."));
        break;
    case QAbstractSocket::OperationError:
        Q_EMIT ConsoleLog(("Была предпринята попытка выполнения операции, когда сокет находился в недопустимом состоянии."));
        break;
    case QAbstractSocket::SslInternalError:
        Q_EMIT ConsoleLog(("Используемая библиотека SSL сообщила о внутренней ошибке."));
        break;
    case QAbstractSocket::SslInvalidUserDataError:
        Q_EMIT ConsoleLog(("Были предоставлены неверные данные (сертификат, ключ, шифр и т. Д.), И их использование привело к ошибке в библиотеке SSL."));
        break;
    case QAbstractSocket::TemporaryError:
        Q_EMIT ConsoleLog(("Произошла временная ошибка (например, операция будет заблокирована, а сокет не блокируется)."));
        break;
    case QAbstractSocket::UnknownSocketError:
        Q_EMIT ConsoleLog(("Произошла неопознанная ошибка."));
        break;
    case QAbstractSocket::HostNotFoundError:
        Q_EMIT ConsoleLog(("Хост не найден"));
        break;

    }
}

void TcpSocket::send(QByteArray array)
{
    //    switch (m_state) {
    //    case SetAndReadData:
    //        Q_EMIT StateConsoleLog("Пытаемся отправить: "+ array.toHex(' '));
    //        break;
    //    case ReadFirmWare:
    //    case WriteFirmWare:
    Q_EMIT FirmwareConsoleLog("Пытаемся отправить: "+ array.toHex(' '));
    //        break;
    //    }
    //    if (m_tcpSocket->state() == QAbstractSocket::ConnectedState)
    //    {
    //    QDataStream out(&array, QIODevice::WriteOnly);
    //    m_tcpSocket->de->sendBytes(b);
    m_tcpSocket->write(array, array.length());
    m_tcpSocket->flush();
    //        switch (m_state) {
    //        case SetAndReadData:
    //            Q_EMIT StateConsoleLog("Отправили");
    //            break;
    //        case ReadFirmWare:
    //        case WriteFirmWare:
    Q_EMIT FirmwareConsoleLog("Отправили");
    //            break;
    //        }
    //    }
    //    else
    //    {
    //        Q_EMIT ConsoleLog("Сначала подключись к мохе");
    //    }
    //     m_tcpSocket->flush();
}



QString TcpSocket::getStringSocketState(QAbstractSocket::SocketState state)
{
    switch (state) {
    case QAbstractSocket::UnconnectedState:
        return "Моха не подключена.";
    case QAbstractSocket::HostLookupState:
        return "Сокет выполняет поиск хоста.";
    case QAbstractSocket::ConnectingState:
        return "Сокет до сих пор устанавливае соединение.";
    case QAbstractSocket::ConnectedState:
        return "Установлено соединение";
    case QAbstractSocket::BoundState:
        return "Сокет уже привязан к адресу и порту.";
    case QAbstractSocket::ListeningState:
        return "Сокет вот-вот закроется";
    case QAbstractSocket::ClosingState:
        return "Только для внутреннего использования.";

    }
}

bool TcpSocket::IsEndOfFirmware(QByteArray &firmwarePart)
{
    QByteArray arr=firmwarePart.toHex();
    if (m_stopByteArray==arr)
    {
        ++m_stopLines;
        if (m_stopLines>m_maxNumOfLinesToStop)
        {
            return true;
        }
    }
    else
    {
        m_stopLines=0;
    }
    return false;

}
