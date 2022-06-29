#include "statemessagegetter.h"

#include <QByteArray>
#include <QDataStream>
#include <QtEndian>
#include <QDebug>
#include <QtMath>

StateMessageGetter::StateMessageGetter(const double f, const double fref, const quint32 distanseToAnswerer)
    : m_indexOfGettingMessageId(1)
    , m_subMessageIndex(m_indexOfGettingMessageId + 1)
    , f(f)
    , Fref(fref)
    , distanseToAnswerer(distanseToAnswerer)
    , m_currentFvco(0)
{

}

StateMessageGetter::~StateMessageGetter()
{

}

QString StateMessageGetter::GetDataFromMessage(const QByteArray &message)
{
    qDebug() << "GET" << message.toHex();
    const quint8 sendedMessageId = message.at(m_indexOfGettingMessageId);
    switch (sendedMessageId)
    {
    case 1:
        return GetFvcoFromFirstMessage(message);
    case 2:
    {
        const quint8 subMessageId = message.at(m_subMessageIndex);
        if (1 == subMessageId)
        {
            return GetFvcoFromSecondMessage(message);
        }
        else
        {
            if (2 == subMessageId)
            {
                return GetDoplerFromSecondMessage(message);
            }
            else
            {
                qFatal("Необрабатываемая ошибка");
            }
        }
        break;
    }
    case 3:
        return GetDistanceFromThirdMessage(message);
    case 4:
        return GetGainTxGainRXFromFourthMessage(message);
    case 5:
        return GetAttenuatorRXFromFiveMessage(message);
    case 6:
        return GetWorkModeFromSixMessage(message);
    default:
        return QStringLiteral("OK");
    }
}

QString StateMessageGetter::GetFvcoFromFirstMessage(const QByteArray &message)
{
    if (message.count() == 8)
    {
        qDebug() << QStringLiteral("Приняли ") << message.toHex();
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

        bool DIV_RX = message.at(7);

        //Значение сидит только здесь, первую парсить не нужно

        double FRACT_RX_BIG = FRACT_RX;
        double pow = qPow(2, 20);
        FRACT_RX_BIG = FRACT_RX_BIG / pow;
        FRACT_RX_BIG = FRACT_RX_BIG + INT_RX + 4.0;
        FRACT_RX_BIG = FRACT_RX_BIG / 2.0;
        FRACT_RX_BIG = FRACT_RX_BIG * Fref * qPow(2, DIV_RX);

        quint16 FRACT_RX_RETURNED = (quint16)qCeil(FRACT_RX_BIG / 1000000.0);
        FRACT_RX_RETURNED = FRACT_RX_RETURNED + 3;

        m_currentFvco = FRACT_RX_RETURNED;
        const QString result = QStringLiteral("Рабочая точка Rx равна Fvco= %1 Мегагерц").arg(FRACT_RX_RETURNED);
        return result;
    }
    return QLatin1String();
}

QString StateMessageGetter::GetFvcoFromSecondMessage(const QByteArray &message)
{
    if (message.count() == 9)
    {
        qDebug() << QStringLiteral("Приняли ") << message.toHex();
        QByteArray arrayINTRX;
        arrayINTRX.append(message.at(3));
        arrayINTRX.append(message.at(4));
        QDataStream IntDataStream(arrayINTRX);
        quint16 INT_RX;
        IntDataStream >> INT_RX;

        QByteArray arrayFRACTRX;
        arrayFRACTRX.append(static_cast<char>(0x00));//иначе будет 0 нужно 4 байта
        arrayFRACTRX.append(message.at(5));
        arrayFRACTRX.append(message.at(6));
        arrayFRACTRX.append(message.at(7));

        QDataStream fractDataStream(arrayFRACTRX);
        quint32 FRACT_RX;
        fractDataStream >> FRACT_RX;

        bool DIV_RX = message.at(8);

        //Значение сидит только здесь, первую парсить не нужно

        double FRACT_RX_BIG = FRACT_RX;
        double pow = qPow(2, 20);
        FRACT_RX_BIG = FRACT_RX_BIG / pow;
        FRACT_RX_BIG = FRACT_RX_BIG + INT_RX + 4.0;
        FRACT_RX_BIG = FRACT_RX_BIG / 2.0;
        FRACT_RX_BIG = FRACT_RX_BIG * Fref * qPow(2, DIV_RX);

        quint16 FRACT_RX_RETURNED = (quint16)qCeil(FRACT_RX_BIG / 1000000.0);
        FRACT_RX_RETURNED = FRACT_RX_RETURNED + 3;

        m_currentFvco = FRACT_RX_RETURNED;
        const QString result = QStringLiteral("Рабочая точка Tx равна Fvco= %1 Мегагерц").arg(FRACT_RX_RETURNED);
        return result;
    }
    return QLatin1String();
}

QString StateMessageGetter::GetDoplerFromSecondMessage(const QByteArray &message)
{
    if (message.count() == 6)
    {
        qDebug() << QStringLiteral("Приняли ") << message.toHex();

        QByteArray arrayDopler;
        arrayDopler.append(static_cast<char>(0x00));//иначе будет 0 нужно 4 байта
        arrayDopler.append(message.at(3));
        arrayDopler.append(message.at(4));
        arrayDopler.append(message.at(5));

        QDataStream doplerDataStream(arrayDopler);
        quint32 dopler;
        doplerDataStream >> dopler;

        const QString result = QStringLiteral("Частота Доплера= %1 герц").arg(qAbs(dopler));
        return result;
    }
    return QLatin1String();
}

QString StateMessageGetter::GetDistanceFromThirdMessage(const QByteArray &message) const
{
    if (message.count() == 4)
    {
        QByteArray distance;
        distance.append(message.at(2));
        distance.append(message.at(3));
        QDataStream IntDataStream(distance);
        quint16 distance_INT;
        IntDataStream >> distance_INT;
        double realDistance = distance_INT - 1.0;
        realDistance = realDistance / 2.0 * c / f;
        realDistance = realDistance + distanseToAnswerer;
        const QString result = QStringLiteral("Дистанция = %1 метров").arg(realDistance);
        return result;
    }
    return QLatin1String();
}

QString StateMessageGetter::GetGainTxGainRXFromFourthMessage(const QByteArray &message) const
{

    if (message.count() == 4)
    {
        const double GAIN_TX = message.at(2) / 2.0;
        const double GAIN_RX = message.at(3) / 2.0;
        const QString result = QStringLiteral("Усиление TX= %1 децибел Усиление RX= %2 децибел").arg(GAIN_TX).arg(GAIN_RX);
        return result;
    }
    return QLatin1String();
}

QString StateMessageGetter::GetAttenuatorRXFromFiveMessage(const QByteArray &message) const
{
    if (message.count() == 3)
    {
        const quint8 Attenuator_RX = quint8(message.at(2));
        const quint8 realValue = atteniatorTable.key(Attenuator_RX);
        const QString result = QStringLiteral("Ослабление= %1 децибел").arg(realValue);
        return result;
    }
    return QLatin1String();
}

QString StateMessageGetter::GetWorkModeFromSixMessage(const QByteArray &message) const
{
    if (message.count() == 3)
    {
        const quint8 WorkModeIndex = quint8(message.at(2));
        if (WorkModeIndex < noiseValues.count())
        {
            const QString result = "Рабочий режим: " + noiseValues.at(WorkModeIndex);
            return result;
        }

    }
    return QLatin1String();
}
