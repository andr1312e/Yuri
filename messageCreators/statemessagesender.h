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
    explicit StateMessageSender(const double f, const double fref);
    ~StateMessageSender();

public:
    QByteArray CreateZeroCommand() const noexcept;
    QByteArray CreateFirstCommand(double Fvco) const noexcept;
    QByteArray CreateSecondCommandOld(double Fvco, double doplerFreq) const noexcept;
    QByteArray CreateSecondCommand(double Fvco) const noexcept;
    QByteArray CreateThirdCommand(double distance, double distanceToUkit) const noexcept;
    QByteArray CreateFourthCommand(double gainTX, double gainRX) const noexcept;
    QByteArray CreateFiveCommand(double AttenuatorDb) const noexcept;
    QByteArray CreateSixCommand(double noiseType, double noiseValue) const noexcept;
    QByteArray CreateSevenCommand(quint8 firstParam, quint8 secondParam = 0) const noexcept;
    QByteArray CreateBparCommand(quint8 checkedFoButtonId, bool isLcm, quint8 tksIndex, bool hasThreshold, quint16 threshold, int distance, double distanceToUkit) const noexcept;
    QByteArray CreateNineCommand(double doplerFreq) const noexcept;
private:
    quint16 CalculateINT_Rx(double Fvco) const noexcept;
    quint32 CalculateFRACT_RxOld(double Fvco) const noexcept;
    quint32 CalculateFRACT_RxNew(double Fvco) const noexcept;
    quint8 CalculateGAIN(double gain) const noexcept;
    quint8 CalculateAtteniator(quint8 atteniatorDb) const;
    bool CalculateDIV_rx(double Fvco) const noexcept;
    quint16 CalculateBparDistance(bool isLcm, double distance, double distanceToUkit) const noexcept;

private:
    const int m_distanceToSolver = 0;
    const double m_c = 299792458.0;
    const double m_f;
    const double m_Fref;
    const QVarLengthArray<quint8, 10> m_messagesIds;
};

#endif // MESSAGECREATORS_STATEMESSAGESENDER_H
