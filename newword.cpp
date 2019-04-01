#include "newword.h"
#include "ui_newword.h"

NewWord::NewWord(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewWord)
{
    ui->setupUi(this);
}

NewWord::~NewWord()
{
    delete ui;
}
