#include "word.h"
#include "globals.h"

#include <QFileInfo>
#include <QJsonArray>
#include <QFile>

#define _DEF_NOT_FETCHED "Sorry can't fetch word translation via inet"
#define LEARNED_COUNT_NOT_STARTED_YET -1


Word::Word(const QJsonObject &json)
{
    try
    {
        auto load_from_json = [&](auto & res, QString nm)
        {
            if constexpr (std::is_same_v<decltype(res), QString>)
                if (json.contains(nm) && json[nm].isString()) res = json[nm].toString();
            if constexpr (std::is_same_v<decltype(res), QVector<QString>>)
                if(json[nm].isArray())
                {
                    QJsonArray arr = json[nm].toArray();
                        for(auto val : arr)
                    if (val.isString()) res.push_back(val.toString());
                }
        };

        load_from_json(m_value, "word");
        load_from_json(m_value_trans, "translation");
        load_from_json(m_lang_from, "from");
        load_from_json(m_lang_to, "to");
        load_from_json(m_alternative_trans, "alternatives");
        load_from_json(m_meanings, "definitions");
        m_learned_count = LEARNED_COUNT_NOT_STARTED_YET;
        m_order = Globals::g_dictionary.GetDictionarySize();
        m_forced = false;

    }
    catch (...)
    {

    }
}

void Word::Dispose()
{

    QString t_sound = Globals::g_path_word_files + (QString)*this + ".mp3";
    QFileInfo check_file(t_sound);
    if (check_file.exists() && check_file.isFile())
    {
        QFile file(t_sound);
            file.remove();
    }
}


bool Word::TrySound( bool play)
{
    QString t_sound = Globals::g_path_word_files + (QString)*this + ".mp3";
    QFileInfo check_file(t_sound);
    if (!check_file.exists())
    {
        t_sound = Globals::g_path_word_files +  GetHashString() + ".mp3";
        if(!check_file.exists())
            if(!Globals::g_delivery_boy.FetchSound( m_value, m_lang_from, t_sound ))
                return false;
    }

    if(play)
        Globals::g_player.PlayMedia(t_sound);
    return true;
}

Json::Value Word::ToJSONNode() const
{

    Json::Value res;
    res["word"] = m_value.ToUTF8().data();
    res["translation"] = m_value_trans.ToUTF8().data();
    res["from"] = m_lang_from.ToUTF8().data();
    res["to"] = m_lang_to.ToUTF8().data();

    if(!m_alternative_trans.empty())
    {
        Json::Value alts;
        for (size_t i = 0; i < m_alternative_trans.size(); i++)
            alts.append(m_alternative_trans[i].ToUTF8().data());
        res["alternatives"] = alts;
    }

    if(!m_meanings.empty())
    {
        Json::Value dfns;
        for (size_t i = 0; i < m_meanings.size(); i++)
            dfns.append(m_meanings[i].ToUTF8().data());
        res["definitions"] = dfns;
    }


    return res;
}

Word::operator const wxString&() const
{
    return m_value;
}

std::string Word::GetHashString()
{
    std::string res =  (std::string)m_value.ToUTF8();
    auto t = std::hash<std::string>()(res);
    return std::to_string(t);

}

bool Word::IsCompleted()
{
    return m_learned_count >= Globals::g_settings.GetCachedAttemptToComplete();
}

bool Word::IsCompletelyEqual(const Word & wrd)
{
    if(m_lang_from != wrd.m_lang_from ||
        m_lang_to !=  wrd.m_lang_to   ||
        m_value    != wrd.m_value     ||
        m_value_trans != wrd.m_value_trans)
        return false;

    if(m_alternative_trans.size() != wrd.m_alternative_trans.size()) return false;

    for(auto a : m_alternative_trans)
        if(wrd.m_alternative_trans.end() == std::find(wrd.m_alternative_trans.begin(), wrd.m_alternative_trans.end(), a))
            return false;

    if(m_meanings.size() != wrd.m_meanings.size()) return false;

    for(auto a : m_meanings)
        if(wrd.m_meanings.end() == std::find(wrd.m_meanings.begin(), wrd.m_meanings.end(), a))
            return false;

    return true;
}

wxString Word::GetProgressAsString()
{
    if(IsCompleted()) return L"DONE";
    int repetition = Globals::g_settings->GetCachedAttemptToComplete();
    if(m_learned_count == (repetition - 1)) return L"Last";
    return m_learned_count > 0 ? wxString::Format(L"%d", m_learned_count * 100 / repetition) + L" %" : L"0%";
}

size_t Word::GetProgressPercentage(size_t amount_of_try)
{
    return m_learned_count * 100 / amount_of_try;
}

Word& Word::operator++()
{
    m_learned_count++;
    if(((GetLearnedCount() * 100)/Globals::g_settings->GetCachedAttemptToComplete()) < 50)
        m_last_time = Globals::GetCurrentTime();
    if (m_learned_count >= Globals::g_settings->GetCachedAttemptToComplete())
    {
        int lc = 1;
        Globals::g_settings->GetValue(_SETTINGS_LEARNED_COUNT, lc);
        if (lc < 0)
            lc = LIMIT_BEFORE_WARNING;
        else
            lc++;
        Globals::g_settings->SetValue(_SETTINGS_LEARNED_COUNT, lc);
    }

    return *this;
}

Word& Word::operator--()
{
    if (m_learned_count > 0)
        m_learned_count--;

    return *this;
}



