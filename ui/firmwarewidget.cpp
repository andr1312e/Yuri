#include "firmwarewidget.h"

FirmWareWidget::FirmWareWidget(QWidget *parent)
    : QWidget(parent)
    , m_firmwarePresenter(new FirmwarePresenter(this))
{
    CreateUI();
    InsertWidgetsIntoLayout();
    FillUI();
    ConnectObjects();
}

FirmWareWidget::~FirmWareWidget()
{
    delete m_firmwarePresenter;

    delete m_mainLayout;

    delete m_firmwareFlasherWidget;
    delete m_firmwareLogWidget;
    delete m_progressBar;

    delete m_splitter;

}

void FirmWareWidget::CreateUI()
{
    m_mainLayout=new QVBoxLayout();


    m_splitter=new QSplitter();

    m_firmwareLogWidget=new FirmwareLogWidget(this);
    m_firmwareFlasherWidget=new FirmwareFlasherWidget(this);
    m_progressBar=new QProgressBar(this);
}

void FirmWareWidget::InsertWidgetsIntoLayout()
{
    m_splitter->addWidget(m_firmwareLogWidget);
    m_splitter->addWidget(m_firmwareFlasherWidget);

    m_mainLayout->addWidget(m_splitter);
    m_mainLayout->addWidget(m_progressBar);

    setLayout(m_mainLayout);
}

void FirmWareWidget::FillUI()
{
    m_progressBar->setRange(0, 100);
    m_splitter->addWidget(m_firmwareFlasherWidget);
    //    SetButtonsEnabled(false);
}

void FirmWareWidget::ConnectObjects()
{
    connect(this, &FirmWareWidget::ToConsoleLog, m_firmwareLogWidget, &FirmwareLogWidget::OnConsoleLog);
    connect(m_firmwareFlasherWidget, &FirmwareFlasherWidget::ToFlash, m_firmwarePresenter, &FirmwarePresenter::OnFlash);
    connect(m_firmwareFlasherWidget, &FirmwareFlasherWidget::ToStartReadingFirmWareFromDevice, m_firmwarePresenter, &FirmwarePresenter::OnStartReadingFirmWareFromDevice);
    connect(m_firmwarePresenter, &FirmwarePresenter::ToConsoleLog,  m_firmwareLogWidget, &FirmwareLogWidget::OnConsoleLog);
    connect(m_firmwarePresenter, &FirmwarePresenter::ToSetButtonsEnabled, this, &FirmWareWidget::OnSetButtonsEnabled);
    connect(m_firmwareLogWidget, &FirmwareLogWidget::ToRestartDevice, [&](){m_firmwarePresenter->SendMessageToQueue(0, 0, 0);});
    connect(m_firmwarePresenter, &FirmwarePresenter::ToSetMaximumCountOfPages, this, &FirmWareWidget::OnSetMaximumProgressBar);
    connect(m_firmwarePresenter, &FirmwarePresenter::ToProgressBarUpdate, m_progressBar, &QProgressBar::setValue);
    connect(m_firmwareFlasherWidget, &FirmwareFlasherWidget::ToSetFirmwareFromFileToPresenter, m_firmwarePresenter, &FirmwarePresenter::OnSetFirmwareFromFileToPresenter);
}

void FirmWareWidget::OnSetButtonsEnabled(bool state)
{
    m_firmwareFlasherWidget->setEnabled(state);
    m_firmwareLogWidget->setEnabled(state);
}

void FirmWareWidget::OnSetMaximumProgressBar(int top)
{
    m_progressBar->reset();
    m_progressBar->setRange(0, top);
}

void FirmWareWidget::OnUpdateProgressBar(int page)
{
    m_progressBar->setValue(page);
}

void FirmWareWidget::DisconnectOldHander()
{
    m_firmwarePresenter->DisconnectOldHandler();
}

void FirmWareWidget::ConnectHander(DataHandler *dataHandler)
{
    m_firmwarePresenter->ConnectHander(dataHandler);
}
