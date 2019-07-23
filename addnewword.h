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

private:
    Ui::AddNewWord *ui;
};

#endif // ADDNEWWORD_H
