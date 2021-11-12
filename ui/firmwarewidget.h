#ifndef UI_FIRMWAREWIDGET_H
#define UI_FIRMWAREWIDGET_H

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QMap>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QStandardPaths>
#include <QProgressBar>
#include <QSplitter>

#include "presenter/firmwarepresenter.h"

#include "ui/firmwareflasherwidget.h"

class TcpHandler;

class FirmWareWidget: public QWidget
{
    Q_OBJECT
public:
    FirmWareWidget(QWidget *parent);
    ~FirmWareWidget();
private:
    void CreateUI();
    void InsertWidgetsIntoLayout();
    void FillUI();
    void ConnectObjects();
public Q_SLOTS:
    void OnConsoleLog(const QString message);
Q_SIGNALS:
    void ToSetStateButtonIdClicked(int id);
    void ToDataHanderChanged();

private Q_SLOTS:
    void OnUserCommandButtonClicked();
    void OnSetButtonsEnabled(int state);
    void OnSetMaximumProgressBar(int top);
    void OnUpdateProgressBar(int page);
    void OnSetWidgetsEnable(bool state);
public:
    void DisconnectOldHander();
    void ConnectHander(DataHandler *dataHandler);
private:
    QVBoxLayout *m_mainLayout;
    QSplitter *m_splitter;


    QVBoxLayout *m_commandsLayout;
    QHBoxLayout *m_flashStateLayout;
    QLabel *m_registerLabel;
    QLabel *m_registerValue;
    QPushButton *m_restarPushButton;

    QHBoxLayout *m_actionsLayout;
    QLabel *m_actionLabel;
    QComboBox *m_actionComboBox;

    QLabel *m_adressForWriteLabel;
    QLineEdit *m_adressForWriteLineEdit;

    QLabel *m_numOfBytesLabel;
    QLineEdit *m_numOfBytesLineEdit;
    QPushButton *m_sendCommandButton;
    QPlainTextEdit *m_log;
    FirmwareFlasherWidget *m_firmwareFlasherWidget;

    QProgressBar *m_progressBar;
private:
    void resetProgressBar(int blocksCount);

private:
    const QMap<QString, quint8> *m_actions;
    FirmwarePresenter *m_firmwarePresenter;
    QByteArray *m_firmWareSource;

};

#endif // UI_FIRMWAREWIDGET_H
