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
    if(ui)
        delete ui;
}

void TrainerWindow::StartExercise()
{
    m_coach.StartExercise();
    ui->bt_check->setText("Next");
    on_bt_check_clicked();
    exec();

}

void TrainerWindow::closeEvent(QCloseEvent * event)
{
    Globals::g_dictionary->SaveExercises();
}

void TrainerWindow::keyPressEvent(QKeyEvent * e)
{
    on_bt_check_clicked();
}

void TrainerWindow::on_bt_check_clicked()
{
    QPalette palette = ui->tb_from->palette();
    if(ui->bt_check->text() == "Check")
    {
        if(m_coach.CheckAnswer(ui->tb_from->toPlainText()))
            palette.setColor(QPalette::Base, Qt::green);
        else
            palette.setColor(QPalette::Base, Qt::red);
        ui->tb_from->setPalette(palette);
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
        palette.setColor(QPalette::Base, Qt::white);
        ui->tb_from->setPalette(palette);
        ui->tb_from->clear();
        ui->tb_to->setText(m_coach.ValueToTranslate());
        ui->bt_check->setText("Check");
    }
}
