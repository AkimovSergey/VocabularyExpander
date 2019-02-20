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

private slots:
    void on_check_clicked();

    void on_pushButton_clicked();

private:
    Ui::TrainerWindow *ui;
};

#endif // TRAINERWINDOW_H
