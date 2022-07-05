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
    QString GetFvcoRxFromFirstMessage(const QByteArray &message);
    QString GetFvcoTxFromSecondMessage(const QByteArray &message);
    QString GetDistanceFromThirdMessage(const QByteArray &message) const;
    QString GetGainTxGainRXFromFourthMessage(const QByteArray &message) const;
    QString GetAttenuatorRXFromFiveMessage(const QByteArray &message) const;
    QString GetWorkModeFromSixMessage(const QByteArray &message) const;
    QString GetDoplerFromNineMessage(const QByteArray &message) const noexcept;
private:
    quint8 GetMask(quint8 pos, quint8 size ) const;
private:
    const int m_indexOfGettingMessageId;
    const int m_subMessageIndex;
    const double m_f;
    const double m_c = 299792458.0;
    const double m_Fref;
    const quint32 m_distanseToAnswerer;
};

#endif // MESSAGECREATORS_STATEMESSAGEGETTER_H
