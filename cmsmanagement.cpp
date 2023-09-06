#include "cmsmanagement.h"
#include "ui_cmsmanagement.h"

CMSManagement::CMSManagement(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMSManagement)
{
    ui->setupUi(this);
    this->setWindowTitle("CMS Yönetimi");
}

CMSManagement::~CMSManagement()
{
    delete ui;
}
