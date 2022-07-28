#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QMainWindow>
#include <QStatusBar>
#include <QSerialPortInfo>
#include <QListWidget>
#include <QApplication>

#include "services/settingfileservice.h"

#include "connections/tcphandler.h"
#include "connections/serialhandler.h"

#include "ui/connectionwidget.h"
#include "ui/statewidget.h"
#include "ui/firmwarewidget.h"
#include "ui/bparwidget.h"

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
    void OnTryConnectToInternetMoxa(const QString &adress, const QString &port);
    void OnTryConnectToUsbMoxa(const QString &comPortName);
    void OnDisconnectFromMoxa();
    void OnGenerateIcon(bool isConnected);
protected:
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
private:
    ConnectionWidget *m_connectionWidget;
    QTabWidget *m_tabWidget;
    StateWidget *m_stateWidget;
    FirmWareWidget *m_firmwareWidget;
    QListWidget *m_docWidget;

    DataHandler *m_currentConnectionInterface;
    TcpHandler *m_tcpHandler;
    SerialHandler *m_serialHandler;
    SettingFileService *m_settingFileService;

private:
    void RegisterHadnler(DataHandler *dataHandler);
    void DisconnectOldHander();
    void ConnectHander(DataHandler *dataHandler);

};
#endif // UI_MAINWINDOW_H
