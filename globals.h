#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>
#include <QDebug>
#include "trainerwindow.h"
#include "deliveryboy.h"
#include "trayicon.h"
#include "settings.h"
#include "dictionary.h"
#include "player.h"
#include "addnewword.h"

class Globals
{
public:
    static QString                       g_path_word_files;
    static QString                       g_path_program_data;
    static DeliveryBoy                   g_delivery_boy;
    static QString                       g_lang_choises[];
    static QScopedPointer<TrayIcon>      g_tray_icon;
    static QScopedPointer<TrainerWindow> g_main_window;
    static QScopedPointer<Settings>      g_settings;
    static QScopedPointer<Dictionary>    g_dictionary;
    static QScopedPointer<AddNewWord>    g_add_new_word;
    static QTimer *                      g_exercise_timer;
    static QScopedPointer<QThread>       g_exercise_thread;
    static Player                        g_player;
    static bool                          g_unloading;
    static void                          Initialize();
    static void                          DisplayError(QString err);
    static void                          Unloading();
    static bool                          IsRegistered();
    static bool                          CheckRegistration();
    static void                          StartExerciseThread();
    // need for test purposes
    static time_t                        GetCurrentTime();
};

#endif // GLOBALS_H
