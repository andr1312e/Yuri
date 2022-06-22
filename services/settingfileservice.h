#ifndef SERVICES_SETTINGFILESERVICE_H
#define SERVICES_SETTINGFILESERVICE_H

#include <QFile>
#include <QDebug>
#include <QObject>
#include <QDomDocument>

class SettingFileService
{
public:
    explicit SettingFileService(const QString &fileName);
    ~SettingFileService();
public:
    QString GetAttribute(const QString &tagName, const QString &attributeName, const QString &defaultValue);
    void SetAttribute(const QString &tagName, const QString &attributeName, const QString &value);
private:
    void ReadSettingsDocument();
    void WriteSettingsDocument() const;
private:
    const QString m_fileName;
    QDomDocument* const m_document;
};

#endif // SERVICES_SETTINGFILESERVICE_H
