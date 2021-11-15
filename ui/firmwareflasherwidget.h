#ifndef UI_FIRMWAREFLASHERWIDGET_H
#define UI_FIRMWAREFLASHERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QByteArray>
#include <QFileDialog>
#include <QIODevice>
#include <QStandardPaths>
#include <QComboBox>
#include <QCheckBox>

class FirmwareFlasherWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FirmwareFlasherWidget(QWidget *parent);
    ~FirmwareFlasherWidget();
private:
    void CreateUI();
    void InsertWidgetsIntoLayout();
    void FillUI();
    void ConnectObjects();

Q_SIGNALS:
    void ToConsoleLog(QString message);
    void ToSetFirmwareFromFileToPresenter(QByteArray &firmwareFromFile);
    void ToFlash(bool isNeedToCheck, bool isNeedToRestartAfterSuccess);
    void ToStartReadingFirmWareFromDevice();
private Q_SLOTS:
    void OnAddNewFirmwareButtonClicked();

private:
    void SetButtonsEnabled(bool state);
private:
    QVBoxLayout *m_mainLayout;
    QLabel *m_fileNameLabel;
    QLabel *m_fileNameValue;
    QPushButton *m_addNewFileButton;
    QPushButton *m_flashButton;
    QPushButton *m_readFirmwareButton;
    QLabel *m_firmWareProcessLabel;
    QCheckBox *m_checkFirmwareCheckBox;
    QCheckBox *m_restartFirmwareCheckBox;
private:
    const qsizetype m_notNeededSymbolsIndex;
};

#endif // UI_FIRMWAREFLASHERWIDGET_H
