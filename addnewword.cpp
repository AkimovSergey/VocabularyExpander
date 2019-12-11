#include "addnewword.h"
#include "ui_addnewword.h"
#include "globals.h"

AddNewWord::AddNewWord(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNewWord)
{
    ui->setupUi(this);
    ui->cb_from->addItems(Globals::g_lang_choises);
    ui->cb_to->addItems(Globals::g_lang_choises);
}

AddNewWord::~AddNewWord()
{
    delete ui;
}


void AddNewWord::on_bt_translate_clicked()
{
    QSharedPointer<Word> res(new Word(ui->tb_from->toPlainText(), ui->cb_from->currentText(), ui->cb_to->currentText()));
    Globals::g_delivery_boy->FetchWord(res);
    if(!res->GetTranslation().isEmpty())
    {
        ui->tb_to->setText(res->GetTranslation());
        for(auto & it : res->GetAlternatives())
            ui->tb_synonyms->append(it);
        for(auto & it : res->GetExamples())
            ui->tb_examples->append(it.first + "\n" + it.second);
    }

}

void AddNewWord::on_tb_add_word_clicked()
{
    Globals::g_dictionary->AddWord({ui->tb_from->toPlainText(),
                                    ui->tb_to->toPlainText(),
                                    ui->cb_from->currentText(),
                                    ui->cb_to->currentText(),
                                    m_alternatives,
                                    m_examples});
}
