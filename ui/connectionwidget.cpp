#include "connectionwidget.h"

ConnectionWidget::ConnectionWidget(QWidget *parent)
    : QWidget(parent)
{
    CreateUI();
    InsertWidgetsIntoLayout();
    CreateConnections();
}

ConnectionWidget::~ConnectionWidget()
{
    delete m_adressAndPortLayout;
    delete m_buttonsLayout;
    delete m_mainLayout;

    delete m_adressAndPortLabel;
    delete m_adressLineEdit;
    delete m_portLineEdit;
    delete m_connectButton;
    delete m_disconnectButton;
}

void ConnectionWidget::SetButtonsEnabled(bool state)
{
    m_connectButton->setDisabled(state);
    m_disconnectButton->setEnabled(state);
}

void ConnectionWidget::CreateUI()
{
    m_mainLayout=new QVBoxLayout();
    m_adressAndPortLayout=new QHBoxLayout();
    m_adressAndPortLabel=new QLabel("Введите адрес и порт подключения");
    m_adressLineEdit=new QLineEdit();
    m_adressLineEdit->setInputMask(QStringLiteral("000.000.000.000;_"));
    m_portLineEdit=new QLineEdit();
    m_portLineEdit->setInputMask("00000;_");
    m_adressLineEdit->setText("192.168.127.254");
    m_portLineEdit->setText("4004");

    m_buttonsLayout=new QHBoxLayout();
    m_connectButton=new QPushButton("Подключится");
    m_disconnectButton=new QPushButton("Отключится");
    m_disconnectButton->setDisabled(true);
}

void ConnectionWidget::InsertWidgetsIntoLayout()
{
    m_adressAndPortLayout->addWidget(m_adressAndPortLabel);
    m_adressAndPortLayout->addWidget(m_adressLineEdit);
    m_adressAndPortLayout->addWidget(m_portLineEdit);

    m_buttonsLayout->addWidget(m_connectButton);
    m_buttonsLayout->addWidget(m_disconnectButton);

    m_mainLayout->addLayout(m_adressAndPortLayout);
    m_mainLayout->addLayout(m_buttonsLayout);

    setLayout(m_mainLayout);
}

void ConnectionWidget::CreateConnections()
{
    connect(m_connectButton, &QPushButton::clicked, [&](){Q_EMIT ConnectToMoxa(m_adressLineEdit->text(), m_portLineEdit->text());});
    connect(m_disconnectButton, &QPushButton::clicked, this, &ConnectionWidget::DisconnectFromMoxa);

}
