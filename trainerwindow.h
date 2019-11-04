#ifndef TRAINERWINDOW_H
#define TRAINERWINDOW_H

#include <QDialog>
#include <QLabel>
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
    void on_bt_examples_clicked();

private:
    bool eventFilter(QObject* o, QEvent* e);
    void SetProgresslabel();
    Ui::TrainerWindow* ui;
    Coach              m_coach;
    QLabel*            m_progress_label;

protected:
    void closeEvent(QCloseEvent * event) override;
};

#endif // TRAINERWINDOW_H
