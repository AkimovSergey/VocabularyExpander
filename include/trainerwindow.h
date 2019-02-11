#ifndef TRAINERWINDOW_H
#define TRAINERWINDOW_H

#include <QMainWindow>

namespace Ui {
class TrainerWindow;
}

class TrainerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TrainerWindow(QWidget *parent = nullptr);
    ~TrainerWindow();

private:
    Ui::TrainerWindow *ui;
};

#endif // TRAINERWINDOW_H
