#include "apicomparison.h"
#include "ui_apicomparison.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QMessageBox>

APIComparison::APIComparison(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::APIComparison)
{
    ui->setupUi(this);
    this->setWindowTitle("API Endpoint Karşılaştırma");
    ui->sourceBrowser->setReadOnly(false);
    ui->targetBrowser->setReadOnly(false);
}

APIComparison::~APIComparison()
{
    delete ui;
}

void APIComparison::on_compareEndpoints_clicked()
{

    ui->sourceProgressBar->setValue(0);
    ui->targetProgressBar->setValue(0);
    ui->issuesBrowser->clear();
    ui->issuesBrowser->setTextColor(QColor("black"));
    ui->structureBrowser->clear();
    ui->structureBrowser->setTextColor(QColor("black"));
    ui->sourceBrowser->clear();
    ui->targetBrowser->clear();


    QString source_url = ui->lineEdit->text();

    QMessageBox msgBox;
    msgBox.setText("API Endpoint Karşılarştırma başlatılacak, emin misiniz?");
    msgBox.setInformativeText("HTTP GET Request işlemleri uzun sürebilir, lütfen bekleyiniz.");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);

    msgBox.setWindowTitle("Uyarı");
    msgBox.setIcon(QMessageBox::Question);

    msgBox.button(QMessageBox::Ok)->setText("Evet");
    msgBox.button(QMessageBox::Cancel)->setText("İptal");


    int ret = msgBox.exec();


    switch (ret) {
    case QMessageBox::Ok:

        ui->issuesBrowser->append("Karşılaştırma işlemleri başlatıldı, lütfen bekleyiniz...");


        //ASYNC olduğu için ayrı slotlara bağla
        QNetworkAccessManager *manager_source = new QNetworkAccessManager(this);
        connect(manager_source, &QNetworkAccessManager::finished,
                this, &APIComparison::sourceRequestFinished);

        reply_1 = manager_source->get(QNetworkRequest(QUrl(source_url)));
        connect(reply_1,&QNetworkReply::downloadProgress,this,&APIComparison::sourceProgressChanged);

        //SLOT sourceRequestFinished() fonksiyonunu çağırır.
        break;

    }

}




void APIComparison::sourceRequestFinished(){

    ui->issuesBrowser->append("Kaynak HTTP req yanıtı alındı, hedef HTTP req yanıtı bekleniyor...");

    //ASYNC olduğu için ayrı slotlara bağla
    QNetworkAccessManager *manager_target = new QNetworkAccessManager(this);
    connect(manager_target, &QNetworkAccessManager::finished,
            this, &APIComparison::targetRequestFinished);

    QString target_url = ui->lineEdit_2->text();
    reply_2 = manager_target->get(QNetworkRequest(QUrl(target_url)));
    connect(reply_2,&QNetworkReply::downloadProgress,this,&APIComparison::targetProgressChanged);

    //SLOT targetRequestFinished() fonksiyonunu çağırır.

}


// REQUESTLER SONRASI ÇALIŞACAK ANA FONKSİYON
void APIComparison::targetRequestFinished(){

    ui->issuesBrowser->append("Hedef HTTP req yanıtı alındı, karşılaştırma yapılıyor...");

    //Testler: branded fare, bundle

    QString strReply_1 = (QString)reply_1->readAll();
    QString strReply_2 = (QString)reply_2->readAll();

    QJsonDocument doc_1 = QJsonDocument::fromJson(strReply_1.toUtf8());
    QJsonDocument doc_2 = QJsonDocument::fromJson(strReply_2.toUtf8());

    QString formattedJsonString_1 = doc_1.toJson(QJsonDocument::Indented);
    ui->sourceBrowser->setText(formattedJsonString_1);

    QString formattedJsonString_2 = doc_2.toJson(QJsonDocument::Indented);
    ui->targetBrowser->setText(formattedJsonString_2);

    QJsonObject obj_1 = doc_1.object();
    QJsonObject obj_2 = doc_2.object();

    // REQUESTLER TAMAMLANDIĞI İÇİN İKİSİ DE > 0 OLMALIDIR.
    if((obj_1.length() && obj_2.length()) > 0){

        QVector<QString> vect_1;
        QVector<QString> vect_2;

        this->extractJson(&obj_1,  &vect_1);
        this->extractJson(&obj_2,  &vect_2);

//        qDebug() << vect_1;
//        qDebug() << vect_2;


        if(vect_1 == vect_2){
            ui->issuesBrowser->setTextColor(QColor("green"));
            ui->issuesBrowser->append("Karşılaştırılan API Endpoint yapıları uyumlu.");
        }

        // Endpointler uyumsuz ise
        else{
            ui->issuesBrowser->setTextColor(QColor("red"));
            ui->issuesBrowser->append("Karşılaştırılan API Endpoint yapıları uyumsuz.");

            ui->issuesBrowser->append("Kaynak JSON Object adedi: " + QString::number(obj_1.size()));
            ui->issuesBrowser->append("Hedef JSON Object adedi: " + QString::number(obj_2.size()));

            ui->issuesBrowser->append("Kaynak Toplam JSON Key adedi: " + QString::number(vect_1.size()));
            ui->issuesBrowser->append("Hedef Toplam JSON Key adedi: " + QString::number(vect_2.size()));

            QMessageBox msgWarning;
            msgWarning.setText("Karşılaştırılan API Endpoint yapıları uyumsuz.\n"
                               "Lütfen yetkili kişiyi bilgilendiriniz.");
            msgWarning.setIcon(QMessageBox::Critical);
            msgWarning.setWindowTitle("Dikkat!");
            msgWarning.exec();
        }


        // YAPI ANALİZİ
        for(int i = 0; i < vect_1.size(); ++i){

            if(i < vect_2.size()){

                if(vect_1[i]==vect_2[i]){
                    ui->structureBrowser->setTextColor(QColor("green"));
                    ui->structureBrowser->append(QString(vect_1[i]+ " = "+ QString(vect_2[i])));
                }
                else{

                    ui->structureBrowser->setTextColor(QColor("red"));
                    ui->structureBrowser->append(QString(vect_1[i]+ " = "+ QString(vect_2[i])));
                }

            }
        }


    }
}

void APIComparison::extractJson(QJsonObject *obj,QVector<QString> *vect){

    foreach (const QString &key , obj->keys()){

        vect->append(key);
        QJsonValue value = obj->value(key);

        //key -> value kısmı obje ise
        if (value.isObject()) {

            //nested object ise recursive olarak çalış.
            if(value.toObject().size()>1){
                QJsonObject obj = value.toObject();
                this->extractJson(&obj, vect);
            }
        }
        else{
            if(!obj->value(key).isArray()){
                vect->append(obj->value(key).toString());
            }
            else{
                //vect->append("A");
                QJsonArray m_DataSourceArray = obj->value(key).toArray();
                foreach (const QJsonValue &paramValue, m_DataSourceArray){
                    foreach(const QString& key, paramValue.toObject().keys()) {
                        vect->append(key);
                    }
                }
            }
        }
    }
}


void APIComparison::sourceProgressChanged(qint64 bytesRead, qint64 totalBytes) {
    ui->sourceProgressBar->setMaximum(totalBytes);
    ui->sourceProgressBar->setValue(bytesRead);
}

void APIComparison::targetProgressChanged(qint64 bytesRead, qint64 totalBytes) {
    ui->targetProgressBar->setMaximum(totalBytes);
    ui->targetProgressBar->setValue(bytesRead);
}
