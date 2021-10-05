#ifndef MESSAGECREATOR_H
#define MESSAGECREATOR_H
#include <QByteArray>
#include <cmath>
#include <array>

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

private:
    std::array<unsigned char, 5> messagesIds={0,1,2,3,4};
    quint16 calculateINT_Rx(quint16 Fvco);
    quint32 calculateFRACT_Rx(quint16 Fvco);
    quint8 calculateGAIN(quint8 gain);
    bool calculateDIV_rx(quint16 Fvco);

private:
    const quint8 Fref=40;
    const quint32 c=299792458;
    const quint32 f=245000000;
};

#endif // MESSAGECREATOR_H
