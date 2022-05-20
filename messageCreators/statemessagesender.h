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
    quint16 CalculateINT_Rx(double Fvco) const;
    quint32 CalculateFRACT_RxOld(double Fvco) const;
    quint32 CalculateFRACT_RxNew(double Fvco) const;
    quint8 CalculateGAIN(double gain) const;
    quint8 CalculateAtteniator(quint8 atteniatorDb) const;
    bool CalculateDIV_rx(double Fvco) const;

private:
    const double c = 299792458.0;
    const double f;
    const double Fref;
    const quint32 distanseToAnswerer;
    const QVarLengthArray<quint8, 9> messagesIds;

};

#endif // MESSAGECREATORS_STATEMESSAGESENDER_H
