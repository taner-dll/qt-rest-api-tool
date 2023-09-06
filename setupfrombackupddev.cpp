#include "setupfrombackupddev.h"
#include "ui_setupfrombackupddev.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>

SetupFromBackupDdev::SetupFromBackupDdev(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupFromBackupDdev)
{
    ui->setupUi(this);
    this->setWindowTitle("Backup Kurulumu (DDEV)");
    ui->textBrowser_3->setReadOnly(false);
}

SetupFromBackupDdev::~SetupFromBackupDdev()
{
    delete ui;
}

void SetupFromBackupDdev::on_selectTarball_3_clicked()
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

    ui->tarballPath_3->setText(filename);

}


void SetupFromBackupDdev::on_selectSQLdumpFile_3_clicked()
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

    ui->sqlDumpPath_3->setText(filename);
}


void SetupFromBackupDdev::on_changeCMSbackupSetupPath_3_clicked()
{

    /**
     * The dialog initially displays the contents of the "/home/tnrdll" directory,
     * and displays files matching the patterns given in the string "Image Files (*.png *.jpg *.bmp)".
     * The parent of the file dialog is set to this, and the window title is set to "Open Image".
     */
    QString dir = QFileDialog::getExistingDirectory(this, tr("Dizin Seç"),
                                                    "/home/tnrdll/Ddev",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->cmsBackupSetupPath_3->setText(dir);

}


bool SetupFromBackupDdev::on_setupFromCMSBackup_3_clicked()
{

    QString tarFile = ui->tarballPath_3->text();
    QString sqlFile = ui->sqlDumpPath_3->text();
    QString path = ui->cmsBackupSetupPath_3->text();
    QString folderName = ui->folderName_3->text();
    QString phpVersion = ui->comboBoxPHP->currentText();

    //QString script = ui->setupFromCMSbackupScriptPath->text();
    QString appPath = QCoreApplication::applicationDirPath();
    QString script = appPath + QString("/scripts/setup_cms_ddev.sh");


//            qDebug() << phpVersion;
//            return false;


    QFileInfo scriptFileInfo(script);
    if(!scriptFileInfo.exists() && !scriptFileInfo.isFile()){
        QMessageBox msgWarning;
        msgWarning.setText("Script Bulunamadı.\n /script/setup_cms_ddev.sh");
        msgWarning.setIcon(QMessageBox::Warning);
        msgWarning.setWindowTitle("Uyarı");
        msgWarning.exec();
        return false;
    }


    QStringList args;
    args << "-n" << folderName << "-t" << tarFile << "-z" << sqlFile << "-p" << phpVersion;


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

        qDebug() << args;


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


/**
 * @brief SetupFromBackup::updateOutputTextEdit
 * shell script prosesini yazdırır.
 */
void SetupFromBackupDdev::updateOutputTextEdit(){
    QByteArray newData = process.readAllStandardOutput();
    QString text = QString::fromLocal8Bit(newData);
    ui->textBrowser_3->append(text);
}

void SetupFromBackupDdev::processFinished(){
    QString folderName = ui->folderName_3->text();
    ui->textBrowser_3->append("<a href='http://"+folderName+".ddev.site:8080'>http://"+folderName+".ddev.site:8080</a>");
    ui->textBrowser_3->append("<a href='http://"+folderName+".ddev.site:8036'>phpMyAdmin (8036)</a>");
    ui->textBrowser_3->setOpenExternalLinks(true);
    ui->textBrowser_3->setReadOnly(true);
}

