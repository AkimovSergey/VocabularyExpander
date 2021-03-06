#include "deliveryboy.h"
#include "globals.h"
#include "settings.h"
#include <optional>
#include <QString>
#include <QVector>
#include <regex>
#include <QUrlQuery>
#define CURL_STATICLIB
#if defined(WIN32) && !defined(UNIX)
#define QT_STRICT_ITERATORS
#include "external_libs/curl/include/curl.h"
#else
#include <curl/curl.h>
#include <QRegularExpression>
#endif

QMap<QString, QString> mp = {{"en", "english"}, {"ru", "russian"}};

template<class T> struct dependent_false : std::false_type {};

template<typename T>
int writer(char *data, size_t size, size_t nmemb,
                  T *writerData)
{
    if (writerData == NULL)
        return 0;
    if constexpr(std::is_same_v<T, QString>)
    {
        data[size*nmemb] = 0;
        writerData->append(data);
    }
    else if constexpr(std::is_same_v<T, std::vector<uint8_t>>)
        writerData->insert(writerData->end(), (uint8_t*)data, (uint8_t*)data + nmemb);
    else
        static_assert(dependent_false<T>::value);

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

template<typename T=QString>
T FetchWebPage(QByteArray rqst)
{
    CURL *curl;
    curl = curl_easy_init();
    T result;
    char error[4096];
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writer<T>);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        curl_easy_setopt(curl, CURLOPT_URL, rqst.data());
        auto ret = curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt ( curl, CURLOPT_ERRORBUFFER, error );
        auto code  = curl_easy_perform(curl);
        return result;
    }

    return result;
}

void DeliveryBoy::FetchWord( QSharedPointer<Word> & wrd)
{
    QString word = wrd->GetWordValue();
    word.replace(' ', '+');
    QUrlQuery query(QString("https://context.reverso.net/translation/%0-%1/%2").arg(mp[wrd->GetLangFrom()],
                    mp[wrd->GetLangTo()], word));
    const QString result = FetchWebPage(query.toString(QUrl().FullyEncoded).toUtf8());
    if(!result.isEmpty())
        WebResultToWord(result, wrd);
}

void DeliveryBoy::WebResultToWord(const QString & result, QSharedPointer<Word> & wrd)
{
    QSet<QString> alts;
    const std::string page = result.toStdString();
    std::regex rgx("title=\"[Noun|Adjective|Verb]\\w*\"><\\/span>[\\s]+<\\/div>[\\s]+(.*)(<\\/a>|<\\/div>)");
    std::smatch match;

    std::string::const_iterator search_start( page.cbegin() );
    while (std::regex_search(search_start, page.end(), match, rgx))
    {
        search_start = match.suffix().first;
        alts.insert(match[1].str().c_str());
        // assign first result for a case if we will not find result in examples
        if(wrd->GetTranslation().isEmpty())
            wrd->SetTranslation(match[1].str().c_str());
    }

    auto expls = ExtractExplsFromWebResult(page);
    // extract translation from examples
    // take first example
    QString trans;
    if(expls.size() != 0)
    {
        auto sr = expls[0].second.toStdString();
        std::regex rgx("<b>(.*)</b>");
        std::smatch match;
        if(std::regex_search(sr.cbegin(), sr.cend(), match, rgx))
            wrd->SetTranslation(match[1].str().c_str());
        else
            return;
    }
    wrd->SetAlts(alts);
    wrd->SetExpls(expls);
}

// define how many examples we want to take from page
static const int NUMBER_OF_EXAMPLES = 4;

QVector<QPair<QString, QString>> DeliveryBoy::ExtractExplsFromWebResult(const std::string & page)
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
    std::regex rgx("<div class=\"src ltr\">[\\s]+<span class=\"text\".*?>[\\s]+(.*)<\\/span>[\\s]+<\\/div>");
    std::smatch match;
    int count = NUMBER_OF_EXAMPLES;
    std::string::const_iterator search_start( page.cbegin() );
    while (std::regex_search(search_start, page.end(), match, rgx) && count--)
    {
        QString r = match[1].str().c_str();
        r.replace("<em>", "<b>").replace("</em>", "</b>");
        res.push_back({r, {}});
        search_start = match.suffix().first;
    }
    // looking for translations
    search_start = page.cbegin();
    std::regex rgx_tr("<div class=\"trg ltr\">[\\s]+<span class=\"icon jump-right\"><\\/span>[\\s]+<span class=\"text\".*?>[\\s.]+(.*)<\\/span>");
    for( auto & it : res)
    {
        if(std::regex_search(search_start, page.cend(), match, rgx_tr))
        {
            std::string rt = match[1].str().c_str();
            std::regex rem_a("<\\/?a[^>]*>");
            rt = std::regex_replace(rt,rem_a,"");
            QString r = rt.c_str();
            r.replace("<em>", "<b>").replace("</em>", "</b>");
            it.second = r;
            search_start = match.suffix().first;
        }
    }
    return res;
}


std::optional<QString> DeliveryBoy::FetchSound(const QString & word, const QString & from, const QString & path_to_save)
{
    QUrlQuery query("https://freetts.com/Home/PlayAudio?Language=en-US&Voice=Male&TextMessage=test");
    const QString result = FetchWebPage(query.toString(QUrl().FullyEncoded).toUtf8());
    const std::string page = result.toStdString();
    if(result.isEmpty())
        return {};
    std::smatch match;
    std::regex rgx("<source src=\"\\/audio\\/(.*\\.mp3)\" type=\"audio\\/mpeg\" \\/>");
    if(std::regex_search(page.cbegin(), page.cend(), match, rgx))
    {
        QString rt = match[1].str().c_str();
        QUrlQuery query_("https://freetts.com/audio/" + rt);
        const auto mp3_data = FetchWebPage<std::vector<uint8_t>>(query.toString(QUrl().FullyEncoded).toUtf8());
        QString file_name = path_to_save + '/' + rt;
        QFile file(file_name);
         if (!file.open(QIODevice::WriteOnly))
             return {};
         QDataStream out(&file);
         out << QVector<uint8_t>::fromStdVector(mp3_data);
         file.close();
        return {file_name};
    }
    return {};
}

void DeliveryBoy::FetchExamples(QSharedPointer<Word> wrd)
{
    const QString result = FetchWebPage(QString("https://context.reverso.net/translation/%0-%1/%2").arg(mp[wrd->GetLangFrom()],
                     mp[wrd->GetLangTo()], QUrl().toPercentEncoding(wrd->GetWordValue())).toUtf8());
    if(!result.isEmpty())
        wrd->SetExpls(ExtractExplsFromWebResult(result.toStdString()));
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




