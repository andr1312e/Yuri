#include "connectionwidget.h"

ConnectionWidget::ConnectionWidget(QSharedPointer<SettingFileService> &settingFileService, QWidget *parent)
    : QWidget(parent)
    , m_settingFileService(settingFileService)
{
    CreateObjects();
    InitObjects();
    CreateUI();
    InsertWidgetsIntoLayout();
    FillUI();
    ConnectObjects();
}

ConnectionWidget::~ConnectionWidget()
{
    delete m_checkSerialPortsTimer;

    delete m_adressAndPortLayout;
    delete m_buttonsLayout;
    delete m_mainLayout;

    delete m_connectionTypeComboBox;
    delete m_adressAndPortLabel;
    delete m_adressLineEdit;
    delete m_portLineEdit;
    delete m_comPortNameComboBox;
    delete m_connectButton;
    delete m_disconnectButton;
}

void ConnectionWidget::CreateObjects()
{
    m_checkSerialPortsTimer=new QTimer(this);
}

void ConnectionWidget::InitObjects()
{
    m_checkSerialPortsTimer->setTimerType(Qt::VeryCoarseTimer);
    m_checkSerialPortsTimer->setInterval(2000);
    m_checkSerialPortsTimer->setSingleShot(false);
    m_checkSerialPortsTimer->start();
}

void ConnectionWidget::CreateUI()
{
    m_mainLayout=new QVBoxLayout();
    m_adressAndPortLayout=new QHBoxLayout();
    m_connectionTypeComboBox=new QComboBox();
    m_adressAndPortLabel=new QLabel();
    m_adressLineEdit=new QLineEdit();
    m_portLineEdit=new QLineEdit();
    m_comPortNameComboBox=new QComboBox();

    m_buttonsLayout=new QHBoxLayout();
    m_connectButton=new QPushButton();
    m_disconnectButton=new QPushButton();

}

void ConnectionWidget::InsertWidgetsIntoLayout()
{
    m_adressAndPortLayout->addWidget(m_connectionTypeComboBox);
    m_adressAndPortLayout->addWidget(m_comPortNameComboBox);
    m_adressAndPortLayout->addWidget(m_adressAndPortLabel);
    m_adressAndPortLayout->addWidget(m_adressLineEdit);
    m_adressAndPortLayout->addWidget(m_portLineEdit);


    m_buttonsLayout->addWidget(m_connectButton);
    m_buttonsLayout->addWidget(m_disconnectButton);

    m_mainLayout->addLayout(m_adressAndPortLayout);
    m_mainLayout->addLayout(m_buttonsLayout);

    setLayout(m_mainLayout);
}

void ConnectionWidget::ConnectObjects()
{
    connect(m_connectionTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),  this, &ConnectionWidget::OnCurrentIndexConnectionTypeComboBoxChanged);
    connect(m_connectButton, &QPushButton::clicked, this, &ConnectionWidget::OnConnectButtonClicked);
    connect(m_disconnectButton, &QPushButton::clicked, this, &ConnectionWidget::ToDisconnectFromMoxa);
    connect(m_checkSerialPortsTimer, &QTimer::timeout, this, &ConnectionWidget::OnNewSerialPortsChecked);
    connect(m_portLineEdit, &QLineEdit::textChanged, this, &ConnectionWidget::OnPortLineEditChange);
    connect(m_adressLineEdit, &QLineEdit::textChanged, this, &ConnectionWidget::OnAdressLineEditChange);
}

void ConnectionWidget::FillUI()
{
    m_connectionTypeComboBox->addItem(QStringLiteral("Ethernet Moxa NPort-5450I"));
    m_connectionTypeComboBox->addItem(QStringLiteral("Usb Moxa UPort 1150. Выбери порт"));
    m_connectionTypeComboBox->setEditable(false);
    m_adressAndPortLabel->setText(QStringLiteral("Введите адрес и порт подключения"));
    m_adressLineEdit->setInputMask(QStringLiteral("000.000.000.000;_"));
    m_portLineEdit->setInputMask(QStringLiteral("00000;_"));

    m_adressLineEdit->setText(m_settingFileService->GetAttribute("connection", "ip", "192.168.127.111"));
    m_portLineEdit->setText(m_settingFileService->GetAttribute("connection", "port", "4004"));

    m_connectButton->setText(QStringLiteral("Подключится"));
    m_disconnectButton->setText(QStringLiteral("Отключится"));
    m_disconnectButton->setDisabled(true);


    m_comPortNameComboBox->setDisabled(true);
    m_comPortNameComboBox->setMinimumWidth(300);
}

void ConnectionWidget::OnCurrentIndexConnectionTypeComboBoxChanged(int index)
{
    if (index==0)
    {
        m_adressLineEdit->setEnabled(true);
        m_portLineEdit->setEnabled(true);
        m_comPortNameComboBox->setDisabled(true);
    }
    else
    {
        m_adressLineEdit->setDisabled(true);
        m_portLineEdit->setDisabled(true);
        m_comPortNameComboBox->setEnabled(true);
    }
}

void ConnectionWidget::OnAdressLineEditChange(const QString &newAdress)
{
    m_settingFileService->SetAttribute("connection", "ip", newAdress);
}

void ConnectionWidget::OnPortLineEditChange(const QString &newPort)
{
    m_settingFileService->SetAttribute("connection", "port", newPort);
}

void ConnectionWidget::OnConnectButtonClicked()
{
    int index=m_connectionTypeComboBox->currentIndex();
    if (0==index)
    {
        Q_EMIT ToConnectEthernetMoxa(m_adressLineEdit->text(), m_portLineEdit->text());
    }
    else
    {
        QString comPortName=m_comPortNameComboBox->currentText();
        comPortName=comPortName.left(m_comPortNameComboBox->currentText().indexOf(' '));
        if (IsCurrentComPortBisy(comPortName))
        {
            Q_EMIT ToConsoleLog("Com порт " +comPortName+ " не доступен для подключения, выберите другой");
        }
        else
        {
            Q_EMIT ToConnectUsbMoxa(comPortName);
        }
    }
}

void ConnectionWidget::OnNewSerialPortsChecked()
{
    QList<QSerialPortInfo> list(QSerialPortInfo::availablePorts());
    if (m_comPortNameComboBox->count()!=list.count())
    {
        m_comPortNameComboBox->clear();
        for (QList<QSerialPortInfo>::const_iterator it=list.cbegin(); it!=list.cend(); ++it)
        {
            QString portState=IsComPortBisy(&(*it)) ? QStringLiteral(" Занят") : QStringLiteral(" Свободен");
            m_comPortNameComboBox->addItem(it->portName()+ " " + it->description()+ " " + it->manufacturer() + portState);
        }
    }
}

void ConnectionWidget::SetButtonsEnabled(bool state)
{
    m_connectButton->setDisabled(state);
    m_disconnectButton->setEnabled(state);
}

bool ConnectionWidget::IsCurrentComPortBisy(QString &comPortName)
{
    QList<QSerialPortInfo> list(QSerialPortInfo::availablePorts());
    for (QList<QSerialPortInfo>::const_iterator iterator=list.cbegin(); iterator!=list.cend(); ++iterator)
    {
        if(iterator->portName()==comPortName)
        {
            return IsComPortBisy(&(*iterator));
        }
    }
    return true;
}

bool ConnectionWidget::IsComPortBisy(const QSerialPortInfo *info)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return info->isBusy();
#else
    return info->isNull();
#endif
}
