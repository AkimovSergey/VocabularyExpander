#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QDialog>
#include <QList>
#include <QSharedPointer>
#include <QStandardItemModel>
#include "word.h"

#define EXERCISES_FN  "exercises.json"
#define DICTIONARY_FN "dictionary.json"


namespace Ui {
class Dictionary;
}

class Dictionary : public QDialog
{
    Q_OBJECT

public:
    explicit Dictionary(QWidget *parent = nullptr);
    ~Dictionary();
    size_t GetDictionarySize(){return m_dictionary.length();}
    const QList<QSharedPointer<Word>> GetDictionary(){return m_dictionary;}
    void Load();

private:
    void InitAndFill();
    void LoadDictionary();
    void SaveDictionary();
    void LoadExercises();
    void SaveExercises();
    void FillModel();
    QSharedPointer<Word> FindWordByValue(QString val);
    Ui::Dictionary                   *ui;
    QList<QSharedPointer<Word>>      m_dictionary;
    QStandardItemModel               m_model;
};

#endif // DICTIONARY_H
