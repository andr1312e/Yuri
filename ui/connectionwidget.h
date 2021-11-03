#ifndef UI_CONNECTIONWIDGET_H
#define UI_CONNECTIONWIDGET_H

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class ConnectionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectionWidget(QWidget *parent);
    ~ConnectionWidget();
    void SetButtonsEnabled(bool state);
private:
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_adressAndPortLayout;
    QLabel *m_adressAndPortLabel;
    QLineEdit *m_adressLineEdit;
    QLineEdit *m_portLineEdit;

    QHBoxLayout *m_buttonsLayout;
    QPushButton *m_connectButton;
    QPushButton *m_disconnectButton;
private:
    void CreateUI();
    void InsertWidgetsIntoLayout();
    void CreateConnections();
Q_SIGNALS:
   void ConnectToMoxa(const QString &adress, const QString &port);
   void DisconnectFromMoxa();

};

#endif // UI_CONNECTIONWIDGET_H
