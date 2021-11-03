#include "firmwarecommansmaker.h"

#include <QDataStream>
#include <QIODevice>



FirmwareMessageMaker::FirmwareMessageMaker()
    : m_ReadRegisterStatusCommand(new QByteArray("\x0b\x05\x0a\x00", 4))
    , m_ReadFromBufferCommand(new QByteArray("\x0d\x00", 2))
    , m_flashIdCommad("\x0b\x09\x0a\x02", 4)
    , m_commands(new QVector<quint8>({10, 11, 12, 13, 14}))
{

}

FirmwareMessageMaker::~FirmwareMessageMaker()
{
    delete m_commands;
}

QByteArray FirmwareMessageMaker::ReadToBufferCommand(quint32 adress, quint8 lenght)
{
    QByteArray result;
    result.append(CreateTypeCommand((quint8)3));
    result.append(CreateAddressCommand(adress));
    result.append(CreateNumberOfBytesLenghtCommand(lenght));
    return result;
}

QByteArray FirmwareMessageMaker::WriteBufferToFlashCommand(quint32 adress, quint16 lenght)
{
    QByteArray result;
    result.append(CreateTypeCommand((quint8)2));
    result.append(CreateAddressCommand(adress));
    result.append(CreateNumberOfBytesLenghtCommand(lenght));
    return result;
}

QByteArray FirmwareMessageMaker::MakeWriteOnlyBufferCommand()
{
    QByteArray result;
    result.append(CreateTypeCommand((quint8)6));
    result.append((quint8)10);
    result.append((quint8)0);
    return result;
}

QByteArray FirmwareMessageMaker::MakeReadOnlyBufferCommand()
{
    QByteArray result;
    result.append(CreateTypeCommand((quint8)4));
    result.append((quint8)10);
    result.append((quint8)0);
    return result;
}

QByteArray FirmwareMessageMaker::EraseCommand()
{
    QByteArray result;
    result.append(CreateTypeCommand((quint8)199));
    result.append((quint8)10);
    result.append((quint8)0);
    return result;
}

//QByteArray FirmwareMessageMaker::GetFlashIdCommand()
//{
//    QByteArray result;
//    result.append(CreateTypeCommand((quint8)9));
//    result.append(CreateNumberOfBytesLenghtCommand(3));
//    return result;
//}

//QByteArray FirmwareMessageMaker::ReadRegisterStatusCommand()
//{
//    QByteArray result;
//    result.append(CreateTypeCommand((quint8)5));
//    result.append((quint8)10);
//    result.append((quint8)0);
//    return result;
//}

QByteArray FirmwareMessageMaker::WriteDataToBufferCommand(bool isNewPage, const QByteArray &data)
{
    if(data.length()>16)
    {
        qFatal("Длина сообщения не может быть больше 16 байт");
    }
    else
    {
        quint8 messageLenght=data.length()-1;
        if(isNewPage)
        {
            messageLenght=messageLenght|m_significantBit;
        }
        QByteArray result;
        result.append(m_commands->at(4));
        result.append((quint8)(messageLenght));
        result.append(data);
        return result;
    }
}

//QByteArray FirmwareMessageMaker::ReadFromBufferCommand()//0d 4 команда чтение буфера в мою программу
//{
//    QByteArray result;
//    result.append(m_commands->at(3));
//    result.append((quint8)(0));
//    return result;
//}

QByteArray FirmwareMessageMaker::RestartFlashCommand()
{
    QByteArray result;
    result.append(m_commands->at(3));
    result.append((quint8)(90));
    return result;
}

QByteArray FirmwareMessageMaker::CreateTypeCommand(quint8 commandId)
{
    QByteArray result;
    result.append(m_commands->at(1));
    result.append(commandId);
    return result;
}

QByteArray FirmwareMessageMaker::CreateAddressCommand(quint32 adress)
{
    quint8 first=(adress >> (8*0)) & 0xff;
    quint8 second=(adress >> (8*1)) & 0xff;
    quint8 third=(adress >> (8*2)) & 0xff;
    QByteArray result;
    QDataStream streamMain(&result, QIODevice::WriteOnly);
    streamMain << (quint8)m_commands->at(2);
    streamMain << first;
    streamMain << second;
    streamMain << third;
    return result;
}

QByteArray FirmwareMessageMaker::CreateNumberOfBytesLenghtCommand(quint16 numberOfBytes)
{
    QByteArray result;
    result.append(m_commands->at(0));
    result.append(quint8(numberOfBytes-1));
    return result;
}
