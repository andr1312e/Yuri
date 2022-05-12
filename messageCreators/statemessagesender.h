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
    QByteArray CreateZeroCommand() const;
    QByteArray CreateFirstCommand(double Fvco) const;
    QByteArray CreateSecondCommand(double Fvco, double doplerFreq) const;
    QByteArray CreateThirdCommand(double distance) const;
    QByteArray CreateFourthCommand(double gainTX, double gainRX) const;
    QByteArray CreateFiveCommand(double AttenuatorDb) const;
    QByteArray CreateSixCommand(double noiseType, double noiseValue) const;
    QByteArray CreateSevenCommand(quint8 param) const;
private:
    const double c = 299792458.0;

    const double f;
    const double Fref;
    const quint32 distanseToAnswerer;
    const std::array<quint8, 9> messagesIds = {0, 1, 2, 3, 4, 5, 6, 7, 8};
private:

    quint16 calculateINT_Rx(double Fvco) const;
    quint32 calculateFRACT_RxOld(double Fvco) const;
    quint32 calculateFRACT_RxNew(double Fvco) const;
    quint8 calculateGAIN(double gain) const;
    quint8 calculateAtteniator(quint16 atteniatorDb) const;
    bool calculateDIV_rx(double Fvco) const;
};

#endif // MESSAGECREATORS_STATEMESSAGESENDER_H
