#ifndef WIDGET_H
#define WIDGET_H


#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QComboBox>
#include <QStringList>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QTabWidget>

#include "tcpsocket.h"
#include "ui/connectionwidget.h"
#include "ui/statewidget.h"
#include "ui/infowidget.h"
#include "ui/firmwarewidget.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent);
    ~Widget();
private Q_SLOTS:
    void ConnectToMoxa(const QString &adress, const QString &port);
    void DisconnectFromMoxa();
private:
    void sendSevenMessageFirstId();
    void sendSevenMessageFourId();
    void sendSevenMessageFiveId();
private:
    void createUI();
    void insertWidgetsIntoLayout();
    void createConnections();
private:
    QVBoxLayout *m_mainLayout;


    ConnectionWidget *m_connectionWidget;
    QTabWidget *m_tabWidget;
    InfoWidget *m_infoWidget;
    StateWidget *m_commangWidget;
    FirmWareWidget *m_firmwareWidget;

    TcpSocket *m_socket;

private:
    bool allRequedFiledsHave(QStringList &listOfFields);
    bool allDataIsValid();
};
#endif // WIDGET_H
