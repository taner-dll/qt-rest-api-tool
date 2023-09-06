#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QTableView>
#include <QTableWidget>


class DBManager
{
public:
    DBManager();
    ~DBManager();

    bool isOpen();
    //void createConnection(const QString &path, const QString &dbType);
    void createConnection();
    void openConnection();
    void closeConnection();
    void setMySQL();
    void testQuery(const QString &table_name);
    void generateDynamicTableView(const QSqlQuery &sql_query_text, QTableView *tv);




private:
    QSqlDatabase db;
};

#endif // DBMANAGER_H
