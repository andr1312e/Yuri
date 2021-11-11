#include "statemessagegetter.h"

#include <QByteArray>
#include <QDataStream>
#include <QtEndian>
#include <QDebug>
#include <QtMath>

StateMessageGetter::StateMessageGetter()
    :m_indexOfGettingMessageId(1)
{

}

StateMessageGetter::~StateMessageGetter()
{

}

const QString StateMessageGetter::getDataFromMessage(const QByteArray &message) const
{
//    qDebug()<< "GET"<<message.toHex();
    quint8 sendedMessageId=message.at(m_indexOfGettingMessageId);
    switch (sendedMessageId) {
    case 1:
        return getFvcoFromFirstMessage(message);
    case 4:
        return getGainTxGainRXFromFourthMessage(message);
    case 5:
        return getAttenuatorRXFromFiveMessage(message);
    default:
        return QStringLiteral(" StateMessageGetter::getDataFromMessage ничего");
    }
}

const QString StateMessageGetter::getFvcoFromFirstMessage(const QByteArray &message) const
{
    if(message.count()==8)
    {
        bool DIV_RX=message.at(7);
        QByteArray arrayINTRX(2, ' ');
        arrayINTRX[0] = message.at(2);
        arrayINTRX[1] = message.at(3);
        QDataStream ds(arrayINTRX);
        quint16 INT_RX=0;
        ds >> INT_RX;
        quint64 INT_RX_BIG=INT_RX;
        INT_RX_BIG=INT_RX_BIG+4;
        INT_RX_BIG=INT_RX_BIG*Fref;
        INT_RX_BIG=INT_RX_BIG*qPow(2,DIV_RX);
        INT_RX_BIG=INT_RX_BIG/2;
        INT_RX_BIG=INT_RX_BIG+3000000.0;
        INT_RX_BIG=INT_RX_BIG/1000000;
        const QString result= QStringLiteral("Рабочая точка равна Fvco= %1 Мегагерц").arg(INT_RX_BIG);
        return result;
    }
    return QLatin1String();
}

const QString StateMessageGetter::getGainTxGainRXFromFourthMessage(const QByteArray &message) const
{

    if (message.count()==4)
    {
        quint8 GAIN_TX=message.at(2)*2.0;
        quint8 GAIN_RX=message.at(3)*2.0;
        const QString result= QStringLiteral("Усиление TX= %1 децибел Усиление RX= %2 децибел").arg(GAIN_TX).arg(GAIN_RX);
        return result;
    }
    return QLatin1String();
}

const QString StateMessageGetter::getAttenuatorRXFromFiveMessage(const QByteArray &message) const
{
    if (message.count()==3)
    {
        quint8 Attenuator_RX=quint8(message.at(2));
        quint8 realValue=atteniatorTable.key(Attenuator_RX);
        const QString result= QStringLiteral("Ослабление= %1 децибел").arg(realValue);
        return result;
    }
    return QLatin1String();
}
