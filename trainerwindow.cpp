#include "trainerwindow.h"
#include "ui_trainerwindow.h"
#include "globals.h"

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
    update();
    m_coach.StartExercise();
    ui->bt_check->setText("Next");
    on_bt_check_clicked();
}

void TrainerWindow::closeEvent(QCloseEvent * event)
{
    Globals::g_dictionary->SaveExercises();
}

void TrainerWindow::on_bt_check_clicked()
{
    if(ui->bt_check->text() == "Check")
    {
        if(m_coach.CheckAnswer(ui->tb_from->toPlainText()))
            ui->tb_from->setTextBackgroundColor(Qt::green);
        else
            ui->tb_from->setTextBackgroundColor(Qt::red);
        ui->tb_from->setText(m_coach.ValueToCheck());
        ui->bt_check->setText("Next");
    }
    else
    {
        if(!m_coach.Next())
        {
            close();
            return;
        }
        ui->tb_from->clear();
        ui->tb_to->setText(m_coach.ValueToTranslate());
        ui->bt_check->setText("Check");
    }
}
