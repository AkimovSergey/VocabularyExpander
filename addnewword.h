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

private:
    Ui::AddNewWord *ui;
};

#endif // ADDNEWWORD_H
