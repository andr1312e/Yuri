#include "statemessagesender.h"

StateMessageSender::StateMessageSender()
{

}

StateMessageSender::~StateMessageSender()
{

}

const QByteArray StateMessageSender::createZeroCommand()
{
    QByteArray command={};
    command.append(messagesIds.at(0));
    command.append(messagesIds.at(0));
    return command;
}

QByteArray StateMessageSender::createFirstCommand(double Fvco)
{
    quint8 id=messagesIds.at(1);
    qint16 INT_Rx=calculateINT_Rx(Fvco);//МГЦ
    qint32 FRACT_Rx=calculateFRACT_Rx(Fvco);//МГЦ
    bool DivRx=calculateDIV_rx(Fvco);


    quint8 FRACT_RxFirst=(FRACT_Rx >> (8*0)) & 0xff;
    quint8 FRACT_RxSecond=(FRACT_Rx >> (8*1)) & 0xff;
    quint8 FRACT_RxThird=(FRACT_Rx >> (8*2)) & 0xff;

    QByteArray command;
    QDataStream streamMain(&command, QIODevice::WriteOnly);
    streamMain << id;
    streamMain << INT_Rx;
    streamMain << FRACT_RxThird;
    streamMain << FRACT_RxSecond;
    streamMain << FRACT_RxFirst;
    streamMain << DivRx;
//    qDebug()<< command.toHex();
    return command;
}

QByteArray StateMessageSender::createSecondCommand(double Fvco, double doplerFreq)
{
    double ResultFreq=Fvco+doplerFreq;
    qint16 INT_Tx=calculateINT_Rx(ResultFreq);
    qint32 FRACT_Tx=calculateFRACT_Rx(ResultFreq);
    bool DivTx=calculateDIV_rx(ResultFreq);


    QByteArray lastThreeBytes;
    QDataStream threeBytesStream(&lastThreeBytes, QIODevice::WriteOnly);
    threeBytesStream << FRACT_Tx;

    QByteArray command;
    QDataStream streamMain(&command, QIODevice::WriteOnly);
    streamMain << messagesIds.at(2);
    streamMain << INT_Tx;
    streamMain << (quint8)lastThreeBytes.at(lastThreeBytes.count()-3);
    streamMain << (quint8)lastThreeBytes.at(lastThreeBytes.count()-2);
    streamMain << (quint8)lastThreeBytes.at(lastThreeBytes.count()-1);
    streamMain <<DivTx;

    return command;
}

QByteArray StateMessageSender::createThirdCommand(double distance)
{
    double secondVal=f/c;
    quint16 DISTANCE=distance*secondVal+1.0;

    QByteArray command;
    QDataStream streamMain(&command, QIODevice::WriteOnly);
    streamMain<<messagesIds.at(3);
    streamMain<<DISTANCE;
    return command;
}

QByteArray StateMessageSender::createFourthCommand(double gainTX, double gainRX)
{
    quint8 GAIN_TX=calculateGAIN(gainTX);
    quint8 GAIN_RX=calculateGAIN(gainRX);

    QByteArray command;
    command.append(messagesIds.at(4));
    command.append(GAIN_TX);
    command.append(GAIN_RX);
    return command;
}

QByteArray StateMessageSender::createFiveCommand(double AttenuatorDb)
{
    quint8 attenuator=calculateAtteniator(AttenuatorDb);

    QByteArray command;
    command.append(messagesIds.at(5));
    command.append(attenuator);
    return command;
}

QByteArray StateMessageSender::createSixCommand(double noiseType, double noiseValue)
{
    QByteArray command;
    command.append(messagesIds.at(6));
    command.append(quint8(noiseType));
    quint32 noiseVal=(quint32)noiseValue;
    quint8 first=(noiseVal >> (8*0)) & 0xff;
    quint8 second=(noiseVal >> (8*1)) & 0xff;
    quint8 third=(noiseVal >> (8*2)) & 0xff;
    command.append(third);
    command.append(second);
    command.append(first);
    return command;
}

QByteArray StateMessageSender::createSevenCommand(quint8 param)
{
    QByteArray command;
    command.append(messagesIds.at(7));
    command.append(quint8(param));
    return command;
}

quint16 StateMessageSender::calculateINT_Rx(double Fvco) const
{
    bool DIV_Rx=calculateDIV_rx(Fvco);
    double INT_RxDouble=Fvco-3000000.0;
    INT_RxDouble=INT_RxDouble*2.0;
    INT_RxDouble=INT_RxDouble/pow(2,DIV_Rx);
    INT_RxDouble=(INT_RxDouble/Fref);
    quint16 INT_Rx=(quint16)(INT_RxDouble);
    INT_Rx-=4;
    return INT_Rx;
}

quint32 StateMessageSender::calculateFRACT_Rx(double Fvco) const
{
    bool DIV_Rx=calculateDIV_rx(Fvco);
    quint32 FRACT_Rx=(pow(2,20));
    double FirstValue=2.0*Fvco;
    double FirstValueDiv=Fref*pow(2, DIV_Rx);
    FirstValue=FirstValue/FirstValueDiv;
    double SecondValue=(qint32)FirstValue;
    FRACT_Rx=FRACT_Rx*(FirstValue-SecondValue);
    return FRACT_Rx;
}

quint8 StateMessageSender::calculateGAIN(quint8 gain) const
{
    if (gain>(63))
    {
        gain=63;
    }
    quint8 GAIN_X=gain/2.0;
    return GAIN_X;
}

quint8 StateMessageSender::calculateAtteniator(quint16 atteniatorDb) const
{
    if (atteniatorTable.count(atteniatorDb))
    {
        quint8 atteniatorValue=atteniatorTable[atteniatorDb];
        return atteniatorValue;
    }
    else
    {
        qFatal("нет значения в таблице");
    }
}

bool StateMessageSender::calculateDIV_rx(double Fvco) const
{
    if (Fvco>2750000000)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
