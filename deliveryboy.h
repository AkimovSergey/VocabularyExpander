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
    std::optional<QSharedPointer<Word>>FetchWord(const QString  word, const QString  from, const QString  to );
    bool FetchSound(const QString & word, const QString & from, const QString & path_to_save );
    QVector<QPair<QString, QString > > FetchExamples(const QString & word, const QString & from, const QString & to );
    void UpdateInstallationCount(bool how, QString from = "", QString to = "");
    QString PostDictionaryServerData(const char * command, const char * data = 0);

private:
    std::optional<QSharedPointer<Word>>WebResultToWord(const QString & result);
    QVector<QPair<QString, QString>> ExtractExplsFromWebResult(const QString & result);

};

#endif // DELIVERYBOY_H
