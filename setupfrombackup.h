#ifndef SETUPFROMBACKUP_H
#define SETUPFROMBACKUP_H

#include <QDialog>
#include <QProcess>

namespace Ui {
class SetupFromBackup;
}

class SetupFromBackup : public QDialog
{
    Q_OBJECT

public:
    explicit SetupFromBackup(QWidget *parent = nullptr);
    ~SetupFromBackup();

private slots:

    void on_selectTarball_clicked();

    void on_selectSQLdumpFile_clicked();

    void on_changeCMSbackupSetupPath_clicked();

    bool on_setupFromCMSBackup_clicked();

//    void on_setupFromCMSbackupScript_clicked();

    void updateOutputTextEdit();
    void processFinished();


private:
    Ui::SetupFromBackup *ui;
    QProcess process;

};

#endif // SETUPFROMBACKUP_H
