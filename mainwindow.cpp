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
    delete m_settingFileService;
    delete m_tcpHandler;
    delete m_serialHandler;
    delete m_statusBar;

    delete m_docWidget;
    delete m_connectionWidget;
    delete m_stateWidget;
    delete m_firmwareWidget;
    delete m_tabWidget;
}

void MainWindow::CreateObjects()
{
    m_tcpHandler = new TcpHandler(this);
    m_tcpHandler->setObjectName(QLatin1Literal("TcpHandler"));
    m_serialHandler = new SerialHandler(this);
    m_serialHandler->setObjectName(QLatin1Literal("SerialHandler"));
    qDebug() << "FIRST m_currentConnectionInterface" << &m_currentConnectionInterface;
    m_currentConnectionInterface = Q_NULLPTR;
    m_settingFileService = new SettingFileService(QLatin1Literal("setting.xml"));
}

void MainWindow::CreateUI()
{
    m_statusBar = new QStatusBar();
    m_connectionWidget = new ConnectionWidget(m_settingFileService, this);
    m_stateWidget = new StateWidget(m_settingFileService, this);
    m_firmwareWidget = new FirmWareWidget(m_settingFileService, this);
    m_docWidget = new QListWidget(this);
    m_tabWidget = new QTabWidget();
    m_tabWidget->addTab(m_stateWidget, QStringLiteral("Окно команд"));
    m_tabWidget->addTab(m_firmwareWidget, QStringLiteral("Окно прошивки"));
    m_tabWidget->addTab(m_docWidget, QStringLiteral("Окно протокола"));
}

void MainWindow::InsertWidgetsIntoMainWindow()
{
//    setStatusBar(m_statusBar);
    setMenuWidget(m_connectionWidget);
    setCentralWidget(m_tabWidget);
}

void MainWindow::FillUI()
{
    const QString qtVersion = qVersion();
    setWindowTitle("Настройка Юстировочного оборудования блок М14ХЛ2 Плата СЮИТ.687263.035 Qt " + qtVersion + " Версия " + APP_VERSION + " Cборка: " + BUILD_DATE);
//    m_statusBar->showMessage(QStringLiteral("Не подключено"));
    for (int i = 0; i < 5; ++i)
    {
        QListWidgetItem *item = new QListWidgetItem(m_docWidget);
        const QString path = ":/doc/" + QString::number(i) + ".jpg";
        const QIcon icon(path);
        item->setIcon(icon);
        m_docWidget->addItem(item);
    }
    m_docWidget->setViewMode(QListView::IconMode);
    m_tabWidget->tabBar()->setDocumentMode(true);
    m_tabWidget->tabBar()->setExpanding(true);
    m_tabWidget->setElideMode(Qt::ElideRight);
    m_tabWidget->setStyleSheet(QLatin1Literal("QTabBar::tab:selected { font: bold 14px; color: black;}"));
}

void MainWindow::ConnectObjects()
{
    connect(m_connectionWidget, &ConnectionWidget::ToConsoleLog, m_stateWidget, &StateWidget::OnConsoleLog);
    connect(m_connectionWidget, &ConnectionWidget::ToConsoleLog, m_firmwareWidget, &FirmWareWidget::ToConsoleLog);
    connect(m_connectionWidget, &ConnectionWidget::ToConnectEthernetMoxa, this, &MainWindow::OnConnectToInternetMoxa );
    connect(m_connectionWidget, &ConnectionWidget::ToConnectUsbMoxa, this, &MainWindow::OnConnectToUsbMoxa);
    connect(m_connectionWidget, &ConnectionWidget::ToDisconnectFromMoxa, this, &MainWindow::OnDisconnectFromMoxa);
    connect(m_firmwareWidget, &FirmWareWidget::ToSetButtonsEnabled, m_stateWidget, &StateWidget::OnSetButtonEnabled);
}

void MainWindow::OnConnectToInternetMoxa(const QString &adress, const QString &port)
{
    m_stateWidget->OnConsoleLog("Попытка подключится по интернет кабелю... Адрес: " + adress  + " Порт: " + port);
    RegisterHadnler(m_tcpHandler);
    m_tcpHandler->TryToConnectToHost(adress, port);

}

void MainWindow::OnConnectToUsbMoxa(const QString &comPortName)
{
    m_stateWidget->OnConsoleLog("Попытка подключится по сериал порт кабелю... Имя порта: " + comPortName);
    RegisterHadnler(m_serialHandler);
    m_serialHandler->TryToConnectToHost(comPortName);
}

void MainWindow::OnDisconnectFromMoxa()
{
    m_currentConnectionInterface->FromHostDisconnect();
    m_connectionWidget->SetButtonsEnabled(true);
//    m_statusBar->showMessage(QStringLiteral("Не подключено"));
    DisconnectOldHander();
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    m_docWidget->setIconSize(event->size() * 2);
}

void MainWindow::RegisterHadnler(DataHandler *dataHandler)
{
    DisconnectOldHander();
    ConnectHander(dataHandler);
}

void MainWindow::DisconnectOldHander()
{
    if (Q_NULLPTR != m_currentConnectionInterface)
    {
        m_currentConnectionInterface->FromHostDisconnected();
        disconnect(m_currentConnectionInterface, &DataHandler::ToButtonsEnabledChanging, m_connectionWidget, &ConnectionWidget::SetButtonsEnabled);
        m_stateWidget->DisconnectOldHander();
        m_firmwareWidget->DisconnectOldHander();
        m_currentConnectionInterface = Q_NULLPTR;
    }
}

void MainWindow::ConnectHander(DataHandler *dataHandler)
{
//    m_statusBar->showMessage(QStringLiteral("Подключено"));
    dataHandler->ClearBuffer();
    connect(dataHandler, &DataHandler::ToButtonsEnabledChanging, m_connectionWidget, &ConnectionWidget::SetButtonsEnabled);
    m_stateWidget->ConnectHander(dataHandler);
    m_firmwareWidget->ConnectHander(dataHandler);
    m_currentConnectionInterface = dataHandler;
}


