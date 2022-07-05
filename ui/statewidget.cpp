#include "statewidget.h"
#include "qdatetime.h"
#include <QDebug>
#include <QRegularExpression>
#include <QTimer>

StateWidget::StateWidget(SettingFileService *settingFileService, QWidget *parent)
    : QWidget(parent)
    , m_settingFileService(settingFileService)
{
    CreateObjects();
    CreateUI();
    InsertWidgetsIntoLayout();
    FillUI();
    ConnectObjects();
}

StateWidget::~StateWidget()
{
    m_file.close();
    delete m_statePresenter;

    delete m_modesLayout;
    delete m_workModeLayout;
    delete m_rawHexLayout;

    delete m_mainLayout;

    delete m_dataWidget;
    delete m_bPraWidget;

    delete m_workModeLabel;
    delete m_workModeComboBox;
    delete m_getCurrentWorkMode;
    delete m_pingButton;
    delete m_restartDevice;


    delete m_rawHexInputEdit;
    delete m_sendRawHexButton;

    delete m_logClearButton;
    delete m_log;
}

void StateWidget::DisconnectOldHander()
{
    m_statePresenter->DisconnectOldHandler();
}

void StateWidget::ConnectHander(DataHandler *dataHandler)
{
    m_statePresenter->ConnectHander(dataHandler);
}

void StateWidget::CreateObjects()
{
    m_statePresenter = new StatePresenter(m_settingFileService, this);
    m_file.setFileName(QDir::currentPath() + "/история ввода.txt");
    m_file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
}

void StateWidget::CreateUI()
{
    m_mainLayout = new QVBoxLayout();

    m_modesLayout = new QHBoxLayout();

    m_dataWidget = new DataWidget(m_settingFileService, this);
    m_bPraWidget = new BParWidget(m_statePresenter, this);

    m_workModeLayout = new QHBoxLayout();
    m_workModeLabel = new QLabel();
    m_workModeComboBox = new QComboBox();
    m_getCurrentWorkMode = new QPushButton();
    m_pingButton = new QPushButton();
    m_restartDevice = new QPushButton();

    m_rawHexLayout = new QHBoxLayout();
    m_rawHexInputEdit = new QLineEdit();
    m_sendRawHexButton = new QPushButton();

    m_logClearButton = new QPushButton();
    m_log = new QPlainTextEdit();
}

void StateWidget::InsertWidgetsIntoLayout()
{
    m_modesLayout->addWidget(m_dataWidget);
    m_modesLayout->addWidget(m_bPraWidget);

    m_workModeLayout->addWidget(m_workModeLabel);
    m_workModeLayout->addWidget(m_workModeComboBox);
    m_workModeLayout->addWidget(m_getCurrentWorkMode);
    m_workModeLayout->addWidget(m_pingButton);
    m_workModeLayout->addWidget(m_restartDevice);

    m_rawHexLayout->addWidget(m_rawHexInputEdit);
    m_rawHexLayout->addWidget(m_sendRawHexButton);

    m_mainLayout->addLayout(m_modesLayout);
    m_mainLayout->addLayout(m_workModeLayout);
    m_mainLayout->addLayout(m_rawHexLayout);
    m_mainLayout->addWidget(m_logClearButton);
    m_mainLayout->addWidget(m_log);

    setLayout(m_mainLayout);
}

void StateWidget::FillUI()
{
    m_workModeLabel->setText(QStringLiteral("Режимы работы:"));

    m_workModeComboBox->addItems(noiseValues);
    m_workModeComboBox->setEditable(false);
    m_getCurrentWorkMode->setText(QStringLiteral("Получить режим работы"));
    m_pingButton->setText(QStringLiteral("Пинг"));
    m_restartDevice->setText(QStringLiteral("Перезагрузить устройство"));
    m_rawHexInputEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("^[0-9a-fA-F]*"), this));
    m_sendRawHexButton->setText(QStringLiteral("Отправить hex сообщение"));
    m_logClearButton->setText(QStringLiteral("Консоль отчистить"));
    m_log->setReadOnly(true);
    m_log->appendPlainText(QStringLiteral("Не подключено к ответчику"));
    m_bPraWidget->setDisabled(true);

    const bool EnableUI = m_settingFileService->GetAttribute(m_settingsAtribute, "uiEnabled", "0").toInt();
}

void StateWidget::ConnectObjects()
{
    connect(m_statePresenter, &StatePresenter::ToConsoleLog, this, &StateWidget::OnConsoleLog);
    connect(m_statePresenter, &StatePresenter::ToSetButtonsEnabled, this, &StateWidget::OnSetButtonEnabled);

    connect(m_dataWidget, &DataWidget::ToSetState, this, &StateWidget::OnSetState);
    connect(m_dataWidget, &DataWidget::ToGetState, this, &StateWidget::OnGetState);
    connect(m_dataWidget, &DataWidget::ToConsoleLog, this, &StateWidget::OnConsoleLog);

    connect(m_workModeComboBox, qOverload<int>(&QComboBox::activated), this, &StateWidget::OnSetWorkMode);
    connect(m_getCurrentWorkMode, &QPushButton::clicked, [&]()
    {
        OnGetState(6);
    });
    connect(m_pingButton, &QPushButton::clicked, [&]()
    {
        m_statePresenter->ToSendMessageToDeivce(QByteArray(QByteArrayLiteral("\x00\x00")));
    });
    connect(m_restartDevice, &QPushButton::clicked, [&]()
    {
        m_statePresenter->ToSendMessageToDeivce(QByteArray(QByteArrayLiteral("\x0d\x5a")));
    });
    connect(m_sendRawHexButton, &QPushButton::clicked, this, QOverload<>::of(&StateWidget::OnSendRawHexMessage));
    connect(m_logClearButton, &QPushButton::clicked, m_log, &QPlainTextEdit::clear);
}

void StateWidget::OnSetState(int messageIdWantToSend, QList<double> params)
{
    if (params.isEmpty())
    {
        OnConsoleLog(" Неверное сообщение. Колво параметров должно быть больше 0");
    }
    else
    {
        if (params.count() == 2)
        {
            m_statePresenter->SetMessageToQueue(messageIdWantToSend, params.front(), params.back());
        }
        else
        {
            m_statePresenter->SetMessageToQueue(messageIdWantToSend, params.front());
        }
    }

}

void StateWidget::UpdateHistoryFile()
{
    const QString time = "\n" + QDateTime::currentDateTime().toString(QStringLiteral("hh:mm:ss"));
//    const int space = 5;
//    const QString fvco = m_fvcoComboBox->currentText().leftJustified(space, ' ');
//    const QString dopler = m_doplerFreqLineEdit->text().leftJustified(space, ' ');
//    const QString speed = m_speedLineEdit->text().leftJustified(space, ' ');
//    const QString range = m_rangeLineEdit->text().leftJustified(space, ' ');
//    const QString gaintTx = QString::number(m_gainTXSlider->GetCurrentDoubleRangeValue(), 'f', 3);
//    const QString gaintRx = QString::number(m_gainRXSlider->GetCurrentDoubleRangeValue(), 'f', 3);
//    const QString attenuator = m_attenuatorComboBox->currentText();
//    const QString noise = m_workModeComboBox->currentText();
//    const QString message = time + "|Fvco(МПЦ)= " + fvco + " |Доплер(ГЦ)= " + dopler + " |Скорость(М/С)= " + speed + " |Дистанция(М)= " + range + " |gainTx= " + gaintTx + " |gainRx= " + gaintRx + " |Ослабление= " + attenuator + " |" + noise;
//    m_file.write(message.toUtf8());
    m_file.flush();
}


void StateWidget::OnConsoleLog(const QString &message)
{
    const QString time = QDateTime::currentDateTime().toString(QStringLiteral("hh:mm:ss"));
    m_log->appendPlainText(time + " " + message);
}

void StateWidget::OnSetWorkMode(int index)
{
    if (index == m_workModeComboBox->count() - 1)
    {
        m_bPraWidget->setEnabled(true);
        m_dataWidget->setEnabled(false);
    }
    else
    {
        if (m_bPraWidget->isEnabled())
        {
            m_bPraWidget->setEnabled(false);
            m_dataWidget->setEnabled(true);
        }
        if (index >= 3)
        {
            bool isSinusValueParced;
            const double noiseValue = m_dataWidget->GetSinusValue().toDouble(&isSinusValueParced);
            if (isSinusValueParced)
            {
                OnConsoleLog("Высылаем шестое сообщение: установка шума, параметр равен: " + m_dataWidget->GetSinusValue());
                m_statePresenter->SetMessageToQueue(6, index, noiseValue);
            }
            else
            {
                OnConsoleLog("Шестое сообщение: не смогли перевесли в число: " + m_dataWidget->GetSinusValue() + ".Берем значение 0");
                m_statePresenter->SetMessageToQueue(6, index);
            }
        }
        else
        {
            m_statePresenter->SetMessageToQueue(6, index);
        }
        UpdateHistoryFile();
    }
}

void StateWidget::OnSendRawHexMessage()
{
    const QByteArray message = QByteArray::fromHex(m_rawHexInputEdit->text().toUtf8());
    m_statePresenter->ToSendMessageToDeivce(message);
}

void StateWidget::OnSendRawHexMessage(const QByteArray &array)
{
    m_statePresenter->ToSendMessageToDeivce(array);
}

void StateWidget::OnGetState(int id)
{
    switch (id)
    {
    case 1:
        OnConsoleLog(QStringLiteral("Запрашиваем рабочую точку Rх"));
        break;
    case 2:
        OnConsoleLog(QStringLiteral("Запрашиваем рабочую точку Tх"));
        break;
    case 3:
        OnConsoleLog(QStringLiteral("Запрашиваем дальность"));
        break;
    case 4:
        OnConsoleLog(QStringLiteral("Запрашиваем усиление"));
        break;
    case 5:
        OnConsoleLog(QStringLiteral("Запрашиваем ослабление"));
        break;
    case 6:
        OnConsoleLog(QStringLiteral("Запрашиваем режим работы"));
        break;
    case 9:
        OnConsoleLog(QStringLiteral("Запрашиваем доплера точку"));
        break;
    default:
        OnConsoleLog(QStringLiteral("Обработка команды не реализована"));
        return;
    }
    m_statePresenter->GetStateFromDevice(id);
}

void StateWidget::OnSetButtonEnabled(bool state)
{
    m_dataWidget->setEnabled(state);
    m_bPraWidget->setEnabled(state);

    m_workModeComboBox->setEnabled(state);
    m_getCurrentWorkMode->setEnabled(state);
    m_pingButton->setEnabled(state);
    m_restartDevice->setEnabled(state);
    m_sendRawHexButton->setEnabled(state);
}

