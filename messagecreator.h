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
    QByteArray createFirstCommand(quint16 Fvco);
    QByteArray createSecondCommand(quint16 Fvco, quint16 doplerFreq);
    QByteArray createThirdCommand(quint16 distance);
    QByteArray createFourthCommand(quint8 gainTX, quint8 gainRX);
    QByteArray createFiveCommand(quint16 AttenuatorDb);
private:
    std::array<unsigned char, 6> messagesIds={0,1,2,3,4, 5};
    quint16 calculateINT_Rx(quint16 Fvco);
    quint32 calculateFRACT_Rx(quint16 Fvco);
    quint8 calculateGAIN(quint8 gain);
    bool calculateDIV_rx(quint16 Fvco);
    quint16 calculateAtteniator(quint16 atteniatorDb);

private:
    const quint8 Fref=40;
    const quint32 c=299792458;
    const quint32 f=245000000;
    std::map<quint16, quint16> atteniatorTable { {0, 15},
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
