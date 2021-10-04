#include "messagecreator.h"

#include <qdatastream.h>


MessageCreator::MessageCreator()
{

}

MessageCreator::~MessageCreator()
{

}

QByteArray MessageCreator::createZeroCommand()
{
    QByteArray command;
    command.append(messagesIds.at(0));
    command.append(messagesIds.at(0));
    return command;
}

QByteArray MessageCreator::createFirstCommand(quint16 Fvco)
{
    quint8 id=messagesIds.at(1);
    Fvco=Fvco-3;
    quint16 INT_Rx=calculateINT_Rx(Fvco);
    quint32 FRACT_Rx=calculateFRACT_Rx(Fvco);
    bool DivRx=calculateDIV_rx(Fvco);

    QByteArray bA;
    QDataStream stream(&bA, QIODevice::WriteOnly);
    stream << FRACT_Rx;

    QByteArray command={};
    QDataStream streamMain(&command, QIODevice::WriteOnly);
    streamMain << id;
    streamMain << INT_Rx;
    streamMain << (unsigned char)bA.at(bA.count()-3);
    streamMain << (unsigned char)bA.at(bA.count()-2);
    streamMain << (unsigned char)bA.at(bA.count()-1);
    streamMain <<DivRx;
    return command;
}

QByteArray MessageCreator::createSecondCommand(quint16 Fvco, quint16 doplerFreq)
{

    Fvco=Fvco-3;
    quint16 ResultFreq=Fvco+doplerFreq;
    quint16 INT_Tx=calculateINT_Rx(ResultFreq);
    quint32 FRACT_Tx=calculateFRACT_Rx(ResultFreq);
    bool DivRx=calculateDIV_rx(ResultFreq);


    QByteArray bA;
    QDataStream stream(&bA, QIODevice::WriteOnly);
    stream << FRACT_Tx;

    QByteArray command;
    QDataStream streamMain(&command, QIODevice::WriteOnly);
    streamMain << messagesIds.at(1);
    streamMain << INT_Tx;
    streamMain << (unsigned char)bA.at(bA.count()-3);
    streamMain << (unsigned char)bA.at(bA.count()-2);
    streamMain << (unsigned char)bA.at(bA.count()-1);
    streamMain <<DivRx;

    return command;
}

QByteArray MessageCreator::createThirdCommand(quint16 distance)
{

    double secondVal=((double)f)/c;
    quint16 DISTANCE=distance*secondVal;

    QByteArray command;
    QDataStream streamMain(&command, QIODevice::WriteOnly);
    streamMain<<messagesIds.at(3);
    streamMain<<DISTANCE;
    return command;
}

QByteArray MessageCreator::createFourthCommand(quint8 gainTX, quint8 gainRX)
{
    QByteArray command;
    quint8 GAIN_TX=calculateGAIN(gainTX);
    quint8 GAIN_RX=calculateGAIN(gainRX);
    command.append(messagesIds.at(4));
    command.append(GAIN_TX);
    command.append(GAIN_RX);
    return command;
}

quint16 MessageCreator::calculateINT_Rx(quint16 Fvco)
{
    bool DIV_Rx=calculateDIV_rx(Fvco);
    quint16 INT_Rx=(2*Fvco);
    INT_Rx=INT_Rx/pow(2,DIV_Rx);
    INT_Rx=(quint16)INT_Rx/Fref;
    INT_Rx-=4;
    return INT_Rx;
}

quint32 MessageCreator::calculateFRACT_Rx(quint16 Fvco)
{
    bool DIV_Rx=calculateDIV_rx(Fvco);
    quint32 FRACT_Rx=(pow(2,20));
    double FirstValue=2.0*Fvco/(Fref*pow(2, DIV_Rx));
    double SecondValue=(quint16)FirstValue;
    FRACT_Rx=FRACT_Rx*(FirstValue-SecondValue);
    return FRACT_Rx;
}

quint8 MessageCreator::calculateGAIN(quint8 gain)
{
    quint8 GAIN_TX=gain*0.5;
    if (GAIN_TX>(quint8)(31.5))
    {
        GAIN_TX=quint8(31.5);
    }
    return GAIN_TX;
}

bool MessageCreator::calculateDIV_rx(quint16 Fvco)
{
    if (Fvco>2750)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
