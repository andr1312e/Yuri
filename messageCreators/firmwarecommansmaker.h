#ifndef MESSAGECREATORS_FIRMWARECOMMANSSENDER_H
#define MESSAGECREATORS_FIRMWARECOMMANSSENDER_H

#include <QByteArray>
#include <QVector>

class FirmwareMessageMaker
{
public:
    explicit FirmwareMessageMaker();
    ~FirmwareMessageMaker();

    QByteArray *ReadToBufferCommand(quint32 adress, quint8 lenght);
    QByteArray WriteBufferToFlashCommand(quint32 adress, quint16 lenght);
    QByteArray WriteDataToBufferCommand(bool isNewPage, const QByteArray &data);//0e 5.0 команда запись

    const QByteArray *m_readRegisterStatusCommand;
    const QByteArray *m_readFromBufferCommand;
    const QByteArray *m_flashIdCommad;
    const QByteArray *m_eraseCommand;
    const QByteArray *m_makeWriteOnlyBufferCommand;
    const QByteArray *m_makeReadOnlyBufferCommand;
    const QByteArray *m_restartHardwareCommand;

    QByteArray *m_readToBufferCommand;

private:
    QByteArray CreateTypeCommand(quint8 commandId);
    QByteArray CreateAddressCommand(quint32 adress);
    QByteArray CreateNumberOfBytesLenghtCommand(quint16 numberOfBytes);
                                      // 0  1  2  3  4
    const QVector<quint8> *m_commands;//0a 0b 0c 0d 0e
    quint8 m_significantBit=128; //10 00 00 00

};

#endif // MESSAGECREATORS_FIRMWARECOMMANSCREATOR_H
