#include "ui/datawidget.h"

DataWidget::DataWidget(SettingFileService *settingFileService, QWidget *parent)
    : QGroupBox(parent)
    , m_settingFileService(settingFileService)
    , m_attenuatorValues{"0", "1", "2", "3", "4", "5", "6", "9", "12", "15", "18", "21", "24", "27", "30"}
{
    CreateObjects();
    CreateUI();
    InsertWidgetsIntoLayout();
    FillUI();
    ConnectObjects();
}

DataWidget::~DataWidget()
{
    delete m_intValidator;
    delete m_gainValidator;

    delete m_doplerAndSpeedLayout;
    delete m_rangeLineLayout;
    delete m_workPointLineLayout;
    delete m_gainLineLayout;
    delete m_attenuatorLineLayout;
    delete m_cooridnatesLayout;

    delete m_fvcoLabel;
    delete m_fvcoComboBox;
    delete m_setRxButton;
    delete m_setTxButton;
    delete m_setTxAndRxButton;
    delete m_getRxButton;
    delete m_getTxButton;

    delete m_DoplerFreqLabel;
    delete m_doplerFreqLineEdit;
    delete m_speedLabel;
    delete m_speedLineEdit;
    delete m_setDoplerButton;
    delete m_getDoplerButton;

    delete m_rangeLabel;
    delete m_rangeLineEdit;
    delete m_setRangeButton;
    delete m_getRangeButton;

    delete m_gainTXLabel;
    delete m_gainTXSlider;
    delete m_gainTxValue;
    delete m_gainRXLabel;
    delete m_gainRXSlider;
    delete m_gainRxValue;
    delete m_setGainButton;
    delete m_getGainButton;

    delete m_attenuatorLabel;
    delete m_attenuatorComboBox;
    delete m_setAttenuatorButton;
    delete m_getAttenuatorButton;

    delete m_sinusValueLabel;
    delete m_sinusLineEdit;

    delete m_coordinatesLabel;
    delete m_coordinatesValue;
    delete m_getCoordinatesButton;
}

void DataWidget::CreateObjects()
{
    m_intValidator = new QIntValidator(0, 2147483647);
    m_gainValidator = new QIntValidator(0, 64);
}

void DataWidget::CreateUI()
{
    m_mainLayout = new QVBoxLayout();

    m_workPointLineLayout = new QHBoxLayout();
    m_fvcoLabel = new QLabel();
    m_fvcoComboBox = new QComboBox();
    m_setRxButton = new QPushButton();
    m_setTxButton = new QPushButton();
    m_setTxAndRxButton = new QPushButton();
    m_getRxButton = new QPushButton();
    m_getTxButton = new QPushButton();

    m_doplerAndSpeedLayout = new QHBoxLayout();
    m_DoplerFreqLabel = new QLabel();
    m_doplerFreqLineEdit = new QLineEdit();
    m_speedLabel = new QLabel();
    m_speedLineEdit = new QLineEdit();
    m_setDoplerButton = new QPushButton();
    m_getDoplerButton = new QPushButton();

    m_rangeLineLayout = new QHBoxLayout();
    m_rangeLabel = new QLabel();
    m_rangeLineEdit = new QLineEdit();
    m_setRangeButton = new QPushButton();
    m_getRangeButton = new QPushButton();

    m_gainLineLayout = new QHBoxLayout();
    m_gainTXLabel = new QLabel();
    m_gainTXSlider = new DoubleSlider(this);
    m_gainTxValue = new QLabel();
    m_setGainButton = new QPushButton();
    m_getGainButton = new QPushButton();

    m_gainRXLabel = new QLabel();
    m_gainRXSlider = new DoubleSlider(this);
    m_gainRxValue = new QLabel();

    m_attenuatorLineLayout = new QHBoxLayout();
    m_attenuatorLabel = new QLabel();
    m_attenuatorComboBox = new QComboBox();
    m_setAttenuatorButton = new QPushButton();
    m_getAttenuatorButton = new QPushButton();

    m_sinusValLineLayout = new QHBoxLayout();
    m_sinusValueLabel = new QLabel();
    m_sinusLineEdit = new QLineEdit();

    m_cooridnatesLayout = new QHBoxLayout();
    m_coordinatesLabel = new QLabel();
    m_coordinatesValue = new QLabel();
    m_getCoordinatesButton = new QPushButton();
}


void DataWidget::InsertWidgetsIntoLayout()
{
    m_workPointLineLayout->addWidget(m_fvcoLabel);
    m_workPointLineLayout->addWidget(m_fvcoComboBox);
    m_workPointLineLayout->addWidget(m_setRxButton);
    m_workPointLineLayout->addWidget(m_setTxButton);
    m_workPointLineLayout->addWidget(m_setTxAndRxButton);
    m_workPointLineLayout->addWidget(m_getRxButton);
    m_workPointLineLayout->addWidget(m_getTxButton);

    m_doplerAndSpeedLayout->addWidget(m_DoplerFreqLabel);
    m_doplerAndSpeedLayout->addWidget(m_doplerFreqLineEdit);
    m_doplerAndSpeedLayout->addWidget(m_speedLabel);
    m_doplerAndSpeedLayout->addWidget(m_speedLineEdit);
    m_doplerAndSpeedLayout->addWidget(m_setDoplerButton);
    m_doplerAndSpeedLayout->addWidget(m_getDoplerButton);

    m_rangeLineLayout->addWidget(m_rangeLabel);
    m_rangeLineLayout->addWidget(m_rangeLineEdit);
    m_rangeLineLayout->addWidget(m_setRangeButton);
    m_rangeLineLayout->addWidget(m_getRangeButton);

    m_gainLineLayout->addWidget(m_gainTXLabel);
    m_gainLineLayout->addWidget(m_gainTXSlider);
    m_gainLineLayout->addWidget(m_gainTxValue);
    m_gainLineLayout->addWidget(m_gainRXLabel);
    m_gainLineLayout->addWidget(m_gainRXSlider);
    m_gainLineLayout->addWidget(m_gainRxValue);
    m_gainLineLayout->addWidget(m_setGainButton);
    m_gainLineLayout->addWidget(m_getGainButton);

    m_attenuatorLineLayout->addWidget(m_attenuatorLabel);
    m_attenuatorLineLayout->addWidget(m_attenuatorComboBox);
    m_attenuatorLineLayout->addWidget(m_setAttenuatorButton);
    m_attenuatorLineLayout->addWidget(m_getAttenuatorButton);

    m_sinusValLineLayout->addWidget(m_sinusValueLabel);
    m_sinusValLineLayout->addWidget(m_sinusLineEdit);

    m_cooridnatesLayout->addWidget(m_coordinatesLabel);
    m_cooridnatesLayout->addWidget(m_coordinatesValue);
    m_cooridnatesLayout->addWidget(m_getCoordinatesButton);

    m_mainLayout->addLayout(m_workPointLineLayout);
    m_mainLayout->addLayout(m_doplerAndSpeedLayout);
    m_mainLayout->addLayout(m_rangeLineLayout);
    m_mainLayout->addLayout(m_gainLineLayout);
    m_mainLayout->addLayout(m_attenuatorLineLayout);
    m_mainLayout->addLayout(m_sinusValLineLayout);
    m_mainLayout->addLayout(m_cooridnatesLayout);
    setLayout(m_mainLayout);
}

void DataWidget::FillUI()
{
    m_fvcoLabel->setText(QStringLiteral("Рабочая точка Fvco для частот Tx и Rx: МЕГАгерцы"));
    m_fvcoComboBox->setEditable(true);
    int minWorkPointValue = 2695;
    QStringList workPointsValues;
    for (int i = 0; i < 30; ++i)
    {
        workPointsValues.append(QString::number(minWorkPointValue));
        minWorkPointValue = minWorkPointValue + 10;
    }
    m_fvcoComboBox->addItems(workPointsValues);
    m_fvcoComboBox->setCurrentText(m_settingFileService->GetAttribute(m_settingFileService->GetDataArribute(), "fvco", workPointsValues.at(0)));
    m_setRxButton->setText(QStringLiteral("Установить RX"));
    m_setTxButton->setText(QStringLiteral("Установить TX"));
    m_setTxAndRxButton->setText(QStringLiteral("Установить RX и ТХ"));
    m_getRxButton->setText(QStringLiteral("Запр.RX"));
    m_getTxButton->setText(QStringLiteral("Запр.TX"));

    m_DoplerFreqLabel->setText(QStringLiteral("Частота Доплера: ГЕРЦЫ"));
    m_doplerFreqLineEdit->setText(m_settingFileService->GetAttribute(m_settingFileService->GetDataArribute(), "dopler", "0"));
    m_doplerFreqLineEdit->setValidator(m_intValidator);
    OnChangeDoplerLineEdit(m_doplerFreqLineEdit->text());
    m_speedLabel->setText(QStringLiteral("Скорость может быть <0: метры/секунду "));
    m_speedLineEdit->setValidator(m_intValidator);
    m_setDoplerButton->setText(QStringLiteral("Установить Доплер"));
    m_getDoplerButton->setText(QStringLiteral("Запросить"));


    m_rangeLabel->setText(QStringLiteral("Дальность ответного сигнала d: метры"));
    m_rangeLineEdit->setValidator(m_intValidator);
    m_rangeLineEdit->setText(m_settingFileService->GetAttribute(m_settingFileService->GetDataArribute(), "range", "60490"));
    m_setRangeButton->setText(QStringLiteral("Установить"));
    m_getRangeButton->setText(QStringLiteral("Запросить"));

    m_gainTXLabel->setText(QStringLiteral("Усиление TX GAIN_TX: децибелы"));
    m_gainRXLabel->setText(QStringLiteral("Усиление RX GAIN_RX: децибелы"));
    m_gainTXSlider->SetDoubleRange(0, 31.5);
    m_gainRXSlider->SetDoubleRange(0, 31.5);
    const int gainTxVal = m_settingFileService->GetAttribute(m_settingFileService->GetDataArribute(), "gainTx", "8").toInt();
    m_gainTxValue->setNum(gainTxVal);
    m_gainTXSlider->setValue(gainTxVal * 10);
    const int gainRxVal = m_settingFileService->GetAttribute(m_settingFileService->GetDataArribute(), "gainRx", "8").toInt();
    m_gainRxValue->setNum(gainRxVal);
    m_gainRXSlider->setValue(gainRxVal * 10);

    m_setGainButton->setText(QStringLiteral("Установить"));
    m_getGainButton->setText(QStringLiteral("Запросить"));


    m_attenuatorLabel->setText(QStringLiteral("Установка ослабления Attenuator_RX: децибелы"));
    m_attenuatorComboBox->addItems(m_attenuatorValues);
    m_attenuatorComboBox->setEditable(false);
    m_attenuatorComboBox->setCurrentIndex(m_settingFileService->GetAttribute(m_settingFileService->GetDataArribute(), "attenuator", "0").toInt());
    m_setAttenuatorButton->setText(QStringLiteral("Установить"));
    m_getAttenuatorButton->setText(QStringLiteral("Запросить"));

    m_sinusValueLabel->setText(QStringLiteral("Значения для синуса МГЦ"));
    m_sinusValueLabel->setToolTip(QStringLiteral("Отправляются шаги * 10^6 *2 "));
    m_sinusLineEdit->setText(m_settingFileService->GetAttribute(m_settingFileService->GetDataArribute(), "sinus", "3"));
    m_sinusLineEdit->setValidator(m_intValidator);
    m_coordinatesLabel->setText(QStringLiteral("Координаты ЮК"));
    m_coordinatesValue->setText(QStringLiteral("Координаты не получены"));
    m_getCoordinatesButton->setText(QStringLiteral("Запросить"));
}


void DataWidget::ConnectObjects()
{
    connect(m_fvcoComboBox, &QComboBox::currentTextChanged, this, &DataWidget::OnChangeFvcoComboBoxValue);
    connect(m_doplerFreqLineEdit, &QLineEdit::textEdited, this, &DataWidget::OnChangeDoplerLineEdit);
    connect(m_speedLineEdit, &QLineEdit::textEdited, this, &DataWidget::OnChangeSpeedLineEdit);
    connect(m_rangeLineEdit, &QLineEdit::textEdited, this, &DataWidget::OnChangeRangeLineEdit);
    connect(m_gainRXSlider, &DoubleSlider::ToValueChanged, this, &DataWidget::OnChangeGainRxLineEdit);
    connect(m_gainTXSlider, &DoubleSlider::ToValueChanged, this, &DataWidget::OnChangeGainTxLineEdit);
    connect(m_attenuatorComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &DataWidget::OnChangeAttenuator);
    connect(m_sinusLineEdit, &QLineEdit::textEdited, this, &DataWidget::OnChangeSinusLineEdit);

    connect(m_setRxButton, &QPushButton::clicked, this, &DataWidget::OnSetRxButtonClicked);
    connect(m_setTxButton, &QPushButton::clicked, this, &DataWidget::OnSetTxButtonClicked);
    connect(m_setTxAndRxButton, &QPushButton::clicked, m_setRxButton, &QPushButton::clicked);
    connect(m_setTxAndRxButton, &QPushButton::clicked, m_setTxButton, &QPushButton::clicked);
    connect(m_getRxButton, &QPushButton::clicked, this, &DataWidget::OnGetRxButtonClicked);
    connect(m_getTxButton, &QPushButton::clicked, this, &DataWidget::OnGetTxButtonClicked);
    connect(m_setDoplerButton, &QPushButton::clicked, this, &DataWidget::OnSetDoplerButtonClicked);
    connect(m_getDoplerButton, &QPushButton::clicked, this, &DataWidget::OnGetDoplerButtonClicked);
    connect(m_setRangeButton, &QPushButton::clicked, this, &DataWidget::OnSetRangeButtonClicked);
    connect(m_getRangeButton, &QPushButton::clicked, this, &DataWidget::OnGetRangeButtonClicked);
    connect(m_setGainButton, &QPushButton::clicked, this, &DataWidget::OnSetGainButtonClicked);
    connect(m_getGainButton, &QPushButton::clicked, this, &DataWidget::OnGetGainButtonClicked);
    connect(m_setAttenuatorButton, &QPushButton::clicked, this, &DataWidget::OnSetAttenuatorButtonClicked);
    connect(m_getAttenuatorButton, &QPushButton::clicked, this, &DataWidget::OnGetAttenuatorButtonClicked);
    connect(m_getCoordinatesButton, &QPushButton::clicked, this, &DataWidget::OnGetCoordinatsButtonClicked);
}

void DataWidget::OnChangeFvcoComboBoxValue(const QString &fvco)
{
    m_settingFileService->SetAttribute(m_settingFileService->GetDataArribute(), "fvco", fvco);
}

void DataWidget::OnChangeDoplerLineEdit(const QString &doplerText)
{
    const double dopler = doplerText.toDouble();
    const double speed = dopler * m_c / (2.0 * m_fvcoComboBox->currentText().toDouble() * 1000000.0);
    m_speedLineEdit->setText(QString::number(speed, 'f'));
    m_settingFileService->SetAttribute(m_settingFileService->GetDataArribute(), QLatin1Literal("dopler"), doplerText);
}

void DataWidget::OnChangeSpeedLineEdit(const QString &speedText)
{
    const double speed = speedText.toDouble();
    const double dopler1 = speed * m_fvcoComboBox->currentText().toDouble() * 1000000.0 * 2.0;
    const double dopler = dopler1 / m_c;
    m_doplerFreqLineEdit->setText(QString::number(dopler, 'f'));
    m_settingFileService->SetAttribute(m_settingFileService->GetDataArribute(), QLatin1Literal("dopler"), QString::number(dopler));
}

void DataWidget::OnChangeRangeLineEdit(const QString &range)
{
    m_settingFileService->SetAttribute(m_settingFileService->GetDataArribute(), QLatin1Literal("range"), range);
}

void DataWidget::OnChangeGainTxLineEdit(double gainTx)
{
    const QString gainTxString = QString::number(gainTx, 'f', 1);
    m_gainTxValue->setText(gainTxString);
    m_settingFileService->SetAttribute(m_settingFileService->GetDataArribute(), QLatin1Literal("gainTx"), gainTxString);
}

void DataWidget::OnChangeGainRxLineEdit(double gainRx)
{
    const QString gainRxString = QString::number(gainRx, 'f', 1);
    m_gainRxValue->setText(gainRxString);
    m_settingFileService->SetAttribute(m_settingFileService->GetDataArribute(), QLatin1Literal("gainRx"), gainRxString);
}

void DataWidget::OnChangeAttenuator(int attenuatorIndex)
{
    m_settingFileService->SetAttribute(m_settingFileService->GetDataArribute(), QLatin1Literal("attenuator"), QString::number(attenuatorIndex));
}

void DataWidget::OnChangeSinusLineEdit(const QString &sinus)
{
    m_settingFileService->SetAttribute(m_settingFileService->GetDataArribute(), QLatin1Literal("sinus"), sinus);
}

void DataWidget::OnSetRxButtonClicked()
{
    if (m_fvcoComboBox->currentText().isEmpty())
    {
        ToConsoleLog(QStringLiteral("Заполните пожалуйста поля: Рабочая точка Fvco: для частоты Rx"));
    }
    else
    {
        bool isParsingOk;
        const double fvcoHz = m_fvcoComboBox->currentText().toDouble(&isParsingOk) * 1000000;
        if (isParsingOk)
        {
            ToConsoleLog("Высылаем первое сообщение: установка частоты RX fvco= " + m_fvcoComboBox->currentText() + " MГЦ");
            Q_EMIT ToSetState(1, QList<double>() << fvcoHz);
        }
        else
        {
            ToConsoleLog("Не смогли перевесли в число " + m_fvcoComboBox->currentText());
        }
    }
}

void DataWidget::OnSetTxButtonClicked()
{
    if (m_fvcoComboBox->currentText().isEmpty())
    {
        ToConsoleLog(QStringLiteral("Заполните пожалуйста поля: Рабочая точка Fvco: для частоты Tx"));
        return;
    }
    else
    {
        bool isParsingOk;
        const double fvcoHz = m_fvcoComboBox->currentText().toDouble(&isParsingOk) * 1000000;
        if (isParsingOk)
        {
            ToConsoleLog("Высылаем первое сообщение: установка частоты TX fvco= " + m_fvcoComboBox->currentText() + " МГЦ");
            Q_EMIT ToSetState(2, QList<double>() << fvcoHz);
        }
        else
        {
            ToConsoleLog("Не смогли перевесли в число " + m_fvcoComboBox->currentText());
        }
    }
}

void DataWidget::OnGetRxButtonClicked()
{
    ToConsoleLog(QStringLiteral("Нажали получить рабочую точку Rх"));
    Q_EMIT ToGetState(1);
}

void DataWidget::OnGetTxButtonClicked()
{
    ToConsoleLog(QStringLiteral("Нажали получить рабочую точку Тх"));
    Q_EMIT ToGetState(2);
}

void DataWidget::OnSetDoplerButtonClicked()
{
    if (m_doplerFreqLineEdit->text().isEmpty())
    {
        ToConsoleLog(QStringLiteral("Заполните пожалуйста поля: Частота Доплера"));
    }
    else
    {
        bool isParsingOk;
        const double dopler = m_doplerFreqLineEdit->text().toDouble(&isParsingOk);
        if (isParsingOk)
        {
            ToConsoleLog("Высылаем сообщение установка Долера. Значение= " + m_doplerFreqLineEdit->text() + " ГЦ");
            Q_EMIT ToSetState(9, QList<double>() << dopler);
        }
    }
}

void DataWidget::OnGetDoplerButtonClicked()
{
    ToConsoleLog(QStringLiteral("Нажали получить частоту Доплера"));
    Q_EMIT ToGetState(9);
}

void DataWidget::OnSetRangeButtonClicked()
{
    if (m_rangeLineEdit->text().isEmpty())
    {
        ToConsoleLog(QStringLiteral("Заполните пожалуйста поля: Дальность ответного сигнала: d"));
    }
    else
    {
        bool isParsingOk;
        const double distance = m_rangeLineEdit->text().toDouble(&isParsingOk);
        if (isParsingOk)
        {
            ToConsoleLog("Высылаем третье сообщение: установка дальности ответного сигнала. Дистанция= " + m_rangeLineEdit->text());
            Q_EMIT ToSetState(3, QList<double>() << distance);
        }
        else
        {
            ToConsoleLog("Третье сообщение: не смогли перевесли в число " + m_rangeLineEdit->text());
        }
    }
}

void DataWidget::OnGetRangeButtonClicked()
{
    ToConsoleLog(QStringLiteral("Нажали получить дальность ответного сигнала"));
    Q_EMIT ToGetState(3);
}

void DataWidget::OnSetGainButtonClicked()
{
    const double gainTx = m_gainTXSlider->GetCurrentDoubleRangeValue();
    const double gainRx = m_gainRXSlider->GetCurrentDoubleRangeValue();
    ToConsoleLog("Высылаем четвертое сообщение: установка усиления. Tx= " + m_gainTXSlider->GetCurrentDoubleRangeText() + " Rx= " + m_gainRXSlider->GetCurrentDoubleRangeText());
    Q_EMIT ToSetState(4, QList<double>() << gainTx << gainRx);
}

void DataWidget::OnGetGainButtonClicked()
{
    ToConsoleLog(QStringLiteral("Нажали получить  усиление Tx и Rx"));
    Q_EMIT ToGetState(4);
}

void DataWidget::OnSetAttenuatorButtonClicked()
{
    bool isParsingOk;
    const double attenuator = m_attenuatorComboBox->currentText().toDouble(&isParsingOk);
    if (isParsingOk)
    {
        ToConsoleLog(QStringLiteral("Высылаем пятое сообщение: установка ослабления. Значение: ") + QString::number(attenuator));
        Q_EMIT ToSetState(5, QList<double>() << attenuator);
    }
    else
    {
        ToConsoleLog("Пятое сообщение: не смогли перевесли в число " + m_attenuatorComboBox->currentText());
    }
}

void DataWidget::OnGetAttenuatorButtonClicked()
{
    ToConsoleLog(QStringLiteral("Нажали получить ослабление"));
    Q_EMIT ToGetState(5);
}

void DataWidget::OnGetCoordinatsButtonClicked()
{
    ToConsoleLog(QStringLiteral("Нажали получить координаты ЮК"));
    Q_EMIT ToGetState(7);
}

quint32 DataWidget::GetSinusValue()
{
    return m_sinusLineEdit->text().toUInt() * 1000000 / 2;
}

void DataWidget::OnUpdateLatLong(const QString &message)
{
    m_coordinatesValue->setText(message);
}

