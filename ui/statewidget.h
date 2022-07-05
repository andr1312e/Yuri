#ifndef UI_STATEWIDGET_H
#define UI_STATEWIDGET_H

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QWidget>
#include <QDir>
#include <QButtonGroup>

#include "services/settingfileservice.h"

#include "constantsandfunctions.h"

#include "connections/datahandler.h"
#include "presenter/statepresenter.h"

#include "ui/datawidget.h"
#include "ui/bparwidget.h"

class StateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StateWidget(SettingFileService *settingFileService, QWidget *parent);
    ~StateWidget();

private:
    void CreateObjects();
    void CreateUI();
    void InsertWidgetsIntoLayout();
    void FillUI();
    void ConnectObjects();

Q_SIGNALS:
    void ToDataHanderChanged();
private Q_SLOTS:
    void OnSetState(int messageIdWantToSend, QList<double> params);
    void OnGetState(int messageWantToGet);
    void OnSendRawHexMessage();
    void OnSendRawHexMessage(const QByteArray &array);

public Q_SLOTS:
    void OnSetWorkMode(int index);
    void OnSetButtonEnabled(bool state);
    void OnConsoleLog(const QString &message);

public:
    void DisconnectOldHander();
    void ConnectHander(DataHandler *dataHandler);

private:
    void UpdateHistoryFile();
private:
    QVBoxLayout *m_mainLayout;

    QHBoxLayout *m_modesLayout;
    DataWidget *m_dataWidget;
    BParWidget *m_bPraWidget;

    QHBoxLayout *m_workModeLayout;
    QLabel *m_workModeLabel;
    QComboBox *m_workModeComboBox;
    QPushButton *m_getCurrentWorkMode;
    QPushButton *m_pingButton;
    QPushButton *m_restartDevice;

    QHBoxLayout *m_rawHexLayout;
    QLineEdit *m_rawHexInputEdit;
    QPushButton *m_sendRawHexButton;

    QPushButton *m_logClearButton;
    QPlainTextEdit *m_log;
private:
    SettingFileService *const m_settingFileService;
    StatePresenter *m_statePresenter;
    const quint32 c = 299792458;
    QFile m_file;

private:
    const QString m_settingsAtribute = "state";

};

#endif // UI_STATEWIDGET_H
