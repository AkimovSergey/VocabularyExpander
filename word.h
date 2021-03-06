#ifndef WORD_H
#define WORD_H

#include <QJsonObject>
#include <QPair>
#include <QSet>
#include <QVector>
#include <vector>

#define LEARNED_COUNT_NOT_STARTED_YET -1

class Word
{
public:
    friend class Dictionary;
    friend class Coach;

    Word(const QJsonObject &json);
    Word(Word && w) = default;
    Word(const QString & value, const QString & trans, const QString & from, const QString & to,
         const QSet<QString> & alt, const QVector<QPair<QString, QString>> & expls);
    Word(const QString & value, const QString & from, const QString & to);
    operator const QString&()const;
    operator QJsonObject () const;
    Word&							operator++();
    Word&							operator--();
    bool                            operator==(const Word & rhs){return rhs.m_value == m_value;}
    bool                            operator<(const Word & rhs) {return m_value < rhs.m_value;}
    bool                            operator<(const QString & rhs) {return m_value < rhs;}

    void                  Dispose();
    QString               GetTranslation(){return m_value_trans;};
    bool                  TrySound(bool play);
    size_t                GetOrder(){return m_order;}
    QString               GetWordValue()const {return m_value;}
    QString               GetHashString();
    int                   GetLearnedCount(){return m_learned_count;}
    QString               GetLangFrom()const{return m_lang_from;}
    QString               GetLangTo()const{return m_lang_to;}
    time_t                GetLastSuccesfulExrciseTime()const {return m_last_time;}
    QVector<QString> &    GetMeanings(){ return m_meanings; }
    QSet<QString> &       GetAlternatives(){ return m_alternative_trans; }
    QVector<QPair<QString, QString> > & GetExamples(){ return m_examples;};
    bool                  HasMeanings()const { return m_meanings.size() > 0; }
    bool                  HasAlternatives()const { return m_alternative_trans.size() > 0; }
    bool                  IsCompleted();
    bool                  IsForced()const{return m_forced;}
    void                  SetForced(bool how){m_forced = how;}
    bool                  IsCompletelyEqual(const Word & wrd);
    QString               GetProgressAsString();
    size_t                GetProgressPercentage(size_t amount_of_try);
    void                  SetAlts(QSet<QString> atrs){m_alternative_trans = atrs;}
    void                  SetTranslation(QString trans){m_value_trans = trans;}
    void                  SetExpls(QVector<QPair<QString,QString>> expls){m_examples = expls;};

private:
    QString                        m_value;
    QString                        m_value_trans;
    QVector<QString>               m_meanings;
    QString                        m_lang_to;
    QSet<QString>                  m_alternative_trans;
    QVector<QPair<QString, QString> > m_examples;
    QString                        m_lang_from;
    int                            m_learned_count;
    time_t                         m_last_time;
    bool                           m_forced;
    //                             in what order word has been added/changed
    size_t                         m_order;
};

#endif // WORD_H
