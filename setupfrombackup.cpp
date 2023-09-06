#include "setupfrombackup.h"
#include "ui_setupfrombackup.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>


SetupFromBackup::SetupFromBackup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupFromBackup)
{
    ui->setupUi(this);
    this->setWindowTitle("Backup Kurulumu (LAMP)");
    ui->textBrowser->setReadOnly(false);
}

SetupFromBackup::~SetupFromBackup()
{
    delete ui;
}


void SetupFromBackup::on_selectTarball_clicked()
{
    /**
     * The dialog initially displays the contents of the "/home/tnrdll" directory,
     * and displays files matching the patterns given in the string "Image Files (*.png *.jpg *.bmp)".
     * The parent of the file dialog is set to this, and the window title is set to "Open Image".
     */
    //QString file: path + filename
    QFile file = QFileDialog::getOpenFileName(this,
                                              tr("Open .tar.gz file"),
                                              "/home/tnrdll/OYAS_archive/downloads",
                                              tr("Files (*.tar.gz)"));

    //Sadece dosyanın adını
    QString filename = QFileInfo(file).fileName();

    ui->tarballPath->setText(filename);

}


void SetupFromBackup::on_selectSQLdumpFile_clicked()
{
    /**
     * The dialog initially displays the contents of the "/home/tnrdll" directory,
     * and displays files matching the patterns given in the string "Image Files (*.png *.jpg *.bmp)".
     * The parent of the file dialog is set to this, and the window title is set to "Open Image".
     */

    //QString file: path + filename
    QFile file = QFileDialog::getOpenFileName(this,
                                              tr("Open .sql.gz file"),
                                              "/home/tnrdll/OYAS_archive/downloads",
                                              tr("Files (*.sql.gz)"));
    //Sadece dosyanın adını
    QString filename = QFileInfo(file).fileName();

    ui->sqlDumpPath->setText(filename);
}


void SetupFromBackup::on_changeCMSbackupSetupPath_clicked()
{
    /**
     * The dialog initially displays the contents of the "/home/tnrdll" directory,
     * and displays files matching the patterns given in the string "Image Files (*.png *.jpg *.bmp)".
     * The parent of the file dialog is set to this, and the window title is set to "Open Image".
     */
    QString dir = QFileDialog::getExistingDirectory(this, tr("Dizin Seç"),
                                                    "/var/www/html/oyas",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->cmsBackupSetupPath->setText(dir);
}


bool SetupFromBackup::on_setupFromCMSBackup_clicked()
{

    QString tarFile = ui->tarballPath->text();
    QString sqlFile = ui->sqlDumpPath->text();
    QString path = ui->cmsBackupSetupPath->text();
    QString folderName = ui->folderName->text();

    //QString script = ui->setupFromCMSbackupScriptPath->text();
    QString appPath = QCoreApplication::applicationDirPath();
    QString script = appPath + QString("/scripts/setup_cms.sh");


    //        qDebug() << script;
    //        return false;


    QFileInfo scriptFileInfo(script);
    if(!scriptFileInfo.exists() && !scriptFileInfo.isFile()){
        QMessageBox msgWarning;
        msgWarning.setText("Script Bulunamadı.\n /script/setup_cms.sh");
        msgWarning.setIcon(QMessageBox::Warning);
        msgWarning.setWindowTitle("Uyarı");
        msgWarning.exec();
        return false;
    }


    QStringList args;
    args << "-n" << folderName << "-t" << tarFile << "-z" << sqlFile;


    if(tarFile.isEmpty()){
        QMessageBox msgWarning;
        msgWarning.setText("Tarball seçilmedi.");
        msgWarning.setIcon(QMessageBox::Warning);
        msgWarning.setWindowTitle("Uyarı");
        msgWarning.exec();
        return false;
    }

    if(sqlFile.isEmpty()){
        QMessageBox msgWarning;
        msgWarning.setText("SQL sump seçilmedi.");
        msgWarning.setIcon(QMessageBox::Warning);
        msgWarning.setWindowTitle("Uyarı");
        msgWarning.exec();
        return false;
    }

    if(folderName.isEmpty()){
        QMessageBox msgWarning;
        msgWarning.setText("Dizin adı girilmedi.");
        msgWarning.setIcon(QMessageBox::Warning);
        msgWarning.setWindowTitle("Uyarı");
        msgWarning.exec();
        return false;
    }





    QMessageBox msgBox;
    msgBox.setText("Backup kurulumu başlatılacak, emin misiniz?");
    msgBox.setInformativeText("İşlem durdurulamaz.");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);

    msgBox.setWindowTitle("Kurulum Uyarısı");
    msgBox.setIcon(QMessageBox::Question);

    //todo translation sonrası kapatılacak
    msgBox.button(QMessageBox::Ok)->setText("Evet");
    msgBox.button(QMessageBox::Cancel)->setText("İptal");



    int ret = msgBox.exec();


    switch (ret) {
    case QMessageBox::Ok:

        //update text when processing.
        connect(&process, SIGNAL(readyReadStandardOutput()),this, SLOT(updateOutputTextEdit()));
        connect(&process, SIGNAL(finished(int,QProcess::ExitStatus)),this, SLOT(processFinished()));

        process.setWorkingDirectory(path);
        process.start(script,args);

        break;

    case QMessageBox::Cancel:
        // Cancel was clicked
        qDebug() << "cancelled";
        break;
    default:
        // should never be reached
        break;
    }


    return true;

}


void SetupFromBackup::updateOutputTextEdit(){
    QByteArray newData = process.readAllStandardOutput();
    QString text = QString::fromLocal8Bit(newData);
    ui->textBrowser->append(text);
}

void SetupFromBackup::processFinished(){
    QString folderName = ui->folderName->text();
    ui->textBrowser->append("<a href='http://127.0.0.1/oyas/"+folderName+"/web/'>http://127.0.0.1/oyas/"+folderName+"/web/</a>");
    ui->textBrowser->setOpenExternalLinks(true);
    ui->textBrowser->setReadOnly(true);
}

//void SetupFromBackup::on_setupFromCMSbackupScript_clicked()
//{
//    /**
//     * The dialog initially displays the contents of the "/home/tnrdll" directory,
//     * and displays files matching the patterns given in the string "Image Files (*.png *.jpg *.bmp)".
//     * The parent of the file dialog is set to this, and the window title is set to "Open Image".
//     */
//    QString file = QFileDialog::getOpenFileName(this,
//                                                tr("Open .sh file"),
//                                                "/home/tnrdll/Shell Scripts/my_shell_scripts",
//                                                tr("Files (*.sh)"));

//    ui->setupFromCMSbackupScriptPath->setText(file);

//}
