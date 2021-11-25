#ifndef MESSAGECREATORS_STATEMESSAGEGETTER_H
#define MESSAGECREATORS_STATEMESSAGEGETTER_H

#include <QByteArray>

#include "constantsandfunctions.h"

class StateMessageGetter
{
public:
    StateMessageGetter();
    ~StateMessageGetter();
    const QString GetDataFromMessage(const QByteArray &message);
private:
    const QString GetFvcoFromFirstMessage(const QByteArray &message);
    const QString GetDoplerFromSecondMessage(const QByteArray &message);
    const QString GetDistanceFromThirdMessage(const QByteArray &message);
    const QString GetGainTxGainRXFromFourthMessage(const QByteArray &message);
    const QString GetAttenuatorRXFromFiveMessage(const QByteArray &message);
    const QString GetWorkModeFromSixMessage(const QByteArray &message);
private:
    quint64 m_currentFvco;
    const int m_indexOfGettingMessageId;
};

#endif // MESSAGECREATORS_STATEMESSAGEGETTER_H
