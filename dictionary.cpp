#include "dictionary.h"
#include "ui_dictionary.h"

Dictionary::Dictionary(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dictionary)
{
    ui->setupUi(this);
}

Dictionary::~Dictionary()
{
    delete ui;
}
