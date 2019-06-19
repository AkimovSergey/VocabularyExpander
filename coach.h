
#pragma once
#include <vector>
#include <deque>
#include <QQueue>
#include "dictionary.h"

class MainWindow;

enum EXERCISE_WAY
{
	DIRECT,
	REVERSE,
	SOUND,
	MAX,
    NOPE
};

class Coach
{
    friend class UnitTest;

public:

	bool StartExercise();
    bool IsReadyToContinue(const time_t & now, size_t attps, const QSharedPointer<Word> & wrd);
    bool FillRandomWithCmp(size_t num, std::function<bool(const QSharedPointer<Word>&)> cmp);

    QString ValueToTranslate();
    QString ValueToCheck();
    QString Tip();
    bool Next();
    QString To();
    QString From();
	bool IsTipUsed() { return m_b_tip_used; }
	bool IsLastTryForWord();
    EXERCISE_WAY GetCurrentExerciseWay() { return m_current_exrcise_way; };
    QSharedPointer<Word> GetCurrentWord();
    bool CheckAnswer(QString & ans);
    QString GetCurrentProgress();


private:

    QVector<QSharedPointer<Word >>  m_exercise_set;
    QQueue<QSharedPointer<Word >>  m_penalty_set;
	EXERCISE_WAY        m_current_exrcise_way;
	bool                m_b_tip_used;


	EXERCISE_WAY RandomExerciseWay();

};
