
#include "globals.h"
#include "settings.h"
#include "coach.h"
#include <algorithm>
#include <QDateTime>


bool Coach::StartExercise()
{
    m_exercise_set.clear();
	m_penalty_set.clear();

    auto & gdic = Globals::g_dictionary->GetDictionary();
	if(!gdic.size()) return false;

    size_t count = std::min(Globals::g_settings->GetValue<int>(SETTINGS_NUMBER_OF_WORDS_IN_EXERCISE), gdic.size());

    time_t now = QDateTime::currentSecsSinceEpoch();
	size_t atteption_complete = Globals::g_settings->GetCachedAttemptToComplete();

    if(!FillRandomWithCmp(count, [=](const QSharedPointer<Word> & ptr){ return ptr->IsForced(); }))
	{
		size_t filled_by_forced = m_exercise_set.size();
        size_t cnt = std::min(Globals::g_settings->GetValue<int>(SETTINGS_NUMBER_OF_WORDS_TO_LEARN), gdic.size());

		if(!FillRandomWithCmp(cnt, std::bind(&Coach::IsReadyToContinue, this, now, atteption_complete, std::placeholders::_1)))
            FillRandomWithCmp(cnt, [=](const QSharedPointer<Word> & ptr){
				if(ptr->GetLearnedCount() == LEARNED_COUNT_NOT_STARTED_YET) {/* mark as taken to exrcises*/++(*ptr) ; return true;}
				else return false;});

		std::random_shuffle(m_exercise_set.begin() + filled_by_forced, m_exercise_set.end());
		if(m_exercise_set.size() > count)
			m_exercise_set.erase(m_exercise_set.begin() + count, m_exercise_set.end());
	}

	if(!(m_exercise_set.empty()))
	{
		m_current_exrcise_way = RandomExerciseWay();
		return true;
	}

    return false;

}


bool Coach::IsReadyToContinue( const time_t & now, size_t attps, const QSharedPointer<Word> & wrd)
{
	auto & gdic = Globals::g_dictionary->GetDictionary();

		if (wrd->m_learned_count != LEARNED_COUNT_NOT_STARTED_YET)
		{
			if(wrd->GetProgressPercentage(attps) >= 50 && wrd->m_last_time != 0)
			{
				auto diff_days = difftime(now, wrd->m_last_time) / 60 / 60 / 24;
				
				if(wrd->GetProgressPercentage(attps) >= 75)
				{
					if(diff_days >= 7)
						return true;
					else
						return false;
				}

				if(diff_days >= 2)
					return true;
			}
			else
				return true;

		}

		return false;
}

bool Coach::FillRandomWithCmp(size_t num, std::function<bool(const QSharedPointer<Word>&)> cmp)
{
    srand(unsigned(time(nullptr)));
	auto & gdic = Globals::g_dictionary->GetDictionary();
	int atteption_complete = Globals::g_settings->GetCachedAttemptToComplete();

    while (m_exercise_set.size() < num)
	{
		int dic_size = (int)gdic.size();
		int index = 0;
		int search_index = std::rand() % dic_size;
		for (;;index++)
		{
			int plus_ind = search_index + index;
			int minus_ind = search_index - index;

			if (minus_ind < 0 && plus_ind >(dic_size - 1))
				goto out;

            auto lbd = [&](int ind)
            {
				auto it = gdic.begin();
				std::advance(it, ind);
				if ((*it)->GetLearnedCount() < atteption_complete &&
					m_exercise_set.end() == std::find(m_exercise_set.begin(), m_exercise_set.end(), *it) &&
					cmp((*it)) )
                {
                    m_exercise_set.push_back(*it);
                    return true;
                }
                return false;
            };

			if (minus_ind >= 0)
                if (lbd(minus_ind))
                    break;
            if (plus_ind <= (dic_size - 1))
                if (lbd(plus_ind))
                    break;

		}
	}

out:

	return m_exercise_set.size() == num;

}

QString Coach::ValueToTranslate()
{
	if (m_current_exrcise_way == EXERCISE_WAY::REVERSE)
		return GetCurrentWord()->GetTranslation();
	else
		return GetCurrentWord()->GetWordValue();
}

QString Coach::ValueToCheck()
{
    if (m_current_exrcise_way == EXERCISE_WAY::REVERSE)
        return GetCurrentWord()->GetWordValue();
    else
        return GetCurrentWord()->GetTranslation();
}

QString Coach::Tip()
{
	m_b_tip_used = true;
    return ValueToCheck().at(0);
}

bool Coach::Next()
{

	m_b_tip_used = false;
    
	if(m_exercise_set.size() > 0)
		m_exercise_set.pop_back();
	else if(m_penalty_set.size() > 0)
		m_penalty_set.pop_back();


	if(m_exercise_set.size() > 0)
		m_current_exrcise_way = RandomExerciseWay();
	else if(m_penalty_set.size() > 0)
		m_current_exrcise_way = EXERCISE_WAY::REVERSE;
	
	return (m_exercise_set.size() > 0 || m_penalty_set.size() > 0);
   
}

QString Coach::To()
{
	if (m_current_exrcise_way == EXERCISE_WAY::REVERSE)
        return GetCurrentWord()->GetLangTo().mid(0,2);
	else
        return GetCurrentWord()->GetLangFrom().mid(0,2);
}
QString Coach::From()
{
	if (m_current_exrcise_way == EXERCISE_WAY::REVERSE)
        return GetCurrentWord()->GetLangFrom().mid(0,2);
	else
        return GetCurrentWord()->GetLangTo().mid(0,2);
}


EXERCISE_WAY Coach::RandomExerciseWay()
{

    // temporary solution just make it work for me
    return EXERCISE_WAY::REVERSE;

    srand(unsigned(time(NULL)));
    int max = 0;
    std::vector<int> arr;
    switch (Globals::g_settings->GetValue<int>(SETTINGS_EXCERCISE_CHECK_WAY))
    {
    case 1:
        arr.push_back((int)EXERCISE_WAY::REVERSE);
        if(Globals::g_settings->GetValue<bool>(SETTINGS_CHECK_WHOLE_WORD) && IsLastTryForWord())
			return EXERCISE_WAY::REVERSE;
        max++;
    case 0:
        arr.push_back((int)EXERCISE_WAY::DIRECT);
        max++;
    }

    if (Globals::g_settings->GetValue<bool>(SETTINGS_USE_SOUND_TO_CHECK))
    {
        arr.push_back((int)EXERCISE_WAY::SOUND);
        max++;
    }

    return (EXERCISE_WAY)(arr[std::rand() % max]);
}

bool Coach::CheckAnswer(QString & ans)
{

	bool res = false;
	auto val = ValueToCheck();
	auto wrd = GetCurrentWord();

	if(m_exercise_set.size() > 0)
	{
		if (ans.length() == 1 && IsTipUsed())
		{
			--(*wrd);
			res = false;
		}
		// need to check full word
        else if (IsLastTryForWord() && Globals::g_settings->GetValue<bool>(SETTINGS_CHECK_WHOLE_WORD))
		{
			if(res = (val == ans))
				++(*wrd);
		}
        else if(res = (ans.length() > 0 && val.startsWith(ans)))
		{
			++(*wrd);

			// bonus if fill full word with reverse but not the last time
			if( val == ans && (GetCurrentExerciseWay() == EXERCISE_WAY::REVERSE) && !IsLastTryForWord())
				++(*wrd);

		}

		if(!res)
		{
			--(*wrd);
			m_penalty_set.push_front(wrd);
		}

	}
	else
	{
		if(val == ans)
		{
			++(*wrd);
			res = true;
		}
		else
			m_penalty_set.push_front(m_penalty_set.back());
	}

	return res;
}

bool Coach::IsLastTryForWord()
{
	return GetCurrentWord()->GetLearnedCount() == (Globals::g_settings->GetCachedAttemptToComplete() - 1);
}

QSharedPointer<Word> Coach::GetCurrentWord()
{ 
	if(m_exercise_set.size() > 0)
		return m_exercise_set.back(); 
	if(m_penalty_set.size() > 0)
		return m_penalty_set.back(); 
	return 0;
}

QString Coach::GetCurrentProgress()
{
	if(m_exercise_set.size())
		return GetCurrentWord()->GetProgressAsString();
	else
        return "FINE";
}
