#include "trayicon.h"
#include "globals.h"

#include <QAction>
#include <QCoreApplication>
#include <QMenu>
#include <QMessageBox>

TrayIcon::TrayIcon():m_tray_icon(QIcon(":res/resources/icon16.bmp"), this)
{

}

void TrayIcon::Start()
{
    auto menu = new QMenu();

    auto add_flash_card_Action = new QAction(QIcon(":res/resources/add.png"), "Add flash card", this);
    connect(add_flash_card_Action, &QAction::triggered, this, [=](){
        Globals::g_add_new_word->exec();
    });
    menu->addAction(add_flash_card_Action);

    auto start_exercise_Action = new QAction(QIcon(":res/resources/play.png"), "Start exercise", this);
    connect(start_exercise_Action, &QAction::triggered, this, [=](){
        Globals::g_main_window->exec();
    });
    menu->addAction(start_exercise_Action);

    auto show_dictionary_Action = new QAction(QIcon(":res/resources/dic.png"), "Show dictionary", this);
    connect(show_dictionary_Action, &QAction::triggered, this, [=](){
        Globals::g_dictionary->exec();
    });
    menu->addAction(show_dictionary_Action);

    auto settings_Action = new QAction(QIcon(":res/resources/sett.png"), "Settings", this);
    connect(settings_Action, &QAction::triggered, this, [=](){
        Globals::g_settings->exec();
    });
    menu->addAction(settings_Action);

    auto activate_deactivate_Action = new QAction(QIcon(":res/resources/paus.png"), "Deactivate", this);
    connect(activate_deactivate_Action, &QAction::triggered, this, [=](){
        Globals::g_settings->SetValue("is_exrcise_active", !Globals::g_settings->GetValue<bool>("is_exrcise_active"));
    });
    menu->addAction(activate_deactivate_Action);

    auto add_about_Action = new QAction(QIcon(":res/resources/about.png"), "About", this);
    connect(add_about_Action, &QAction::triggered, this, [=](){
        QMessageBox msgBox;
        msgBox.setText("Not implemented yet.");
        msgBox.exec();
    });
    menu->addAction(add_about_Action);

    auto quitAction = new QAction(QIcon(":res/resources/exit.png"), "&Quit", this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    menu->addAction(quitAction);

    m_tray_icon.setContextMenu(menu);
    m_tray_icon.show();
}
