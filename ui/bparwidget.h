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

class BParWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit BParWidget(QWidget *parent);
    ~BParWidget();
private:
    void CreateObjects();
    void CreateUI();
    void InsertWidgetsIntoLayout();
    void FillUI();
    void ConnectObjects();
Q_SIGNALS:
    void ToSendBParMessage(const QByteArray &message);
private Q_SLOTS:
    void OnSetRadioImpulsEnabled(bool state);
    void OnCollectParam();
private:
    QHBoxLayout *m_mainLayout;


    QVBoxLayout *m_powerAndDelayLayout;
    QPushButton *m_onBparButton;

    QHBoxLayout *m_radioImpulsLayout;
    QRadioButton *m_radioImpulsButton;
    QComboBox* m_tksComboBox;

    QRadioButton *m_lchmButton;
    QLabel *m_answerDelayLabel;
    QSpinBox *m_answerDelayValue;

    QVBoxLayout *m_foLayouyt;
    QButtonGroup *m_foRadioButtons;
};

#endif // BPARWIDGET_H
