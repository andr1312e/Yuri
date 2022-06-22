#include "services/settingfileservice.h"

SettingFileService::SettingFileService(const QString &fileName)
    : m_fileName(fileName)
    , m_document(new QDomDocument())
{
    ReadSettingsDocument();
}

SettingFileService::~SettingFileService()
{
    WriteSettingsDocument();
    delete m_document;
}

QString SettingFileService::GetAttribute(const QString &tagName, const QString &attributeName, const QString &defaultValue)
{
    const QDomElement rootElem = m_document->firstChildElement();
    const QDomNodeList settingsList = rootElem.childNodes();
    for (int i = 0; i < settingsList.count(); ++i)
    {
        const QDomElement setting = settingsList.at(i).toElement();
        if (tagName == setting.tagName())
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
    QDomElement rootElem = m_document->firstChildElement();
    const QDomNodeList settingsList = rootElem.childNodes();
    for (int i = 0; i < settingsList.count(); ++i)
    {
        QDomElement settingElement = settingsList.at(i).toElement();
        if (tagName == settingElement.tagName())
        {
            settingElement.setAttribute(attributeName, value);
            return;
        }
    }
    QDomElement lastElem = m_document->createElement(tagName);
    lastElem.setAttribute(attributeName, value);
    rootElem.appendChild(lastElem);
}

void SettingFileService::ReadSettingsDocument()
{
    QFile file(m_fileName);
    if (file.open(QIODevice::ReadOnly))
    {
        const QByteArray fileContent = file.readAll();
        file.close();
        const bool successfulRead = m_document->setContent(fileContent);
        if (!successfulRead)
        {
            m_document->clear();
            const QDomElement firstElem = m_document->createElement("settings");
            m_document->appendChild(firstElem);
        }
    }
    else
    {
        qWarning("%s", file.errorString().toUtf8().constData());
    }
}

void SettingFileService::WriteSettingsDocument() const
{
    const QByteArray stringDocumetString = m_document->toString().toUtf8();
    QFile settingFile(m_fileName);
    if (settingFile.open(QIODevice::WriteOnly))
    {
        settingFile.write(stringDocumetString);
        settingFile.close();
    }
    else
    {
        qWarning("%s", QString("Settings file is not avaliable %1").arg(settingFile.errorString()).toUtf8().constData());
    }
}
