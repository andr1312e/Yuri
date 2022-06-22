#ifndef UI_FIRMWAREWIDGET_H
#define UI_FIRMWAREWIDGET_H

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QMap>
#include <QProgressBar>
#include <QSplitter>

#include "services/settingfileservice.h"

#include "presenter/firmwarepresenter.h"

#include "ui/firmwarelogwidget.h"
#include "ui/firmwareflasherwidget.h"

class TcpHandler;

class FirmWareWidget: public QWidget
{
    Q_OBJECT
public:
    explicit FirmWareWidget(SettingFileService *settingFileService, QWidget *parent);
    ~FirmWareWidget();
private:
    void CreateObjects();
    void CreateUI();
    void InsertWidgetsIntoLayout();
    void FillUI();
    void ConnectObjects();

Q_SIGNALS:
    void ToConsoleLog(const QString &message);
    void ToSetStateButtonIdClicked(int id);
    void ToDataHanderChanged();
    void ToSetButtonsEnabled(bool state);
private Q_SLOTS:
    void OnSetButtonsEnabled(bool state);
    void OnSetMaximumProgressBar(int top);
    void OnUpdateProgressBar(int page);

public:
    void DisconnectOldHander();
    void ConnectHander(DataHandler *dataHandler);

private:
    QVBoxLayout *m_mainLayout;
    QSplitter *m_splitter;

    FirmwareLogWidget *m_firmwareLogWidget;

    FirmwareFlasherWidget *m_firmwareFlasherWidget;

    QProgressBar *m_progressBar;
private:
    void ResetProgressBar(int blocksCount);

private:
    SettingFileService *m_settingFileService;
    FirmwarePresenter *m_firmwarePresenter;
};

#endif // UI_FIRMWAREWIDGET_H
