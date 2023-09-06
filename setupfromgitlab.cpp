#include "setupfromgitlab.h"
#include "ui_setupfromgitlab.h"

#include <QFileDialog>
#include <QMessageBox>

SetupFromGitlab::SetupFromGitlab(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupFromGitlab)
{
    ui->setupUi(this);
    this->setWindowTitle("GitLab Deposundan Kurulum");
    ui->textBrowser->setReadOnly(false);
}

SetupFromGitlab::~SetupFromGitlab()
{
    delete ui;
}

bool SetupFromGitlab::on_run_button_clicked()
{
    QString gitLabName = ui->gitLabName_lineEdit->text();
    QString dbName = ui->dbName_lineEdit->text();
    QString path = ui->path_lineEdit->text();

    QString appPath = QCoreApplication::applicationDirPath();
    QString script = appPath + QString("/scripts/setup_cms_gitlab.sh");

    QStringList args;
    args << "-n" << gitLabName << "-d" << dbName;

    QFileInfo scriptFileInfo(script);
    if(!scriptFileInfo.exists() && !scriptFileInfo.isFile()){
        QMessageBox msgWarning;
        msgWarning.setText("Script Bulunamadı.\n /script/setup_cms.sh");
        msgWarning.setIcon(QMessageBox::Warning);
        msgWarning.setWindowTitle("Uyarı");
        msgWarning.exec();
        return false;
    }

    if(gitLabName.isEmpty()){
        QMessageBox msgWarning;
        msgWarning.setText("GitLab Name girilmedi.");
        msgWarning.setIcon(QMessageBox::Warning);
        msgWarning.setWindowTitle("Uyarı");
        msgWarning.exec();
        return false;
    }

    if(dbName.isEmpty()){
        QMessageBox msgWarning;
        msgWarning.setText("DB Name girilmedi.");
        msgWarning.setIcon(QMessageBox::Warning);
        msgWarning.setWindowTitle("Uyarı");
        msgWarning.exec();
        return false;
    }

    if(path.isEmpty()){
        QMessageBox msgWarning;
        msgWarning.setText("Path seçilmedi.");
        msgWarning.setIcon(QMessageBox::Warning);
        msgWarning.setWindowTitle("Uyarı");
        msgWarning.exec();
        return false;
    }

    QMessageBox msgBox;
    msgBox.setText("Kurulum başlatılacak, emin misiniz?");
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

        ui->textBrowser->append("kurulum başlatıldı.");
        ui->textBrowser->append("işlem uzun sürebilir, lütfen bekleyiniz...");

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

/**
 * @brief SetupFromBackup::updateOutputTextEdit
 * shell script prosesini yazdırır.
 */
void SetupFromGitlab::updateOutputTextEdit(){
    QByteArray newData = process.readAllStandardOutput();
    QString text = QString::fromLocal8Bit(newData);
    ui->textBrowser->append(text);
}

void SetupFromGitlab::processFinished(){
    QString gitLabName = ui->gitLabName_lineEdit->text();
    ui->textBrowser->append("<a href='http://127.0.0.1/oyas/"+gitLabName+"/web/'>http://127.0.0.1/oyas/"+gitLabName+"/web/</a>");
    ui->textBrowser->setOpenExternalLinks(true);
    ui->textBrowser->setReadOnly(true);
}

void SetupFromGitlab::on_path_button_clicked()
{
    /**
     * The dialog initially displays the contents of the "/home/tnrdll" directory,
     * and displays files matching the patterns given in the string "Image Files (*.png *.jpg *.bmp)".
     * The parent of the file dialog is set to this, and the window title is set to "Open Image".
     */
    QString dir = QFileDialog::getExistingDirectory(this, tr("Dizin Seç"),
                                                    "/var/www/html/oyas",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->path_lineEdit->setText(dir);
}
