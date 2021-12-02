#include "settingfileservice.h"

SettingFileService::SettingFileService(const QString &fileName)
    : m_fileName(fileName)
{
    m_document=new QDomDocument();
    ReadSettingsDocument();
}

SettingFileService::~SettingFileService()
{
    WriteSettingsDocument();
    delete m_document;
}

const QString SettingFileService::GetAttribute(const QString &tagName, const QString &attributeName, const QString &defaultValue)
{
    QDomElement settings=m_document->firstChildElement();
    QDomNodeList settigList=settings.childNodes();
    for (int i=0; i<settigList.count(); ++i)
    {
        QDomElement setting=settigList.at(i).toElement();
        if (tagName==setting.tagName())
        {
            if (setting.hasAttribute(attributeName))
            {
                return setting.attribute(attributeName);
            }
        }
    }
    SetAttribute(tagName, attributeName, defaultValue);
    return defaultValue;
}

void SettingFileService::SetAttribute(const QString &tagName, const QString &attributeName, const QString &value)
{
    QDomElement settings=m_document->firstChildElement();
    QDomNodeList settigList=settings.childNodes();
    for (int i=0; i<settigList.count(); ++i)
    {
        QDomElement setting=settigList.at(i).toElement();
        if (tagName==setting.tagName())
        {
            setting.setAttribute(attributeName, value);
            return;
        }
    }
    QDomElement lastElem=m_document->createElement(tagName);
    lastElem.setAttribute(attributeName, value);
    settings.appendChild(lastElem);
}

void SettingFileService::ReadSettingsDocument()
{
    QFile file(m_fileName);
    if (file.open(QIODevice::ReadWrite))
    {
        QByteArray arr=file.readAll();
        file.close();
        bool sucsesfullReal=m_document->setContent(arr);
        if(!sucsesfullReal)
        {
            m_document->clear();
            QDomElement firstElem=m_document->createElement("settings");
            m_document->appendChild(firstElem);
        }
    }
    else
    {
        qWarning(file.errorString().toLatin1());
    }
}

void SettingFileService::WriteSettingsDocument()
{
    QByteArray stringDocumetString=m_document->toString().toLatin1();
    QFile settingFile(m_fileName);
    if (settingFile.open(QIODevice::WriteOnly))
    {
        settingFile.write(stringDocumetString);
        settingFile.close();
    }
    else
    {
        qWarning("Settings file is not avaliable" + settingFile.errorString().toLatin1());
    }
}
