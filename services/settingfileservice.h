#ifndef SETTINGFILESERVICE_H
#define SETTINGFILESERVICE_H

#include <QFile>
#include <QDebug>
#include <QObject>
#include <QDomDocument>

class SettingFileService
{
public:
    explicit SettingFileService(const QString &fileName);
    ~SettingFileService();
    const QString GetAttribute(const QString &tagName, const QString &attributeName, const QString &defaultValue);
    void SetAttribute(const QString &tagName, const QString &attributeName, const QString &value);
private:
    void ReadSettingsDocument();
    void WriteSettingsDocument();
private:
    const QString m_fileName;
    QDomDocument* const m_document;
};

#endif // SETTINGFILESERVICE_H
