#include "tcphandler.h"
#include <QNetworkProxy>

TcpHandler::TcpHandler(QObject *parent)
    : DataHandler(parent)


{
    CreateHander();
    ConnectObjects();
}

TcpHandler::~TcpHandler()
{
    delete m_connectionPort;
}

void TcpHandler::CreateHander()
{
    m_connectionPort=new QTcpSocket(this);
    m_connectionPort->setProxy(QNetworkProxy::NoProxy);
}

void TcpHandler::ConnectObjects()
{
    connect(m_connectionPort, &QTcpSocket::connected, this, &TcpHandler::WhenSocketConnected);
    connect(m_connectionPort, &QTcpSocket::readyRead, this, &TcpHandler::WhenReadyRead, Qt::DirectConnection);
    connect(m_connectionPort, &QTcpSocket::disconnected, this, &TcpHandler::WhenDisconnectedFromHost);
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    connect(m_connectionPort, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &TcpHandler::WhenErrorOccurred);
#else
    connect(m_connectionPort, &QTcpSocket::errorOccurred, this, &TcpHandler::WhenErrorOccurred);
#endif
}



void TcpHandler::WhenReadyRead()
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

void TcpHandler::WhenErrorOccurred(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::ConnectionRefusedError:
        Q_EMIT ToConsoleLog(("Истекло время ожидания"));
        break;
    case QAbstractSocket::RemoteHostClosedError:
        Q_EMIT ToConsoleLog(("Удаленный хост закрыл соединение"));
        break;
    case QAbstractSocket::SocketAccessError:
        Q_EMIT ToConsoleLog(("Адрес хоста не найден"));
        break;
    case QAbstractSocket::SocketResourceError:
        Q_EMIT ToConsoleLog(("Приложению не хватало необходимых прав"));
        break;
    case QAbstractSocket::SocketTimeoutError:
        Q_EMIT ToConsoleLog(("Слишком много сокетов"));
        break;
    case QAbstractSocket::DatagramTooLargeError:
        Q_EMIT ToConsoleLog(("Размер дейтаграммы превышал предел операционной системы"));
        break;
    case QAbstractSocket::NetworkError:
        Q_EMIT ToConsoleLog(("Произошла ошибка сети (например, сетевой кабель был случайно отключен)"));
        break;
    case QAbstractSocket::AddressInUseError:
        Q_EMIT ToConsoleLog(("Слишком много сокетов"));
        break;
    case QAbstractSocket::SocketAddressNotAvailableError:
        Q_EMIT ToConsoleLog(("Адрес, уже используется в каком то соединении"));
        break;
    case QAbstractSocket::UnsupportedSocketOperationError:
        Q_EMIT ToConsoleLog(("Адрес не принадлежит хосту"));
        break;
    case QAbstractSocket::UnfinishedSocketOperationError:
        Q_EMIT ToConsoleLog(("Запрошенная операция сокета не поддерживается локальной операционной системой"));
        break;
    case QAbstractSocket::ProxyAuthenticationRequiredError:
        Q_EMIT ToConsoleLog(("Подтверждение связи SSL / TLS не удалось, поэтому соединение было закрыто "));
        break;
    case QAbstractSocket::SslHandshakeFailedError:
        Q_EMIT ToConsoleLog(("Последняя попытка операции еще не завершена"));
        break;
    case QAbstractSocket::ProxyConnectionRefusedError:
        Q_EMIT ToConsoleLog(("Не удалось связаться с прокси-сервером, потому что соединение с этим сервером было отказано"));
        break;
    case QAbstractSocket::ProxyConnectionClosedError:
        Q_EMIT ToConsoleLog(("Соединение с прокси-сервером было неожиданно закрыто"));
        break;
    case QAbstractSocket::ProxyConnectionTimeoutError:
        Q_EMIT ToConsoleLog(("Время ожидания подключения к прокси-серверу истекло или прокси-сервер перестал отвечать на этапе проверки подлинности."));
        break;
    case QAbstractSocket::ProxyNotFoundError:
        Q_EMIT ToConsoleLog(("Адрес прокси, заданный с помощью setProxy () (или прокси приложения), не найден."));
        break;
    case QAbstractSocket::ProxyProtocolError:
        Q_EMIT ToConsoleLog(("Согласование соединения с прокси-сервером не удалось, потому что ответ прокси-сервера не был понят."));
        break;
    case QAbstractSocket::OperationError:
        Q_EMIT ToConsoleLog(("Была предпринята попытка выполнения операции, когда сокет находился в недопустимом состоянии."));
        break;
    case QAbstractSocket::SslInternalError:
        Q_EMIT ToConsoleLog(("Используемая библиотека SSL сообщила о внутренней ошибке."));
        break;
    case QAbstractSocket::SslInvalidUserDataError:
        Q_EMIT ToConsoleLog(("Были предоставлены неверные данные (сертификат, ключ, шифр и т. Д.), И их использование привело к ошибке в библиотеке SSL."));
        break;
    case QAbstractSocket::TemporaryError:
        Q_EMIT ToConsoleLog(("Произошла временная ошибка (например, операция будет заблокирована, а сокет не блокируется)."));
        break;
    case QAbstractSocket::UnknownSocketError:
        Q_EMIT ToConsoleLog(("Произошла неопознанная ошибка."));
        break;
    case QAbstractSocket::HostNotFoundError:
        Q_EMIT ToConsoleLog(("Хост не найден"));
        break;
    }
}

void TcpHandler::WhenSocketConnected()
{
    Q_EMIT ToConsoleLog("Подключились");
    Q_EMIT ToButtonsEnabledChanging(true);
}

void TcpHandler::ConnectToHost(const QString &adress,const QString &port)
{
    m_connectionPort->connectToHost(adress, port.toInt(),  QIODevice::ReadWrite);
}

void TcpHandler::SetConnectionState(quint8 state)
{
    m_gettingMessageType=static_cast<HandlerState>(state);
}

void TcpHandler::WhenDisconnectedFromHost()
{
    Q_EMIT ToButtonsEnabledChanging(false);
    Q_EMIT ToConsoleLog("Не подключено");
    Q_EMIT ToConsoleLog("Отключено от сокета юзером");
}

void TcpHandler::WhenDisconnectByUserFromHost()
{
    m_connectionPort->disconnectFromHost();
}

void TcpHandler::WriteMessageToBuffer(const QByteArray &part)
{
    m_connectionPort->write(part);
}

void TcpHandler::FlushBuffer()
{
    m_connectionPort->flush();
}

QString TcpHandler::getStringSocketState(QAbstractSocket::SocketState state)
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
