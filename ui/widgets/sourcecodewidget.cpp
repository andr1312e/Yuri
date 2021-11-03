#include "sourcecodewidget.h"



SourceCodeWidget::SourceCodeWidget(const quint8 blockSize, const bool writeMode, QWidget *parent)
    : QWidget(parent)
    , m_blockSize(blockSize)
    , m_writeMode(writeMode)
{
    CreateUI();
    InsertWidgetsIntoLayout();
    FillUI();
    CreateConnections();
}

SourceCodeWidget::~SourceCodeWidget()
{

}

QByteArray *SourceCodeWidget::getFirmWareSource()
{
//    return nullptr;
    return &m_firmWareSource;
}

void SourceCodeWidget::CreateUI()
{
    m_firmWareLayout=new QVBoxLayout();
    m_firmWareLabel=new QLabel();
    m_firmWareValue=new QLabel ();
    m_firmWareText=new FirmwareSourceViewer(nullptr);
    m_clearFirmware=new QPushButton();
    m_getFirmware= new QPushButton();
    m_clearFirmware =new QPushButton();
    m_firmWareProcessLabel=new QLabel();
    m_processBar=new QProgressBar();
    m_flashButton=new QPushButton();
}

void SourceCodeWidget::InsertWidgetsIntoLayout()
{
    m_firmWareLayout->addWidget(m_firmWareLabel);
    m_firmWareLayout->addWidget(m_firmWareValue);
    m_firmWareLayout->addWidget(m_firmWareText);
    m_firmWareLayout->addWidget(m_getFirmware);
    m_firmWareLayout->addWidget(m_clearFirmware);
    m_firmWareLayout->addWidget(m_firmWareProcessLabel);
    m_firmWareLayout->addWidget(m_processBar);
    m_firmWareLayout->addWidget(m_flashButton);
    setLayout(m_firmWareLayout);
}

void SourceCodeWidget::FillUI()
{

    if (m_writeMode)
    {
        m_firmWareLabel->setText("Наша прошивка:(С файла сейчас)");
        m_firmWareValue->setText("Прошивка не выбрана");
        m_getFirmware->setText("Добавить прошивку");
        m_clearFirmware->setText("Убрать прошивку");
        m_firmWareProcessLabel->setText("Прошивка не загружается");
        m_clearFirmware->setDisabled(true);
        m_flashButton->setText("Прошить");
        m_flashButton->setDisabled(true);
    }
    else
    {
        m_firmWareLabel->setText("Ваша прошивка:(На устройстве сейчас)");
        m_firmWareValue->setText("Прошивка не считана");
        m_getFirmware->setText("Считать прошивку с устройства");
        m_clearFirmware->setText("Очистить поля");
        m_firmWareProcessLabel->setText("Прошивка не считывается");
        m_flashButton->setText("Считать");
    }
    m_processBar->setOrientation(Qt::Horizontal);
    m_processBar->setRange(0, 100);
    QFont font=m_flashButton->font();
    font.setPixelSize(30);
    m_flashButton->setFont(font);
    m_flashButton->setFixedHeight(70);

}

void SourceCodeWidget::CreateConnections()
{
    if(m_writeMode)
    {
        connect(m_getFirmware, &QPushButton::clicked, this, &SourceCodeWidget::LoadFirmWareFromFile);
    }
    else
    {
        connect(m_getFirmware, &QPushButton::clicked, this, &SourceCodeWidget::StartReadingFirmWareFromDevice);
    }
    connect(m_clearFirmware, &QPushButton::clicked, this, &SourceCodeWidget::ClearFirmWareSource);
    connect(m_flashButton, &QPushButton::clicked, this, &SourceCodeWidget::Flash);
}

void SourceCodeWidget::ResetProgressBar(int blocksCount)
{
    m_processBar->setMinimum(0);
    m_processBar->setMaximum(blocksCount);
    m_processBar->reset();
}

QByteArray SourceCodeWidget::GetSubArrayFromQByteArray(int index,const int &firmwareSize, QByteArray &firmware)
{
    if (index+m_blockSize<firmwareSize)
    {
        return firmware.mid(index, m_blockSize);
    }
    else
    {
        return firmware.mid(index);
    }
}

void SourceCodeWidget::LoadFirmWareFromFile()
{
    QString strDesktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString pathToFirmware=QFileDialog::getOpenFileName(nullptr, "Выберите файл прошивки", strDesktop,  "Файл настроек (*.bit)");
    QFile file(pathToFirmware);
    if (file.open(QIODevice::ReadOnly))
    {
        m_firmWareSource=file.readAll();//107
        m_firmWareSource.remove(0, 107);
        qDebug()<< m_firmWareSource.length();
//        qDebug()<< ba.toHex();
        m_firmWareProcessLabel->setText("Прошивка сейчас загружается...");
        int arrSize=m_firmWareSource.size();
        int blocksCount=arrSize/(m_blockSize);
//        ResetProgressBar(blocksCount);
//        for (int currentIndex=0,  progressBarValue=0; currentIndex<arrSize; currentIndex+=(m_blockSize))
//        {
//            m_firmWareText->appendPlainText(GetSubArrayFromQByteArray(currentIndex, arrSize, m_firmWareSource).toHex());
//            m_processBar->setValue(progressBarValue);
//            progressBarValue++;
//        }
        m_flashButton->setEnabled(true);
        m_clearFirmware->setEnabled(true);
        m_firmWareValue->setText(file.fileName().section("/",-1,-1));
        m_processBar->setValue(m_processBar->maximum());
        m_firmWareProcessLabel->setText("Прошивка загружена");
    }
    else
    {
        ClearFirmWareSource();
    }
}

void SourceCodeWidget::ClearFirmWareSource()
{
    if(m_writeMode)
    {
        m_clearFirmware->setDisabled(true);
        m_flashButton->setDisabled(true);
        m_firmWareValue->setText("Прошивка не выбрана");
        m_firmWareProcessLabel->setText("Прошивка не загружается");
    }
    else
    {
        m_firmWareValue->setText("Прошивка не считана");
        m_firmWareProcessLabel->setText("Прошивка не считывается");
    }
    m_firmWareSource.clear();
    m_processBar->reset();
    m_firmWareText->clear();


}

void SourceCodeWidget::SendFirmwareSourcePartFromDevice(QByteArray &firmware)
{
    QByteArray firmwareHexFormat=firmware.toHex();
    m_firmWareText->appendPlainText(firmwareHexFormat);
}
