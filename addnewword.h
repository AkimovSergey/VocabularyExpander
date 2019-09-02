#ifndef ADDNEWWORD_H
#define ADDNEWWORD_H

#include <QDialog>

namespace Ui {
class AddNewWord;
}

class AddNewWord : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewWord(QWidget *parent = nullptr);
    ~AddNewWord();

private slots:
    void on_bt_translate_clicked();
    void on_tb_add_word_clicked();

private:
    Ui::AddNewWord *                 ui;
    QString                          m_value;
    QString                          m_trans;
    QVector<QString>                 m_alternatives;
    QVector<QPair<QString, QString>> m_examples;

};

#endif // ADDNEWWORD_H
