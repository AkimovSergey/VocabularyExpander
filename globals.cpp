#include "globals.h"

#include <QDir>
#include <QStandardPaths>

QString                      Globals::g_path_word_files;
QString                      Globals::g_path_program_data;
QScopedPointer<Dictionary>   Globals::g_dictionary;
DeliveryBoy                  Globals::g_delivery_boy;
QScopedPointer<TrayIcon>     Globals::g_tray_icon;
QScopedPointer<TrainerWindow> Globals::g_main_window;
QScopedPointer<Settings>     Globals::g_settings;
Player                       Globals::g_player;

void Globals::Initialize()
{

    if (g_path_program_data.isEmpty())
        g_path_program_data = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator();
    if(!QDir(g_path_program_data).exists())
        QDir().mkdir(g_path_program_data);

    g_path_word_files = g_path_program_data + "word_files" + QDir::separator();
    if(!QDir(g_path_word_files).exists())
        QDir().mkdir(g_path_word_files);

    g_settings.reset(new Settings());
    g_settings->Load();
    g_main_window.reset(new TrainerWindow());
    g_tray_icon.reset(new TrayIcon());
    g_tray_icon->Start();
    g_dictionary.reset(new Dictionary());
    g_dictionary->Load();

}

