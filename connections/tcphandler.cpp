#include "tcphandler.h"

TcpHandler::TcpHandler(QObject *parent)
    : DataHandler(parent)
{
    CreateHander();
    ConnectObjects();
}

TcpHandler::~TcpHandler()
{
    FromHostDisconnect();
    delete m_connectionPort;
}

void TcpHandler::CreateHander()
{
    m_connectionPort=new QTcpSocket(this);
}

void TcpHandler::ConnectObjects()
{
    connect(m_connectionPort, &QTcpSocket::connected, [&](){DataHandler::ToHostConnected();});
    connect(m_connectionPort, &QTcpSocket::readyRead, this, &TcpHandler::OnReadyRead);
    connect(m_connectionPort, &QTcpSocket::disconnected, [&](){DataHandler::FromHostDisconnected();});
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    connect(m_connectionPort, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &TcpHandler::OnErrorOccurred);
#else
    connect(m_connectionPort, &QTcpSocket::errorOccurred, this, &TcpHandler::OnErrorOccurred);
#endif
}

void TcpHandler::OnReadyRead()
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

void TcpHandler::OnErrorOccurred(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::ConnectionRefusedError:
        Q_EMIT ToConsoleLog(QStringLiteral("Истекло время ожидания"));
        break;
    case QAbstractSocket::RemoteHostClosedError:
        Q_EMIT ToConsoleLog(QStringLiteral("Удаленный хост закрыл соединение"));
        break;
    case QAbstractSocket::SocketAccessError:
        Q_EMIT ToConsoleLog(QStringLiteral("Адрес хоста не найден"));
        break;
    case QAbstractSocket::SocketResourceError:
        Q_EMIT ToConsoleLog(QStringLiteral("Приложению не хватало необходимых прав"));
        break;
    case QAbstractSocket::SocketTimeoutError:
        Q_EMIT ToConsoleLog(QStringLiteral("Слишком много сокетов"));
        break;
    case QAbstractSocket::DatagramTooLargeError:
        Q_EMIT ToConsoleLog(QStringLiteral("Размер дейтаграммы превышал предел операционной системы"));
        break;
    case QAbstractSocket::NetworkError:
        Q_EMIT ToConsoleLog(QStringLiteral("Произошла ошибка сети (например, сетевой кабель был случайно отключен)"));
        break;
    case QAbstractSocket::AddressInUseError:
        Q_EMIT ToConsoleLog(QStringLiteral("Слишком много сокетов"));
        break;
    case QAbstractSocket::SocketAddressNotAvailableError:
        Q_EMIT ToConsoleLog(QStringLiteral("Адрес, уже используется в каком то соединении"));
        break;
    case QAbstractSocket::UnsupportedSocketOperationError:
        Q_EMIT ToConsoleLog(QStringLiteral("Адрес не принадлежит хосту"));
        break;
    case QAbstractSocket::UnfinishedSocketOperationError:
        Q_EMIT ToConsoleLog(QStringLiteral("Запрошенная операция сокета не поддерживается локальной операционной системой"));
        break;
    case QAbstractSocket::ProxyAuthenticationRequiredError:
        Q_EMIT ToConsoleLog(QStringLiteral("Подтверждение связи SSL / TLS не удалось, поэтому соединение было закрыто "));
        break;
    case QAbstractSocket::SslHandshakeFailedError:
        Q_EMIT ToConsoleLog(QStringLiteral("Последняя попытка операции еще не завершена"));
        break;
    case QAbstractSocket::ProxyConnectionRefusedError:
        Q_EMIT ToConsoleLog(QStringLiteral("Не удалось связаться с прокси-сервером, потому что соединение с этим сервером было отказано"));
        break;
    case QAbstractSocket::ProxyConnectionClosedError:
        Q_EMIT ToConsoleLog(QStringLiteral("Соединение с прокси-сервером было неожиданно закрыто"));
        break;
    case QAbstractSocket::ProxyConnectionTimeoutError:
        Q_EMIT ToConsoleLog(QStringLiteral("Время ожидания подключения к прокси-серверу истекло или прокси-сервер перестал отвечать на этапе проверки подлинности."));
        break;
    case QAbstractSocket::ProxyNotFoundError:
        Q_EMIT ToConsoleLog(QStringLiteral("Адрес прокси, заданный с помощью setProxy () (или прокси приложения), не найден."));
        break;
    case QAbstractSocket::ProxyProtocolError:
        Q_EMIT ToConsoleLog(QStringLiteral("Согласование соединения с прокси-сервером не удалось, потому что ответ прокси-сервера не был понят."));
        break;
    case QAbstractSocket::OperationError:
        Q_EMIT ToConsoleLog(QStringLiteral("Была предпринята попытка выполнения операции, когда сокет находился в недопустимом состоянии."));
        break;
    case QAbstractSocket::SslInternalError:
        Q_EMIT ToConsoleLog(QStringLiteral("Используемая библиотека SSL сообщила о внутренней ошибке."));
        break;
    case QAbstractSocket::SslInvalidUserDataError:
        Q_EMIT ToConsoleLog(QStringLiteral("Были предоставлены неверные данные (сертификат, ключ, шифр и т. Д.), И их использование привело к ошибке в библиотеке SSL."));
        break;
    case QAbstractSocket::TemporaryError:
        Q_EMIT ToConsoleLog(QStringLiteral("Произошла временная ошибка (например, операция будет заблокирована, а сокет не блокируется)."));
        break;
    case QAbstractSocket::UnknownSocketError:
        Q_EMIT ToConsoleLog(QStringLiteral("Произошла неопознанная ошибка."));
        break;
    case QAbstractSocket::HostNotFoundError:
        Q_EMIT ToConsoleLog(QStringLiteral("Хост не найден"));
        break;
    }
}

void TcpHandler::TryToConnectToHost(const QString &adress,const QString &port)
{
    m_connectionPort->connectToHost(adress, port.toInt(),  QIODevice::ReadWrite);
}

void TcpHandler::WriteMessageToBuffer(const QByteArray &part)
{
    m_connectionPort->write(part);
}

void TcpHandler::FlushBuffer()
{
    m_connectionPort->flush();
}

void TcpHandler::FromHostDisconnect()
{
    m_connectionPort->disconnectFromHost();
}


