#ifndef MESSAGECREATORS_STATEMESSAGESENDER_H
#define MESSAGECREATORS_STATEMESSAGESENDER_H

#include <QByteArray>
#include <QIODevice>
#include <QDataStream>
#include <cmath>
#include <constantsandfunctions.h>

class StateMessageSender
{
public:
    explicit StateMessageSender();
    ~StateMessageSender();
    const QByteArray createZeroCommand();
    QByteArray createFirstCommand(double Fvco);
    QByteArray createSecondCommand(double Fvco, double doplerFreq);
    QByteArray createThirdCommand(double distance);
    QByteArray createFourthCommand(double gainTX, double gainRX);
    QByteArray createFiveCommand(double AttenuatorDb);
    QByteArray createSixCommand(double noiseType, double noiseValue);
    QByteArray createSevenCommand(quint8 param);
private:
    const double c=299792458.0;
    const double f=245000000.0;
private:
    quint16 calculateINT_Rx(double Fvco) const;
    quint32 calculateFRACT_Rx(double Fvco) const;
    quint8 calculateGAIN(quint8 gain) const;
    quint8 calculateAtteniator(quint16 atteniatorDb) const;
};

#endif // MESSAGECREATORS_STATEMESSAGESENDER_H
