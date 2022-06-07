#ifndef DATABASE_H
#define DATABASE_H

#include <QSql>
#include <QFile>
#include <QDate>
#include <QDebug>
#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>

#define DATABASE_HOSTNAME       "hostname"
#define DATABASE_NAME           "esd.db"

#define TABLE                   "Clients"

#define TABLE_FNAME             "Name"
#define TABLE_SNAME             "Surname"
#define TABLE_PAT               "Patronymic"
#define TABLE_MOD               "Modulus"
#define TABLE_EXP               "Open_exp"
#define TABLE_EMAIL             "Email"

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    void ConnectDatabase();
    QString FindKey(const QString email);
    bool InserIntoTable(const QString &fname, const QString &sname,
                        const QString &pat,   const QString &mod,
                        const QString &exp,   const QString &e_mail);
    bool CheckClient(const QString &e_mail);

private:

    QSqlDatabase    db;
    bool OpenDatabase();
    bool RestoreDatabase();
    void CloseDatabase();
    bool CreateTable();

    bool InserIntoTable(const QVariantList &data);
    bool RemoveRecord(const int id);
};

#endif // DATABASE_H
