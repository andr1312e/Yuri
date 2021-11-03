#include "newfirmwarewidget.h"

NewFirmwareWidget::NewFirmwareWidget(QWidget *parent)
    : QWidget(parent)
{

}

NewFirmwareWidget::~NewFirmwareWidget()
{
    delete m_mainLayout;
    delete m_mainLayout;
    delete m_fileNameLabel;
    delete m_fileNameValue;
    delete m_addNewFileButton;
    delete m_flashButton;
    delete m_firmWareProcessLabel;
    delete m_progressBar;
}

void NewFirmwareWidget::CreateUI()
{
    m_mainLayout=new QVBoxLayout();
    m_fileNameLabel=new QLabel();
    m_fileNameValue=new QLabel();
    m_addNewFileButton=new QPushButton();
    m_flashButton=new QPushButton();
    m_firmWareProcessLabel=new QLabel();
    m_progressBar=new QProgressBar();
}

void NewFirmwareWidget::InsertWidgetsIntoLayout()
{
    m_mainLayout->addWidget(m_fileNameLabel);
    m_mainLayout->addWidget(m_fileNameValue);
    m_mainLayout->addWidget(m_addNewFileButton);
    m_mainLayout->addWidget(m_flashButton);
    m_mainLayout->addWidget(m_firmWareProcessLabel);
    m_mainLayout->addWidget(m_progressBar);
    setLayout(m_mainLayout);
}

void NewFirmwareWidget::FillUI()
{
    m_fileNameLabel->setText("ПРОШИВКА С ПК \n Имя файла:");
    m_fileNameValue->setText("Файл не выбран");
    m_addNewFileButton->setText("Добавить файл");
    m_flashButton->setText("Прошить");

    m_firmWareProcessLabel->setText("Прошивка не загружается");
    m_progressBar->setRange(0, 2);
}

void NewFirmwareWidget::CreateConnections()
{
    connect(m_addNewFileButton, &QPushButton::clicked, this, &NewFirmwareWidget::AddNewFirmWare);
    connect(m_flashButton, &QPushButton::clicked, this, &NewFirmwareWidget::Flash);
}

void NewFirmwareWidget::AddNewFirmWare()
{
    m_progressBar->setValue(1);
    QString strDesktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString pathToFirmware=QFileDialog::getOpenFileName(nullptr, "Выберите файл прошивки", strDesktop,  "Файл настроек (*.bit)");
    QFile file(pathToFirmware);
    if (file.open(QIODevice::ReadOnly))
    {
        m_firmwareFromFile=file.readAll();//107
        m_firmwareFromFile.remove(0, 107);
        m_flashButton->setEnabled(true);
        m_progressBar->setValue(2);
        m_firmWareProcessLabel->setText("Прошивка загружена");
    }
    else
    {
        Q_EMIT ConsoleLog("Ошибка открытия файла " +file.errorString());
        m_flashButton->setDisabled(true);
        m_progressBar->setValue(1);
    }
}
