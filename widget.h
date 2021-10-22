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
#include <QDateTime>
#include <QDir>
#include <QFile>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private Q_SLOTS:
    void setButtonState(bool state);
    void tryToConnect();
    void changeSpeedFiled();
    void changeDoplerFiled(const QString &value);
private:
    void sendZeroMessage();
    void sendFirstMessage();
    void sendSecondMessage();
    void sendThirdMessage();
    void sendFourthMessage();
    void sendFiveMessage();
    void sendSixMessage();
    void sendSevenMessageFirstId();
    void sendSevenMessageFourId();
    void sendSevenMessageFiveId();
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
    QLabel *m_speedLabel;
    QLineEdit *m_speedLineEdit;

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

    QHBoxLayout *m_attenuatorLineLayout;
    QLabel *m_attenuatorLabel;
    QComboBox *m_attenuatorComboBox;

    QHBoxLayout *m_noiseLineLayout;
    QLabel *m_noiseLabel;
    QComboBox *m_noiseComboBox;

    QHBoxLayout *m_messageSendButtonsLayout;
    QPushButton *m_zeroMessageSendButton;
    QPushButton *m_firstMessageSendButton;
    QPushButton *m_secondMessageSendButton;
    QPushButton *m_thirdMessageSendButton;
    QPushButton *m_forthMessageSendButton;
    QPushButton *m_fiveMessageSendButton;
    QPushButton *m_sixMessageSendButton;

    QHBoxLayout *m_messageGetButtonsLayout;

    QPushButton *m_sevenMessageFirstSendButton;
    QPushButton *m_sevenMessageFourSendButton;
    QPushButton *m_sevenMessageFiveSendButton;

    QHBoxLayout *m_stateLayout;
    QLabel *m_state;
    QPushButton *m_logClearButton;

    QPlainTextEdit *m_log;

private:
    QFile *m_file;
    QStringList workPointsValues, m_attenuatorValues, m_noiseValues;
    TcpSocket *m_socket;
    QIntValidator *m_intValidator;
    QIntValidator *m_gainValidator;

    const quint32 c=299792458;

private:
    bool allRequedFiledsHave(QStringList &listOfFields);
    bool allDataIsValid();
    void updateHistory();
};
#endif // WIDGET_H
