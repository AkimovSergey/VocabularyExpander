#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QVariantMap>


#define _SETTINGS_IS_EXERCISE_ACTIVE                "is_exrcise_active"
#define _SETTINGS_LIST_OF_WORD_INDEXES_TO_LEARN     "list_of_word_indexes_to_learn"
#define _SETTINGS_INSTALLATION_COUNT_UPDATED        "installation_count_updated"
#define _SETTINGS_DIC_DIR                           "dic_dir"
#define _SETTINGS_LEARNED_COUNT                     "learned_count"

//default settings
#define _SETTINGS_UNIQ_INSTANCE_ID                  "uniq_instance_id"
// configs in window
#define _SETTINGS_REPEAT_EXERCISE_TIME              "repeat_exercise_time"
#define _SETTINGS_NUMBER_OF_WORDS_TO_LEARN          "amount_of_words_to_learn"
#define _SETTINGS_NUMBER_OF_WORDS_IN_EXERCISE       "amount_of_words_in_exercise"
#define _SETTINGS_EXCERCISE_CHECK_WAY               "exercise_check_way"
#define _SETTINGS_CHECK_WHOLE_WORD                  "check_whole_word"
#define _SETTINGS_USE_SOUND_TO_CHECK                "use_sound_to_check"
#define _SETTINGS_SUCCESSFUL_ATTEPTS_TO_COMPLETE    "successful_attempts_to_complete"
#define _SETTINGS_SAVE_IN_SERVER                    "save_in_server"
#define _SETTINGS_USE_DEFAULT_DIR_FOR_DIC           "use_default_dir_for_dic"
#define _SETTINGS_FONT_FOR_EXERCISE_WND             "font_for_exercise_window"
#define _SETTINGS_BLINKING_IF_WRONG                 "blinking_if_wrong"

#define _SETTINGS_EXAMPLES_SITE                     "examples_site"
#define _SETTINGS_FCD_FROM                          "fcd_from"
#define _SETTINGS_FCD_TO                            "fcd_to"




namespace Ui {
class Settings;
}
class Settings : public QDialog
{
      Q_OBJECT
public:

    Settings(QWidget *parent = nullptr);
    ~Settings(){}

    void InitializeDefault();
    void Load();
    void Save();
    template<class T>
    void SetValue(const char * name, T value);
    template<class T>
    bool GetValue(const char * name, T value);
    int GetCachedAttemptToComplete();

private:
    int                                m_cached_attempt_to_complete;
    void InitializeDefaults();
    QVariantMap    m_settings;

};
#endif // SETTINGS_H
