#include "bparwidget.h"

#include <QDataStream>

BParWidget::BParWidget(StatePresenter *presenter, QWidget *parent)
    : QGroupBox(parent)
    , m_presenter(presenter)
{
    CreateObjects();
    CreateUI();
    InsertWidgetsIntoLayout();
    FillUI();
    ConnectObjects();
}

BParWidget::~BParWidget()
{
    delete m_foLayouyt;
    delete m_radioImpulsLayout;
    delete m_powerAndDelayLayout;
    delete m_mainLayout;

    delete m_onBparButton;
    delete m_radioImpulsButton;
    delete m_tksComboBox;

    delete m_lchmButton;
    delete m_answerDelayLabel;
    delete m_answerDelayValue;
    delete m_enableThreshold;
    delete m_thresholdComboBox;
    for (QAbstractButton *button : m_foRadioButtons->buttons())
    {
        delete button;
    }
    delete m_foRadioButtons;
}

void BParWidget::CreateObjects()
{

}

void BParWidget::CreateUI()
{
    m_mainLayout = new QHBoxLayout();

    m_powerAndDelayLayout = new QVBoxLayout();
    m_onBparButton = new QPushButton();

    m_radioImpulsLayout = new QHBoxLayout();
    m_radioImpulsButton = new QRadioButton();
    m_tksComboBox = new QComboBox();

    m_lchmButton = new QRadioButton();
    m_answerDelayLabel = new QLabel();
    m_answerDelayValue = new QSpinBox();
    m_enableThreshold = new QCheckBox();
    m_thresholdComboBox = new QComboBox();

    m_foLayouyt = new QVBoxLayout();
    m_foRadioButtons = new QButtonGroup();
    for (int i = 0; i < 6; ++i)
    {
        m_foRadioButtons->addButton(new QRadioButton(QStringLiteral("Fo ") + QString::number(i + 1)), i);
    }
}

void BParWidget::InsertWidgetsIntoLayout()
{
    m_powerAndDelayLayout->addWidget(m_onBparButton);

    m_radioImpulsLayout->addWidget(m_radioImpulsButton);
    m_radioImpulsLayout->addWidget(m_tksComboBox);

    m_powerAndDelayLayout->addLayout(m_radioImpulsLayout);
    m_powerAndDelayLayout->addWidget(m_lchmButton);
    m_powerAndDelayLayout->addWidget(m_answerDelayLabel);
    m_powerAndDelayLayout->addWidget(m_answerDelayValue);
    m_powerAndDelayLayout->addWidget(m_enableThreshold);
    m_powerAndDelayLayout->addWidget(m_thresholdComboBox);
    for (QAbstractButton *radioButton : m_foRadioButtons->buttons())
    {
        m_foLayouyt->addWidget(radioButton);
    }

    m_mainLayout->addLayout(m_powerAndDelayLayout);
    m_mainLayout->addLayout(m_foLayouyt);

    setLayout(m_mainLayout);
}

void BParWidget::FillUI()
{
    setTitle(QStringLiteral("БПАР"));
    m_onBparButton->setText("Отправить инфу");
    m_radioImpulsButton->setText(QStringLiteral("Радиоимпульс"));
    m_radioImpulsButton->setChecked(true);
    m_lchmButton->setText(QStringLiteral("ЛЧМ (линей частот модул)"));
    m_tksComboBox->addItems(QStringList() << "ТК1" << "TK2");
    m_tksComboBox->setEditable(false);

    m_answerDelayLabel->setText(QStringLiteral("Установить дальность(метры)"));
    m_answerDelayValue->setMaximum(9999999);
    m_foRadioButtons->button(0)->setChecked(true);
    m_enableThreshold->setText(QStringLiteral("Включить порог"));
    m_thresholdComboBox->addItems(QStringList() << "500" << "1000");
    m_thresholdComboBox->setDisabled(true);
    m_thresholdComboBox->setEditable(true);

}

void BParWidget::ConnectObjects()
{
    connect(m_onBparButton, &QPushButton::clicked, this,  &BParWidget::OnCollectParam);
    connect(m_radioImpulsButton, &QRadioButton::toggled, this,  &BParWidget::OnSetRadioImpulsEnabled);
    connect(m_enableThreshold, &QCheckBox::stateChanged, this, &BParWidget::OnThresoldComboBoxChecked);
}

void BParWidget::OnSetRadioImpulsEnabled(bool state)
{
    m_tksComboBox->setEnabled(state);
    m_thresholdComboBox->setCurrentIndex(1 - state);
}

void BParWidget::OnCollectParam()
{
    quint8 buttonId = 0;
    bool hasLcm = true, hasThreshold = false;
    quint8 tksIndex = 0;
    quint16 threshold = 0;
    for (QAbstractButton *const foRadioButton : m_foRadioButtons->buttons())
    {
        if (foRadioButton->isChecked())
        {
            buttonId = m_foRadioButtons->id(foRadioButton); //000 - 3 байте
            break;
        }
    }
    if (m_radioImpulsButton->isChecked())
    {
        hasLcm = false;
        tksIndex = (quint8)m_tksComboBox->currentIndex() + 1;
    }
    if (m_enableThreshold->isChecked())
    {
        hasThreshold=true;
        threshold=(quint16)m_thresholdComboBox->currentText().toUInt();
    }
    m_presenter->SendBparMessage(buttonId, hasLcm, tksIndex, hasThreshold, threshold, m_answerDelayValue->value());
}

void BParWidget::OnThresoldComboBoxChecked(int state)
{
    Qt::CheckState checkBoxState = static_cast<Qt::CheckState>(state);
    switch (checkBoxState)
    {
    case Qt::Unchecked:
    case Qt::PartiallyChecked:
        m_thresholdComboBox->setDisabled(true);
        break;
    case Qt::Checked:
        m_thresholdComboBox->setEnabled(true);
        break;
    default:
        break;
    }
}
