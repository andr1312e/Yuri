#include "messagesgetter.h"

#include <QByteArray>
#include <QDataStream>
#include <QtEndian>
#include <QDebug>

MessagesGetter::MessagesGetter()
    :indexOfSendedMessage(2)
{

}

MessagesGetter::~MessagesGetter()
{

}

QString MessagesGetter::getDataFromMessage(QByteArray &message)
{
    qDebug()<< message.toHex();
    quint8 sendedMessageId=message.at(2);
    switch (sendedMessageId) {
    case 1:
        return getFvcoFromFirstMessage(message);
    case 4:
        return getGainTxGainRXFromFourthMessage(message);
    case 5:
        return getAttenuatorRXFromFiveMessage(message);
    default:
        return " ничего";
    }
}

QString MessagesGetter::getFvcoFromFirstMessage(QByteArray &message)
{
    if(message.size()>8)
    {
        bool DIV_RX=message.at(8);
        QByteArray arrayINTRX(2, ' ');
        arrayINTRX[0] = message[3];
        arrayINTRX[1] = message[4];
        QDataStream ds(arrayINTRX);
        quint16 INT_RX=0;
        ds >> INT_RX;
        quint64 INT_RX_BIG=INT_RX;
        INT_RX_BIG=INT_RX_BIG+4;
        INT_RX_BIG=INT_RX_BIG*Fref;
        INT_RX_BIG=INT_RX_BIG*pow(2,DIV_RX);
        INT_RX_BIG=INT_RX_BIG/2;
        INT_RX_BIG=INT_RX_BIG+3000000.0;
        INT_RX_BIG=INT_RX_BIG/1000000;
        const QString result= QString("Рабочая точка равна Fvco= %1 Мегагерц").arg(INT_RX_BIG);
        return result;
    }
    return QString::fromLatin1("");
}

const QString MessagesGetter::getGainTxGainRXFromFourthMessage(QByteArray &message)
{

    if (message.size()==5)
    {
        quint8 GAIN_RX=message.at(3);
        quint8 GAIN_TX=message.at(4);
        const QString result= QString("Усиление TX= %1 децибел Усиление RX= %2 децибел").arg(GAIN_TX).arg(GAIN_RX);
        return result;
    }
    return QString::fromLatin1("");
}

QString MessagesGetter::getAttenuatorRXFromFiveMessage(QByteArray &message)
{
    if (message.size()==4)
    {
        quint8 Attenuator_RX=quint8(message.at(3));
        quint8 realValue=atteniatorTable.key(Attenuator_RX);
        const QString result= QString("Ослабление= %1 децибел").arg(realValue);
        return result;
    }
    return QString::fromLatin1("");
}
