#ifndef SOURCECODEWIDGET_H
#define SOURCECODEWIDGET_H

#include "firmwaresourceviewer.h"

#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QByteArray>
#include <QFile>
#include <QFileDialog>
#include <QStandardPaths>

class SourceCodeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SourceCodeWidget(const quint8 blockSize, const bool writeMode, QWidget *parent);
    ~SourceCodeWidget();
    QByteArray *getFirmWareSource();
private:
    void CreateUI();
    void InsertWidgetsIntoLayout();
    void FillUI();
    void CreateConnections();

public Q_SLOTS:
    void SendFirmwareSourcePartFromDevice(QByteArray &firmware);
Q_SIGNALS:
    void Flash();
    void StartReadingFirmWareFromDevice();
private Q_SLOTS:
    void LoadFirmWareFromFile();
    void ClearFirmWareSource();

private:
    QVBoxLayout *m_firmWareLayout;
    QLabel *m_firmWareLabel;
    QLabel *m_firmWareValue;
    FirmwareSourceViewer *m_firmWareText;
    QPushButton *m_getFirmware;
    QPushButton *m_clearFirmware;
    QLabel *m_firmWareProcessLabel;
    QProgressBar *m_processBar;
    QPushButton *m_flashButton;

    QByteArray m_firmWareSource;
    const quint8 m_blockSize;//Колво +1 тк вычитаем потом
    const bool m_writeMode;

private:
    void ResetProgressBar(int blocksCount);
    QByteArray GetSubArrayFromQByteArray(int index, const int &firmwareSize, QByteArray &firmware);
};

#endif // SOURCECODEWIDGET_H
