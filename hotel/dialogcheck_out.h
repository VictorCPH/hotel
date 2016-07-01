#ifndef DIALOGCHECK_OUT_H
#define DIALOGCHECK_OUT_H

#include <QDialog>
#include "ui_dialogcheck_out.h"
#include <QMessageBox>
#include"hotel.h"
#include "QDebug"
#include <QTextBrowser>
#include"log_manage.h"

namespace Ui {
class Dialogcheck_out;
}

class Dialogcheck_out : public QDialog
{
    Q_OBJECT


public:
    explicit Dialogcheck_out(QWidget *parent = 0);
    ~Dialogcheck_out();

private slots:
    void on_pushButton_account_clicked();

    void on_pushButton_no_clicked();

    void on_pushButton_yes_clicked();

private:
    Ui::Dialogcheck_out *ui;
};

#endif // DIALOGCHECK_OUT_H
