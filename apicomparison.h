#ifndef APICOMPARISON_H
#define APICOMPARISON_H

#include <QDialog>
#include <QNetworkReply>
#include <QVector>


namespace Ui {
class APIComparison;
}

class APIComparison : public QDialog
{
    Q_OBJECT

public:
    explicit APIComparison(QWidget *parent = nullptr);
    ~APIComparison();

private slots:
    void on_compareEndpoints_clicked();

private:
    Ui::APIComparison *ui;
    QNetworkReply *reply_1;
    QNetworkReply *reply_2;
    void sourceRequestFinished();
    void targetRequestFinished();
    void extractJson(QJsonObject *obj,QVector<QString> *vect);
    void sourceProgressChanged(qint64 bytesRead, qint64 totalBytes);
    void targetProgressChanged(qint64 bytesRead, qint64 totalBytes);

};

#endif // APICOMPARISON_H
