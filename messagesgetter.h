#ifndef MESSAGESGETTER_H
#define MESSAGESGETTER_H

#include <QByteArray>

#include "constantsandfunctions.h"

class MessagesGetter
{
public:
    MessagesGetter();
    ~MessagesGetter();
    QString getDataFromMessage(QByteArray &message);
    QString getFvcoFromFirstMessage(QByteArray &message);
    const QString getGainTxGainRXFromFourthMessage(QByteArray &message);
    QString getAttenuatorRXFromFiveMessage(QByteArray &message);
private:
    const int indexOfSendedMessage;
};

#endif // MESSAGESGETTER_H
