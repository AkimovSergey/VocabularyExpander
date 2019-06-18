#include "settings.h"
#include "globals.h"
#include "ui_settings.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QFileDialog>


template<>
bool Settings::GetValue<bool>(const char * name)
{
    if(!m_settings.contains(name))
        return false;
    return m_settings[name].toBool();
}

template<>
QString Settings::GetValue<QString>(const char * name)
{
    if(!m_settings.contains(name))
        return "";
    return m_settings[name].toString();
}

template<>
int Settings::GetValue<int>(const char * name)
{
    if(!m_settings.contains(name))
        return  -1;
    return m_settings[name].toInt();
}

Settings::Settings(QWidget *parent):  QDialog(parent),
    ui(new Ui::Settings), m_settings{{SETTINGS_IS_EXERCISE_ACTIVE , false},
                                     {SETTINGS_USE_DEFAULT_DIR_FOR_DIC, true}}
{
    ui->setupUi(this);
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
    else
       Save();
    UpdateView();
}

void Settings::UpdateView()
{
    if(GetValue<bool>(SETTINGS_USE_DEFAULT_DIR_FOR_DIC))
    {

    }
}

void Settings::Save()
{
    auto t_settings_full_path = Globals::g_path_program_data + "settings.json";
    QFile jsonFile(t_settings_full_path);
    jsonFile.open(QFile::WriteOnly);
    QJsonDocument json = QJsonDocument::fromVariant(m_settings);
    jsonFile.write(json.toJson());
}

int Settings::GetCachedAttemptToComplete()
{
    /*if(m_cached_attempt_to_complete == 0 )
        m_cached_attempt_to_complete = GetDefValueInt(_SETTINGS_SUCCESSFUL_ATTEPTS_TO_COMPLETE);*/

    return m_cached_attempt_to_complete;

}

QString Settings::GetUserDictionaryDirectoryOrDefault()
{

    if (GetValue<bool>(SETTINGS_USE_DEFAULT_DIR_FOR_DIC))
            return Globals::g_path_program_data;
    return Globals::g_settings->GetValue<QString>(SETTINGS_DIC_DIR);
}

void Settings::on_bt_choose_directory_clicked()
{
    // cannot pass this it become frozen
    QFileDialog dialog(nullptr);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.exec();
}
