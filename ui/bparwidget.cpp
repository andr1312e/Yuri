#include "bparwidget.h"

#include <QDataStream>

BParWidget::BParWidget(QWidget *parent)
    : QGroupBox(parent)
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

    m_foLayouyt = new QVBoxLayout();
    m_foRadioButtons = new QButtonGroup();
    for (int i = 0; i < 6; ++i)
    {
        m_foRadioButtons->addButton(new QRadioButton(QLatin1Literal("Fo ") + QString::number(i + 1)), i);
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

    m_answerDelayLabel->setText(QStringLiteral("Установить задержку ответа (макс 65535)"));
    m_answerDelayValue->setMaximum(65535);
    m_foRadioButtons->button(0)->setChecked(true);

}

void BParWidget::ConnectObjects()
{
    connect(m_onBparButton, &QPushButton::clicked, this,  &BParWidget::OnCollectParam);
    connect(m_radioImpulsButton, &QRadioButton::toggled, this,  &BParWidget::OnSetRadioImpulsEnabled);
}

void BParWidget::OnSetRadioImpulsEnabled(bool state)
{
    m_tksComboBox->setEnabled(state);
}

void BParWidget::OnCollectParam()
{
    QByteArray bParMessage;
    QDataStream stream(&bParMessage, QIODevice::WriteOnly);
    quint8 bpar_mode;
    for (QAbstractButton *foRadioButton : m_foRadioButtons->buttons())
    {
        if (foRadioButton->isChecked())
        {
            bpar_mode = m_foRadioButtons->id(foRadioButton); //000 - 3 байте
        }
    }
    if (m_lchmButton->isChecked())
    {
        const quint8 lcmMode = 8; //1000
        bpar_mode += lcmMode;
    }
    else
    {
        const quint8 tkComboVal=m_tksComboBox->currentIndex() + 1;
        const quint8 tkVal = tkComboVal << 4;//10000 или 20000
        bpar_mode += tkVal;
    }
    const quint16 delay = quint16(m_answerDelayValue->value());
    stream << (quint8)6;
    stream << (quint8)5;
    stream << bpar_mode;
    stream << delay;
    Q_EMIT ToSendBParMessage(bParMessage);

}
