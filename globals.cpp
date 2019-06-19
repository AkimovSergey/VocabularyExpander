#include "globals.h"

#include <QDir>
#include <QStandardPaths>
#include <QThread>
#include <QTimer>

QString                       Globals::g_path_word_files;
QString                       Globals::g_path_program_data;
QScopedPointer<AddNewWord>    Globals::g_add_new_word;
QScopedPointer<Dictionary>    Globals::g_dictionary;
DeliveryBoy                   Globals::g_delivery_boy;
QScopedPointer<TrayIcon>      Globals::g_tray_icon;
QScopedPointer<TrainerWindow> Globals::g_main_window;
QScopedPointer<Settings>      Globals::g_settings;
Player                        Globals::g_player;
QScopedPointer<QThread>       Globals::g_exercise_thread;
QTimer *                      Globals::g_exercise_timer;

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
    g_add_new_word.reset(new AddNewWord);
    g_settings->Load();
    g_main_window.reset(new TrainerWindow());
    g_tray_icon.reset(new TrayIcon());
    g_tray_icon->Start();
    g_dictionary.reset(new Dictionary());
    g_dictionary->Load();
    StartExerciseThread();

}

void Globals::StartExerciseThread()
{
    g_exercise_thread.reset(new QThread(nullptr));
            //(QThread::create([]{g_exercise_timer->start(); }));
    g_exercise_timer = new QTimer(nullptr);
    g_exercise_timer->setInterval(g_settings->GetValue<int>(SETTINGS_REPEAT_EXERCISE_TIME) * 1000 * 60);
    g_exercise_timer->moveToThread(g_exercise_thread.get());
    g_exercise_timer->connect(g_exercise_thread.get(), SIGNAL(started()), SLOT(start()));
    g_exercise_timer->connect(g_exercise_timer, SIGNAL(timeout()), g_main_window.get(), SLOT(StartExercise()), Qt::DirectConnection);
    g_exercise_thread->start();

}

