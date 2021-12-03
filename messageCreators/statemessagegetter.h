#ifndef MESSAGECREATORS_STATEMESSAGEGETTER_H
#define MESSAGECREATORS_STATEMESSAGEGETTER_H

#include <QByteArray>

#include "constantsandfunctions.h"

class StateMessageGetter
{
public:
    StateMessageGetter(const double f, const double fref, const quint32 distanseToAnswerer);
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
    const int m_indexOfGettingMessageId;
    const double f;
    const double c=299792458.0;
    const double Fref;
    const quint32 distanseToAnswerer;
private:
    quint64 m_currentFvco;

};

#endif // MESSAGECREATORS_STATEMESSAGEGETTER_H
