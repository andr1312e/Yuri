#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QStatusBar>
#include <QSerialPortInfo>

#include "connections/tcphandler.h"
#include "connections/serialhandler.h"

#include "ui/connectionwidget.h"
#include "ui/statewidget.h"
#include "ui/infowidget.h"
#include "ui/firmwarewidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent);
    ~MainWindow();
private:
    void CreateObjects();
    void CreateUI();
    void InsertWidgetsIntoMainWindow();
    void FillUI();
    void ConnectObjects();
private Q_SLOTS:
    void WhenConnectToInternetMoxa(const QString &adress, const QString &port);
    void WhenConnectToUsbMoxa(const QString &comPortName);
    void WhenDisconnectFromInternetMoxa();

private:
    QStatusBar *m_statusBar;
    ConnectionWidget *m_connectionWidget;
    QTabWidget *m_tabWidget;
    InfoWidget *m_infoWidget;
    StateWidget *m_stateWidget;
    FirmWareWidget *m_firmwareWidget;

    DataHandler *m_currentConnectionInterface;
    TcpHandler *m_tcpHandler;
    SerialHandler *m_serialHandler;

private:
    void RegisterHadnler(DataHandler *dataHandler);
    void DisconnectOldHander();
    void ConnectHander(DataHandler *dataHandler);
};
#endif // MAINWINDOW_H