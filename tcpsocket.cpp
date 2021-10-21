#include "tcpsocket.h"

TcpSocket::TcpSocket(QObject *parent)
    : QObject(parent)
    , m_tcpSocket(new QTcpSocket(this))
    , m_timer(new QTimer())
    , m_messageSetter(new MessagesSetter())
{
    initCheckingConnectionTimer();
    createConnections();
}

TcpSocket::~TcpSocket()
{
    delete m_timer;
    delete m_tcpSocket;
    delete m_messageSetter;
}

void TcpSocket::initCheckingConnectionTimer()
{
    m_timer->setSingleShot(true);
    m_timer->setInterval(10000);
}

void TcpSocket::createConnections()
{
    connect(m_timer, &QTimer::timeout, this, &TcpSocket::checkConnection);
    connect(m_tcpSocket, &QTcpSocket::connected, this, &TcpSocket::connected);
    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &TcpSocket::readyRead);
    connect(m_tcpSocket, &QTcpSocket::disconnected, this, &TcpSocket::disconnected);
    connect(m_tcpSocket, &QTcpSocket::errorOccurred, this, &TcpSocket::getErrorMessqage);
}

void TcpSocket::connected()
{
    Q_EMIT setState(QStringLiteral("Подключено"));
    Q_EMIT sendMessageToLog("Подключились");
    Q_EMIT setButtonsEnabled(true);
}

void TcpSocket::readyRead()
{
    QByteArray gettingMessage=m_tcpSocket->readAll();
//    gettingMessage.remove(0, 1);
//    gettingMessage.remove(gettingMessage.length()-2, 2);
    if (gettingMessage.at(1)==7)
    {
        sendMessageToLog("Получили данные "+ m_messageGetter->getDataFromMessage(gettingMessage));
    }
    else
    {
      sendMessageToLog("Получили сообщение ("+ QString(gettingMessage)+") - OK");
    }

}

void TcpSocket::connectTo(QString &ip, QString &port)
{
    m_tcpSocket->connectToHost(ip, port.toInt(),  QIODevice::ReadWrite);
    m_timer->start();
}

void TcpSocket::createMessages(quint8 messageId, double firstParam, double SecondParam)
{
    QByteArray message;
    switch (messageId) {
    case 1:
    {
        message=m_messageSetter->createFirstCommand(firstParam);
        break;
    }
    case 2:
    {
        message=m_messageSetter->createSecondCommand(firstParam, SecondParam);
        break;
    }
    case 3:
    {
        message=m_messageSetter->createThirdCommand(firstParam);
        break;
    }
    case 4:
    {
        message=m_messageSetter->createFourthCommand(firstParam, SecondParam);
        break;
    }
    case 5:
    {
        message=m_messageSetter->createFiveCommand(firstParam);
        break;
    }
    case 6:
    {
        message=m_messageSetter->createSixCommand(firstParam);
        break;
    }
    case 7:
    {
        message=m_messageSetter->createSevenCommand(firstParam);
        break;
    }
    default:
    {
        message=m_messageSetter->createZeroCommand();
        break;
    }

    }
    Q_EMIT sendMessageToLog("Отправляемое сообщeние " +message.toHex() + " его размер: " + QString::number(message.size()) + " байт сформировано");
    send(message);
}

void TcpSocket::disconnected()
{
    Q_EMIT setButtonsEnabled(false);
    Q_EMIT setState("Не подключено");
    Q_EMIT sendMessageToLog("отключено от сокета юзером");
}

void TcpSocket::disconnect()
{
    m_tcpSocket->disconnectFromHost();
}

void TcpSocket::getErrorMessqage(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::ConnectionRefusedError:
        Q_EMIT sendMessageToLog(("Истекло время ожидания"));
        break;
    case QAbstractSocket::RemoteHostClosedError:
        Q_EMIT sendMessageToLog(("Удаленный хост закрыл соединение"));
        break;
    case QAbstractSocket::SocketAccessError:
        Q_EMIT sendMessageToLog(("Адрес хоста не найден"));
        break;
    case QAbstractSocket::SocketResourceError:
        Q_EMIT sendMessageToLog(("Приложению не хватало необходимых прав"));
        break;
    case QAbstractSocket::SocketTimeoutError:
        Q_EMIT sendMessageToLog(("Слишком много сокетов"));
        break;
    case QAbstractSocket::DatagramTooLargeError:
        Q_EMIT sendMessageToLog(("Размер дейтаграммы превышал предел операционной системы"));
        break;
    case QAbstractSocket::NetworkError:
        Q_EMIT sendMessageToLog(("Произошла ошибка сети (например, сетевой кабель был случайно отключен)"));
        break;
    case QAbstractSocket::AddressInUseError:
        Q_EMIT sendMessageToLog(("Слишком много сокетов"));
        break;
    case QAbstractSocket::SocketAddressNotAvailableError:
        Q_EMIT sendMessageToLog(("Адрес, уже используется в каком то соединении"));
        break;
    case QAbstractSocket::UnsupportedSocketOperationError:
        Q_EMIT sendMessageToLog(("Адрес не принадлежит хосту"));
        break;
    case QAbstractSocket::UnfinishedSocketOperationError:
        Q_EMIT sendMessageToLog(("Запрошенная операция сокета не поддерживается локальной операционной системой"));
        break;
    case QAbstractSocket::ProxyAuthenticationRequiredError:
        Q_EMIT sendMessageToLog(("Подтверждение связи SSL / TLS не удалось, поэтому соединение было закрыто "));
        break;
    case QAbstractSocket::SslHandshakeFailedError:
        Q_EMIT sendMessageToLog(("Последняя попытка операции еще не завершена"));
        break;
    case QAbstractSocket::ProxyConnectionRefusedError:
        Q_EMIT sendMessageToLog(("Не удалось связаться с прокси-сервером, потому что соединение с этим сервером было отказано"));
        break;
    case QAbstractSocket::ProxyConnectionClosedError:
        Q_EMIT sendMessageToLog(("Соединение с прокси-сервером было неожиданно закрыто"));
        break;
    case QAbstractSocket::ProxyConnectionTimeoutError:
        Q_EMIT sendMessageToLog(("Время ожидания подключения к прокси-серверу истекло или прокси-сервер перестал отвечать на этапе проверки подлинности."));
        break;
    case QAbstractSocket::ProxyNotFoundError:
        Q_EMIT sendMessageToLog(("Адрес прокси, заданный с помощью setProxy () (или прокси приложения), не найден."));
        break;
    case QAbstractSocket::ProxyProtocolError:
        Q_EMIT sendMessageToLog(("Согласование соединения с прокси-сервером не удалось, потому что ответ прокси-сервера не был понят."));
        break;
    case QAbstractSocket::OperationError:
        Q_EMIT sendMessageToLog(("Была предпринята попытка выполнения операции, когда сокет находился в недопустимом состоянии."));
        break;
    case QAbstractSocket::SslInternalError:
        Q_EMIT sendMessageToLog(("Используемая библиотека SSL сообщила о внутренней ошибке."));
        break;
    case QAbstractSocket::SslInvalidUserDataError:
        Q_EMIT sendMessageToLog(("Были предоставлены неверные данные (сертификат, ключ, шифр и т. Д.), И их использование привело к ошибке в библиотеке SSL."));
        break;
    case QAbstractSocket::TemporaryError:
        Q_EMIT sendMessageToLog(("Произошла временная ошибка (например, операция будет заблокирована, а сокет не блокируется)."));
        break;
    case QAbstractSocket::UnknownSocketError:
        Q_EMIT sendMessageToLog(("Произошла неопознанная ошибка."));
        break;
    case QAbstractSocket::HostNotFoundError:
        Q_EMIT sendMessageToLog(("Хост не найден"));
        break;

    }
}

void TcpSocket::send(QByteArray &array)
{
    Q_EMIT sendMessageToLog("Пытаемся отправить");
    if (m_tcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        m_tcpSocket->write(array);
        m_tcpSocket->flush();
        Q_EMIT sendMessageToLog("Отправили");
    }
    else
    {
        Q_EMIT sendMessageToLog("Сначала подключись к мохе");
    }

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

void TcpSocket::checkConnection()
{
    if (m_tcpSocket->state()==QAbstractSocket::ConnectedState)
    {
        Q_EMIT setButtonsEnabled(true);
        Q_EMIT sendMessageToLog(QString("Проверяем соединение: Подключено к мохе успешно. Ура"));
    }
    else
    {
        QString socketState=getStringSocketState(m_tcpSocket->state());
        Q_EMIT sendMessageToLog("Проверяем соединение:  Не смогли подключится к мохе: " + socketState);
    }
}
