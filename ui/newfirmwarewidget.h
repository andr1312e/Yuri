#ifndef NEWFIRMWAREWIDGET_H
#define NEWFIRMWAREWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QByteArray>
#include <QFileDialog>
#include <QIODevice>
#include <QStandardPaths>

class NewFirmwareWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NewFirmwareWidget(QWidget *parent);
    ~NewFirmwareWidget();
private:
    void CreateUI();
    void InsertWidgetsIntoLayout();
    void FillUI();
    void CreateConnections();
Q_SIGNALS:
    void ConsoleLog(QString message);
    void Flash();
private Q_SLOTS:
    void AddNewFirmWare();
private:
    QVBoxLayout *m_mainLayout;
    QLabel *m_fileNameLabel;
    QLabel *m_fileNameValue;
    QPushButton *m_addNewFileButton;
    QPushButton *m_flashButton;
    QLabel *m_firmWareProcessLabel;
    QProgressBar *m_progressBar;
private:
    QByteArray m_firmwareFromFile;

};

#endif // NEWFIRMWAREWIDGET_H
