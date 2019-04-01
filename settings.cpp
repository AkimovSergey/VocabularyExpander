#include "settings.h"

Settings::Settings(QWidget *parent):m_settings{{"is_exrcise_active", false}}
{

}

int Settings::GetCachedAttemptToComplete()
{
    /*if(m_cached_attempt_to_complete == 0 )
        m_cached_attempt_to_complete = GetDefValueInt(_SETTINGS_SUCCESSFUL_ATTEPTS_TO_COMPLETE);*/

    return m_cached_attempt_to_complete;

}

template<>
bool Settings::GetValue<bool>(const char * name)
{
  return true;
}
