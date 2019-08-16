#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QVariantMap>


#define SETTINGS_IS_EXERCISE_ACTIVE                "is_exrcise_active"
#define SETTINGS_LIST_OF_WORD_INDEXES_TO_LEARN     "list_of_word_indexes_to_learn"
#define SETTINGS_DIC_DIR                           "dic_dir"
#define SETTINGS_LEARNED_COUNT                     "learned_count"

//default settings
#define SETTINGS_UNIQ_INSTANCE_ID                  "uniq_instance_id"
// configs in window
#define SETTINGS_REPEAT_EXERCISE_TIME              "repeat_exercise_time"
#define SETTINGS_AMOUNT_OF_WORDS_TO_LEARN          "amount_of_words_to_learn"
#define SETTINGS_AMOUNT_OF_WORDS_IN_EXERCISE       "amount_of_words_in_exercise"
#define SETTINGS_EXCERCISE_CHECK_WAY               "exercise_check_way"
#define SETTINGS_CHECK_WHOLE_WORD                  "check_whole_word"
#define SETTINGS_USE_SOUND_TO_CHECK                "use_sound_to_check"
#define SETTINGS_SUCCESSFUL_ATTEPTS_TO_COMPLETE    "successful_attempts_to_complete"
#define SETTINGS_SAVE_IN_SERVER                    "save_in_server"
#define SETTINGS_USE_DEFAULT_DIR_FOR_DIC           "use_default_dir_for_dic"
#define SETTINGS_FONT_FOR_EXERCISE_WND             "font_for_exercise_window"
#define SETTINGS_BLINKING_IF_WRONG                 "blinking_if_wrong"

#define SETTINGS_EXAMPLES_SITE                     "examples_site"
#define SETTINGS_FCD_FROM                          "fcd_from"
#define SETTINGS_FCD_TO                            "fcd_to"




namespace Ui {
class Settings;
}
class Settings : public QDialog
{
      Q_OBJECT
public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings(){}

    void InitializeDefault();
    void Load();
    void Save();
    template<class T>
    void SetValue(const char * name, T value)
    {
        m_settings[name].setValue(value);
    }
    template<class T>
    T GetValue(const char * name);
    int GetCachedAttemptToComplete();
    QString GetUserDictionaryDirectoryOrDefault();

private slots:
    void on_bt_choose_directory_clicked();
    void on_cb_use_default_dir_for_dic_stateChanged(int arg1);

private:
    int                                m_cached_attempt_to_complete;
    void InitializeDefaults();
    void UpdateView();
    QVariantMap    m_settings;
    Ui::Settings *ui;

protected:
    void closeEvent(QCloseEvent *event);

};



#endif // SETTINGS_H
