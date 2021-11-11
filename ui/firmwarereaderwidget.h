#ifndef FIRMWAREREADERWIDGET_H
#define FIRMWAREREADERWIDGET_H

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class FirmwareReaderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FirmwareReaderWidget(QWidget *parent);
    ~FirmwareReaderWidget();
private:
    void CreateUI();
    void InsertWidgetsIntoLayout();
    void FillUI();
    void CreateConnections();

Q_SIGNALS:
    void StartReadingFirmWareFromDevice();
public Q_SLOTS:
    void PageUpdated(int currentAdress);
private:
    QVBoxLayout *m_mainLayout;
    QPushButton *m_readFirmwareButton;
    QLabel *m_firmWareProcessLabel;
    QLabel *m_firmWareProcessValue;
    QLabel *m_pageNumbereLabel;
    QLabel *m_pageNumberValue;
};

#endif // FIRMWAREREADERWIDGET_H
