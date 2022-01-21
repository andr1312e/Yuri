#ifndef UI_CONNECTIONWIDGET_H
#define UI_CONNECTIONWIDGET_H

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>
#include <QWidget>
#include <QTimer>
#include <QDebug>
#include <QSettings>
#include <QFile>
#include <QSerialPortInfo>

#include "services/settingfileservice.h"

class ConnectionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectionWidget(QSharedPointer<SettingFileService> &settingFileService, QWidget *parent);
    ~ConnectionWidget();
private:
    void CreateObjects();
    void InitObjects();
    void CreateUI();
    void InsertWidgetsIntoLayout();
    void ConnectObjects();
    void FillUI();
Q_SIGNALS:
    void ToConsoleLog(const QString &message);
    void ToConnectEthernetMoxa(const QString &adress, const QString &port);
    void ToConnectUsbMoxa(const QString &comPortName);
    void ToDisconnectFromMoxa();

    void CheckSerialPortsTimerChanged();

private Q_SLOTS:
    void OnCurrentIndexConnectionTypeComboBoxChanged(int index);
    void OnAdressLineEditChange(const QString &newAdress);
    void OnPortLineEditChange(const QString &newPort);
    void OnConnectButtonClicked();
    void OnNewSerialPortsChecked();
public:
    void SetButtonsEnabled(bool state);
    bool IsCurrentComPortBisy(QString &comboBoxText);
    bool IsComPortBisy(const QSerialPortInfo *info);

    QTimer *checkSerialPortsTimer() const;
    void setCheckSerialPortsTimer(QTimer *newCheckSerialPortsTimer);

private:
    QVBoxLayout *m_mainLayout;
    QComboBox *m_connectionTypeComboBox;
    QHBoxLayout *m_adressAndPortLayout;
    QLabel *m_adressAndPortLabel;
    QLineEdit *m_adressLineEdit;
    QLineEdit *m_portLineEdit;
    QComboBox *m_comPortNameComboBox;

    QHBoxLayout *m_buttonsLayout;
    QPushButton *m_connectButton;
    QPushButton *m_disconnectButton;

    QTimer *m_checkSerialPortsTimer;
    const QSharedPointer<SettingFileService> m_settingFileService;
};

#endif // UI_CONNECTIONWIDGET_H
