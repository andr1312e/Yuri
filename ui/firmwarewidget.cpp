#include "firmwarewidget.h"

FirmWareWidget::FirmWareWidget(QWidget *parent)
    : QWidget(parent)
    , m_actions(new QMap<QString, quint8>{{"Чтение буфера", 3}, {"Запись в буфер", 2}, {"Сделать доступной на запись", 6}, {"Сделать недоступной на запись", 4}, {"Cтирание(перед перепрошивкой)", 199}, {"Cтатус флешки(что работает)", 9}, {"Чтение регистра", 5}})
    , m_firmwarePresenter(new FirmwarePresenter(m_actions, this))
{
    CreateUI();
    InsertWidgetsIntoLayout();
    FillUI();
    ConnectObjects();
}

FirmWareWidget::~FirmWareWidget()
{
    delete m_flashStateLayout;
    delete m_actionsLayout;
    delete m_commandsLayout;

    delete m_mainLayout;


    delete m_actions;
    delete m_firmwarePresenter;

    delete m_registerLabel;
    delete m_registerValue;
    delete m_restarPushButton;
    delete m_actionLabel;
    delete m_actionComboBox;

    delete m_adressForWriteLabel;
    delete m_adressForWriteLineEdit;

    delete m_numOfBytesLabel;
    delete m_numOfBytesLineEdit;
    delete m_sendCommandButton;
    delete m_log;

    delete m_firmwareReaderWidget;
    delete m_firmwareFlasherWidget;

    delete m_splitter;
    delete m_progressBar;
}

void FirmWareWidget::WhenConsoleLog(const QString message)
{
    m_log->appendPlainText(message);
}

void FirmWareWidget::CreateUI()
{
    m_mainLayout=new QVBoxLayout();

    m_commandsLayout=new QVBoxLayout();
    m_flashStateLayout=new QHBoxLayout();
    m_actionsLayout=new QHBoxLayout();

    m_splitter=new QSplitter();


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

    m_firmwareReaderWidget=new FirmwareReaderWidget(this);
    m_firmwareFlasherWidget=new FirmwareFlasherWidget(this);

    m_progressBar=new QProgressBar();
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
    m_mainLayout->addWidget(m_progressBar);

    setLayout(m_mainLayout);
}

void FirmWareWidget::FillUI()
{
    m_progressBar->setRange(0, 100);
    m_splitter->addWidget(m_firmwareReaderWidget);
    m_splitter->addWidget(m_firmwareFlasherWidget);
    m_adressForWriteLineEdit->setText(QStringLiteral("0"));
    m_numOfBytesLineEdit->setText(QStringLiteral("15"));
    m_registerLabel->setText("Значение регистров: ");
    m_registerValue->setText("Недоступна для записи");
    m_restarPushButton->setText("Перезагрузить");
    m_actionComboBox->addItems(m_actions->keys());
    m_actionComboBox->setEditable(false);
    m_actionLabel->setText(QStringLiteral("Текущее действие:"));
    m_adressForWriteLabel->setText(QStringLiteral("Адрес для записи:"));
    m_numOfBytesLabel->setText(QStringLiteral("Колличество байт для записи"));
    m_sendCommandButton->setText(QStringLiteral("Отправить"));
    m_log->setReadOnly(true);
    //    SetButtonsEnabled(false);
}

void FirmWareWidget::ConnectObjects()
{
    connect(m_firmwareFlasherWidget, &FirmwareFlasherWidget::ToFlash, m_firmwarePresenter, &FirmwarePresenter::WhenFlash);
    connect(m_sendCommandButton, &QPushButton::clicked, this, &FirmWareWidget::OnUserCommandButtonClicked);
    connect(m_firmwareReaderWidget, &FirmwareReaderWidget::StartReadingFirmWareFromDevice, m_firmwarePresenter, &FirmwarePresenter::WhenStartReadingFirmWareFromDevice);
    connect(m_firmwarePresenter, &FirmwarePresenter::ToConsoleLog,  this, &FirmWareWidget::WhenConsoleLog);
    connect(m_firmwarePresenter, &FirmwarePresenter::ToProgressBarUpdate, m_firmwareReaderWidget, &FirmwareReaderWidget::WhenProgressBarUpdated);
    connect(m_firmwarePresenter, &FirmwarePresenter::ToSetButtonsEnabled, this, &FirmWareWidget::SetButtonsEnabled);
    connect(m_restarPushButton, &QPushButton::clicked, [&](){m_firmwarePresenter->SendMessageToQueue(0, 0, 0);});
    connect(m_firmwarePresenter, &FirmwarePresenter::ToSetMaximumCountOfPages, this, &FirmWareWidget::SetMaximumProgressBar);
    connect(m_firmwarePresenter, &FirmwarePresenter::ToProgressBarUpdate, m_progressBar, &QProgressBar::setValue);
    connect(m_firmwarePresenter, &FirmwarePresenter::ToWidgetsEnable, this, &FirmWareWidget::SetWidgetsEnable);
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
    m_firmwareReaderWidget->setEnabled(state);
    m_firmwareFlasherWidget->setEnabled(state);
    m_sendCommandButton->setEnabled(state);
}

void FirmWareWidget::SetMaximumProgressBar(int top)
{
    m_progressBar->reset();
    m_progressBar->setRange(0, top);
}

void FirmWareWidget::UpdateProgressBar(int page)
{
    m_progressBar->setValue(page);
}

void FirmWareWidget::SetWidgetsEnable(bool state)
{
    m_firmwareReaderWidget->setEnabled(state);
    m_firmwareFlasherWidget->setEnabled(state);
    m_sendCommandButton->setEnabled(state);
    m_restarPushButton->setEnabled(state);
}

void FirmWareWidget::DisconnectOldHander()
{
    m_firmwarePresenter->DisconnectOldHandler();
}

void FirmWareWidget::ConnectHander(DataHandler *dataHandler)
{
    m_firmwarePresenter->ConnectHander(dataHandler);
}
