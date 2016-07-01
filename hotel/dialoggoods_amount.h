#ifndef DIALOGGOODS_AMOUNT_H
#define DIALOGGOODS_AMOUNT_H

#include <QDialog>

namespace Ui {
class Dialoggoods_amount;
}

class Dialoggoods_amount : public QDialog
{
    Q_OBJECT

public:
    explicit Dialoggoods_amount(QWidget *parent = 0);
    ~Dialoggoods_amount();

private slots:
    void on_pushButton_yes_clicked();

private:
    Ui::Dialoggoods_amount *ui;
};

#endif // DIALOGGOODS_AMOUNT_H
