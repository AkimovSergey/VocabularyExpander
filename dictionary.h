#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QDialog>
#include <QList>
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
    void Load();

private:
    void InitAndFill();
    void LoadDictionary();
    Ui::Dictionary *ui;
    QList<QSharedPointer<Word>>      m_dictionary;
};

#endif // DICTIONARY_H
