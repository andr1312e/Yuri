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
    explicit StateMessageSender(const double f, const double fref, const quint32 distanfeToAnswerer);
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

    const double f;
    const double Fref;
    const quint32 distanseToAnswerer;
    const std::array<quint8, 9> messagesIds={0,1,2,3,4,5,6,7,8};
private:

    quint16 calculateINT_Rx(double Fvco) const;
    quint32 calculateFRACT_Rx(double Fvco) const;
    quint8 calculateGAIN(quint8 gain) const;
    quint8 calculateAtteniator(quint16 atteniatorDb) const;
    bool calculateDIV_rx(double Fvco) const;
};

#endif // MESSAGECREATORS_STATEMESSAGESENDER_H
