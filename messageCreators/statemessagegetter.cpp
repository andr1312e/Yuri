#include "statemessagegetter.h"

#include <QByteArray>
#include <QDataStream>
#include <QtEndian>
#include <QDebug>
#include <QtMath>

StateMessageGetter::StateMessageGetter(const double f, const double fref, QObject *parent)
    : QObject(parent)
    , m_indexOfGettingMessageId(1)
    , m_subMessageIndex(m_indexOfGettingMessageId + 1)
    , m_f(f)
    , m_Fref(fref)
{
}

StateMessageGetter::~StateMessageGetter()
{

}

QString StateMessageGetter::GetDataFromMessage(const QByteArray &message)
{
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
        qFatal("Wrong Message");
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
    //pos - позиция числа
    return ~( ~0ull << size ) << pos;
}

void StateMessageGetter::ParceKoordinatesMessage()
{
//    qDebug() << m_collectedKoordinatesData;
    const int commaCount = m_collectedKoordinatesData.count(',');
    if (13 == commaCount)
    {
        double latitude = 0.0, longtutude = 0.0;
        QVector<int> indexOfCommasArray{m_collectedKoordinatesData.indexOf(',')};
        for (int i = 1; i < commaCount; ++i)
        {
            indexOfCommasArray.append(m_collectedKoordinatesData.indexOf(',', indexOfCommasArray.at(i - 1) + 1));
        }
        if (indexOfCommasArray.at(3) == indexOfCommasArray.at(4) - 2)
        {
            const QByteArray latitudeByteArray = m_collectedKoordinatesData.mid(indexOfCommasArray.at(2) + 1, indexOfCommasArray.at(3) - indexOfCommasArray.at(2) - 1);
            const char latitudeDirection = m_collectedKoordinatesData.at(indexOfCommasArray.at(4) - 1);
            const int directionValue = latitudeDirection == 'N' ? 1 : -1;
            latitude = ParceLatitdeValue(latitudeByteArray, directionValue);
        }
        else
        {
            Q_EMIT ToConsoleLog("Сообщение не верно");
        }


        if (indexOfCommasArray.at(5) == indexOfCommasArray.at(6) - 2)
        {
            const QByteArray longtutudeByteArray = m_collectedKoordinatesData.mid(indexOfCommasArray.at(4) + 1, indexOfCommasArray.at(5) - indexOfCommasArray.at(4) - 1);
            const char longtutudeDirection = m_collectedKoordinatesData.at(indexOfCommasArray.at(6) - 1);
            const int directionValue = longtutudeDirection == 'E' ? 1 : -1;
            longtutude = ParceLongtitude(longtutudeByteArray, directionValue);
        }
        else
        {
            Q_EMIT ToConsoleLog("Сообщение не верно");
        }
        const QString message = "Широта:↑ " + QString::number(latitude, 'f', 10) + " Долгота→: " + QString::number(longtutude, 'f', 10);
        Q_EMIT ToUpdateLatLong(message);
        m_collectedKoordinatesData.clear();
        Q_EMIT ToConsoleLog(message);
    }
    else
    {
        Q_EMIT ToConsoleLog("Сообщение не верно. Колличество запятых не 13");
    }
}

double StateMessageGetter::ParceLatitdeValue(const QByteArray &latitudeByteArray, int directionValue) const
{
    if (latitudeByteArray.count() == 10)
    {
        //"ГГММ.МММММ". Например  "5546.95900" = 55°46.959' = 55.78265°.
        //ГГ - градусы; ММ.ММММ - минуты и десятитысячные доли минут.
        //Для ГГММ.ММММ → ГГ.ГГГГГГ перевода необходимо использовать формулу ГГ + (ММ.ММММ ÷ 60):
        const char gradusesFirst = latitudeByteArray.front();
        const char gradusesSecond = latitudeByteArray.at(1);
        const int graduses = (gradusesFirst - 48) * 10.0 + (gradusesSecond - 48);
        const char minutesDecimalFirst = latitudeByteArray.at(2);
        const char minutesDecimalSecond = latitudeByteArray.at(3);
        const char minutesFractFirst = latitudeByteArray.at(5);
        const char minutesFractSecond = latitudeByteArray.at(6);
        const char minutesFractThird = latitudeByteArray.at(7);
        const char minutesFractFourth = latitudeByteArray.at(8);
        const char minutesFractFive = latitudeByteArray.at(9);
        const double minutes = (minutesDecimalFirst - 48.0) * 10.0 + (minutesDecimalSecond - 48.0)
                               + (minutesFractFirst - 48.0) / 10.0 + (minutesFractSecond - 48.0) / 100.0f
                               + (minutesFractThird - 48.0) / 1000.0 + (minutesFractFourth - 48.0) / 10000.0f +
                               (minutesFractFive - 48.0) / 100000.0;
        //    qDebug()<< "ParceLatitde " << graduses << "   " << minutes;
        const double realCoordinate =  directionValue * (graduses + minutes / 60.0);
        return realCoordinate;
    }
    else
    {
        return 0;
    }
}

double StateMessageGetter::ParceLongtitude(const QByteArray &longtitudeByteArray, int directionValue) const
{
    if (longtitudeByteArray.count() == 11)
    {
        const char gradusesFirst = longtitudeByteArray.front();
        const char gradusesSecond = longtitudeByteArray.at(1);
        const char gradusesThird = longtitudeByteArray.at(2);
        const int graduses = (gradusesFirst - 48) * 100 + (gradusesSecond - 48) * 10 + (gradusesThird - 48);
        const char minutesDecimalFirst = longtitudeByteArray.at(3);
        const char minutesDecimalSecond = longtitudeByteArray.at(4);
        const char minutesFractFirst = longtitudeByteArray.at(6);
        const char minutesFractSecond = longtitudeByteArray.at(7);
        const char minutesFractThird = longtitudeByteArray.at(8);
        const char minutesFractFourth = longtitudeByteArray.at(9);
        const char minutesFractFive = longtitudeByteArray.at(10);
        const float minutes = (minutesDecimalFirst - 48.0) * 10.0 + (minutesDecimalSecond - 48.0)
                              + (minutesFractFirst - 48.0) / 10.0 + (minutesFractSecond - 48.0) / 100.0
                              + (minutesFractThird - 48.0) / 1000.0 + (minutesFractFourth - 48.0) / 10000.0 +
                              (minutesFractFive - 48.0) / 100000.0;
        const double realCoordinate = directionValue * graduses + minutes / 60.0;
        return realCoordinate;
    }
    else
    {
        return 0;
    }
}

double StateMessageGetter::ParceHeight() const
{
    const QByteArray arr = m_collectedKoordinatesData.mid(54, 5);
    const float height = arr.toDouble();
    const char typeOfHeight = m_collectedKoordinatesData.at(60);
    if (typeOfHeight == 'M')
    {
        return height;
    }
    else
    {
        qFatal("Wrong height");
    }
}

quint32 StateMessageGetter::ParceDelay(bool isLcm, quint16 distance) const
{
    const double secondVal = m_c / m_f;
    double distanceDouble = secondVal * (distance - 1.0) / 2.0;
    distanceDouble = qAbs(distanceDouble - m_rangeToAnswerer);
    if (isLcm)
    {
        distanceDouble = qAbs(distanceDouble - 117.488879661);
    }
    else
    {
        distanceDouble = qAbs(distanceDouble - 176.233319492);
    }
    return distanceDouble;
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
        realDistance = realDistance + m_rangeToAnswerer;
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
        if (message.count() == 8)
        {
            if (message.at(2) == 5)
            {
                const quint8 bpar_mode = message.at(3);
                //3 бита
                //было  11000000
                //стало 00000110
                //прибавляем к результату 1 так как смотри таблицу, там первый начинается с 0
                const quint8 foId = (bpar_mode & 7) + 1;

                //позиция начинается справа
                //было  11011000  = 216
                //маска 00010000  = 16
                //сдвигаем на 4 разряда и получаем или 0 или 1
                const quint8 hasLcmMask = GetMask(3, sizeof(  quint8 ) );
                const bool hasLcm = ( bpar_mode & hasLcmMask ) >> 3;
                const QString modeString = hasLcm ? "ЛЧМ" : "Радиоимпульс";
                //позиция начинается справа
                //было  11011000  = 216
                //маска 00001100  = 12
                //сдвигаем на 4 разряда и получаем или 2 битовое число
                quint8 tKmode = 0;
                if (!hasLcm)
                {
                    quint8 tKMask = GetMask(4, sizeof(  quint8 ) * 2 );
                    tKmode = ( bpar_mode & tKMask ) >> 4;
                }
                const QString tkModeString = hasLcm ? "" : " Тк-" + QString::number(tKmode);
                QByteArray signalDelayArray;
                signalDelayArray.append(message.at(4));
                signalDelayArray.append(message.at(5));

                QDataStream sinusDataStream(signalDelayArray);
                quint16 signalDelay;
                sinusDataStream >> signalDelay;
                const quint32 realDelay = ParceDelay(hasLcm, signalDelay);

                return QString ("Режим БПАР. Параметры работы:"
                                " F" + QString::number(foId) +
                                " Режим: " + modeString + tkModeString +
                                " Задержка сигнала: " + QString::number(realDelay));
            }
        }
    }
    return QLatin1String();
}

void StateMessageGetter::GetKoordinatesFromSevenMessage(const QByteArray &message)
{
    ++iter;
    qInfo() << iter << "KOORD " << message;
    const int indexOfHeader = message.indexOf("$GNR");
    if (-1 == indexOfHeader)
    {
        if (m_hasHeader)
        {
            qInfo() << iter << " Header uzhe est";
            int indexOfAsterisk = message.indexOf('*');
            if (indexOfAsterisk == -1)
            {
                qInfo() << iter << " * nenashli";
                m_collectedKoordinatesData.append(message);

            }
            else
            {
                qInfo() << iter << " * nashli";
                m_collectedKoordinatesData.append(message.left(indexOfAsterisk));
                m_hasHeader = false;
                ParceKoordinatesMessage();
            }
        }
    }
    else
    {

        m_hasHeader = true;
        m_collectedKoordinatesData.append(message.left(indexOfHeader + 1));
        const int indexOfAsterisk = message.indexOf('*', indexOfHeader);
        if (-1 == indexOfAsterisk)
        {
            qInfo() << iter << " Header tolko est";
            m_collectedKoordinatesData = message.mid(indexOfHeader);
        }
        else
        {
            qInfo() << iter << " Vse est parsim";
            m_collectedKoordinatesData = message.mid(indexOfHeader, indexOfAsterisk - indexOfHeader + 1);
            m_hasHeader = false;
            ParceKoordinatesMessage();
        }
    }
}

void StateMessageGetter::OnChangeRangeToUkit(const QString &rangeToUkit)
{
    const quint32 newRangeToUkit = rangeToUkit.toUInt();
    m_rangeToAnswerer = newRangeToUkit;
}

