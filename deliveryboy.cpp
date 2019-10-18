#include "deliveryboy.h"
#include "globals.h"
#include "settings.h"
#include <optional>
#include <QString>
#include <QVector>
#include <regex>
#define CURL_STATICLIB
#if defined(WIN32) && !defined(UNIX)
#include "external_libs/curl/include/curl.h"
#else
#include <curl/curl.h>

#include <QRegularExpression>
#endif

int writer(char *data, size_t size, size_t nmemb,
                  QString *writerData)
{
    if (writerData == NULL)
        return 0;
    data[size*nmemb] = 0;
    writerData->append(data);

    return size * nmemb;
};

DeliveryBoy::DeliveryBoy(QObject*)
{
    curl_global_init(CURL_GLOBAL_ALL);
}

DeliveryBoy::~DeliveryBoy()
{

    curl_global_cleanup();

}

void DeliveryBoy::FetchWord( QSharedPointer<Word> & wrd)
{
    CURL *curl;
    QMap<QString, QString> mp = {{wrd->GetLangFrom(), "english"}, {wrd->GetLangTo(), "russian"}};
    curl = curl_easy_init();
    QString result;
    if(curl)
    {
        auto rqst = (QString("https://context.reverso.net/translation/%0-%1/%2").arg(mp[wrd->GetLangFrom()],
                     mp[wrd->GetLangTo()], QUrl().toPercentEncoding(wrd->GetWordValue()))).toUtf8();
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writer);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        curl_easy_setopt(curl, CURLOPT_URL, rqst.data());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

        curl_easy_perform(curl);
        if(result.isEmpty())
            return;
        WebResultToWord(result, wrd);
    }
}

void DeliveryBoy::WebResultToWord(const QString & result, QSharedPointer<Word> & wrd)
{
    QSet<QString> alts;
    const std::string s = result.toStdString();
    std::regex rgx("title=\"[Noun|Adjective|Verb]\\w*\"><\\/span>[\\s]+<\\/div>[\\s]+(.*)(<\\/a>|<\\/div>)");
    std::smatch match;

    std::string::const_iterator search_start( s.cbegin() );
    while (std::regex_search(search_start, s.end(), match, rgx))
    {
        qDebug() << match[1].str().c_str();
        search_start = match.suffix().first;
        alts.insert(match[1].str().c_str());
    }

    auto expls = ExtractExplsFromWebResult(result);
    // extract translation from examples
    // take first example
    QString trans;
    if(expls.size() != 0)
    {
        std::regex rgx("<em>(.*)</em>");
        std::smatch match;
        if(std::regex_search(expls[0].second.toStdString().cbegin(), s.end(), match, rgx))
            wrd->SetTranslation(match[1].str().c_str());
        else
            return;
    }
    wrd->SetAlts(alts);
    wrd->SetExpls(expls);
}

QVector<QPair<QString, QString>> DeliveryBoy::ExtractExplsFromWebResult(const QString & result)
{
 /*QString r;
    QRegularExpression re("<div class=\"trg ltr\">[\\s]+<span class=\"icon jump-right\"><\\/span>[\\s]+<span class=\"text\" lang='ru'>([\\S\\s.]+)<\\/div>",QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatch mch = re.match(result);
    if (mch.hasMatch()) {
        int startOffset = mch.capturedStart(1); // startOffset == 6
        int endOffset = mch.capturedEnd(1); // endOffset == 9
        r = result.mid(startOffset, endOffset - startOffset);
        qDebug()<<r;
    }*/


    QVector<QPair<QString, QString>> res;
    const std::string s = result.toStdString();
    std::regex rgx("<div class=\"src ltr\">[\\s]+<span class=\"text\">[\\s]+(.*)<\\/span>[\\s]+<\\/div>");
    std::smatch match;
    // just take max 4 examples
    int count = 4;
    std::string::const_iterator search_start( s.cbegin() );
    while (std::regex_search(search_start, s.end(), match, rgx) && count--)
    {
        qDebug() << match[1].str().c_str();
        res.push_back({match[1].str().c_str(), {}});
        search_start = match.suffix().first;
    }
    // looking for translations
    search_start = s.cbegin();
    std::regex rgx_tr("<div class=\"trg ltr\">[\\s]+<span class=\"icon jump-right\"><\\/span>[\\s]+<span class=\"text\" lang='ru'>[\\s.]+(.*)<\\/span>");
    for( auto & it : res)
    {
        if(std::regex_search(search_start, s.end(), match, rgx_tr))
        {
            std::string rt = match[1].str().c_str();
            std::regex rem_a("<\\/?a[^>]*>");
            rt = std::regex_replace(rt,rem_a,"");
            it.second = rt.c_str();
            search_start = match.suffix().first;
        }
    }
    return res;
}


/*int64_t GetTKKFromGoogle(CURL * curl)
{

    std::string result;
    std::string rqst = std::string("https://translate.google.com");

    //std::string rqst = std::string("translate.google.com/translate_tts?tl=") + frm + "&q=" + wrd;

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
    curl_easy_setopt(curl, CURLOPT_URL, rqst.c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

    auto r = curl_easy_perform(curl);


    if(result.empty())
        return false;

    std::string fnd = "TKK=eval('((function(){var a\\x3d";
    auto pos  = result.find(fnd);
    if(pos == std::string::npos) return 0; else pos += fnd.length();
    fnd = ';';
    auto pos2  = result.find(fnd, pos);
    if(pos2 == std::string::npos) return 0;
    fnd = result.substr(pos, pos2 - pos);
    int64_t a, b;
    sscanf(fnd.c_str(), "%lld", &a);

    fnd = "b\\x3d";
    pos  = result.find(fnd, pos2);
    if(pos == std::string::npos) return 0; else pos += fnd.length();
    fnd = ';';
    pos2  = result.find(fnd, pos);
    if(pos2 == std::string::npos) return 0;
    fnd = result.substr(pos, pos2 - pos);
    sscanf(fnd.c_str(), "%lld", &b);


    return (int64_t)a + (int64_t)b;

}

std::string TranslateStringToWebString(CURL * curl, const wxString & str)
{
    std::string result = (std::string)str.ToUTF8();
    auto to_delete = curl_easy_escape(curl, result.c_str(), result.length());
    result = to_delete;
    curl_free(to_delete);
    return result;

}

int64_t GetTE(int64_t a, const char * str)
{
    size_t len = strlen(str);
    for( size_t i = 0; i < len - 2; i += 3)
    {
        int32_t d = str[ i + 2 ];
        d = d >= 'a' ? d - 87 : d - 0x30;
        d =	str[ i + 1 ] == '+' ? a >> d : a << d ;
        a = str[i] == '+' ? (a + d) & 0xFFFFFFFF : a ^ d;
    }
    return a;
}

std::string GetTK(const wxString & word, CURL * curl)
{
    const char * Vb = "+-a^+6";
    const char * Ub = "+-3^+b+-f";
    static int32_t e;
    static int64_t d;

    if( !e || e != time(nullptr)/3600)
    {
        e = time(nullptr)/3600;
        d = GetTKKFromGoogle(curl);
    }
    int32_t b = e;
    int32_t a = e;

    auto str = word.ToUTF8();
    size_t len = str.length();
    for(size_t y = 0; y < len; y++)
    {
        a += (uint8_t)str[y];
        a = GetTE(a, Vb);
    }

    a = GetTE(a, Ub);

    a ^= d;

    uint32_t ta = *((uint32_t*)&a);

    //if( a < 0 )
        //a = ((uint32_t)a & 0x7FFFFFFF) + 0x80000000;

    ta = ta % 1000000;

    return std::to_string(ta) + "." + std::to_string( ta ^ b );

    /*
        int64_t RL(int64_t a, const char * b)
{
    for( size_t i = 0; i < strlen(b) - 2; i += 3)
    {
        int64_t d = b[ i + 2 ];
        d = d >= 'a' ? d - 87 : Number(d) 54 '6';
        d =	b[ i + 1 ] == '+' ? a >> d : a << d ;

        a = b[i] == '+' ? a + (d & 0xFFFFFFFF) : a ^ d;
    }

    return a;
}


int _tmain(int argc, _TCHAR* argv[])
{

    const char * Vb = "+-a^+6";
    const char * Ub = "+-3^+b+-f";
    int8_t d[] = {100, 111, 103};
    int64_t a = 403190;

    for(auto i: d)
    {
        a += i;
        a = RL(a, Vb);
    }
    a = RL(a, Ub);
}*/

bool DeliveryBoy::FetchSound(const QString & word, const QString & from, const QString & path_to_save )
{

    /*std::string result;
    CURL *curl;

    curl = curl_easy_init();
    if(curl)
    {

        std::string frm = TranslateStringToWebString(curl, from);
        std::string wrd = TranslateStringToWebString(curl, word);


        std::string rqst = std::string("https://translate.google.com/translate_tts?ie=UTF-8&q=" + wrd + "&tl=" +
            frm + "&total=1&idx=0&textlen=" + std::to_string(word.Length()) + "&tk=" + GetTK(word, curl) + "&client=t");

        //std::string rqst = std::string("translate.google.com/translate_tts?tl=") + frm + "&q=" + wrd;

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writer);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        curl_easy_setopt(curl, CURLOPT_URL, rqst.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

        auto r = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

    }
    if(result.empty())
        return false;

    wxFile res(path_to_save, wxFile::OpenMode::write);
    if(res.IsOpened())
        res.Write(result.c_str(), result.length());


    return true;

    {
    /*ISpVoice * p_voice;
    CoInitialize(0);
    HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&p_voice);
    if( SUCCEEDED( hr ) )
    {
        p_voice->SetVolume(100);
        //pVoice->SetVoice();
        hr = p_voice->Speak(L"Hello papa", SPF_IS_XML, NULL);
        p_voice->Release();
        p_voice = NULL;
    }
    }
    {
    const unsigned int HTTP_TIMEOUT = 10;

    const RequestsHolder::Request & request = Globals::g_requests_holder.GetRequest(_REQUEST_GET_SOUND);
    wxString rqst = wxString::Format(request.m_request,from, word);

    wxURI uri(rqst);
    rqst = uri.BuildURI();

    wxHTTP http;
    http.SetHeader(
        wxT("User-Agent"),
        wxT("Mozilla/5.0 (X11; U; FreeBSD i386; ru-RU; rv:1.9.2.13) Gecko/20110202 Firefox/3.6.13")
      );
    if ( !http.Connect(request.m_address) )
    {
        Globals::DisplayError(L"Cannot connect to server. Check network connection.");
        return false;
    }

     wxInputStream* inputStream = http.GetInputStream(rqst);
     wxProtocolError err;
    if ((err = http.GetError()) != wxPROTO_NOERR)
    {
        Globals::DisplayError(L"Server return error. Probably there is no audio for this word or language.");
        return false;
    }

    wxFile res(path_to_save, wxFile::OpenMode::write);
    wxFileOutputStream out_stream(res);
    inputStream->Read(out_stream);
    res.Close();

    }*/
 return true;
}

//bool DeliveryBoy::FetchTranslation(QString & word, QString & from, QString & to, QString & trans, QVector<QString> & defs, QVector<QString> & alts)
//{
    /*std::string result;

    UpdateInstallationCount(true, from, to);

    auto curl = curl_easy_init();

    if(curl)
    {
        std::string frm = TranslateStringToWebString(curl, from);
        std::string t = TranslateStringToWebString(curl, to);
        std::string wrd = TranslateStringToWebString(curl, word);
        std::string rqst = std::string("https://glosbe.com/gapi/translate?from=" + frm + "&dest=" +
            to + "&format=json&phrase=" + wrd);


        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writer);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_URL, rqst.c_str());

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        //curl_free(output);

        if(res != CURLE_OK)
            return false;

        Json::Reader reader;
        Json::Value root;
        if(reader.parse(result, root, false))
        {
            try
            {
                if("ok" == root["result"].asString())
                {
                    auto items = root["tuc"];
                    if(items.size() == 0 )
                        return false;

                    for(int i = 0; i < items.size(); ++i)
                    {
                        Json::Value def_value;
                        auto val = items[i].get("phrase", def_value);
                        if(val != def_value)
                        {
                            if( i == 0)
                                trans = (wxString::FromUTF8(val["text"].asCString()));
                            else
                            {
                                auto wxs = wxString::FromUTF8(val["text"].asCString());
                                if(wxs != trans && alts.end() == std::find(alts.begin(), alts.end(), wxs))
                                    alts.push_back(wxs);
                            }
                        }

                        val = items[i].get("meanings", def_value);
                        if(val != def_value && val.isArray())
                            for(int e = 0; e < val.size(); e++)
                            {
                                auto wxs = wxString::FromUTF8(val[e]["text"].asCString());
                                if(defs.end() == std::find(defs.begin(), defs.end(), wxs))
                                    defs.push_back(wxs);
                            }
                    }

                }

            }
            catch(...)
            {
                return false;
            }
        }
    }

   // return true;
}*/

void DeliveryBoy::UpdateInstallationCount( bool how, QString from, QString to)
{

    /*std::string req = "http://www.vocabularyexpander.com/php/installation_count.php?how=";

    if(how)
    {
        bool check = false;
        Globals::g_settings->GetValue(_SETTINGS_INSTALLATION_COUNT_UPDATED, check);
        if(!check)
        {
            Globals::g_settings->SetValue(_SETTINGS_INSTALLATION_COUNT_UPDATED, true);
            req += "1";
        }
        else
            return;
    }
    else
        req += "0";

    if(from.length() > 0)
        req += "&from=" + from.ToStdString();

    if(to.length() > 0)
        req += "&to=" + to.ToStdString();

    CURL *curl;
    curl = curl_easy_init();
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, req.c_str());

        curl_easy_perform(curl);
        curl_easy_cleanup(curl);

    }*/
    /*const unsigned int HTTP_TIMEOUT = 10;

    const RequestsHolder::Request & request = Globals::g_requests_holder.GetRequest(_REQUEST_GET_SOUND);

    wxURI uri(request.m_request);
    auto rqst = uri.BuildURI();

    wxHTTP http;
    http.SetHeader(
        wxT("User-Agent"),
        wxT("Mozilla/5.0 (X11; U; FreeBSD i386; ru-RU; rv:1.9.2.13) Gecko/20110202 Firefox/3.6.13")
      );
    if ( !http.Connect(request.m_address) )
        return false;

    wxInputStream* inputStream = http.GetInputStream(rqst);
    wxProtocolError err;
    if ((err = http.GetError()) != wxPROTO_NOERR) return false;*/

}


QString DeliveryBoy::PostDictionaryServerData(const char * command, const char * data)
{
    /*CURL *curl;
    CURLcode res;

    wxString result;
 #if defined(WIN32) && !defined(UNIX)
    /* In windows, this will init the winsock stuff */
    /*curl_global_init(CURL_GLOBAL_ALL);
    wxString name, pass;
    int id;
    curl = curl_easy_init();
    if(curl)
    {
        if(!Globals::g_settings->GetValue(_SETTINGS_DB_USER_NAME, name)) return result;
        if(!Globals::g_settings->GetValue(_SETTINGS_DB_USER_PASSWORD, pass)) return result;
        if(!Globals::g_settings->GetValue(_SETTINGS_UNIQ_INSTANCE_ID , id)) return result;
        std::string nm = (std::string)name.ToUTF8();
        std::string ps = (std::string)pass.ToUTF8();
        if(!data)
        {
            auto to_delete = curl_easy_escape(curl, nm.c_str(), nm.length());
            nm = to_delete;
            curl_free(to_delete);
            to_delete = curl_easy_escape(curl, ps.c_str(), ps.length());
            ps = to_delete;
            curl_free(to_delete);
        }
        std::string rqst = std::string("user=") + nm + "&password=" +
            ps + "&command=" + command + "&instance_id=" + std::to_string(id);

        //char *output = curl_easy_escape(curl, rqst.c_str(), rqst.length());

        /* get a curl handle */
        //curl = curl_easy_init();
        /*curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writer);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);

        if(data)
        {
            curl_easy_setopt(curl, CURLOPT_URL, "http://www.vocabularyexpander.com/php/dictionary_manager.php");
            rqst += "&data=";
            rqst += data;
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, rqst.c_str());
        }
        else
        {
            rqst.insert(0, "http://www.vocabularyexpander.com/php/dictionary_manager.php?");
            curl_easy_setopt(curl, CURLOPT_URL, rqst.c_str());
        }

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

    }
#endif*/
    return "";//result;
}


QVector<QString> FetchExamplesFromBabla(const QString & word, const QString & from, const QString & to )
{
    /*std::list<wxString> result;
    std::string wrd = (std::string)word.ToUTF8();
    std::string res;
    std::string rqst = std::string("http://en.bab.la/dictionary/" + mmap[from] + "-" + mmap[to] + "/" + wrd);

    auto curl = curl_easy_init();
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writer);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);
        curl_easy_setopt(curl, CURLOPT_URL, rqst.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

        auto r = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    if(res.length() > 0)
    {
        int pos = 0, pos_end = 0;

        while(true)
        {
            pos = res.find("span6 result-left", pos);
            std::string fnd = "<b>" + wrd + "</b>";
            if( std::string::npos == (pos = res.find(fnd, pos))) break;
            pos_end = pos + fnd.length();

            while( res[pos] != '>')
                pos--;
            pos +=1 ;//exclude '>'
            while( res[pos_end] != '<')
                pos_end++;

            result.push_back(res.substr(pos, pos_end - pos));

            pos = res.find("span6 result-right", pos_end);

            if( std::string::npos == (pos = res.find("<b>", pos))) break;
            if( std::string::npos == (pos_end = res.find("</b>", pos + 3))) break;

            pos_end += 4;

            while( res[pos] != '>')
                pos--;
            pos +=1 ;//exclude '>'
            while( res[pos_end] != '<')
                pos_end++;

            result.push_back(res.substr(pos, pos_end - pos));

            pos = pos_end;
        }
    }*/

    return {""};//result;
}




