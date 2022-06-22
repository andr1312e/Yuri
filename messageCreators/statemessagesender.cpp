#include "statemessagesender.h"
#include <QtDebug>
#include <QtMath>

StateMessageSender::StateMessageSender(const double f, const double fref, const quint32 distanseToAnswerer)
    : f(f)
    , Fref(fref)
    , distanseToAnswerer(distanseToAnswerer)
    , messagesIds({0, 1, 2, 3, 4, 5, 6, 7, 8})
{
}

StateMessageSender::~StateMessageSender()
{

}

QByteArray StateMessageSender::CreateZeroCommand() const noexcept
{
    QByteArray command = {};
    command.append(messagesIds.at(0));
    command.append(messagesIds.at(0));
    return command;
}

QByteArray StateMessageSender::CreateFirstCommand(double Fvco) const
{
    const quint8 id = messagesIds.at(1);
    const qint16 INT_Rx = CalculateINT_Rx(Fvco); //МГЦ
    const qint32 FRACT_Rx = CalculateFRACT_RxNew(Fvco); //МГЦ
    const bool DivRx = CalculateDIV_rx(Fvco);

    QByteArray lastThreeBytes;
    QDataStream threeBytesStream(&lastThreeBytes, QIODevice::WriteOnly);
    threeBytesStream << FRACT_Rx;


    QByteArray command;
    QDataStream streamMain(&command, QIODevice::WriteOnly);
    streamMain << id;
    streamMain << INT_Rx;
    streamMain << (quint8)lastThreeBytes.at(1);
    streamMain << (quint8)lastThreeBytes.at(2);
    streamMain << (quint8)lastThreeBytes.at(3);
    streamMain << DivRx;
    qDebug() << QStringLiteral(" Выслали ") << command.toHex();
    return command;
}

QByteArray StateMessageSender::CreateSecondCommand(double Fvco, double doplerFreq) const
{
    const double ResultFreq = Fvco + doplerFreq;
    const qint16 INT_Tx = CalculateINT_Rx(ResultFreq);
    const qint32 FRACT_Tx = CalculateFRACT_RxNew(ResultFreq);
    const bool DivTx = CalculateDIV_rx(ResultFreq);


    QByteArray lastThreeBytes;
    QDataStream threeBytesStream(&lastThreeBytes, QIODevice::WriteOnly);
    threeBytesStream << FRACT_Tx;

    QByteArray command;
    QDataStream streamMain(&command, QIODevice::WriteOnly);
    streamMain << messagesIds.at(2);
    streamMain << INT_Tx;
    streamMain << (quint8)lastThreeBytes.at(1);
    streamMain << (quint8)lastThreeBytes.at(2);
    streamMain << (quint8)lastThreeBytes.at(3);
    //    streamMain << (unsigned char)lastThreeBytes.at(lastThreeBytes.count()-3);
    //    streamMain << (unsigned char)lastThreeBytes.at(lastThreeBytes.count()-2);
    //    streamMain << (unsigned char)lastThreeBytes.at(lastThreeBytes.count()-1);
    streamMain << DivTx;

    return command;
}

QByteArray StateMessageSender::CreateThirdCommand(double distance) const
{
    distance += 26, 9434889941;
    const double secondVal = f / c;
    const double distanceDouble = 2.0 * qAbs(distance - distanseToAnswerer) * secondVal + 1.0;
    const quint16 DISTANCE = distanceDouble;

    QByteArray command;
    QDataStream streamMain(&command, QIODevice::WriteOnly);
    streamMain << messagesIds.at(3);
    streamMain << DISTANCE;
    return command;
}

QByteArray StateMessageSender::CreateFourthCommand(double gainTX, double gainRX) const
{
    const quint8 GAIN_TX = CalculateGAIN(gainTX);
    const quint8 GAIN_RX = CalculateGAIN(gainRX);

    QByteArray command;
    command.append(messagesIds.at(4));
    command.append(GAIN_TX);
    command.append(GAIN_RX);
    return command;
}

QByteArray StateMessageSender::CreateFiveCommand(double AttenuatorDb) const
{
    const quint8 attenuator = CalculateAtteniator(AttenuatorDb);

    QByteArray command;
    command.append(messagesIds.at(5));
    command.append(attenuator);
    return command;
}

QByteArray StateMessageSender::CreateSixCommand(double noiseType, double noiseValue) const
{
    QByteArray command;
    command.append(messagesIds.at(6));
    command.append(quint8(noiseType));
    if (noiseType > 2 && noiseType < 4)
    {
        quint32 noiseVal = (quint32)noiseValue;
        const quint8 first = (noiseVal >> (8 * 0)) & 0xff;
        const quint8 second = (noiseVal >> (8 * 1)) & 0xff;
        const quint8 third = (noiseVal >> (8 * 2)) & 0xff;
        command.append(third);
        command.append(second);
        command.append(first);
    }
    return command;
}

QByteArray StateMessageSender::CreateSevenCommand(quint8 param) const
{
    QByteArray command;
    command.append(messagesIds.at(7));
    command.append(quint8(param));
    return command;
}

QByteArray StateMessageSender::CreateBparCommand(quint8 checkedFoButtonId, bool isLcm, quint8 tksIndex, bool hasThreshold, quint16 threshold, int distance) const
{
    QByteArray bParMessage;
    QDataStream stream(&bParMessage, QIODevice::WriteOnly);
    quint8 bpar_mode=checkedFoButtonId;
    if (isLcm)
    {
        const quint8 lcmMode = 8; //1000
        bpar_mode += lcmMode;
    }
    else
    {
        const quint8 tkVal = tksIndex << 4;//10000 или 20000
        bpar_mode += tkVal;
    }
    if (hasThreshold)
    {
        bpar_mode += 64;//0100000
    }
    const quint16 delay = CalculateBparDistance(distance);
    stream << (quint8)6;
    stream << (quint8)5;
    stream << bpar_mode;
    stream << delay;
    stream << threshold;
}

quint16 StateMessageSender::CalculateINT_Rx(double Fvco) const noexcept
{
    const bool DIV_Rx = CalculateDIV_rx(Fvco);
    double INT_RxDouble = Fvco - 3000000.0;
    INT_RxDouble = INT_RxDouble * 2.0;
    INT_RxDouble = INT_RxDouble / pow(2, DIV_Rx);
    INT_RxDouble = (INT_RxDouble / Fref);
    quint16 INT_Rx = (quint16)qFloor(INT_RxDouble);
    INT_Rx -= 4;
    return INT_Rx;
}

quint32 StateMessageSender::CalculateFRACT_RxOld(double Fvco) const noexcept
{
    const bool DIV_Rx = CalculateDIV_rx(Fvco);
    double povValue = (pow(2, 20));
    double FirstValue = 2.0 * Fvco;
    double FirstValueDiv = Fref * pow(2, DIV_Rx);
    FirstValue = FirstValue / FirstValueDiv;
    double SecondValue = (qint32)FirstValue;
    double difference = FirstValue - SecondValue;
    quint32 FRACT_Rx = (quint32)qFloor(povValue * difference);
    return FRACT_Rx;
}

quint32 StateMessageSender::CalculateFRACT_RxNew(double Fvco) const noexcept
{
    const bool DIV_Rx = CalculateDIV_rx(Fvco);
    double povValue = (qPow(2, 20));
    double RealFvco = Fvco - 3000000.0;
    double FirstValue = RealFvco * 2.0;
    FirstValue = FirstValue / Fref / qPow(2, DIV_Rx);
    double SecondValue = (qint32)FirstValue;
    double difference = FirstValue - SecondValue;
    quint32 FRACT_Rx = (quint32)qFloor(povValue * difference);
    return FRACT_Rx;
}

quint8 StateMessageSender::CalculateGAIN(double gain) const noexcept
{
    if (gain > (31.5))
    {
        gain = 31.5;
    }
    const quint8 GAIN_X = gain * 2.0;
    return GAIN_X;
}

quint8 StateMessageSender::CalculateAtteniator(quint8 atteniatorDb) const
{
    if (atteniatorTable.contains(atteniatorDb))
    {
        const quint8 atteniatorValue = atteniatorTable[atteniatorDb];
        return atteniatorValue;
    }
    else
    {
        qFatal("нет значения в таблице");
    }
}

bool StateMessageSender::CalculateDIV_rx(double Fvco) const noexcept
{
    return Fvco > 2750000000;
}

quint16 StateMessageSender::CalculateBparDistance(int distance) const noexcept
{
    return (distance-m_distanceToSolver)/f*c;
}
