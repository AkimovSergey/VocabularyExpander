#include "trayicon.h"

#include <QAction>
#include <QCoreApplication>
#include <QMenu>

TrayIcon::TrayIcon():m_tray_icon(QIcon(":res/resources/icon16.bmp"), this)
{

}

void TrayIcon::s_AddFlashCard()
{

}

void TrayIcon::Start()
{
    auto quitAction = new QAction(QIcon(":res/resources/exit.png"), "&Quit", this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    auto menu = new QMenu();
    menu->addAction(quitAction);
    auto add_flash_card_Action = new QAction(QIcon(":res/resources/add.png"), "Add flash card", this);
    connect(add_flash_card_Action, SIGNAL(triggered()), this, SLOT(s_AddFlashCard()));
    menu->addAction(add_flash_card_Action);
    m_tray_icon.setContextMenu(menu);
    m_tray_icon.show();
}
