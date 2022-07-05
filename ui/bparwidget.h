#ifndef BPARWIDGET_H
#define BPARWIDGET_H

#include <QHBoxLayout>
#include <QList>
#include <QWidget>
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QButtonGroup>
#include <QGroupBox>
#include <QDebug>
#include <QCheckBox>

#include "presenter/statepresenter.h"

class BParWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit BParWidget(StatePresenter *presenter, QWidget *parent);
    ~BParWidget();
private:
    void CreateObjects();
    void CreateUI();
    void InsertWidgetsIntoLayout();
    void FillUI();
    void ConnectObjects();
private Q_SLOTS:
    void OnSetRadioImpulsEnabled(bool state);
    void OnCollectParam();
    void OnThresoldComboBoxChecked(int state);
    void OnGetBpar();
private:
    StatePresenter *const m_presenter;

private:
    QHBoxLayout *m_mainLayout;


    QVBoxLayout *m_powerAndDelayLayout;
    QPushButton *m_sendBparButton;

    QHBoxLayout *m_radioImpulsLayout;
    QRadioButton *m_radioImpulsButton;
    QComboBox *m_tksComboBox;

    QRadioButton *m_lchmButton;
    QLabel *m_answerDelayLabel;
    QSpinBox *m_answerDelayValue;
    QCheckBox *m_enableThreshold;
    QComboBox *m_thresholdComboBox;

    QVBoxLayout *m_foLayouyt;
    QButtonGroup *m_foRadioButtons;
    QPushButton *m_getBparInfoButton;
};

#endif // BPARWIDGET_H
