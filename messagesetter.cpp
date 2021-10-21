#include "messagesetter.h"

#include <QIODevice>
#include <qdatastream.h>


MessagesSetter::MessagesSetter()
{

}

MessagesSetter::~MessagesSetter()
{

}

QByteArray MessagesSetter::createZeroCommand()
{
    QByteArray command={};
    command.append(messagesIds.at(0));
    command.append(messagesIds.at(0));
    return command;
}

QByteArray MessagesSetter::createFirstCommand(double Fvco)
{
    quint8 id=messagesIds.at(1);
    qint16 INT_Rx=calculateINT_Rx(Fvco);//МГЦ
    qint32 FRACT_Rx=calculateFRACT_Rx(Fvco);//МГЦ
    bool DivRx=calculateDIV_rx(Fvco);

    QByteArray lastThreeBytes;
    QDataStream threeBytesStream(&lastThreeBytes, QIODevice::WriteOnly);
    threeBytesStream << FRACT_Rx;

    QByteArray command={};
    QDataStream streamMain(&command, QIODevice::WriteOnly);
    streamMain << id;
    streamMain << INT_Rx;
    streamMain << (unsigned char)lastThreeBytes.at(lastThreeBytes.count()-3);
    streamMain << (unsigned char)lastThreeBytes.at(lastThreeBytes.count()-2);
    streamMain << (unsigned char)lastThreeBytes.at(lastThreeBytes.count()-1);
    streamMain << DivRx;
    return command;
}

QByteArray MessagesSetter::createSecondCommand(double Fvco, double doplerFreq)
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
    streamMain << (unsigned char)lastThreeBytes.at(lastThreeBytes.count()-3);
    streamMain << (unsigned char)lastThreeBytes.at(lastThreeBytes.count()-2);
    streamMain << (unsigned char)lastThreeBytes.at(lastThreeBytes.count()-1);
    streamMain <<DivTx;

    return command;
}

QByteArray MessagesSetter::createThirdCommand(double distance)
{
    double secondVal=f/c;
    quint16 DISTANCE=distance*secondVal+1;

    QByteArray command;
    QDataStream streamMain(&command, QIODevice::WriteOnly);
    streamMain<<messagesIds.at(3);
    streamMain<<DISTANCE;
    return command;
}

QByteArray MessagesSetter::createFourthCommand(double gainTX, double gainRX)
{
    quint8 GAIN_TX=calculateGAIN(gainTX);
    quint8 GAIN_RX=calculateGAIN(gainRX);

    QByteArray command;
    command.append(messagesIds.at(4));
    command.append(GAIN_TX);
    command.append(GAIN_RX);
    return command;
}

QByteArray MessagesSetter::createFiveCommand(double AttenuatorDb)
{
    quint8 attenuator=calculateAtteniator(AttenuatorDb);

    QByteArray command;
    command.append(messagesIds.at(5));
    command.append(attenuator);
    return command;
}

QByteArray MessagesSetter::createSixCommand(double noiseValue)
{
    QByteArray command;
    command.append(messagesIds.at(6));
    command.append(quint8(noiseValue));
    return command;
}

QByteArray MessagesSetter::createSevenCommand(double param)
{
    QByteArray command;
    command.append(messagesIds.at(7));
    command.append(quint8(param));
    return command;
}

quint16 MessagesSetter::calculateINT_Rx(double Fvco)
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

quint32 MessagesSetter::calculateFRACT_Rx(double Fvco)
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

quint8 MessagesSetter::calculateGAIN(quint8 gain)
{
    if (gain>(63))
    {
        gain=63;
    }
    quint8 GAIN_X=gain;
    //    quint8 GAIN_X=gain*0.5;
    return GAIN_X;
}

quint8 MessagesSetter::calculateAtteniator(quint16 atteniatorDb)
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
