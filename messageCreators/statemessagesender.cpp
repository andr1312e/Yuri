#include "statemessagesender.h"
#include <QtDebug>
#include <QtMath>

StateMessageSender::StateMessageSender(const double f, const double fref, const quint32 distanseToAnswerer)
    : f(f)
    , Fref(fref)
    , distanseToAnswerer(distanseToAnswerer)
{

}

StateMessageSender::~StateMessageSender()
{

}

QByteArray StateMessageSender::CreateZeroCommand() const
{
    QByteArray command = {};
    command.append(messagesIds.at(0));
    command.append(messagesIds.at(0));
    return command;
}

QByteArray StateMessageSender::CreateFirstCommand(double Fvco) const
{
    quint8 id = messagesIds.at(1);
    qint16 INT_Rx = calculateINT_Rx(Fvco); //МГЦ
    qint32 FRACT_Rx = calculateFRACT_RxNew(Fvco); //МГЦ
    bool DivRx = calculateDIV_rx(Fvco);

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
    double ResultFreq = Fvco + doplerFreq;
    qint16 INT_Tx = calculateINT_Rx(ResultFreq);
    qint32 FRACT_Tx = calculateFRACT_RxOld(ResultFreq);
    bool DivTx = calculateDIV_rx(ResultFreq);


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
    double secondVal = f / c;
    double distanceDouble = 2.0 * qAbs(distance - distanseToAnswerer) * secondVal + 1.0;
    quint16 DISTANCE = distanceDouble;

    QByteArray command;
    QDataStream streamMain(&command, QIODevice::WriteOnly);
    streamMain << messagesIds.at(3);
    streamMain << DISTANCE;
    return command;
}

QByteArray StateMessageSender::CreateFourthCommand(double gainTX, double gainRX) const
{
    quint8 GAIN_TX = calculateGAIN(gainTX);
    quint8 GAIN_RX = calculateGAIN(gainRX);

    QByteArray command;
    command.append(messagesIds.at(4));
    command.append(GAIN_TX);
    command.append(GAIN_RX);
    return command;
}

QByteArray StateMessageSender::CreateFiveCommand(double AttenuatorDb) const
{
    quint8 attenuator = calculateAtteniator(AttenuatorDb);

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
    if (noiseType > 2)
    {
        quint32 noiseVal = (quint32)noiseValue;
        quint8 first = (noiseVal >> (8 * 0)) & 0xff;
        quint8 second = (noiseVal >> (8 * 1)) & 0xff;
        quint8 third = (noiseVal >> (8 * 2)) & 0xff;
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

quint16 StateMessageSender::calculateINT_Rx(double Fvco) const
{
    bool DIV_Rx = calculateDIV_rx(Fvco);
    double INT_RxDouble = Fvco - 3000000.0;
    INT_RxDouble = INT_RxDouble * 2.0;
    INT_RxDouble = INT_RxDouble / pow(2, DIV_Rx);
    INT_RxDouble = (INT_RxDouble / Fref);
    quint16 INT_Rx = (quint16)qFloor(INT_RxDouble);
    INT_Rx -= 4;
    return INT_Rx;
}

quint32 StateMessageSender::calculateFRACT_RxOld(double Fvco) const
{
    bool DIV_Rx = calculateDIV_rx(Fvco);
    double povValue = (pow(2, 20));
    double FirstValue = 2.0 * Fvco;
    double FirstValueDiv = Fref * pow(2, DIV_Rx);
    FirstValue = FirstValue / FirstValueDiv;
    double SecondValue = (qint32)FirstValue;
    double difference = FirstValue - SecondValue;
    quint32 FRACT_Rx = (quint32)qFloor(povValue * difference);
    return FRACT_Rx;
}

quint32 StateMessageSender::calculateFRACT_RxNew(double Fvco) const
{
    bool DIV_Rx = calculateDIV_rx(Fvco);
    double povValue = (qPow(2, 20));
    double RealFvco = Fvco - 3000000.0;
    double FirstValue = RealFvco * 2.0;
    FirstValue = FirstValue / Fref / qPow(2, DIV_Rx);
    double SecondValue = (qint32)FirstValue;
    double difference = FirstValue - SecondValue;
    quint32 FRACT_Rx = (quint32)qFloor(povValue * difference);
    return FRACT_Rx;
}

quint8 StateMessageSender::calculateGAIN(double gain) const
{
    if (gain > (31.5))
    {
        gain = 31.5;
    }
    quint8 GAIN_X = gain * 2.0;
    return GAIN_X;
}

quint8 StateMessageSender::calculateAtteniator(quint16 atteniatorDb) const
{
    if (atteniatorTable.count(atteniatorDb))
    {
        quint8 atteniatorValue = atteniatorTable[atteniatorDb];
        return atteniatorValue;
    }
    else
    {
        qFatal("нет значения в таблице");
    }
}

bool StateMessageSender::calculateDIV_rx(double Fvco) const
{
    if (Fvco > 2750000000)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
