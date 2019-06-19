#include "trainerwindow.h"
#include "ui_trainerwindow.h"

TrainerWindow::TrainerWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrainerWindow)
{
    ui->setupUi(this);
}

TrainerWindow::~TrainerWindow()
{
    delete ui;
}

void TrainerWindow::StartExercise()
{
    show();
    m_coach.StartExercise();
    on_bt_check_clicked();
}

void TrainerWindow::on_bt_check_clicked()
{
    if(!m_coach.Next())
    {
        close();
        return;
    }
    ui->tb_from->setText(m_coach.ValueToTranslate());
}
