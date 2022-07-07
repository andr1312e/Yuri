#include "statemessagegetter.h"

#include <QByteArray>
#include <QDataStream>
#include <QtEndian>
#include <QDebug>
#include <QtMath>

StateMessageGetter::StateMessageGetter(const double f, const double fref, const quint32 distanseToAnswerer)
    : m_indexOfGettingMessageId(1)
    , m_subMessageIndex(m_indexOfGettingMessageId + 1)
    , m_f(f)
    , m_Fref(fref)
    , m_distanseToAnswerer(distanseToAnswerer)
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
        return GetFvcoRxFromFirstMessage(message);
    case 2:
        return GetFvcoTxFromSecondMessage(message);
    case 3:
        return GetDistanceFromThirdMessage(message);
    case 4:
        return GetGainTxGainRXFromFourthMessage(message);
    case 5:
        return GetAttenuatorRXFromFiveMessage(message);
    case 6:
        return GetWorkModeFromSixMessage(message);
    case 9:
        return GetDoplerFromNineMessage(message);
    default:
        return QStringLiteral("OK");
    }
}

QString StateMessageGetter::GetFvcoRxFromFirstMessage(const QByteArray &message)
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
        const double pow = qPow(2, 20);
        FRACT_RX_BIG = FRACT_RX_BIG / pow;
        FRACT_RX_BIG = FRACT_RX_BIG + INT_RX + 4.0;
        FRACT_RX_BIG = FRACT_RX_BIG / 2.0;
        FRACT_RX_BIG = FRACT_RX_BIG * m_Fref * qPow(2, DIV_RX);

        quint16 FRACT_RX_RETURNED = (quint16)qCeil(FRACT_RX_BIG / 1000000.0);
        FRACT_RX_RETURNED = FRACT_RX_RETURNED + 3;

        const QString result = QStringLiteral("Рабочая точка Rx равна Fvco= %1 Мегагерц").arg(FRACT_RX_RETURNED);
        return result;
    }
    return QLatin1String();
}

QString StateMessageGetter::GetFvcoTxFromSecondMessage(const QByteArray &message)
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

        const bool DIV_RX = message.at(7);

        //Значение сидит только здесь, первую парсить не нужно

        double FRACT_RX_BIG = FRACT_RX;
        const double pow = qPow(2, 20);
        FRACT_RX_BIG = FRACT_RX_BIG / pow;
        FRACT_RX_BIG = FRACT_RX_BIG + INT_RX + 4.0;
        FRACT_RX_BIG = FRACT_RX_BIG / 2.0;
        FRACT_RX_BIG = FRACT_RX_BIG * m_Fref * qPow(2, DIV_RX);

        quint16 FRACT_RX_RETURNED = (quint16)qCeil(FRACT_RX_BIG / 1000000.0);
        FRACT_RX_RETURNED = FRACT_RX_RETURNED + 3;

        const QString result = QStringLiteral("Рабочая точка Tx равна Fvco= %1 Мегагерц").arg(FRACT_RX_RETURNED);
        return result;
    }
    return QLatin1String();
}

QString StateMessageGetter::GetDoplerFromNineMessage(const QByteArray &message) const noexcept
{
    if (message.count() == 5)
    {
        qDebug() << QStringLiteral("Приняли ") << message.toHex();

        QByteArray arrayDopler;
        arrayDopler.append(static_cast<char>(0x00));//иначе будет 0, нужно 4 байта
        arrayDopler.append(message.at(2));
        arrayDopler.append(message.at(3));
        arrayDopler.append(message.at(4));

        QDataStream doplerDataStream(arrayDopler);
        quint32 dopler;
        doplerDataStream >> dopler;

        const QString result = QStringLiteral("Частота Доплера= %1 герц").arg(qAbs(dopler));
        return result;
    }
    return QLatin1String();
}

quint8 StateMessageGetter::GetMask(quint8 pos, quint8 size) const
{
    //size - размер числа * колво бит нужно
    //pos - позиция числаы
    return ~( ~0ull << size ) << pos;
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
        realDistance = realDistance / 2.0 * m_c / m_f;
        realDistance = realDistance + m_distanseToAnswerer;
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
    else
    {
        if (message.count() == 6)
        {
            if (message.at(2) == 3)
            {
                QByteArray arraySinusVal;
                arraySinusVal.append(static_cast<char>(0x00));//иначе будет 0, нужно 4 байта
                arraySinusVal.append(message.at(3));
                arraySinusVal.append(message.at(4));
                arraySinusVal.append(message.at(5));

                QDataStream sinusDataStream(arraySinusVal);
                quint32 sinus;
                sinusDataStream >> sinus;
                const QString result = "Рабочий режим: " + noiseValues.at(2) + " значение синуса равно: " + QString::number(sinus);
            }
            else
            {
                const quint8 bpar_mode = message.at(3);

                //было  11000000
                //стало 00000110
                //-1 так как смотри таблицу, там первый начинается с 0
                const quint8 fo = (bpar_mode >> 5) - 1;

                //позиция начинается справа
                //было  11011000  = 216
                //маска 00010000  = 16
                //сдвигаем на 4 разряда и получаем или 0 или 1
                const quint8 hasLcmPos = 4;
                const quint8 hasLcmMask = GetMask(hasLcmPos, sizeof(  quint8 ) );
                const bool hasLcm = ( bpar_mode & hasLcmMask ) >> hasLcmPos;
                const QString modeString = hasLcm ? "ЛЧМ" : "Радиоимпульс";
                //позиция начинается справа
                //было  11011000  = 216
                //маска 00001100  = 12
                //сдвигаем на 4 разряда и получаем или 2 битовое число
                quint8 tKmode = 0;
                if (!hasLcm)
                {
                    const quint8 tKmodePos = 2;
                    quint8 tKMask = GetMask(tKmodePos, sizeof(  quint8 ) * 2 );
                    tKmode = ( bpar_mode & tKMask ) >> tKmodePos;
                }
                const QString tkModeString = hasLcm ? "" : " Тк:" + QString::number(tKmode);
                QByteArray signalDelayArray;
                signalDelayArray.append(message.at(4));
                signalDelayArray.append(message.at(5));

                QDataStream sinusDataStream(signalDelayArray);
                quint16 signalDelay;
                sinusDataStream >> signalDelay;

                return QString ("Режим БПАР. Параметры работы:"
                                " F" + QString::number(fo) +
                                " Режим: " + modeString + tkModeString +
                                " Задержка сигнала: " + QString::number(signalDelay));
            }
        }
    }
    return QLatin1String();
}

