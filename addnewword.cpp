#include "addnewword.h"
#include "ui_addnewword.h"

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
