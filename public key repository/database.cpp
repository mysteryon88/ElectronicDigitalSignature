#include <iostream>
#include "database.h"

Database::Database(QObject *parent)
    : QObject{parent}
{
}

void Database::ConnectDatabase()
{
    if(!QFile(DATABASE_NAME).exists())
    {
        RestoreDatabase();
        std::cout << "Database has been restored" << std::endl;
    }
    else
    {
        OpenDatabase();
        std::cout << "Database connection successful" << std::endl;
    }
}

bool Database::RestoreDatabase()
{
    if(OpenDatabase())
        return (CreateTable()) ? true : false;
    else
    {
        std::cout << "Failed to restore database" << std::endl;
        return false;
    }
}

bool Database::OpenDatabase()
{

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName(DATABASE_NAME);

    if(db.open()) return true;
    else return false;

}

void Database::CloseDatabase()
{
    db.close();
}

bool Database::CreateTable()
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " TABLE " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            TABLE_FNAME     " VARCHAR(255)    NOT NULL,"
                            TABLE_SNAME     " VARCHAR(255)    NOT NULL,"
                            TABLE_PAT       " VARCHAR(255)    NOT NULL,"
                            TABLE_MOD       " VARCHAR(255)    NOT NULL,"
                            TABLE_EXP       " VARCHAR(255)    NOT NULL,"
                            TABLE_EMAIL     " VARCHAR(255)    NOT NULL"
                        " )"))
    {
        std::cout << "Database: error of create " << TABLE << std::endl;
        std::cout << query.lastError().text().toStdString() << std::endl;
        return false;
    }
    else return true;
}

bool Database::InserIntoTable(const QVariantList &data)
{
    QSqlQuery query;

    query.prepare("INSERT INTO " TABLE " ( " TABLE_FNAME ", "
                                             TABLE_SNAME ", "
                                             TABLE_PAT   ", "
                                             TABLE_MOD   ", "
                                             TABLE_EXP   ", "
                                             TABLE_EMAIL " ) "
        "VALUES (:FName, :SName, :Pat, :Mod, :Exp, :Email)");
    query.bindValue(":FName",       data[0].toString());
    query.bindValue(":SName",       data[1].toString());
    query.bindValue(":Pat",         data[2].toString());
    query.bindValue(":Mod",         data[3].toString());
    query.bindValue(":Exp",         data[4].toString());
    query.bindValue(":Email",       data[5].toString());


    if(!query.exec()) {
        std::cout <<  "Database: Error insert into " << TABLE << std::endl;
        std::cout << query.lastError().text().toStdString() << std::endl;
        return false;
    }
    else return true;
}

bool Database::InserIntoTable(const QString &fname, const QString &sname,
                              const QString &pat,   const QString &mod,
                              const QString &exp,   const QString &e_mail)
{
    QVariantList data;
    data.append(fname);
    data.append(sname);
    data.append(pat);
    data.append(mod);
    data.append(exp);
    data.append(e_mail);

    if(InserIntoTable(data)) return true;
    else return false;
}

bool Database::RemoveRecord(const int id)
{
    QSqlQuery query;

    query.prepare("DELETE FROM " TABLE " WHERE id= :ID ;");
    query.bindValue(":ID", id);

    if(!query.exec())
    {
        std::cout << "Database: Error delete row " << TABLE << std::endl;
        std::cout << query.lastError().text().toStdString() << std::endl;
        return false;
    }
    else return true;
}

QString Database::FindKey(const QString email)
{
    QSqlQuery query;
    query.prepare("SELECT Modulus, Open_exp FROM " TABLE " WHERE Email = '" + email + "'");

    if (!query.exec()) {
        std::cout << "Database: error of select " << TABLE << std::endl;
        std::cout << query.lastError().text().toStdString() << std::endl;
        return "err1";
    }

    QString key;
    if (query.first())
    {
        key = query.value(0).toString();
        key += " ";
        key += query.value(1).toString();
        std::cout << key.toStdString() << std::endl;
        return key;
    }
    else
    {
        std::cout << "No email in " << TABLE << std::endl;
        return "err2";
    }
}
