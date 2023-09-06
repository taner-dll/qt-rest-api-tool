#ifndef SETUPFROMBACKUPDDEV_H
#define SETUPFROMBACKUPDDEV_H

#include <QDialog>
#include <QProcess>

namespace Ui {
class SetupFromBackupDdev;
}

class SetupFromBackupDdev : public QDialog
{
    Q_OBJECT

public:
    explicit SetupFromBackupDdev(QWidget *parent = nullptr);
    ~SetupFromBackupDdev();

private slots:
    void on_selectTarball_3_clicked();

    void on_selectSQLdumpFile_3_clicked();

    void on_changeCMSbackupSetupPath_3_clicked();

    bool on_setupFromCMSBackup_3_clicked();

    void updateOutputTextEdit();
    void processFinished();

private:
    Ui::SetupFromBackupDdev *ui;
    QProcess process;

};

#endif // SETUPFROMBACKUPDDEV_H
