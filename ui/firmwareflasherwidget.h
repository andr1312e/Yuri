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
    void ToFlash(QByteArray *firmware);
    void StartReadingFirmWareFromDevice();

private Q_SLOTS:
    void WhenAddNewFirmwareButtonClicked();
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
    QByteArray m_firmwareFromFile;
};

#endif // UI_FIRMWAREFLASHERWIDGET_H
