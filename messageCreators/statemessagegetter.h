#ifndef MESSAGECREATORS_STATEMESSAGEGETTER_H
#define MESSAGECREATORS_STATEMESSAGEGETTER_H

#include <QByteArray>

#include "constantsandfunctions.h"

class StateMessageGetter
{
public:
    explicit StateMessageGetter(const double f, const double fref, const quint32 distanseToAnswerer);
    ~StateMessageGetter();
    QString GetDataFromMessage(const QByteArray &message);
private:
    QString GetFvcoFromFirstMessage(const QByteArray &message);
    QString GetDoplerFromSecondMessage(const QByteArray &message);
    QString GetDistanceFromThirdMessage(const QByteArray &message) const;
    QString GetGainTxGainRXFromFourthMessage(const QByteArray &message) const;
    QString GetAttenuatorRXFromFiveMessage(const QByteArray &message) const;
    QString GetWorkModeFromSixMessage(const QByteArray &message) const;
private:
    const int m_indexOfGettingMessageId;
    const double f;
    const double c = 299792458.0;
    const double Fref;
    const quint32 distanseToAnswerer;
private:
    quint64 m_currentFvco;

};

#endif // MESSAGECREATORS_STATEMESSAGEGETTER_H
