#ifndef TRAINERWINDOW_H
#define TRAINERWINDOW_H

#include <QDialog>

namespace Ui {
class TrainerWindow;
}

class TrainerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TrainerWindow(QWidget *parent = nullptr);
    ~TrainerWindow();

private:
    Ui::TrainerWindow *ui;
};

#endif // TRAINERWINDOW_H
