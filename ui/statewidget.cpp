#include "statewidget.h"
#include "qdatetime.h"
#include <QDebug>
#include <QRegularExpression>
#include <QTimer>

StateWidget::StateWidget(SettingFileService *settingFileService, QWidget *parent)
    : QWidget(parent)
    , m_settingFileService(settingFileService)
    , m_attenuatorValues{"0", "1", "2", "3", "4", "5", "6", "9", "12", "15", "18", "21", "24", "27", "30"}
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

    delete m_intValidator;
    delete m_gainValidator;

    delete m_speedLayout;
    delete m_rangeLineLayout;
    delete m_workPointLineLayout;
    delete m_gainLineLayout;
    delete m_attenuatorLineLayout;
    delete m_stateLayout;
    delete m_noiseLineLayout;
    delete m_mainLayout;


    delete m_DoplerFreqLabel;
    delete m_doplerFreqLineEdit;

    delete m_speedLabel;
    delete m_speedLineEdit;

    delete m_rangeLabel;
    delete m_rangeLineEdit;

    delete m_fvcoLabel;
    delete m_fvcoComboBox;

    delete m_gainTXLabel;
    delete m_gainTXSlider;
    delete m_gainTxValue;
    delete m_gainRXLabel;
    delete m_gainRXSlider;
    delete m_gainRxValue;

    delete m_attenuatorLabel;
    delete m_attenuatorComboBox;

    delete m_noiseComboBox;
    delete m_noiseLabel;

    qDeleteAll(m_sendStateButtonsGroup->buttons());
    delete m_sendStateButtonsGroup;
    delete m_getStateButtonGroup;

    delete m_logClearButton;
    delete m_restartDevice;

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
    m_file.setFileName(QDir::currentPath() + "/история запросов.txt");
    m_intValidator = new QIntValidator();
    m_gainValidator = new QIntValidator(0, 64);
    m_file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    int minWorkPointValue = 2695;
    for (int i = 0; i < 30; ++i)
    {
        workPointsValues.append(QString::number(minWorkPointValue));
        minWorkPointValue = minWorkPointValue + 10;
    }
}


void StateWidget::CreateUI()
{
    m_mainLayout = new QVBoxLayout();

    m_modesLayout = new QHBoxLayout();

    m_modesWidget = new QGroupBox();
    m_leftLayout = new QVBoxLayout();

    m_speedLayout = new QHBoxLayout();
    m_rangeLineLayout = new QHBoxLayout();
    m_workPointLineLayout = new QHBoxLayout();
    m_gainLineLayout = new QHBoxLayout();
    m_attenuatorLineLayout = new QHBoxLayout();
    m_stateLayout = new QHBoxLayout();
    m_noiseLineLayout = new QHBoxLayout();

    m_fvcoLabel = new QLabel();
    m_fvcoComboBox = new QComboBox();

    m_DoplerFreqLabel = new QLabel();
    m_doplerFreqLineEdit = new QLineEdit();

    m_speedLabel = new QLabel();
    m_speedLineEdit = new QLineEdit();

    m_rangeLabel = new QLabel();
    m_rangeLineEdit = new QLineEdit();


    m_gainTXLabel = new QLabel();
    m_gainTXSlider = new DoubleSlider(this);
    m_gainTxValue = new QLabel();


    m_gainRXLabel = new QLabel();
    m_gainRXSlider = new DoubleSlider(this);
    m_gainRxValue = new QLabel();


    m_attenuatorLabel = new QLabel();
    m_attenuatorComboBox = new QComboBox();



    m_noiseValueLabel = new QLabel();
    m_noiseLineEdit = new QLineEdit();

    m_deviceModeLayout = new QHBoxLayout();
    m_noiseLabel = new QLabel();
    m_noiseComboBox = new QComboBox();

    m_sendStateButtonsGroup = new QButtonGroup();
    FillButtonGroup();

    m_bPraWidget = new BParWidget(m_statePresenter, this);



    m_logClearButton = new QPushButton();
    m_restartDevice = new QPushButton();

    m_rawHexLayout = new QHBoxLayout();
    m_rawHexInputEdit = new QLineEdit();
    m_sendRawHexButton = new QPushButton();

    m_log = new QPlainTextEdit();
    //    m_log->setStyleSheet(QStringLiteral("color: white; background-color: black;"));
}

void StateWidget::FillButtonGroup()
{

    m_sendStateButtonsGroup->addButton(new QPushButton(QStringLiteral("Установить RX")), 1);
    m_sendStateButtonsGroup->addButton(new QPushButton(QStringLiteral("Установить TX")), 21);
    m_sendStateButtonsGroup->addButton(new QPushButton(QStringLiteral("Установить RX и ТХ")), 121);
    m_sendStateButtonsGroup->addButton(new QPushButton(QStringLiteral("Установить Доплер")), 22);
    m_sendStateButtonsGroup->addButton(new QPushButton(QStringLiteral("Установить")), 3);

    m_sendStateButtonsGroup->addButton(new QPushButton(QStringLiteral("Установить")), 4);
    m_sendStateButtonsGroup->addButton(new QPushButton(QStringLiteral("Установить")), 5);
    m_sendStateButtonsGroup->addButton(new QPushButton(QStringLiteral("Пинг")), 0);

    m_getStateButtonGroup = new QButtonGroup();
    m_getStateButtonGroup->addButton(new QPushButton(QStringLiteral("Запр.RX")), 1);
    m_getStateButtonGroup->addButton(new QPushButton(QStringLiteral("Запр.TX")), 21);
    m_getStateButtonGroup->addButton(new QPushButton(QStringLiteral("Запросить")), 22);
    m_getStateButtonGroup->addButton(new QPushButton(QStringLiteral("Запросить")), 3);
    m_getStateButtonGroup->addButton(new QPushButton(QStringLiteral("Запросить")), 4);
    m_getStateButtonGroup->addButton(new QPushButton(QStringLiteral("Запросить")), 5);
    m_getStateButtonGroup->addButton(new QPushButton(QStringLiteral("Запросить")), 6);
}

void StateWidget::InsertWidgetsIntoLayout()
{
    const QList<QAbstractButton *> sendStateButtonList = m_sendStateButtonsGroup->buttons();
    const QList<QAbstractButton *> getStateButtonList = m_getStateButtonGroup->buttons();
    QList<QAbstractButton *>::const_iterator setStateButtonListInterator = sendStateButtonList.cbegin();
    QList<QAbstractButton *>::const_iterator getStateButtonLIstIterator = getStateButtonList.cbegin();

    m_workPointLineLayout->addWidget(m_fvcoLabel);
    m_workPointLineLayout->addWidget(m_fvcoComboBox);
    AddButtonFromGroupToLayout(&setStateButtonListInterator, &setStateButtonListInterator, m_workPointLineLayout);
    AddButtonFromGroupToLayout(&setStateButtonListInterator, Q_NULLPTR, m_workPointLineLayout);
    AddButtonFromGroupToLayout(&getStateButtonLIstIterator, &getStateButtonLIstIterator, m_workPointLineLayout);

    m_speedLayout->addWidget(m_DoplerFreqLabel);
    m_speedLayout->addWidget(m_doplerFreqLineEdit);
    m_speedLayout->addWidget(m_speedLabel);
    m_speedLayout->addWidget(m_speedLineEdit);
    AddButtonFromGroupToLayout(&setStateButtonListInterator, &getStateButtonLIstIterator, m_speedLayout);

    m_rangeLineLayout->addWidget(m_rangeLabel);
    m_rangeLineLayout->addWidget(m_rangeLineEdit);
    AddButtonFromGroupToLayout(&setStateButtonListInterator, &getStateButtonLIstIterator, m_rangeLineLayout);

    m_gainLineLayout->addWidget(m_gainTXLabel);
    m_gainLineLayout->addWidget(m_gainTXSlider);
    m_gainLineLayout->addWidget(m_gainTxValue);
    m_gainLineLayout->addWidget(m_gainRXLabel);
    m_gainLineLayout->addWidget(m_gainRXSlider);
    m_gainLineLayout->addWidget(m_gainRxValue);
    AddButtonFromGroupToLayout(&setStateButtonListInterator, &getStateButtonLIstIterator, m_gainLineLayout);

    m_attenuatorLineLayout->addWidget(m_attenuatorLabel);
    m_attenuatorLineLayout->addWidget(m_attenuatorComboBox);
    AddButtonFromGroupToLayout(&setStateButtonListInterator, &getStateButtonLIstIterator, m_attenuatorLineLayout);

    m_noiseLineLayout->addWidget(m_noiseValueLabel);
    m_noiseLineLayout->addWidget(m_noiseLineEdit);


    m_leftLayout->addLayout(m_workPointLineLayout);
    m_leftLayout->addLayout(m_speedLayout);
    m_leftLayout->addLayout(m_rangeLineLayout);
    m_leftLayout->addLayout(m_gainLineLayout);
    m_leftLayout->addLayout(m_attenuatorLineLayout);
    m_leftLayout->addLayout(m_noiseLineLayout);


    m_modesWidget->setLayout(m_leftLayout);
    m_modesLayout->addWidget(m_modesWidget);
    m_modesLayout->addWidget(m_bPraWidget);
    m_mainLayout->addLayout(m_modesLayout);

    m_deviceModeLayout->addWidget(m_noiseLabel);
    m_deviceModeLayout->addWidget(m_noiseComboBox);
    AddButtonFromGroupToLayout(&getStateButtonLIstIterator, Q_NULLPTR, m_deviceModeLayout);
    AddButtonFromGroupToLayout(&setStateButtonListInterator, Q_NULLPTR, m_deviceModeLayout);
    m_deviceModeLayout->addWidget(m_restartDevice);

    m_mainLayout->addLayout(m_deviceModeLayout);
    m_rawHexLayout->addWidget(m_rawHexInputEdit);
    m_rawHexLayout->addWidget(m_sendRawHexButton);

    m_mainLayout->addLayout(m_rawHexLayout);
    m_mainLayout->addWidget(m_logClearButton);
    m_mainLayout->addWidget(m_log);

    setLayout(m_mainLayout);
}

void StateWidget::FillUI()
{
    m_fvcoLabel->setText(QStringLiteral("Рабочая точка Fvco для частот Tx и Rx: МЕГАгерцы"));
    m_DoplerFreqLabel->setText(QStringLiteral("Частота Доплера (по желанию) для частоты Tx: ГЕРЦЫ"));
    m_speedLabel->setText(QStringLiteral("Скорость может быть <0: метры/секунду "));
    m_rangeLabel->setText(QStringLiteral("Дальность ответного сигнала d: метры"));
    m_gainTXLabel->setText(QStringLiteral("Усиление TX GAIN_TX: децибелы"));
    m_gainRXLabel->setText(QStringLiteral("Усиление RX GAIN_RX: децибелы"));
    m_gainTxValue->setNum(0);
    m_gainRxValue->setNum(0);
    m_attenuatorLabel->setText(QStringLiteral("Установка ослабления Attenuator_RX: децибелы"));
    m_noiseValueLabel->setText(QStringLiteral("Значения для синуса и м сигнала"));
    m_noiseLabel->setText(QStringLiteral("Режимы работы:"));

    m_speedLineEdit->setValidator(m_intValidator);
    m_doplerFreqLineEdit->setValidator(m_intValidator);
    m_rangeLineEdit->setValidator(m_intValidator);

    m_fvcoComboBox->setEditable(true);
    m_fvcoComboBox->addItems(workPointsValues);
    m_fvcoComboBox->setCurrentText(m_settingFileService->GetAttribute(m_settingsAtribute, "fvco", workPointsValues.at(0)));

    m_doplerFreqLineEdit->setText(m_settingFileService->GetAttribute(m_settingsAtribute, "dopler", "0"));
    OnChangeDoplerLineEdit(m_settingFileService->GetAttribute(m_settingsAtribute, "dopler", "0"));
    m_rangeLineEdit->setText(m_settingFileService->GetAttribute(m_settingsAtribute, "range", "0"));
    m_noiseLineEdit->setText(m_settingFileService->GetAttribute(m_settingsAtribute, "noise", "0"));
    m_gainTXSlider->SetDoubleRange(0, 31.5);
    m_gainRXSlider->SetDoubleRange(0, 31.5);
    m_noiseLineEdit->setText(m_settingFileService->GetAttribute(m_settingsAtribute, "noise", "0"));

    m_attenuatorComboBox->addItems(m_attenuatorValues);
    m_attenuatorComboBox->setCurrentIndex(8);
    m_noiseComboBox->addItems(noiseValues);
    m_noiseComboBox->setEditable(false);
    m_restartDevice->setText(QStringLiteral("Перезагрузить устройство"));
    m_rawHexInputEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("^[0-9a-fA-F]*"), this));
    m_sendRawHexButton->setText(QStringLiteral("Отправить hex сообщение"));
    m_logClearButton->setText(QStringLiteral("Консоль отчистить"));
    m_log->setReadOnly(true);
    m_log->appendPlainText(QStringLiteral("Не подключено к ответчику"));
    m_bPraWidget->setDisabled(true);


    const bool EnableUI = m_settingFileService->GetAttribute(m_settingsAtribute, "uiEnabled", "0").toInt();
    m_modesWidget->setTitle("Состояния устройства");
//    OnSetButtonEnabled(EnableUI);

}

void StateWidget::AddButtonFromGroupToLayout(QList<QAbstractButton *>::const_iterator *setButtonIterator, QList<QAbstractButton *>::const_iterator *getButtonIterator, QBoxLayout *layout)
{
    layout->addWidget(**setButtonIterator);
    setButtonIterator->operator++();
    if (Q_NULLPTR != getButtonIterator)
    {
        layout->addWidget(**getButtonIterator);
        getButtonIterator->operator++();
    }
}

void StateWidget::ConnectObjects()
{
    connect(m_fvcoComboBox, &QComboBox::currentTextChanged, this, &StateWidget::OnChangeFvcoComboBoxValue);
    connect(m_speedLineEdit, &QLineEdit::textEdited, this, &StateWidget::OnChangeSpeedLineEdit);
    connect(m_doplerFreqLineEdit, &QLineEdit::textEdited, this, &StateWidget::OnChangeDoplerLineEdit);
    connect(m_rangeLineEdit, &QLineEdit::textEdited, this, &StateWidget::OnChangeRangeLineEdit);
    connect(m_gainRXSlider, &DoubleSlider::ToValueChanged, this, &StateWidget::OnChangeGainRxLineEdit);
    connect(m_gainTXSlider, &DoubleSlider::ToValueChanged, this, &StateWidget::OnChangeGainTxLineEdit);
    connect(m_noiseLineEdit, &QLineEdit::textEdited, this, &StateWidget::OnChangeNoiseLineEdit);

    connect(m_logClearButton, &QPushButton::clicked, m_log, &QPlainTextEdit::clear);
    connect(m_restartDevice, &QPushButton::clicked, this, &StateWidget::OnRestartDevice);
    connect(m_sendRawHexButton, &QPushButton::clicked, this, QOverload<>::of(&StateWidget::OnSendRawHexMessage));
    connect(m_statePresenter, &StatePresenter::ToConsoleLog, this, &StateWidget::OnConsoleLog);
    connect(m_statePresenter, &StatePresenter::ToSetButtonsEnabled, this, &StateWidget::OnSetButtonEnabled);
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    connect(m_sendStateButtonsGroup, &QButtonGroup::idClicked, this, &StateWidget::OnSetStateButtonIdClicked);
    connect(m_getStateButtonGroup, &QButtonGroup::idClicked, this, &StateWidget::OnGetStateButtonIdClicked);
#else
    connect(m_sendStateButtonsGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &StateWidget::OnSetStateButtonIdClicked);
    connect(m_getStateButtonGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &StateWidget::OnGetStateButtonIdClicked);
#endif
    connect(m_noiseComboBox, QOverload<int>::of(&QComboBox::activated), [&](int index)
    {
        if (index == m_noiseComboBox->count() - 1)
        {
            m_bPraWidget->setEnabled(true);
            m_modesWidget->setEnabled(false);
        }
        else
        {
            if (m_bPraWidget->isEnabled())
            {
                m_bPraWidget->setEnabled(false);
                m_modesWidget->setEnabled(true);
            }
            OnSetStateButtonIdClicked(6);
        }
    });
}

void StateWidget::UpdateHistoryFile()
{
    const QString time = "\n" + QDateTime::currentDateTime().toString(QStringLiteral("hh:mm:ss"));
    const int space = 5;
    const QString fvco = m_fvcoComboBox->currentText().leftJustified(space, ' ');
    const QString dopler = m_doplerFreqLineEdit->text().leftJustified(space, ' ');
    const QString speed = m_speedLineEdit->text().leftJustified(space, ' ');
    const QString range = m_rangeLineEdit->text().leftJustified(space, ' ');
    const QString gaintTx = QString::number(m_gainTXSlider->GetCurrentDoubleRangeValue(), 'f', 3);
    const QString gaintRx = QString::number(m_gainRXSlider->GetCurrentDoubleRangeValue(), 'f', 3);
    const QString attenuator = m_attenuatorComboBox->currentText();
    const QString noise = m_noiseComboBox->currentText();
    const QString message = time + "|Fvco(МПЦ)= " + fvco + " |Доплер(ГЦ)= " + dopler + " |Скорость(М/С)= " + speed + " |Дистанция(М)= " + range + " |gainTx= " + gaintTx + " |gainRx= " + gaintRx + " |Ослабление= " + attenuator + " |" + noise;
    m_file.write(message.toUtf8());
    m_file.flush();
}


void StateWidget::OnConsoleLog(const QString &message)
{
    const QString time = QDateTime::currentDateTime().toString(QStringLiteral("hh:mm:ss"));
    m_log->appendPlainText(time + " " + message);
}

void StateWidget::OnSetStateButtonIdClicked(int id)
{
    switch (id)
    {
    case 0:
    {
        OnConsoleLog(QStringLiteral("Высылаем сообщение пинга"));
        m_statePresenter->SetMessageToQueue(id);
        break;
    }
    case 1:
    {
        if (m_fvcoComboBox->currentText().isEmpty())
        {
            OnConsoleLog(QStringLiteral("Заполните пожалуйста поля: Рабочая точка Fvco: для частоты Rx"));
        }
        else
        {
            bool isOk;
            const double fvco = m_fvcoComboBox->currentText().toDouble(&isOk) * 1000000;
            if (isOk)
            {
                OnConsoleLog("Высылаем первое сообщение: установка частоты RX fvco= " + m_fvcoComboBox->currentText() + " MГЦ");
                m_statePresenter->SetMessageToQueue(id, fvco);
            }
            else
            {
                OnConsoleLog("Не смогли перевесли в число " + m_fvcoComboBox->currentText());
            }
        }
        break;
    }
    case 21:
    {
        if (m_fvcoComboBox->currentText().isEmpty())
        {
            OnConsoleLog(QStringLiteral("Заполните пожалуйста поля: Рабочая точка Fvco: для частоты Tx"));
            return;
        }
        else
        {
            bool isOk1;
            const double fvco = m_fvcoComboBox->currentText().toDouble(&isOk1) * 1000000;
            if (isOk1)
            {
                OnConsoleLog("Высылаем сообщение fvco= " + m_fvcoComboBox->currentText() + " МГЦ");
                m_statePresenter->SetMessageToQueue(id, fvco);
            }
            else
            {
                OnConsoleLog("Не смогли перевесли в число " + m_fvcoComboBox->currentText());
            }
        }
        break;
    }
    case 121:
    {
        if (m_fvcoComboBox->currentText().isEmpty())
        {
            OnConsoleLog(QStringLiteral("Заполните пожалуйста поля: Рабочая точка Fvco: для частот Tx и Rx"));
            return;
        }
        else
        {
            bool isOk1;
            const double fvco = m_fvcoComboBox->currentText().toDouble(&isOk1) * 1000000;
            if (isOk1)
            {
                OnConsoleLog("Высылаем сообщение fvco= " + m_fvcoComboBox->currentText() + " МГЦ");
                m_statePresenter->SetMessageToQueue(id, fvco);
            }
            else
            {
                OnConsoleLog("Не смогли перевесли в число " + m_fvcoComboBox->currentText());
            }
        }
        break;
    }
    case 22:
    {
        if (m_doplerFreqLineEdit->text().isEmpty())
        {
            OnConsoleLog(QStringLiteral("Заполните пожалуйста поля: Частота Доплера"));
            return;
        }
        else
        {
            bool isOk1;
            const double doplers = m_doplerFreqLineEdit->text().toDouble(&isOk1);
            if (isOk1)
            {
                OnConsoleLog("Высылаем сообщение установка Долера. Значение= " + m_doplerFreqLineEdit->text() + " ГЦ");
                m_statePresenter->SetMessageToQueue(id, doplers);
            }
        }
        break;
    }
    case 3:
    {
        if (m_rangeLineEdit->text().isEmpty())
        {
            OnConsoleLog(QStringLiteral("Заполните пожалуйста поля: Дальность ответного сигнала: d"));
        }
        else
        {
            bool isOk;
            const double distance = m_rangeLineEdit->text().toDouble(&isOk);
            if (isOk)
            {
                OnConsoleLog("Высылаем третье сообщение: установка дальности ответного сигнала. Дистанция= " + m_rangeLineEdit->text());
                m_statePresenter->SetMessageToQueue(id, distance);
            }
            else
            {
                OnConsoleLog("Третье сообщение: не смогли перевесли в число " + m_rangeLineEdit->text());
            }
        }
        break;
    }
    case 4:
    {
        const double gainTx = m_gainTXSlider->GetCurrentDoubleRangeValue();
        const double gainRx = m_gainRXSlider->GetCurrentDoubleRangeValue();
        OnConsoleLog("Высылаем четвертое сообщение: установка усиления. Tx= " + m_gainTXSlider->GetCurrentDoubleRangeText() + " Rx= " + m_gainRXSlider->GetCurrentDoubleRangeText());
        m_statePresenter->SetMessageToQueue(id, gainTx, gainRx);
        break;
    }
    case 5:
    {
        bool isOk;
        const double attenuator = m_attenuatorComboBox->currentText().toDouble(&isOk);
        if (isOk)
        {
            OnConsoleLog(QStringLiteral("Высылаем пятое сообщение: установка ослабления"));
            m_statePresenter->SetMessageToQueue(id, attenuator);
        }
        else
        {
            OnConsoleLog("Пятое сообщение: не смогли перевесли в число " + m_attenuatorComboBox->currentText());
        }
        break;
    }
    case 6:
    {
        const double noise = m_noiseComboBox->currentIndex();
        if (noise >= 3)
        {
//            bool fvcoParced, doplerParced;
//            double fvco;
//            if (3 == noise)
//            {
//                fvco = (m_fvcoComboBox->currentText().toDouble(&fvcoParced) - 3) * 1000000;
//            }
//            else
//            {
//                fvco = m_fvcoComboBox->currentText().toDouble(&fvcoParced) * 1000000;
//            }
//            if (fvcoParced)
//            {
//                m_statePresenter->SetMessageToQueue(1, fvco);
//                const double doplers = m_doplerFreqLineEdit->text().toDouble(&doplerParced);
//                if (doplerParced)
//                {
//                    m_statePresenter->SetMessageToQueue(2, fvco, doplerParced);
//                }
//                else
//                {
//                    m_statePresenter->SetMessageToQueue(2, fvco);
//                    OnConsoleLog("Шестое сообщение: доплер не можем распарсить, берем значение 0");
//                }
            bool noiseValParced;
            const double noiseValue = m_noiseLineEdit->text().toDouble(&noiseValParced);
            if (noiseValParced)
            {
                OnConsoleLog("Высылаем шестое сообщение: установка шума, параметр равен: " + m_noiseLineEdit->text());
                m_statePresenter->SetMessageToQueue(id, noise, noiseValue);
            }
            else
            {
                OnConsoleLog("Шестое сообщение: не смогли перевесли в число: " + m_noiseLineEdit->text() + ".Берем значение 0");
                m_statePresenter->SetMessageToQueue(id, noise);
            }
//            }
//            else
//            {
//                OnConsoleLog("Шестое сообщение: сначала установите рабочую точку");
//            }

        }
        else
        {
//            bool fvcoParced, doplerParced;
//            const double fvco = (m_fvcoComboBox->currentText().toDouble(&fvcoParced)) * 1000000;
//            if (fvcoParced)
//            {
//                m_statePresenter->SetMessageToQueue(1, fvco);
//                const double doplers = m_doplerFreqLineEdit->text().toDouble(&doplerParced);
//                if (doplerParced)
//                {
//                    m_statePresenter->SetMessageToQueue(2, fvco, doplers);
//                }
//                else
//                {
//            m_statePresenter->SetMessageToQueue(2, fvco);
//            OnConsoleLog("Шестое сообщение: доплер не можем распарсить, берем значение 0");
//                }
            m_statePresenter->SetMessageToQueue(id, noise);
//            }
//            else
//            {
//                OnConsoleLog("Шестое сообщение: сначала установите рабочую точку");
//            }
        }
        break;
    }
    case 12:
    {
        if (!m_fvcoComboBox->currentText().isEmpty() && !m_doplerFreqLineEdit->text().isEmpty())
        {
            bool isOk1, isOk2;
            const double Fvco = m_fvcoComboBox->currentText().toDouble(&isOk1) * 1000000;
            const double dopler = m_doplerFreqLineEdit->text().toDouble(&isOk2);
            if (isOk1)
            {
                if (isOk2)
                {

                    OnConsoleLog("Высылаем второе и первое сообщения: установка частоты Tx и Rх fvco= " + m_fvcoComboBox->currentText() + " МГЦ и доплер= " + m_doplerFreqLineEdit->text() + " ГЦ");
                    m_statePresenter->SetMessageToQueue(1, Fvco);
                    m_statePresenter->SetMessageToQueue(2, Fvco, dopler);
                }
                else
                {
                    OnConsoleLog("Не смогли перевесли в число " + m_doplerFreqLineEdit->text());
                    OnConsoleLog("Высылаем второе и первое сообщения: fvco= " + m_fvcoComboBox->currentText() + " МГЦ доплер= 0 ГЦ");
                    m_statePresenter->SetMessageToQueue(1, Fvco);
                    m_statePresenter->SetMessageToQueue(2, Fvco);
                }
            }
            else
            {
                OnConsoleLog("Не смогли перевесли в число " + m_fvcoComboBox->currentText());
            }
        }
        else
        {
            OnConsoleLog(QStringLiteral("Заполните пожалуйста поля: Рабочая точка Fvco: для частот Tx и Rx"));
        }
        break;
    }
    default:
    {
        OnConsoleLog(QStringLiteral("Обработка не написана void StateWidget::buttonIdClicked(int id)"));
        return;
    }
    }

    UpdateHistoryFile();
}

void StateWidget::OnRestartDevice()
{
    m_statePresenter->ToSendMessageToDeivce(QByteArray("\x0d\x5a"));
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

void StateWidget::OnGetStateButtonIdClicked(int id)
{
    switch (id)
    {
    case 1:
    {
        OnConsoleLog(QStringLiteral("Получаем рабочую точку ТХ"));
        m_statePresenter->GetStateFromDevice(id);
        break;
    }
    case 21:
    {
        OnConsoleLog(QStringLiteral("Получаем рабочую точку RX"));
        m_statePresenter->GetStateFromDevice(id, 1);
        break;
    }
    case 22:
    {
        OnConsoleLog(QStringLiteral("Получаем доплера точку"));
        m_statePresenter->GetStateFromDevice(id, 2);
//        id = 1;
//        QTimer::singleShot(1000, [ = ]()
//        {
//            m_statePresenter->GetStateFromDevice(2);
//        });
        break;
    }
    case 3:
    {
        OnConsoleLog(QStringLiteral("Получаем дальность"));
        m_statePresenter->GetStateFromDevice(id);
        break;
    }
    case 4:
    {
        OnConsoleLog(QStringLiteral("Получаем усиление"));
        m_statePresenter->GetStateFromDevice(id);
        break;
    }
    case 5:
    {
        OnConsoleLog(QStringLiteral("Получаем ослабление"));
        m_statePresenter->GetStateFromDevice(id);
        break;
    }
    case 6:
    {
        OnConsoleLog(QStringLiteral("Получаем режим работы"));
        m_statePresenter->GetStateFromDevice(id);
        break;
    }
    default:
    {
        OnConsoleLog(QStringLiteral("Обработка команды не реализована"));
        m_statePresenter->GetStateFromDevice(id);
        return;
    }
    }
}

void StateWidget::OnSetButtonEnabled(bool state)
{
    m_attenuatorComboBox->setEnabled(state);
    m_noiseComboBox->setEnabled(state);
    const QList<QAbstractButton *> buttonsSetState = m_sendStateButtonsGroup->buttons();
    for (QAbstractButton *const button : buttonsSetState  )
    {
        button->setEnabled(state);
    }
    const QList<QAbstractButton *> buttonsGetState = m_getStateButtonGroup->buttons();
    for (QAbstractButton *const button : buttonsGetState )
    {
        button->setEnabled(state);
    }
}

void StateWidget::OnChangeDoplerLineEdit(const QString &doplerText)
{
    const double dopler = doplerText.toDouble();
    const double speed = dopler * c / (2.0 * m_fvcoComboBox->currentText().toDouble() * 1000000.0);
    m_speedLineEdit->setText(QString::number(speed, 'f'));
    m_settingFileService->SetAttribute(m_settingsAtribute, "dopler", doplerText);
}

void StateWidget::OnChangeSpeedLineEdit(const QString &speedText)
{
    const double speed = speedText.toDouble();
    const double dopler1 = speed * m_fvcoComboBox->currentText().toDouble() * 1000000.0 * 2.0;
    const double dopler = dopler1 / c;
    m_doplerFreqLineEdit->setText(QString::number(dopler, 'f'));
    m_settingFileService->SetAttribute(m_settingsAtribute, "dopler", QString::number(dopler));
}

void StateWidget::OnChangeFvcoComboBoxValue(const QString &fvco)
{
    m_settingFileService->SetAttribute(m_settingsAtribute, "fvco", fvco);
}

void StateWidget::OnChangeRangeLineEdit(const QString &range)
{
    m_settingFileService->SetAttribute(m_settingsAtribute, "range", range);
}

void StateWidget::OnChangeGainTxLineEdit(double gainTx)
{
    const QString gainTxString = QString::number(gainTx, 'f', 1);
    m_gainTxValue->setText(gainTxString);
    m_settingFileService->SetAttribute(m_settingsAtribute, "gainTx", gainTxString);
}

void StateWidget::OnChangeGainRxLineEdit(double gainRx)
{
    const QString gainRxString = QString::number(gainRx, 'f', 1);
    m_gainRxValue->setText(gainRxString);
    m_settingFileService->SetAttribute(m_settingsAtribute, "gainRx", gainRxString);
}

void StateWidget::OnChangeNoiseLineEdit(const QString &noise)
{
    m_settingFileService->SetAttribute(m_settingsAtribute, "noise", noise);
}

void StateWidget::OnChangeSinusLineEdit(const QString &sinus)
{
    m_settingFileService->SetAttribute(m_settingsAtribute, "sinus", sinus);
}
