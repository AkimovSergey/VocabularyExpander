#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>
#include <QDebug>
#include <QThread>
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
    static QScopedPointer<DeliveryBoy>   g_delivery_boy;
    static QString                       g_lang_choises[];
    static QScopedPointer<TrainerWindow> g_main_window;
    static QScopedPointer<Dictionary>    g_dictionary;
    static QScopedPointer<AddNewWord>    g_add_new_word;
    static QTimer *                      g_exercise_timer;
    static QScopedPointer<QThread>       g_exercise_thread;
    static Player                        g_player;
    static QScopedPointer<Settings>      g_settings;
    static QScopedPointer<TrayIcon>      g_tray_icon;
    static bool                          g_unloading;
    static void                          Initialize();
    static void                          DisplayError(QString err);
    static void                          Unloading();
    static bool                          IsRegistered();
    static bool                          CheckRegistration();
    static void                          StartExerciseThread();
    // need for test purposes
    static time_t                        GetCurrentTime();

    static void AboutToQuit()
    {
        g_exercise_thread->exit();
        //g_exercise_timer->stop();
        g_exercise_thread->wait();
        g_dictionary.reset();
        g_main_window.reset();

    }
};

#endif // GLOBALS_H
