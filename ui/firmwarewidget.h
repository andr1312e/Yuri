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

#include "ui/widgets/sourcecodewidget.h"

class TcpSocket;

class FirmWareWidget:public QWidget
{
    Q_OBJECT
public:
    FirmWareWidget(TcpSocket *m_socket, QWidget *parent);
    ~FirmWareWidget();
public Q_SLOTS:
    void ConsoleLog(const QString message);
Q_SIGNALS:
    void SetStateButtonIdClicked(int id);
    void SendFirmwareToTextEditViewFromDevice(QByteArray &firmwarePart);
private:
    void CreateUI();
    void InsertWidgetsIntoLayout();
    void FillUI();
    void CreateConnections();
private Q_SLOTS:
    void OnUserCommandButtonClicked();
    void SetButtonsEnabled(int state);
private:
    QHBoxLayout *m_mainLayout;
    QSplitter *m_splitter;
    SourceCodeWidget *m_readerFirmwareDataWidget;

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

    SourceCodeWidget *m_writerFirmwareDataWidget;
private:
    void resetProgressBar(int blocksCount);

private:
    const int m_blockSize;
    const QMap<QString, quint8> *m_actionsValues;
    FirmwarePresenter *m_firmwarePresenter;
    QByteArray *m_firmWareSource;

};

#endif // UI_FIRMWAREWIDGET_H
