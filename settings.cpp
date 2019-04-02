#include "settings.h"
#include "globals.h"

#include <QJsonArray>
#include <QJsonDocument>

Settings::Settings(QWidget *parent):m_settings{{_SETTINGS_IS_EXERCISE_ACTIVE , false}}
{

}

void Settings::Load()
{
    auto t_settings_full_path = Globals::g_path_program_data + "settings.json";
    QFile loadFile(t_settings_full_path);

   if (loadFile.open(QIODevice::ReadOnly))
   {
       QByteArray saveData = loadFile.readAll();
       QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
       m_settings = loadDoc.object().toVariantMap();
   }
}

int Settings::GetCachedAttemptToComplete()
{
    /*if(m_cached_attempt_to_complete == 0 )
        m_cached_attempt_to_complete = GetDefValueInt(_SETTINGS_SUCCESSFUL_ATTEPTS_TO_COMPLETE);*/

    return m_cached_attempt_to_complete;

}

template<>
bool Settings::GetValue<bool>(const char * name)
{
  return m_settings[name].toBool();
}

template<>
QString Settings::GetValue<QString>(const char * name)
{
  return m_settings[name].toString();
}

template<>
int Settings::GetValue<int>(const char * name)
{
  return m_settings[name].toInt();
}

QString Settings::GetUserDictionaryDirectoryOrDefault(bool silent)
{

    if (GetValue<bool>(_SETTINGS_USE_DEFAULT_DIR_FOR_DIC))
            return Globals::g_path_program_data;
    return Globals::g_settings->GetValue<QString>(_SETTINGS_DIC_DIR);
}
