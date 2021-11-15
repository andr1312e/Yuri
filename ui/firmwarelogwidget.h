#ifndef UI_FIRMWARELOGWIDGET_H
#define UI_FIRMWARELOGWIDGET_H

#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class FirmwareLogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FirmwareLogWidget(QWidget *parent);
    ~FirmwareLogWidget();
private:
    void CreateUI();
    void InsertWidgetsIntoLayout();
    void FillUI();
    void ConnectObjects();

Q_SIGNALS:
    void ToRestartDevice();
public Q_SLOTS:
    void OnConsoleLog(QString message);
private:
    QVBoxLayout *m_mainLayout;

    QPushButton *m_restarPushButton;
    QPushButton *m_logClearButton;
    QPlainTextEdit *m_log;
};

#endif // UI_FIRMWARELOGWIDGET_H
