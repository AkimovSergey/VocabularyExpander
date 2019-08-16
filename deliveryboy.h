#ifndef DELIVERYBOY_H
#define DELIVERYBOY_H

#include <QString>
#include <QVector>
#include <QPair>
#include <QSharedPointer>
#include "word.h"


class DeliveryBoy: public QObject
{
    Q_OBJECT

public:
    explicit DeliveryBoy(QObject* parent = 0);
    QSharedPointer<Word>FetchWord(const QString  word, const QString  from, const QString  to );
    bool FetchSound(const QString & word, const QString & from, const QString & path_to_save );
    bool FetchTranslation(QString & word, QString & from, QString & to, QString & trans, QVector<QString> & defs, QVector<QString> & alts);
    QVector<QPair<QString, QString > > FetchExamples(const QString & word, const QString & from, const QString & to );
    void UpdateInstallationCount(bool how, QString from = "", QString to = "");
    QString PostDictionaryServerData(const char * command, const char * data = 0);

};

#endif // DELIVERYBOY_H
