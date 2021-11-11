#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    CreateObjects();
    CreateUI();
    InsertWidgetsIntoMainWindow();
    FillUI();
    ConnectObjects();
}

MainWindow::~MainWindow()
{
    delete m_connectionWidget;
    delete m_stateWidget;
}

void MainWindow::CreateObjects()
{
    m_tcpHandler=new TcpHandler(this);
    m_tcpHandler->setObjectName(QStringLiteral("TcpHandler"));
    m_serialHandler=new SerialHandler(this);
    m_serialHandler->setObjectName("SerialHandler");
    qDebug()<< "FIRST m_currentConnectionInterface" << &m_currentConnectionInterface;
    m_currentConnectionInterface=Q_NULLPTR;
}

void MainWindow::CreateUI()
{
    m_statusBar=new QStatusBar();
    m_connectionWidget =new ConnectionWidget(this);
    qDebug()<<"CreateUI_FIRST m_currentConnectionInterface" << &m_currentConnectionInterface;
    m_stateWidget=new StateWidget(this);
    qDebug()<<"CreateUI_SECOND m_currentConnectionInterface" << &m_currentConnectionInterface;
    m_firmwareWidget=new FirmWareWidget(this);
    qDebug()<<"CreateUI_THIRD m_currentConnectionInterface" << &m_currentConnectionInterface;
    m_tabWidget =new QTabWidget();
    m_tabWidget->addTab(m_stateWidget, QStringLiteral("Окно команд"));
    m_tabWidget->addTab(m_firmwareWidget, QStringLiteral("Окно прошивки микросхемы памяти MX25L12835F"));
    m_tabWidget->addTab(new QWidget(this), QStringLiteral("Окно прошивки"));
}

void MainWindow::InsertWidgetsIntoMainWindow()
{
    setStatusBar(m_statusBar);
    setMenuWidget(m_connectionWidget);
    setCentralWidget(m_tabWidget);
}

void MainWindow::FillUI()
{
    setWindowTitle(QStringLiteral("Настройка Юстировочного оборудования блок М14ХЛ2 Плата СЮИТ.687263.035"));
    m_statusBar->showMessage(QStringLiteral("Не подключено"));
}

void MainWindow::ConnectObjects()
{
    connect(m_connectionWidget, &ConnectionWidget::ToConsoleLog, m_stateWidget, &StateWidget::WhenConsoleLog);
    connect(m_connectionWidget, &ConnectionWidget::ToConsoleLog, m_firmwareWidget, &FirmWareWidget::WhenConsoleLog);
    connect(m_connectionWidget, &ConnectionWidget::ToConnectEthernetMoxa, this, &MainWindow::WhenConnectToInternetMoxa );
    connect(m_connectionWidget, &ConnectionWidget::ToConnectUsbMoxa, this, &MainWindow::WhenConnectToUsbMoxa);
    connect(m_connectionWidget, &ConnectionWidget::ToDisconnectFromMoxa,this, &MainWindow::WhenDisconnectFromInternetMoxa);
}

void MainWindow::WhenConnectToInternetMoxa(const QString &adress, const QString &port)
{
    m_stateWidget->WhenConsoleLog("Попытка подключится... Адрес: " + adress  + " Порт: " + port);
    RegisterHadnler(m_tcpHandler);
    m_tcpHandler->ConnectToHost(adress, port);

}

void MainWindow::WhenConnectToUsbMoxa(const QString &comPortName)
{
    m_stateWidget->WhenConsoleLog("Попытка подключится... Имя порта: " + comPortName);
    RegisterHadnler(m_serialHandler);
    m_serialHandler->ConnectToHost(comPortName);
}

void MainWindow::WhenDisconnectFromInternetMoxa()
{
    m_tcpHandler->WhenDisconnectByUserFromHost();
    m_stateWidget->WhenConsoleLog(QStringLiteral("Отключились от интернетной мохи..."));
    m_connectionWidget->SetButtonsEnabled(false);
    m_statusBar->showMessage(QStringLiteral("Не подключено"));
}

void MainWindow::RegisterHadnler(DataHandler *dataHandler)
{
    DisconnectOldHander();
    ConnectHander(dataHandler);
    m_currentConnectionInterface=dataHandler;
}

void MainWindow::DisconnectOldHander()
{
    if (m_currentConnectionInterface!=Q_NULLPTR)
    {
        disconnect(m_currentConnectionInterface, &DataHandler::ToButtonsEnabledChanging, m_connectionWidget, &ConnectionWidget::SetButtonsEnabled);
        m_stateWidget->DisconnectOldHander();
        m_firmwareWidget->DisconnectOldHander();
    }
}

void MainWindow::ConnectHander(DataHandler *dataHandler)
{
    connect(m_currentConnectionInterface, &DataHandler::ToButtonsEnabledChanging, m_connectionWidget, &ConnectionWidget::SetButtonsEnabled);
    m_stateWidget->ConnectHander(dataHandler);
    m_firmwareWidget->ConnectHander(dataHandler);
}


