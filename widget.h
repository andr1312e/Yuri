#ifndef WIDGET_H
#define WIDGET_H
#include "tcpsocket.h"

#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QComboBox>
#include <QStringList>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private Q_SLOTS:
    void tryToConnect();
    void sendFirstMessage();
    void sendSecondMessage();
    void sendThirdMessage();
    void sendFourthMessage();
private:
    void createUI();
    void insertWidgetsIntoLayout();
    void createConnections();
private:

    QVBoxLayout *m_mainLayout;

    QHBoxLayout *m_adressAndPortLayout;
    QLabel *m_adressAndPortLabel;
    QLineEdit *m_adressLineEdit;
    QLineEdit *m_portLineEdit;

    QHBoxLayout *m_buttonsLayout;
    QPushButton *m_connectButton;
    QPushButton *m_disconnectButton;

    QHBoxLayout *m_speedLayout;
    QLabel *m_DoplerFreqLabel;
    QLineEdit *m_DoplerFreqLineEdit;

    QHBoxLayout *m_rangeLineLayout;
    QLabel *m_rangeLabel;
    QLineEdit *m_rangeLineEdit;

    QHBoxLayout *m_workPointLineLayout;
    QLabel *m_fvcoLabel;
    QComboBox *m_fvcoComboBox;

    QHBoxLayout *m_gainLineLayout;
    QLabel *m_gainTXLabel;
    QLineEdit *m_gainTXLineEdit;
    QLabel *m_gainRXLabel;
    QLineEdit *m_gainRXLineEdit;

    QHBoxLayout *m_messageSendButtonsLayout;
    QPushButton *m_firstMessageSendButton;
    QPushButton *m_secondMessageSendButton;
    QPushButton *m_thirdMessageSendButton;
    QPushButton *m_forthMessageSendButton;

    QHBoxLayout *m_stateLayout;
    QLabel *m_state;
    QLabel *m_ping;

    QPlainTextEdit *m_log;

    QStringList workPointsValues;

private:
    TcpSocket *m_socket;
    QIntValidator *m_intValidator;
    QIntValidator *m_gainValidator;

private:
    bool allRequedFiledsHave(QStringList &listOfFields);
    bool allDataIsValid();
};
#endif // WIDGET_H
