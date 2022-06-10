//#include "datawidget.h"

//DataWidget::DataWidget(QWidget *parent)
//    :QGroupBox(parent)
//{

//}

//DataWidget::~DataWidget()
//{
//    delete m_speedLayout;
//    delete m_rangeLineLayout;
//    delete m_workPointLineLayout;
//    delete m_gainLineLayout;
//    delete m_attenuatorLineLayout;

//    delete m_DoplerFreqLabel;
//    delete m_doplerFreqLineEdit;

//    delete m_speedLabel;
//    delete m_speedLineEdit;

//    delete m_rangeLabel;
//    delete m_rangeLineEdit;

//    delete m_fvcoLabel;
//    delete m_fvcoComboBox;

//    delete m_gainTXLabel;
//    delete m_gainTXSlider;
//    delete m_gainTxValue;
//    delete m_gainRXLabel;
//    delete m_gainRXSlider;
//    delete m_gainRxValue;

//    delete m_attenuatorLabel;
//    delete m_attenuatorComboBox;
//}

//void DataWidget::CreateObjects()
//{

//}

//void DataWidget::CreateUI()
//{
//    m_leftLayout=new QVBoxLayout();

//    m_speedLayout = new QHBoxLayout();
//    m_rangeLineLayout = new QHBoxLayout();
//    m_workPointLineLayout = new QHBoxLayout();
//    m_gainLineLayout = new QHBoxLayout();
//    m_attenuatorLineLayout = new QHBoxLayout();

//    m_fvcoLabel = new QLabel();
//    m_fvcoComboBox = new QComboBox();

//    m_DoplerFreqLabel = new QLabel();
//    m_doplerFreqLineEdit = new QLineEdit();

//    m_speedLabel = new QLabel();
//    m_speedLineEdit = new QLineEdit();

//    m_rangeLabel = new QLabel();
//    m_rangeLineEdit = new QLineEdit();


//    m_gainTXLabel = new QLabel();
//    m_gainTXSlider = new DoubleSlider(this);
//    m_gainTxValue = new QLabel();


//    m_gainRXLabel = new QLabel();
//    m_gainRXSlider = new DoubleSlider(this);
//    m_gainRxValue = new QLabel();


//    m_attenuatorLabel = new QLabel();
//    m_attenuatorComboBox = new QComboBox();
//}

//void DataWidget::FillButtonGroup()
//{

//}

//void DataWidget::InsertWidgetsIntoLayout()
//{
//    m_workPointLineLayout->addWidget(m_fvcoLabel);
//    m_workPointLineLayout->addWidget(m_fvcoComboBox);
//    AddButtonFromGroupToLayout(&setStateButtonListInterator, &setStateButtonListInterator, m_workPointLineLayout);
//    AddButtonFromGroupToLayout(&getStateButtonLIstIterator, Q_NULLPTR, m_workPointLineLayout);

//    m_speedLayout->addWidget(m_DoplerFreqLabel);
//    m_speedLayout->addWidget(m_doplerFreqLineEdit);
//    m_speedLayout->addWidget(m_speedLabel);
//    m_speedLayout->addWidget(m_speedLineEdit);
//    AddButtonFromGroupToLayout(&setStateButtonListInterator, &getStateButtonLIstIterator, m_speedLayout);

//    m_rangeLineLayout->addWidget(m_rangeLabel);
//    m_rangeLineLayout->addWidget(m_rangeLineEdit);
//    AddButtonFromGroupToLayout(&setStateButtonListInterator, &getStateButtonLIstIterator, m_rangeLineLayout);

//    m_gainLineLayout->addWidget(m_gainTXLabel);
//    m_gainLineLayout->addWidget(m_gainTXSlider);
//    m_gainLineLayout->addWidget(m_gainTxValue);
//    m_gainLineLayout->addWidget(m_gainRXLabel);
//    m_gainLineLayout->addWidget(m_gainRXSlider);
//    m_gainLineLayout->addWidget(m_gainRxValue);
//    AddButtonFromGroupToLayout(&setStateButtonListInterator, &getStateButtonLIstIterator, m_gainLineLayout);

//    m_attenuatorLineLayout->addWidget(m_attenuatorLabel);
//    m_attenuatorLineLayout->addWidget(m_attenuatorComboBox);
//    AddButtonFromGroupToLayout(&setStateButtonListInterator, &getStateButtonLIstIterator, m_attenuatorLineLayout);
//}

//void DataWidget::AddButtonFromGroupToLayout(QList::const_iterator *setButtonIterator, QList::const_iterator *getButtonIterator, QBoxLayout *layout)
//{
//    layout->addWidget(**setButtonIterator);
//    setButtonIterator->operator++();
//    if (Q_NULLPTR != getButtonIterator)
//    {
//        layout->addWidget(**getButtonIterator);
//        getButtonIterator->operator++();
//    }
//}

//void DataWidget::FillUI()
//{
//    m_fvcoLabel->setText(QStringLiteral("Рабочая точка Fvco для частот Tx и Rx: МЕГАгерцы"));
//    m_DoplerFreqLabel->setText(QStringLiteral("Частота Доплера (по желанию) для частоты Tx: ГЕРЦЫ"));
//    m_speedLabel->setText(QStringLiteral("Скорость может быть <0: метры/секунду "));
//    m_rangeLabel->setText(QStringLiteral("Дальность ответного сигнала d: метры"));
//    m_gainTXLabel->setText(QStringLiteral("Усиление TX GAIN_TX: децибелы"));
//    m_gainRXLabel->setText(QStringLiteral("Усиление RX GAIN_RX: децибелы"));
//    m_gainTxValue->setText("0");
//    m_gainRxValue->setText("0");
//    m_attenuatorLabel->setText(QStringLiteral("Установка ослабления Attenuator_RX: децибелы"));

//    m_speedLineEdit->setValidator(m_intValidator);
//    m_doplerFreqLineEdit->setValidator(m_intValidator);
//    m_rangeLineEdit->setValidator(m_intValidator);
//    m_fvcoComboBox->setEditable(true);
//    m_fvcoComboBox->addItems(workPointsValues);
//    m_fvcoComboBox->setCurrentText(m_settingFileService->GetAttribute(m_settingsAtribute, "fvco", workPointsValues.at(0)));
//}
