#ifndef MESSAGECREATORS_STATEMESSAGESENDER_H
#define MESSAGECREATORS_STATEMESSAGESENDER_H
#include <QByteArray>
#include <cmath>
#include <constantsandfunctions.h>

class StateMessageSender
{
public:
    StateMessageSender();
    ~StateMessageSender();
    QByteArray createZeroCommand();
    QByteArray createFirstCommand(double Fvco);
    QByteArray createSecondCommand(double Fvco, double doplerFreq);
    QByteArray createThirdCommand(double distance);
    QByteArray createFourthCommand(double gainTX, double gainRX);
    QByteArray createFiveCommand(double AttenuatorDb);
    QByteArray createSixCommand(double noiseValue);
    QByteArray createSevenCommand(quint8 param);
private:
    quint16 calculateINT_Rx(double Fvco);
    quint32 calculateFRACT_Rx(double Fvco);
    quint8 calculateGAIN(quint8 gain);
    quint8 calculateAtteniator(quint16 atteniatorDb);
};

#endif // MESSAGECREATORS_STATEMESSAGESENDER_H
