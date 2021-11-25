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

const QString StateMessageGetter::GetDataFromMessage(const QByteArray &message) const
{
    //    qDebug()<< "GET"<<message.toHex();
    quint8 sendedMessageId=message.at(m_indexOfGettingMessageId);
    switch (sendedMessageId) {
    case 1:
        return GetFvcoFromFirstMessage(message);
    case 4:
        return GetGainTxGainRXFromFourthMessage(message);
    case 5:
        return GetAttenuatorRXFromFiveMessage(message);
    default:
        return QStringLiteral(" StateMessageGetter::getDataFromMessage ничего");
    }
}

const QString StateMessageGetter::GetFvcoFromFirstMessage(const QByteArray &message) const
{
    if(message.count()==8)
    {
        qDebug()<< QStringLiteral("Приняли ") << message.toHex();
        QByteArray arrayINTRX;
        arrayINTRX.append(message.at(2));
        arrayINTRX.append(message.at(3));
        QDataStream IntDataStream(arrayINTRX);
        quint16 INT_RX;
        IntDataStream >> INT_RX;

        QByteArray arrayFRACTRX;
        arrayFRACTRX.append(static_cast<char>(0x00));//иначе будет 0 нужно 4 байта
        arrayFRACTRX.append(message.at(4));
        arrayFRACTRX.append(message.at(5));
        arrayFRACTRX.append(message.at(6));

        QDataStream fractDataStream(arrayFRACTRX);
        quint32 FRACT_RX;
        fractDataStream >> FRACT_RX;

        bool DIV_RX=message.at(7);

        //Значение сидит только здесь, первую парсить не нужно

        double FRACT_RX_BIG=FRACT_RX;
        double pow=qPow(2, 20);
        FRACT_RX_BIG=FRACT_RX_BIG/pow;
        FRACT_RX_BIG=FRACT_RX_BIG+INT_RX+4.0;
        FRACT_RX_BIG=FRACT_RX_BIG/2.0;
        FRACT_RX_BIG=FRACT_RX_BIG*Fref*qPow(2, DIV_RX);

        quint32 FRACT_RX_RETURNED=(quint32)qCeil(FRACT_RX_BIG/1000000.0);
        FRACT_RX_RETURNED=FRACT_RX_RETURNED+3;


        const QString result= QStringLiteral("Рабочая точка равна Fvco= %1 Мегагерц").arg(FRACT_RX_RETURNED);
        return result;
    }
    return QLatin1String();
}

const QString StateMessageGetter::GetGainTxGainRXFromFourthMessage(const QByteArray &message) const
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

const QString StateMessageGetter::GetAttenuatorRXFromFiveMessage(const QByteArray &message) const
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
