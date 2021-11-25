#ifndef MESSAGECREATORS_STATEMESSAGEGETTER_H
#define MESSAGECREATORS_STATEMESSAGEGETTER_H

#include <QByteArray>

#include "constantsandfunctions.h"

class StateMessageGetter
{
public:
    StateMessageGetter();
    ~StateMessageGetter();
    const QString GetDataFromMessage(const QByteArray &message) const;
private:
    const QString GetFvcoFromFirstMessage(const QByteArray &message) const;
    const QString GetGainTxGainRXFromFourthMessage(const QByteArray &message) const;
    const QString GetAttenuatorRXFromFiveMessage(const QByteArray &message) const;
private:
    const int m_indexOfGettingMessageId;
};

#endif // MESSAGECREATORS_STATEMESSAGEGETTER_H
