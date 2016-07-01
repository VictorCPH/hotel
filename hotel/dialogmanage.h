#ifndef DIALOGMANAGE_H
#define DIALOGMANAGE_H

#include <QDialog>
#include "hotel.h"

namespace Ui {
class Dialogmanage;
}

class Dialogmanage : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogmanage(QWidget *parent = 0);
    void showprice();
    ~Dialogmanage();

private slots:
    void on_pushButton_yesforprice_clicked();

    void on_pushButton_noforprice_clicked();

    void on_pushButton_yesforservice_clicked();

    void on_pushButton_noforservice_clicked();

    void on_pushButton_yesforfree_clicked();

    void on_pushButton_noforfree_clicked();

private:
    Ui::Dialogmanage *ui;
};

void changeRoomlinkuseroom(int a,int b,Roomlink newptr);

#endif // DIALOGMANAGE_H
