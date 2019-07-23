#include "addnewword.h"
#include "ui_addnewword.h"
#include "globals.h"

AddNewWord::AddNewWord(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNewWord)
{
    ui->setupUi(this);
}

AddNewWord::~AddNewWord()
{
    delete ui;
}

void AddNewWord::on_bt_translate_clicked()
{
    Globals::g_delivery_boy.FetchWord(ui->tb_from->toPlainText(), ui->cb_from->currentText(), ui->cb_to->currentText());
}
