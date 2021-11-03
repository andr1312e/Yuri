#include "firmwarewidget.h"

FirmWareWidget::FirmWareWidget(TcpSocket *m_socket, QWidget *parent)
    : QWidget(parent)
    , m_blockSize(16)
    , m_actionsValues(new QMap<QString, quint8>{{"Чтение буфера", 3}, {"Запись в буфер", 2}, {"Сделать доступной на запись", 6}, {"Сделать недоступной на запись", 4}, {"Cтирание(перед перепрошивкой)", 199}, {"Cтатус флешки(что работает)", 9}, {"Чтение регистра", 5}})
    , m_firmwarePresenter(new FirmwarePresenter(m_blockSize,m_actionsValues, m_socket))
{
    CreateUI();
    InsertWidgetsIntoLayout();
    FillUI();
    CreateConnections();
    m_firmwarePresenter->SetFirmwareSource(m_writerFirmwareDataWidget->getFirmWareSource());
}

FirmWareWidget::~FirmWareWidget()
{
    delete m_actionsValues;
    delete m_firmwarePresenter;
}

void FirmWareWidget::ConsoleLog(const QString message)
{
//    QString time=QDateTime::currentDateTime().toString("hh:mm:ss");
    m_log->appendPlainText(/*time+ " "+*/ message);
}

void FirmWareWidget::CreateUI()
{
    m_mainLayout=new QHBoxLayout();

    m_commandsLayout=new QVBoxLayout();
    m_flashStateLayout=new QHBoxLayout();
    m_actionsLayout=new QHBoxLayout();

    m_splitter=new QSplitter();
    m_readerFirmwareDataWidget=new SourceCodeWidget(m_blockSize, false, this);

    m_registerLabel=new QLabel();
    m_registerValue=new QLabel();
    m_restarPushButton=new QPushButton();
    m_actionLabel=new QLabel();
    m_actionComboBox=new QComboBox();

    m_adressForWriteLabel=new QLabel();
    m_adressForWriteLineEdit=new QLineEdit();

    m_numOfBytesLabel=new QLabel();
    m_numOfBytesLineEdit=new QLineEdit();
    m_sendCommandButton=new QPushButton();
    m_log=new QPlainTextEdit();

    m_writerFirmwareDataWidget=new SourceCodeWidget(m_blockSize, true, this);
}

void FirmWareWidget::InsertWidgetsIntoLayout()
{
    m_flashStateLayout->addWidget(m_registerLabel);
    m_flashStateLayout->addWidget(m_registerValue);
    m_flashStateLayout->addWidget(m_restarPushButton);

    m_actionsLayout->addWidget(m_actionLabel);
    m_actionsLayout->addWidget(m_actionComboBox);
    m_actionsLayout->addWidget(m_adressForWriteLabel);
    m_actionsLayout->addWidget(m_adressForWriteLineEdit);
    m_actionsLayout->addWidget(m_numOfBytesLabel);
    m_actionsLayout->addWidget(m_numOfBytesLineEdit);
    m_actionsLayout->addWidget(m_sendCommandButton);

    m_commandsLayout->addLayout(m_flashStateLayout);
    m_commandsLayout->addLayout(m_actionsLayout);
    m_commandsLayout->addWidget(m_log);

    QWidget *widget=new QWidget();
    widget->setLayout(m_commandsLayout);
    m_splitter->addWidget(widget);

    m_mainLayout->addWidget(m_splitter);

    setLayout(m_mainLayout);
}

void FirmWareWidget::FillUI()
{
    m_splitter->addWidget(m_readerFirmwareDataWidget);
    m_splitter->addWidget(m_writerFirmwareDataWidget);
    m_adressForWriteLineEdit->setText("0");
    m_numOfBytesLineEdit->setText("15");
    m_registerLabel->setText("Значение регистров: ");
    m_registerValue->setText("Недоступна для записи");
    m_restarPushButton->setText("Перезагрузить");
    m_actionComboBox->addItems(m_actionsValues->keys());
    m_actionComboBox->setEditable(false);
    m_actionLabel->setText("Текущее действие:");
    m_adressForWriteLabel->setText("Адрес для записи:");
    m_numOfBytesLabel->setText("Колличество байт для записи");
    m_sendCommandButton->setText("Отправить");
    m_log->setReadOnly(true);
//    SetButtonsEnabled(false);
}

void FirmWareWidget::CreateConnections()
{
    connect(m_writerFirmwareDataWidget, &SourceCodeWidget::Flash, m_firmwarePresenter, &FirmwarePresenter::Flash);
    connect(m_sendCommandButton, &QPushButton::clicked, this, &FirmWareWidget::OnUserCommandButtonClicked);
    connect(m_readerFirmwareDataWidget, &SourceCodeWidget::StartReadingFirmWareFromDevice, m_firmwarePresenter, &FirmwarePresenter::StartReadingFirmWareFromDevice);
    connect(m_firmwarePresenter, &FirmwarePresenter::ConsoleLog,  this, &FirmWareWidget::ConsoleLog);
    connect(m_firmwarePresenter, &FirmwarePresenter::SendFirmwareSourcePartFromDevice, m_readerFirmwareDataWidget, &SourceCodeWidget::SendFirmwareSourcePartFromDevice);
    connect(m_firmwarePresenter, &FirmwarePresenter::SetButtonsEnabled, this, &FirmWareWidget::SetButtonsEnabled);
    connect(m_restarPushButton, &QPushButton::clicked, [&](){m_firmwarePresenter->SendMessageToQueue(0, 0, 0);});
}

void FirmWareWidget::OnUserCommandButtonClicked()
{
    const QString comboBoxValue(m_actionComboBox->currentText());
    QString adress(m_adressForWriteLineEdit->text());
    QString lenght(m_numOfBytesLineEdit->text());
    m_firmwarePresenter->GetDataFromWidget(comboBoxValue, adress, lenght);
}

void FirmWareWidget::SetButtonsEnabled(int state)
{
    m_actionComboBox->setEnabled(state);
    m_readerFirmwareDataWidget->setEnabled(state);
    m_writerFirmwareDataWidget->setEnabled(state);
    m_sendCommandButton->setEnabled(state);
}
