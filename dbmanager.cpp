/**
* @author Taner Deliloglu
* Contact: tnrdll@gmail.com
*/

#include "dbmanager.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>

#include <QTableView>
#include <QtWidgets>



//constructor
DBManager::DBManager()
{
    QSqlDatabase db;
}


//void DBManager::createConnection(const QString &path, const QString &dbType){
//    this->db = QSqlDatabase::addDatabase(dbType); //QMYSQL, QSQLITE...
//    this->db.setDatabaseName(path); //db name or path
//}

void DBManager::createConnection(){
    QString db_path = QCoreApplication::applicationDirPath();
    db_path =  db_path + QString("/cms.db");
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setDatabaseName(db_path);

}


void DBManager::openConnection(){
    this->createConnection();
    this->db.open();
    qDebug() << "Database: connection has been started";
}


void DBManager::closeConnection(){

    this->db.close();
    qDebug() << "Database: connection has been closed";
}



void DBManager::generateDynamicTableView(const QSqlQuery &sql_query_text, QTableView *tv){

    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(std::move(sql_query_text));

    //sorting enabled
    QSortFilterProxyModel *proxy_model = new QSortFilterProxyModel();
    proxy_model->setSourceModel(model);

    tv->setModel(proxy_model);
    tv->setSortingEnabled(true);

    tv->show();
}




void DBManager::setMySQL()
{
    this->db.setHostName("127.0.0.1");
    this->db.setUserName("root");
    this->db.setPassword("");

    if(!this->db.open())
        qDebug() << "Database: MySQL connection failed!";
    else
        qDebug() << "Database: MySQL connection has been successfully opened";
}






void DBManager::testQuery(const QString &table_name){

    QSqlQuery query;
    query.exec("SELECT id FROM "+table_name);
    while (query.next()) {
        int id = query.value(0).toInt();
        //QString email = query.value(1).toString();
        qDebug() << id;
    }
}


bool DBManager::isOpen(){

    return this->db.isOpen();
}

DBManager::~DBManager()
{
    if (this->db.isOpen())
    {
        this->db.close();
    }
}
