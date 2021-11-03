#ifndef MESSAGECREATORS_STATEMESSAGEGETTER_H
#define MESSAGECREATORS_STATEMESSAGEGETTER_H

#include <QByteArray>

#include "constantsandfunctions.h"

class StateMessageGetter
{
public:
    StateMessageGetter();
    ~StateMessageGetter();
    const QString getDataFromMessage(QByteArray &message);
private:
    const QString getFvcoFromFirstMessage(QByteArray &message);
    const QString getGainTxGainRXFromFourthMessage(QByteArray &message);
    const QString getAttenuatorRXFromFiveMessage(QByteArray &message);
private:
    const int m_indexOfGettingMessageId;
};

#endif // MESSAGECREATORS_STATEMESSAGEGETTER_H
