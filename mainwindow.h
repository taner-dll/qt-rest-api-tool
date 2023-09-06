#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dbmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadAPIEndpoints();
    void filterAPIEndpoints(QString searchTerm);

private slots:

//    void on_btnLoadAPIEndpoints_clicked();
//    void on_searchAPIEndpoints_textChanged(const QString &arg1);
//    void on_searchAPIEndpoints_editingFinished();

    void on_setupFromBackupButton_clicked();

    void on_setupFromGitlabButton_clicked();

    void on_setupFromBackupDdev_clicked();

    void on_apiCompareButton_clicked();

    void on_apiManagementButton_clicked();

    void on_cmsManagementButton_clicked();

protected:
    void showEvent(QShowEvent *ev);


private:
    Ui::MainWindow *ui;
    DBManager dbm;

};
#endif // MAINWINDOW_H
