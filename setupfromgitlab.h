#ifndef SETUPFROMGITLAB_H
#define SETUPFROMGITLAB_H

#include <QDialog>
#include <QProcess>

namespace Ui {
class SetupFromGitlab;
}

class SetupFromGitlab : public QDialog
{
    Q_OBJECT

public:
    explicit SetupFromGitlab(QWidget *parent = nullptr);
    ~SetupFromGitlab();

private slots:
    bool on_run_button_clicked();
//    void on_script_button_clicked();
    void updateOutputTextEdit();
    void processFinished();
    void on_path_button_clicked();

private:
    Ui::SetupFromGitlab *ui;
    QProcess process;
};

#endif // SETUPFROMGITLAB_H
