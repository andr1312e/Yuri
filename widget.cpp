#include "widget.h"

#include <QDateTime>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , m_attenuatorValues({"0", "1", "2", "3", "4", "5", "6", "9", "12", "15", "18", "21", "24", "27", "30"})
    , m_noiseValues({"шумогенератор ВЫКЛЮЧЕН, ответчик ВЫКЛЮЧЕН", "шумогенератор ВЫКЛЮЧЕН, ответчик ВКЛЮЧЕН", "шумогенератор ВКЛЮЧЕН, ответчик ВЫКЛЮЧЕН"})
    , m_socket(new TcpSocket(this))
    , m_intValidator(new QIntValidator(0, 2147483647))
    , m_gainValidator(new QIntValidator(0, 100))
{
    long long minWorkPointValue=2695;
    for (int i=0; i<30; i++)
    {
        workPointsValues.append(QString::number(minWorkPointValue));
        minWorkPointValue=minWorkPointValue+10;
    }
    createUI();
    insertWidgetsIntoLayout();
    createConnections();
}

Widget::~Widget()
{
    delete m_socket;
    delete m_intValidator;
    delete m_gainValidator;

    delete m_adressAndPortLayout;
    delete m_buttonsLayout;
    delete m_speedLayout;
    delete m_rangeLineLayout;
    delete m_workPointLineLayout;
    delete m_gainLineLayout;
    delete m_attenuatorLineLayout;
    delete m_messageSendButtonsLayout;
    delete m_stateLayout;
    delete m_noiseLineLayout;
    delete m_mainLayout;

    delete m_adressAndPortLabel;
    delete m_adressLineEdit;
    delete m_portLineEdit;

    delete m_connectButton;
    delete m_disconnectButton;

    delete m_DoplerFreqLabel;
    delete m_DoplerFreqLineEdit;

    delete m_speedLabel;
    delete m_speedLineEdit;

    delete m_rangeLabel;
    delete m_rangeLineEdit;

    delete m_fvcoLabel;
    delete m_fvcoComboBox;

    delete m_gainTXLabel;
    delete m_gainTXLineEdit;
    delete m_gainRXLabel;
    delete m_gainRXLineEdit;

    delete m_attenuatorLabel;
    delete m_attenuatorComboBox;

    delete m_noiseComboBox;
    delete m_noiseLabel;

    delete m_firstMessageSendButton;
    delete m_secondMessageSendButton;
    delete m_thirdMessageSendButton;
    delete m_forthMessageSendButton;
    delete m_fiveMessageSendButton;
    delete m_sixMessageSendButton;
    delete m_sevenMessageSendButton;

    delete m_state;
    delete m_logClearButton;

    delete m_log;
}

void Widget::setButtonState(bool state)
{
    m_zeroMessageSendButton->setEnabled(state);
    m_firstMessageSendButton->setEnabled(state);
    m_secondMessageSendButton->setEnabled(state);
    m_thirdMessageSendButton->setEnabled(state);
    m_forthMessageSendButton->setEnabled(state);
    m_fiveMessageSendButton->setEnabled(state);
    m_sixMessageSendButton->setEnabled(state);
    m_sevenMessageSendButton->setEnabled(state);
    m_disconnectButton->setEnabled(state);
    m_attenuatorComboBox->setEnabled(state);
    m_noiseComboBox->setEnabled(state);
    m_connectButton->setDisabled(state);
}

void Widget::tryToConnect()
{
    QString time=QDateTime::currentDateTime().toString("hh:mm:ss");
    QString port=m_portLineEdit->text();
    QString ipAdress=m_adressLineEdit->text();
    m_log->appendPlainText(time + " Попытка подключится... Адрес: " + ipAdress  + " Порт: " + port);
    m_socket->connectTo(ipAdress, port);
}

void Widget::changeSpeedFiled()
{
    double dopler=m_DoplerFreqLineEdit->text().toDouble();
    double speed=dopler*c/(2.0*m_fvcoComboBox->currentText().toDouble()*1000000.0);
    m_speedLineEdit->setText(QString::number(speed, 'f'));
}

void Widget::changeDoplerFiled(const QString &value)
{
    double speed=value.toDouble();
    double dopler1=speed*m_fvcoComboBox->currentText().toDouble()*1000000.0*2.0;
    double dopler=dopler1/c;
    m_DoplerFreqLineEdit->setText(QString::number(dopler, 'f'));
}

void Widget::sendZeroMessage()
{
    m_log->appendPlainText("Высылаем сообщение пинга");
    m_socket->createMessages(0, 0, 0);
}

void Widget::sendFirstMessage()
{
    QStringList params;
    params<<m_fvcoComboBox->currentText();
    if (allRequedFiledsHave(params))
    {
        bool isOk;
        double fvcoMegaHertz=params.first().toDouble(&isOk);
        if (isOk)
        {
            double fvcoHertz=fvcoMegaHertz*1000000;
            m_log->appendPlainText("Высылаем первое сообщение установка частоты RX fvco= " +QString::number(fvcoMegaHertz, 'f'));
            m_socket->createMessages(1, fvcoHertz, 0);
        }
        else
        {
            m_log->appendPlainText("Не смогли перевесли в число " + m_fvcoComboBox->currentText());
        }
    }
    else
    {
        m_log->appendPlainText("Заполните пожалуйста поля: Рабочая точка Fvco: для частот Tx и Rx");
    }
}

void Widget::sendSecondMessage()
{
    QStringList params;
    params<<m_fvcoComboBox->currentText();
    params<<m_DoplerFreqLineEdit->text();
    if (allRequedFiledsHave(params))
    {
        bool isOk1, isOk2;
        double fvcoMegaHertz=params.first().toDouble(&isOk1);
        double dopler=params.last().toDouble(&isOk2);
        if (isOk1)
        {
            double fvcoHertz=fvcoMegaHertz*1000000;
            if (isOk2)
            {

                m_log->appendPlainText("Высылаем второе сообщение установка частоты Tx fvco= " +QString::number(fvcoMegaHertz, 'f') + " МГЦ и доплер= "+ QString::number(dopler, 'f')+ " ГЦ");
                m_socket->createMessages(2, fvcoHertz, dopler);
            }
            else
            {
                m_log->appendPlainText("Не смогли перевесли в число " + m_DoplerFreqLineEdit->text());
                m_log->appendPlainText("Высылаем сообщение fvco= "+QString::number(fvcoMegaHertz, 'f') + " МГЦ доплер= 0 ГЦ");
                m_socket->createMessages(2, fvcoHertz, 0);
            }

        }
        else
        {
            m_log->appendPlainText("Не смогли перевесли в число " + m_fvcoComboBox->currentText());
        }
    }
    else
    {
        m_log->appendPlainText("Заполните пожалуйста поля: Рабочая точка Fvco: для частот Tx и Rx");
    }
}

void Widget::sendThirdMessage()
{
    QStringList params;
    params<<m_rangeLineEdit->text();
    if (allRequedFiledsHave(params))
    {
        bool isOk;
        double range=params.first().toDouble(&isOk);
        if (isOk)
        {
            m_log->appendPlainText("Высылаем 3 сообщение установка дальности ответного сигнала. Дистанция" + QString::number(range, 'f') );
            m_socket->createMessages(3, range, 0);
        }
        else
        {
            m_log->appendPlainText(" не смогли перевесли в число " + m_rangeLineEdit->text());
        }
    }
    else
    {
        m_log->appendPlainText("Заполните пожалуйста поля: Дальность ответного сигнала: d");
    }
}

void Widget::sendFourthMessage()
{
    QStringList params;
    params<<m_gainTXLineEdit->text();
    params<<m_gainRXLineEdit->text();
    if (allRequedFiledsHave(params))
    {
        bool isOk1, isOk2;
        double gainTX=params.last().toDouble(&isOk1);
        double gainRX=params.first().toDouble(&isOk2);
        if (isOk1&&isOk2)
        {
            m_log->appendPlainText("Высылаем 4 сообщение установка усиления");
            m_socket->createMessages(4, gainTX, gainRX);
        }
        else
        {
            m_log->appendPlainText(" не смогли перевесли в число " + m_gainRXLineEdit->text() + " или  "+ m_gainTXLineEdit->text());
        }
    }
    else
    {
        m_log->appendPlainText("Заполните пожалуйста поля: Усиление RX а так же Усиление TX");
    }
}

void Widget::sendFiveMessage()
{
    QStringList params;
    params<<m_attenuatorComboBox->currentText();
    bool isOk;
    double AttenuatorValue=params.first().toDouble(&isOk);
    if (isOk)
    {
        m_log->appendPlainText("Высылаем сообщение");
        m_socket->createMessages(5, AttenuatorValue, 0);
    }
    else
    {
        m_log->appendPlainText(" не смогли перевесли в число " + m_attenuatorComboBox->currentText());
    }
}

void Widget::sendSixMessage()
{
    int noiseValue=m_noiseComboBox->currentIndex();
    m_log->appendPlainText("Высылаем сообщение");
    m_socket->createMessages(6, noiseValue, 0);
}

void Widget::sendSevenMessage()
{
    m_log->appendPlainText("Высылаем сообщение");
    m_socket->createMessages(7, 0, 0);
}

void Widget::createUI()
{
    m_mainLayout=new QVBoxLayout();
    m_adressAndPortLayout=new QHBoxLayout();
    m_buttonsLayout=new QHBoxLayout();
    m_speedLayout=new QHBoxLayout();
    m_rangeLineLayout=new QHBoxLayout();
    m_workPointLineLayout=new QHBoxLayout();
    m_gainLineLayout=new QHBoxLayout();
    m_attenuatorLineLayout=new QHBoxLayout();
    m_messageSendButtonsLayout=new QHBoxLayout();
    m_stateLayout=new QHBoxLayout();
    m_noiseLineLayout=new QHBoxLayout();


    m_adressAndPortLabel=new QLabel("Введите адрес и порт подлючения");
    m_adressLineEdit=new QLineEdit();
    m_adressLineEdit->setInputMask(QStringLiteral("000.000.000.000;_"));
    m_portLineEdit=new QLineEdit();
    m_portLineEdit->setInputMask("00000;_");
    m_adressLineEdit->setText("192.168.111.254");
    m_portLineEdit->setText("4004");

    m_connectButton=new QPushButton("Подключится");
    m_disconnectButton=new QPushButton("Отключится");
    m_disconnectButton->setDisabled(true);

    m_fvcoLabel=new QLabel("Рабочая точка Fvco для частот Tx и Rx: МЕГАгерцы");
    m_fvcoComboBox=new QComboBox();
    m_fvcoComboBox->setEditable(true);
    m_fvcoComboBox->addItems(workPointsValues);

    m_DoplerFreqLabel=new QLabel("Частота Доплера (по желанию) для частоты Tx: ГЕРЦЫ");
    m_DoplerFreqLineEdit=new QLineEdit();
    m_DoplerFreqLineEdit->setValidator(m_intValidator);
    m_DoplerFreqLineEdit->setText("0");

    m_speedLabel=new QLabel("Скорость может быть <0: метры/секунду ");
    m_speedLineEdit=new QLineEdit();
    m_speedLineEdit->setValidator(m_intValidator);

    m_rangeLabel=new QLabel("Дальность ответного сигнала d: метры");
    m_rangeLineEdit=new QLineEdit();
    m_rangeLineEdit->setValidator(m_intValidator);

    m_gainTXLabel=new QLabel("Усиление TX GAIN_TX: децибелы");
    m_gainTXLineEdit=new QLineEdit();
    m_gainTXLineEdit->setValidator(m_gainValidator);

    m_gainRXLabel=new QLabel("Усиление RX GAIN_RX: децибелы");
    m_gainRXLineEdit=new QLineEdit();
    m_gainRXLineEdit->setValidator(m_gainValidator);

    m_attenuatorLabel=new QLabel("Установка ослабления Attenuator_RX: децибелы");
    m_attenuatorComboBox=new QComboBox();
    m_attenuatorComboBox->addItems(m_attenuatorValues);

    m_noiseLabel=new QLabel("Генератор шума");
    m_noiseComboBox=new QComboBox();
    m_noiseComboBox->addItems(m_noiseValues);
    m_noiseComboBox->setEditable(false);

    m_zeroMessageSendButton=new QPushButton("Пинг");
    m_firstMessageSendButton=new QPushButton("установка частоты Rx");
    m_secondMessageSendButton=new QPushButton("установка частоты Tx");
    m_thirdMessageSendButton=new QPushButton("установка дальности ответного сигнала");
    m_forthMessageSendButton=new QPushButton("установка усиления Rx Tx");
    m_fiveMessageSendButton=new QPushButton("Установка ослабления");
    m_sixMessageSendButton=new QPushButton("Генератор шума");
    m_sevenMessageSendButton=new QPushButton("Получаем сообщения");

    m_state=new QLabel("Адрес: адрес мохи с 1 в конце или 192.168.111.1 на РЛС ТИ. Порт: через  ЛК Operation Settings. Посмотреть http://192.168.127.254/ снизу мохи адрес Лог:Пасс admin:moxa");
    m_logClearButton=new QPushButton("Консоль отчистить");

    m_log=new QPlainTextEdit();
    m_log->setReadOnly(true);
    m_log->appendPlainText("Не подлючено к ответчику");

        setButtonState(false);
    setWindowTitle("Настройка Юстировочного оборудования блок М14ХЛ2 Плата СЮИТ.687263.035");
}

void Widget::insertWidgetsIntoLayout()
{
    m_adressAndPortLayout->addWidget(m_adressAndPortLabel);
    m_adressAndPortLayout->addWidget(m_adressLineEdit);
    m_adressAndPortLayout->addWidget(m_portLineEdit);

    m_buttonsLayout->addWidget(m_connectButton);
    m_buttonsLayout->addWidget(m_disconnectButton);

    m_speedLayout->addWidget(m_DoplerFreqLabel);
    m_speedLayout->addWidget(m_DoplerFreqLineEdit);
    m_speedLayout->addWidget(m_speedLabel);
    m_speedLayout->addWidget(m_speedLineEdit);

    m_rangeLineLayout->addWidget(m_rangeLabel);
    m_rangeLineLayout->addWidget(m_rangeLineEdit);

    m_workPointLineLayout->addWidget(m_fvcoLabel);
    m_workPointLineLayout->addWidget(m_fvcoComboBox);

    m_gainLineLayout->addWidget(m_gainTXLabel);
    m_gainLineLayout->addWidget(m_gainTXLineEdit);
    m_gainLineLayout->addWidget(m_gainRXLabel);
    m_gainLineLayout->addWidget(m_gainRXLineEdit);

    m_attenuatorLineLayout->addWidget(m_attenuatorLabel);
    m_attenuatorLineLayout->addWidget(m_attenuatorComboBox);

    m_noiseLineLayout->addWidget(m_noiseLabel);
    m_noiseLineLayout->addWidget(m_noiseComboBox);

    m_messageSendButtonsLayout->addWidget(m_zeroMessageSendButton);
    m_messageSendButtonsLayout->addWidget(m_firstMessageSendButton);
    m_messageSendButtonsLayout->addWidget(m_secondMessageSendButton);
    m_messageSendButtonsLayout->addWidget(m_thirdMessageSendButton);
    m_messageSendButtonsLayout->addWidget(m_forthMessageSendButton);
    m_messageSendButtonsLayout->addWidget(m_fiveMessageSendButton);
    m_messageSendButtonsLayout->addWidget(m_sixMessageSendButton);
    m_messageSendButtonsLayout->addWidget(m_sevenMessageSendButton);

    m_mainLayout->addLayout(m_adressAndPortLayout);
    m_mainLayout->addLayout(m_buttonsLayout);
    m_mainLayout->addLayout(m_workPointLineLayout);
    m_mainLayout->addLayout(m_speedLayout);
    m_mainLayout->addLayout(m_rangeLineLayout);
    m_mainLayout->addLayout(m_gainLineLayout);
    m_mainLayout->addLayout(m_attenuatorLineLayout);
    m_mainLayout->addLayout(m_noiseLineLayout);
    m_mainLayout->addLayout(m_messageSendButtonsLayout);

    m_mainLayout->addWidget(m_state);
    m_mainLayout->addWidget(m_logClearButton);
    m_mainLayout->addWidget(m_log);
    setLayout(m_mainLayout);
}

void Widget::createConnections()
{
    connect(m_connectButton, &QPushButton::clicked, this, &Widget::tryToConnect);
    connect(m_socket, &TcpSocket::sendMessageToLog,  [=](const QString &message ) {
        QString time=QDateTime::currentDateTime().toString("hh:mm:ss");
        m_log->appendPlainText(time+ " "+  message);});
    connect(m_socket, &TcpSocket::setState, [=](const QString &state){m_state->setText(state);});
    connect(m_zeroMessageSendButton, &QPushButton::clicked, this, &Widget::sendZeroMessage);
    connect(m_firstMessageSendButton, &QPushButton::clicked, this, &Widget::sendFirstMessage);
    connect(m_secondMessageSendButton, &QPushButton::clicked, this, &Widget::sendSecondMessage);
    connect(m_thirdMessageSendButton, &QPushButton::clicked, this, &Widget::sendThirdMessage);
    connect(m_forthMessageSendButton, &QPushButton::clicked, this, &Widget::sendFourthMessage);
    connect(m_fiveMessageSendButton, &QPushButton::clicked, this, &Widget::sendFiveMessage);
    connect(m_sixMessageSendButton, &QPushButton::clicked, this, &Widget::sendSixMessage);
    connect(m_sevenMessageSendButton, &QPushButton::clicked, this, &Widget::sendSevenMessage);

    connect(m_socket, &TcpSocket::setButtonsEnabled, this, &Widget::setButtonState);
    connect(m_logClearButton, &QPushButton::clicked, m_log, &QPlainTextEdit::clear);
    connect(m_disconnectButton, &QPushButton::clicked, m_socket, &TcpSocket::disconnect);
    connect(m_DoplerFreqLineEdit, &QLineEdit::textEdited, this, &Widget::changeSpeedFiled);
    connect(m_speedLineEdit, &QLineEdit::textEdited, this, &Widget::changeDoplerFiled);
    connect(m_fvcoComboBox, &QComboBox::editTextChanged, this, &Widget::changeSpeedFiled);
}

bool Widget::allRequedFiledsHave(QStringList &listOfFields)
{
    for(QStringList::iterator it=listOfFields.begin(); it!=listOfFields.end(); ++it)
    {
        if (*it=="")
            return false;
    }
    return true;
}

