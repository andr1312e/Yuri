#ifndef MESSAGECREATORS_STATEMESSAGEGETTER_H
#define MESSAGECREATORS_STATEMESSAGEGETTER_H

#include <QByteArray>

#include "constantsandfunctions.h"

class StateMessageGetter
{
public:
    StateMessageGetter();
    ~StateMessageGetter();
    const QString getDataFromMessage(const QByteArray &message) const;
private:
    const QString getFvcoFromFirstMessage(const QByteArray &message) const;
    const QString getGainTxGainRXFromFourthMessage(const QByteArray &message) const;
    const QString getAttenuatorRXFromFiveMessage(const QByteArray &message) const;
private:
    const int m_indexOfGettingMessageId;
};

#endif // MESSAGECREATORS_STATEMESSAGEGETTER_H
