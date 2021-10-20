#ifndef MESSAGECREATOR_H
#define MESSAGECREATOR_H
#include <QByteArray>
#include <cmath>
#include <array>
#include <map>

class MessageCreator
{
public:
    MessageCreator();
    ~MessageCreator();
    QByteArray createZeroCommand();
    QByteArray createFirstCommand(double Fvco);
    QByteArray createSecondCommand(double Fvco, double doplerFreq);
    QByteArray createThirdCommand(double distance);
    QByteArray createFourthCommand(double gainTX, double gainRX);
    QByteArray createFiveCommand(double AttenuatorDb);
    QByteArray createSixCommand(double noiseValue);
    QByteArray createSevenCommand();
private:
    std::array<quint8, 8> messagesIds={0,1,2,3,4,5,6,7};
    quint16 calculateINT_Rx(double Fvco);
    quint32 calculateFRACT_Rx(double Fvco);
    quint8 calculateGAIN(quint8 gain);
    bool calculateDIV_rx(double Fvco);
    quint8 calculateAtteniator(quint16 atteniatorDb);

private:
    const double Fref=40000000.0;
    const quint32 c=299792458;
    const quint32 f=245000000;
    std::map<quint8, quint8> atteniatorTable { {0, 15},
                                                 {1, 14},
                                                 {2, 13},
                                                 {3, 12},
                                                 {4, 11},
                                                 {5, 10},
                                                 {6, 9},
                                                 {9, 8},
                                                 {12, 7},
                                                 {15, 6},
                                                 {18, 5},
                                                 {21, 4},
                                                 {24, 3},
                                                 {27, 2},
                                                 {30, 1},};
};

#endif // MESSAGECREATOR_H
