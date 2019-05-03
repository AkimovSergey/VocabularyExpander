#include "dictionary.h"
#include "ui_dictionary.h"
#include "globals.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardItemModel>

Dictionary::Dictionary(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dictionary)
{
    ui->setupUi(this);
    ui->tv_dictionary->setSelectionBehavior(QAbstractItemView::SelectRows);
}

Dictionary::~Dictionary()
{
    SaveExercises();
    SaveDictionary();
    delete ui;
}

void Dictionary::InitAndFill()
{
    static QStringList horizontalHeader = {"Frc", "State", "Value", "From", "Translation", "To"};
    m_model.setHorizontalHeaderLabels(horizontalHeader);
    FillModel();
    ui->tv_dictionary->setModel(&m_model);
    ui->tv_dictionary->resizeRowsToContents();
    ui->tv_dictionary->resizeColumnsToContents();
}

void Dictionary::FillModel()
{
    for(auto & it : m_dictionary)
    {
        QStandardItem * checkable = new QStandardItem();
        checkable->setCheckable(true);
        checkable->setCheckState(it->IsForced() ? Qt::Checked : Qt::Unchecked);
        m_model.appendRow({checkable, new QStandardItem(it->GetProgressAsString()),
                           new QStandardItem(it->GetWordValue()),
                           new QStandardItem(it->GetLangFrom()),
                           new QStandardItem(it->GetTranslation()),
                           new QStandardItem(it->GetLangTo())});
    }
}

void Dictionary::Load()
{
    LoadDictionary();
    LoadExercises();
    InitAndFill();
}

QJsonArray LoadJsonArrayFromDefDirectoryFile(QString file)
{
    auto t_file_path = Globals::g_settings->GetUserDictionaryDirectoryOrDefault(false);
    if(t_file_path.isEmpty())
        throw QString("No such file %1").arg(file);
    auto t_file_full_path = t_file_path + file;
    QFile loadFile(t_file_full_path);
    if (loadFile.open(QIODevice::ReadOnly))
    {
       QByteArray saveData = loadFile.readAll();
       QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
       return loadDoc.array();
    }
}

void Dictionary::LoadDictionary()
{
    m_dictionary.clear();
    try
    {
       auto array = LoadJsonArrayFromDefDirectoryFile("dictionary.json");
       foreach (const QJsonValue & v, array)
           m_dictionary.append(QSharedPointer<Word>::create(v.toObject()));
    }
    catch (QString err)
    {
        // May be for future log
    }
}

void Dictionary::SaveDictionary()
{

}

void Dictionary::LoadExercises()
{
    auto load_from_json = [&](auto & res, QString nm, const QJsonValue & val)
    {
        QJsonObject obj = val.toObject();
        if constexpr (std::is_same_v<decltype(res), QString &>)
        {
            if (obj.contains(nm) && obj[nm].isString()) res = obj[nm].toString();
        }
        else  res = obj[nm].toInt();
    };

    try
    {
        QString word;
        auto array = LoadJsonArrayFromDefDirectoryFile("exercises.json");
        foreach (const QJsonValue & v, array)
        {
           load_from_json(word, "word", v);
           auto wrd = FindWordByValue(word);
           if(wrd)
           {
               load_from_json(wrd->m_last_time, "time", v);
               load_from_json(wrd->m_learned_count, "count", v);
           }
        }
    }
    catch (QString err)
    {
        // May be for future log
    }

}

void Dictionary::SaveExercises()
{

}

QSharedPointer<Word> Dictionary::FindWordByValue(QString val)
{
    auto res = std::find_if(m_dictionary.begin(), m_dictionary.end(), [&](QSharedPointer<Word> & ptr){return ptr->GetWordValue() == val;});
    if(res == m_dictionary.end())
        return {};
    else return *res;
}
