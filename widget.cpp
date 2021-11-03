#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , m_socket(new TcpSocket(this))
{
    createUI();
    insertWidgetsIntoLayout();
    createConnections();
    setWindowTitle("Настройка Юстировочного оборудования блок М14ХЛ2 Плата СЮИТ.687263.035");
}

Widget::~Widget()
{
    delete m_mainLayout;
    delete m_connectionWidget;
    delete m_commangWidget;
}

void Widget::ConnectToMoxa(const QString &adress, const QString &port)
{
    m_socket->connectTo(adress, port);
    m_commangWidget->ConsoleLog("Попытка подключится... Адрес: " + adress  + " Порт: " + port);
}

void Widget::DisconnectFromMoxa()
{
    m_socket->disconnect();
    m_commangWidget->ConsoleLog("Отключились...");
    m_connectionWidget->SetButtonsEnabled(false);
}

void Widget::createUI()
{
    m_mainLayout=new QVBoxLayout();

    m_connectionWidget =new ConnectionWidget(this);
    m_commangWidget=new StateWidget(m_socket, this);
    m_firmwareWidget=new FirmWareWidget(m_socket, this);

    m_tabWidget =new QTabWidget();
    m_tabWidget->addTab(m_commangWidget, "Окно команд");
    m_tabWidget->addTab(m_firmwareWidget, "Окно прошивки микросхемы памяти MX25L12835F");
    m_tabWidget->addTab(new QWidget(this), "Окно прошивки");
}

void Widget::insertWidgetsIntoLayout()
{
    m_mainLayout->addWidget(m_connectionWidget);
    m_mainLayout->addWidget(m_tabWidget);
    setLayout(m_mainLayout);

}

void Widget::createConnections()
{
    connect(m_connectionWidget, &ConnectionWidget::ConnectToMoxa, this, &Widget::ConnectToMoxa );
    connect(m_connectionWidget, &ConnectionWidget::DisconnectFromMoxa,this, &Widget::DisconnectFromMoxa);
    connect(m_socket, &TcpSocket::SetButtonsEnabled, m_connectionWidget, &ConnectionWidget::SetButtonsEnabled);
}

bool Widget::allRequedFiledsHave(QStringList &listOfFields)
{
    for(QStringList::iterator it=listOfFields.begin(); it!=listOfFields.end(); ++it)
    {
        if (it->isEmpty())
            return false;
    }
    return true;
}



