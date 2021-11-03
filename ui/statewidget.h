#ifndef UI_STATEWIDGET_H
#define UI_STATEWIDGET_H

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QWidget>
#include <QDir>
#include <QButtonGroup>
#include <tcpsocket.h>

#include <presenter/statepresenter.h>

class StateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StateWidget(TcpSocket *m_socket, QWidget *parent);
    ~StateWidget();
private:
    void CreateUI();
    void FillButtonGroup();
    void InsertWidgetsIntoLayout();
    void FillUI();
    void CreateConnections();

Q_SIGNALS:
    void SetStateMessage(quint8 messageId, double fistParam=0.0, double SecondParam=0.0);


public Q_SLOTS:
    void SetStateButtonIdClicked(int id);
    void GetStateButtonIdClicked(int messageWantToGet);
    void SetButtonEnabled(bool state);
    void ConsoleLog(QString message);

private:
    void updateHistory();
private:
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_speedLayout;
    QLabel *m_DoplerFreqLabel;
    QLineEdit *m_doplerFreqLineEdit;
    QLabel *m_speedLabel;
    QLineEdit *m_speedLineEdit;

    QHBoxLayout *m_rangeLineLayout;
    QLabel *m_rangeLabel;
    QLineEdit *m_rangeLineEdit;

    QHBoxLayout *m_workPointLineLayout;
    QLabel *m_fvcoLabel;
    QComboBox *m_fvcoComboBox;

    QHBoxLayout *m_gainLineLayout;
    QLabel *m_gainTXLabel;
    QLineEdit *m_gainTXLineEdit;
    QLabel *m_gainRXLabel;
    QLineEdit *m_gainRXLineEdit;

    QHBoxLayout *m_attenuatorLineLayout;
    QLabel *m_attenuatorLabel;
    QComboBox *m_attenuatorComboBox;

    QHBoxLayout *m_noiseLineLayout;
    QLabel *m_noiseLabel;
    QComboBox *m_noiseComboBox;

    QHBoxLayout *m_messageSendButtonsLayout;
    QButtonGroup *m_sendStateButtonsGroup;

    QHBoxLayout *m_messageGetButtonsLayout;

    QButtonGroup *m_getStateButtonGroup;

    QHBoxLayout *m_stateLayout;
    QPushButton *m_logClearButton;

    QPlainTextEdit *m_log;
private:
    StatePresenter *m_statePresenter;
    const quint32 c=299792458;
    QFile *m_file;
    QStringList workPointsValues;
    QStringList m_attenuatorValues;
    QStringList m_noiseValues;
    QIntValidator *m_intValidator;
    QIntValidator *m_gainValidator;
private Q_SLOTS:
    void ChangeSpeedLineEdit(const QString &doplerText);
    void ChangeDoplerLineEdit(const QString &speedText);
};

#endif // UI_STATEWIDGET_H
