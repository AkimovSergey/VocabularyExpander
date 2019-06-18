#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>

class TrayIcon: public QObject {
    Q_OBJECT

public:
    TrayIcon();
    void Start();
private:
    QSystemTrayIcon m_tray_icon;
public slots:
   void closing();
};

#endif // TRAYICON_H
