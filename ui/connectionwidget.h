#ifndef UI_CONNECTIONWIDGET_H
#define UI_CONNECTIONWIDGET_H

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>
#include <QWidget>

class ConnectionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectionWidget(QWidget *parent);
    ~ConnectionWidget();
private:
    void CreateUI();
    void InsertWidgetsIntoLayout();
    void ConnectObjects();
    void FillUI();
Q_SIGNALS:
    void ToConsoleLog(QString message);
    void ToConnectEthernetMoxa(const QString &adress, const QString &port);
    void ToConnectUsbMoxa(const QString &comPortName);
    void ToDisconnectFromMoxa();

private Q_SLOTS:
    void WhenCurrentIndexConnectionTypeComboBoxChanged(int index);
    void WhenConnectButtonClicked();
public:
    void SetButtonsEnabled(bool state);
    bool IsComPortValid(QString &comboBoxText);

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
};

#endif // UI_CONNECTIONWIDGET_H
