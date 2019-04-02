#include "dictionary.h"
#include "ui_dictionary.h"
#include "globals.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

Dictionary::Dictionary(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dictionary)
{
    ui->setupUi(this);
}

Dictionary::~Dictionary()
{
    delete ui;
}

void Dictionary::Load()
{
    LoadDictionary();
    //LoadExercises(false);
}

void Dictionary::LoadDictionary()
{
    m_dictionary.clear();
    auto t_dictionary_path = Globals::g_settings->GetUserDictionaryDirectoryOrDefault(false);
    if(t_dictionary_path.isEmpty()) return;

    auto t_dictionary_full_path = t_dictionary_path + "dictionary.json";

    QFile loadFile(t_dictionary_full_path);

   if (loadFile.open(QIODevice::ReadOnly))
   {
       QByteArray saveData = loadFile.readAll();
       QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
       QJsonArray array = loadDoc.array();

       foreach (const QJsonValue & v, array)
           m_dictionary.append(QSharedPointer<Word>::create(v.toObject()));
   }

}
