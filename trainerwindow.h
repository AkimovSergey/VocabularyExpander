#ifndef TRAINERWINDOW_H
#define TRAINERWINDOW_H

#include <QDialog>
#include "coach.h"

namespace Ui {
class TrainerWindow;
}

class TrainerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TrainerWindow(QWidget *parent = nullptr);
    ~TrainerWindow() override;
public slots:
    void StartExercise();
private slots:
    void on_bt_check_clicked();

private:
    bool eventFilter(QObject* o, QEvent* e);
    Ui::TrainerWindow *ui;
    Coach             m_coach;

protected:
    void closeEvent(QCloseEvent * event) override;
};

#endif // TRAINERWINDOW_H
