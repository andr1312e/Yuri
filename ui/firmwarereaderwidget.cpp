#include "firmwarereaderwidget.h"

FirmwareReaderWidget::FirmwareReaderWidget(QWidget *parent)
    : QWidget(parent)
{
    CreateUI();
    InsertWidgetsIntoLayout();
    FillUI();
    CreateConnections();
}

FirmwareReaderWidget::~FirmwareReaderWidget()
{
    delete m_mainLayout;

    delete m_readFirmwareButton;
    delete m_firmWareProcessLabel;
    delete m_firmWareProcessValue;
    delete m_pageNumbereLabel;
    delete m_pageNumberValue;
}

void FirmwareReaderWidget::CreateUI()
{
    m_mainLayout= new QVBoxLayout();
    m_readFirmwareButton=new QPushButton();
    m_firmWareProcessLabel=new QLabel ();
    m_firmWareProcessValue=new QLabel();
    m_pageNumbereLabel=new QLabel();
    m_pageNumberValue=new QLabel();
}

void FirmwareReaderWidget::InsertWidgetsIntoLayout()
{
    m_mainLayout->addWidget(m_readFirmwareButton);
    m_mainLayout->addWidget(m_firmWareProcessLabel);
    m_mainLayout->addWidget(m_firmWareProcessValue);
    m_mainLayout->addWidget(m_pageNumberValue);
    m_mainLayout->addWidget(m_pageNumberValue);
    setLayout(m_mainLayout);
}

void FirmwareReaderWidget::FillUI()
{
    m_readFirmwareButton->setText("Cчитать прошивку с устройства");
    m_firmWareProcessLabel->setText("Текущее действие:");
    m_firmWareProcessValue->setText("Прошивка не считывается");
    m_pageNumbereLabel->setText("Текущая страница");
    m_pageNumberValue->setText("0");
}

void FirmwareReaderWidget::CreateConnections()
{
    connect(m_readFirmwareButton, &QPushButton::clicked, this, &FirmwareReaderWidget::StartReadingFirmWareFromDevice);
}

void FirmwareReaderWidget::PageUpdated(int currentAdress)
{
    int pageNumber=currentAdress/16;
    m_pageNumberValue->setText(QString::number(pageNumber));
}
