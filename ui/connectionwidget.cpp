#include "connectionwidget.h"
#include <QDebug>
#include <QSerialPortInfo>

ConnectionWidget::ConnectionWidget(QWidget *parent)
    : QWidget(parent)
{
    CreateUI();
    InsertWidgetsIntoLayout();
    FillUI();
    ConnectObjects();
}

ConnectionWidget::~ConnectionWidget()
{
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
    m_adressAndPortLayout->addWidget(m_adressAndPortLabel);
    m_adressAndPortLayout->addWidget(m_adressLineEdit);
    m_adressAndPortLayout->addWidget(m_portLineEdit);
    m_adressAndPortLayout->addWidget(m_comPortNameComboBox);

    m_buttonsLayout->addWidget(m_connectButton);
    m_buttonsLayout->addWidget(m_disconnectButton);

    m_mainLayout->addLayout(m_adressAndPortLayout);
    m_mainLayout->addLayout(m_buttonsLayout);

    setLayout(m_mainLayout);
}

void ConnectionWidget::ConnectObjects()
{
    connect(m_connectionTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),  this, &ConnectionWidget::WhenCurrentIndexConnectionTypeComboBoxChanged);
    connect(m_connectButton, &QPushButton::clicked, this, &ConnectionWidget::WhenConnectButtonClicked);
    connect(m_disconnectButton, &QPushButton::clicked, this, &ConnectionWidget::ToDisconnectFromMoxa);
}

void ConnectionWidget::FillUI()
{
    m_connectionTypeComboBox->addItem(QStringLiteral("Ethernet Moxa NPort-5450I"));
    m_connectionTypeComboBox->addItem(QStringLiteral("Usb Moxa UPort 1150"));
    m_connectionTypeComboBox->setEditable(false);
    m_adressAndPortLabel->setText(QStringLiteral("Введите адрес и порт подключения"));
    m_adressLineEdit->setInputMask(QStringLiteral("000.000.000.000;_"));
    m_portLineEdit->setInputMask(QStringLiteral("00000;_"));
    m_adressLineEdit->setText(QStringLiteral("192.168.127.254"));
    m_portLineEdit->setText(QStringLiteral("4004"));
    m_connectButton->setText(QStringLiteral("Подключится"));
    m_disconnectButton->setText(QStringLiteral("Отключится"));
    m_disconnectButton->setDisabled(true);

    QList<QSerialPortInfo> list(QSerialPortInfo::availablePorts());
    for (QList<QSerialPortInfo>::const_iterator it=list.cbegin(); it!=list.cend(); ++it)
    {
        QString portState=it->isBusy() ? " Занят" : " Свободен";
        m_comPortNameComboBox->addItem(it->portName()+ " " + it->description()+ " " + it->manufacturer() + portState);
    }
    m_comPortNameComboBox->setDisabled(true);
}

void ConnectionWidget::WhenCurrentIndexConnectionTypeComboBoxChanged(int index)
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

void ConnectionWidget::WhenConnectButtonClicked()
{
    int connectionType=m_connectionTypeComboBox->currentIndex();
    if (connectionType==0)
    {
        Q_EMIT ToConnectEthernetMoxa(m_adressLineEdit->text(), m_portLineEdit->text());
    }
    else
    {
        QString comPortName=m_comPortNameComboBox->currentText();
        comPortName=comPortName.left(m_comPortNameComboBox->currentText().indexOf(' '));
        if (IsComPortValid(comPortName))
        {
            Q_EMIT ToConnectUsbMoxa(comPortName);
        }
        else
        {
            Q_EMIT ToConsoleLog("Com порт " +comPortName+ " не доступен для подключения, выберите другой");
        }
    }
}

void ConnectionWidget::SetButtonsEnabled(bool state)
{
    m_connectButton->setDisabled(state);
    m_disconnectButton->setEnabled(state);
}

bool ConnectionWidget::IsComPortValid(QString &comPortName)
{
    QList<QSerialPortInfo> list(QSerialPortInfo::availablePorts());
    for (QList<QSerialPortInfo>::const_iterator iterator=list.cbegin(); iterator!=list.cend(); ++iterator)
    {
        qDebug()<< iterator->portName() << " " << iterator->isValid();
        if(iterator->portName()==comPortName)
        {

            return iterator->isValid();
        }
    }
}
