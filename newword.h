#ifndef NEWWORD_H
#define NEWWORD_H

#include <QDialog>

namespace Ui {
class NewWord;
}

class NewWord : public QDialog
{
    Q_OBJECT

public:
    explicit NewWord(QWidget *parent = nullptr);
    ~NewWord();

private:
    Ui::NewWord *ui;
};

#endif // NEWWORD_H
