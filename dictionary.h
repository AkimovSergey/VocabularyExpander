#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QDialog>
#include <QSet>
#include <QSharedPointer>
#include "word.h"


namespace Ui {
class Dictionary;
}

class Dictionary : public QDialog
{
    Q_OBJECT

public:
    explicit Dictionary(QWidget *parent = nullptr);
    ~Dictionary();
    size_t GetDictionarySize(){return m_dictionary.size();}

private:
    Ui::Dictionary *ui;
    QSet<QSharedPointer<Word>>      m_dictionary;
};

#endif // DICTIONARY_H
