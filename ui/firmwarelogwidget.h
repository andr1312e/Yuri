#ifndef UI_FIRMWARELOGWIDGET_H
#define UI_FIRMWARELOGWIDGET_H
#include <QDateTime>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>

#include "services/settingfileservice.h"

class FirmwareLogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FirmwareLogWidget(SettingFileService *settingFileService, QWidget *parent);
    ~FirmwareLogWidget();
private:
    void CreateUI();
    void InsertWidgetsIntoLayout();
    void FillUI();
    void ConnectObjects();

Q_SIGNALS:
    void ToRestartDevice();
public Q_SLOTS:
    void OnConsoleLog(const QString &message);
    void OnResultLabelSetText(const QString &message);
public:
    void SetWidgetEnabled(bool state);
private:
    SettingFileService *const m_settingFileService;
private:
    QVBoxLayout *m_mainLayout;
    QPushButton *m_restarPushButton;
    QPushButton *m_logClearButton;
    QPlainTextEdit *m_log;
    QLabel *m_resultLabel;

};

#endif // UI_FIRMWARELOGWIDGET_H
