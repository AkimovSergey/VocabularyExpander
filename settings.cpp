#include "settings.h"
#include "globals.h"
#include "ui_settings.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>


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
    auto res = QFileDialog::getExistingDirectory();
    auto prev = Globals::g_settings->GetValue<QString>(SETTINGS_DIC_DIR);
    QDir qd;

       if (!res.isEmpty() && prev != res)
       {
           if (!QFileInfo(res + DICTIONARY_FN).exists())
           {
               if(QFileInfo(prev + DICTIONARY_FN).exists())
               {
                   if((QMessageBox::Yes == QMessageBox::question(this, "Question", "Selected folder doesn't contain dictionary file.\r\n"
                       "Do you want to save current dictionary here?",  QMessageBox::Yes | QMessageBox::No)))
                       {
                           if (!qd.rename(prev + DICTIONARY_FN, res + DICTIONARY_FN))
                           {
                               QMessageBox::warning(this, "Warning", "Can't move dictionary file, check if you have access");
                               return;
                           }

                           if(QFileInfo(prev + EXERCISES_FN).exists())
                               qd.rename(prev + EXERCISES_FN, res + EXERCISES_FN);
                           SetValue(SETTINGS_DIC_DIR, res);
                       }
                       else
                       {
                           SetValue(SETTINGS_DIC_DIR, res);
                           Globals::g_dictionary->Load();
                       }
               }

           }
           else
           {
               if(QFileInfo(prev + DICTIONARY_FN).exists())
               {
                   if((QMessageBox::No == QMessageBox::question(this, "Question", "New folder already has the dictionary.\r\n"
                       "Do you want to load dictionary from this directory (YES) or replace with current dictionary? (NO)", QMessageBox::Yes | QMessageBox::No)))
                   {
                       if (!qd.remove(res + DICTIONARY_FN))
                       {
                           QMessageBox::warning(this, "Warning", "Can't delete dictionary file, check if you have access");
                           return;
                       }
                       qd.rename(prev + DICTIONARY_FN, res + DICTIONARY_FN);
                       if(QFileInfo(prev + DICTIONARY_FN).exists())
                       {
                           qd.remove(res + EXERCISES_FN);
                           qd.rename(prev + EXERCISES_FN, res + EXERCISES_FN);
                       }
                   }
                   else
                   {
                       SetValue(SETTINGS_DIC_DIR, res);
                       Globals::g_dictionary->Load();
                   }
               }
           }
           SetValue(SETTINGS_DIC_DIR, res);
           ui->st_dic_dir_value->setText(res);
       }

}
