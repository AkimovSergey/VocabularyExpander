#ifndef DELIVERYBOY_H
#define DELIVERYBOY_H

#include <QString>
#include <QVector>
#include <QPair>
#include <QSharedPointer>
#include <optional>
#include "word.h"


class DeliveryBoy
{

public:
    explicit DeliveryBoy(QObject* parent = 0);
    ~DeliveryBoy();
    void FetchWord( QSharedPointer<Word> & wrd);
    std::optional<QString> FetchSound(const QString & word, const QString & path_to_save);
    void FetchExamples(QSharedPointer<Word> wrd);
    void UpdateInstallationCount(bool how, QString from = "", QString to = "");
    QString PostDictionaryServerData(const char * command, const char * data = 0);

private:
    void WebResultToWord(const QString & result, QSharedPointer<Word> & wrd);
    QVector<QPair<QString, QString>> ExtractExplsFromWebResult(const std::string & result);

};

#endif // DELIVERYBOY_H
