#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "setupfrombackup.h"
#include "setupfromgitlab.h"
#include "setupfrombackupddev.h"
#include "apicomparison.h"
#include "cmsmanagement.h"

#include <QSqlQuery>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);
//    ui->statusBar->showMessage("deneme", 19999);

}

void MainWindow::on_setupFromBackupButton_clicked()
{
    SetupFromBackup setupFromBackup;
    setupFromBackup.setModal(true);
    setupFromBackup.exec();
}

void MainWindow::on_setupFromGitlabButton_clicked()
{
    SetupFromGitlab setupFromGitlab;
    setupFromGitlab.setModal(true);
    setupFromGitlab.exec();
}

void MainWindow::on_setupFromBackupDdev_clicked()
{
    SetupFromBackupDdev setupFromBackupDdev;
    setupFromBackupDdev.setModal(true);
    setupFromBackupDdev.exec();
}

void MainWindow::on_apiCompareButton_clicked()
{
    APIComparison apiComparison;
    apiComparison.setModal(true);
    apiComparison.exec();
}

void MainWindow::on_apiManagementButton_clicked()
{

}

//void MainWindow::on_btnLoadAPIEndpoints_clicked()
//{
//    loadAPIEndpoints();
//}


//void MainWindow::on_searchAPIEndpoints_textChanged(const QString &arg1)
//{
//    if(arg1.length() >= 3){
//        if(!dbm.isOpen()){
//            dbm.openConnection();
//        }
//        filterAPIEndpoints(arg1);
//    }
//}
//void MainWindow::on_searchAPIEndpoints_editingFinished()
//{
//    dbm.closeConnection();
//}


void MainWindow::loadAPIEndpoints()
{
//    dbm.openConnection();
//    QSqlQuery query;
//    query.prepare("SELECT path FROM api_endpoints");
//    query.exec();
//    dbm.generateDynamicTableView(query, ui->tableAPIEndpoints);
//    ui->tableAPIEndpoints->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    dbm.closeConnection();
}

void MainWindow::filterAPIEndpoints(QString searchTerm)
{

//    QSqlQuery query;
//    query.prepare("SELECT path FROM api_endpoints WHERE path LIKE :name");
//    query.bindValue(":name", "%"+searchTerm+"%");
//    query.exec();

//    dbm.generateDynamicTableView(query, ui->tableAPIEndpoints);
//    ui->tableAPIEndpoints->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void MainWindow::on_cmsManagementButton_clicked()
{
    CMSManagement cmsManagement;
    cmsManagement.setModal(true);
    cmsManagement.exec();
}

