#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>
#include "trainerwindow.h"
#include "deliveryboy.h"
#include "trayicon.h"
#include "settings.h"
#include "dictionary.h"
#include "player.h"

class Globals
{
public:
    static QString                      g_path_word_files;
    static QString                      g_path_program_data;
    static DeliveryBoy                  g_delivery_boy;
    static QString                      g_lang_choises[];
    static TrayIcon                     g_tray_icon;
    static TrainerWindow                g_main_window;
    static Settings                     g_settings;
    static Dictionary                   g_dictionary;
    static QTimer                       g_timer;
    static Player                       g_player;
    static bool                         g_unloading;
    static void                         Initialize();
    static void                         DisplayError(QString err);
    static void                         Unloading();
    static bool                         IsRegistered();
    static bool                         CheckRegistration();
    // need for test purposes
    static time_t                       GetCurrentTime();

};


#endif // GLOBALS_H
