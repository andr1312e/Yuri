#ifndef UI_DATAWIDGET_H
#define UI_DATAWIDGET_H

#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStringList>
#include <QVBoxLayout>

#include "doubleslider.h"
#include "services/settingfileservice.h"

class DataWidget : public QGroupBox
{
    Q_OBJECT
public:
    DataWidget(SettingFileService *settingFileService, QWidget *parent);
    ~DataWidget();
private:
    void CreateObjects();
    void CreateUI();
    void FillUI();
    void InsertWidgetsIntoLayout();
    void ConnectObjects();
Q_SIGNALS:
    void ToSetState(int messageId, QList<double> params);
    void ToGetState(int messageId);
    void ToConsoleLog(const QString &message);
private Q_SLOTS:
    void OnChangeFvcoComboBoxValue(const QString &fvco);
    void OnChangeDoplerLineEdit(const QString &doplerText);
    void OnChangeSpeedLineEdit(const QString &speedText);
    void OnChangeRangeLineEdit(const QString &range);
    void OnChangeGainTxLineEdit(double gainTx);
    void OnChangeGainRxLineEdit(double gainRx);
    void OnChangeAttenuator(int attenuatorIndex);
    void OnChangeSinusLineEdit(const QString &sinus);
private Q_SLOTS:
    void OnSetRxButtonClicked();
    void OnSetTxButtonClicked();
    void OnGetRxButtonClicked();
    void OnGetTxButtonClicked();
    void OnSetDoplerButtonClicked();
    void OnGetDoplerButtonClicked();
    void OnSetRangeButtonClicked();
    void OnGetRangeButtonClicked();
    void OnSetGainButtonClicked();
    void OnGetGainButtonClicked();
    void OnSetAttenuatorButtonClicked();
    void OnGetAttenuatorButtonClicked();
    void OnGetCoordinatsButtonClicked();
public Q_SLOTS:
    void OnUpdateLatLong(const QString &message);
public:
    quint32 GetSinusValue();
private:
    SettingFileService *const m_settingFileService;
    QStringList m_attenuatorValues;
    const quint32 m_c = 299792458;
    QIntValidator *m_intValidator;
    QIntValidator *m_gainValidator;
private:
    QVBoxLayout *m_mainLayout;

    QHBoxLayout *m_workPointLineLayout;
    QLabel *m_fvcoLabel;
    QComboBox *m_fvcoComboBox;
    QPushButton *m_setRxButton;
    QPushButton *m_setTxButton;
    QPushButton *m_setTxAndRxButton;
    QPushButton *m_getRxButton;
    QPushButton *m_getTxButton;

    QHBoxLayout *m_doplerAndSpeedLayout;
    QLabel *m_DoplerFreqLabel;
    QLineEdit *m_doplerFreqLineEdit;
    QLabel *m_speedLabel;
    QLineEdit *m_speedLineEdit;
    QPushButton *m_setDoplerButton;
    QPushButton *m_getDoplerButton;

    QHBoxLayout *m_rangeLineLayout;
    QLabel *m_rangeLabel;
    QLineEdit *m_rangeLineEdit;
    QPushButton *m_setRangeButton;
    QPushButton *m_getRangeButton;

    QHBoxLayout *m_gainLineLayout;
    QLabel *m_gainTXLabel;
    DoubleSlider *m_gainTXSlider;
    QLabel *m_gainTxValue;
    QLabel *m_gainRXLabel;
    DoubleSlider *m_gainRXSlider;
    QLabel *m_gainRxValue;
    QPushButton *m_setGainButton;
    QPushButton *m_getGainButton;

    QHBoxLayout *m_attenuatorLineLayout;
    QLabel *m_attenuatorLabel;
    QComboBox *m_attenuatorComboBox;
    QPushButton *m_setAttenuatorButton;
    QPushButton *m_getAttenuatorButton;

    QHBoxLayout *m_sinusValLineLayout;
    QLabel *m_sinusValueLabel;
    QLineEdit *m_sinusLineEdit;

    QHBoxLayout *m_cooridnatesLayout;
    QLabel *m_coordinatesLabel;
    QLabel *m_coordinatesValue;
    QPushButton *m_getCoordinatesButton;
};

#endif // UI_DATAWIDGET_H
