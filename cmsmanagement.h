#ifndef CMSMANAGEMENT_H
#define CMSMANAGEMENT_H

#include <QDialog>

namespace Ui {
class CMSManagement;
}

class CMSManagement : public QDialog
{
    Q_OBJECT

public:
    explicit CMSManagement(QWidget *parent = nullptr);
    ~CMSManagement();

private:
    Ui::CMSManagement *ui;
};

#endif // CMSMANAGEMENT_H
