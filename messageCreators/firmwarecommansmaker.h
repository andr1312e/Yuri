#ifndef MESSAGECREATORS_FIRMWARECOMMANSSENDER_H
#define MESSAGECREATORS_FIRMWARECOMMANSSENDER_H

#include <QByteArray>
#include <QVector>

class FirmwareMessageMaker
{
public:
    explicit FirmwareMessageMaker();
    ~FirmwareMessageMaker();
public:
    const QByteArray &ReadToBufferCommand(quint32 adress, quint8 lenght);
    QByteArray WriteBufferToFlashCommand(quint32 adress, quint16 lenght) const;
    QByteArray WriteDataToBufferCommand(bool isNewPage, const QByteArray &data) const;//0e 5.0 команда запись

    const QByteArray m_readRegisterStatusCommand;
    const QByteArray m_readFromBufferCommand;
    const QByteArray m_flashIdCommad;
    const QByteArray m_eraseCommand;
    const QByteArray m_makeWriteOnlyBufferCommand;
    const QByteArray m_makeReadOnlyBufferCommand;
    const QByteArray m_restartHardwareCommand;
private:
    QByteArray m_readToBufferCommand;

private:
    QByteArray CreateTypeCommand(quint8 commandId) const;
    QByteArray CreateAddressCommand(quint32 adress) const;
    QByteArray CreateNumberOfBytesLenghtCommand(quint16 numberOfBytes) const;
private:
    // 0  1  2  3  4
    const QVector<quint8> m_commands;//0a 0b 0c 0d 0e
    const quint8 m_significantBit = 128; //10 00 00 00

};

#endif // MESSAGECREATORS_FIRMWARECOMMANSCREATOR_H
