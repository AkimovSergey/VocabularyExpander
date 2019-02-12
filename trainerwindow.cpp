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
