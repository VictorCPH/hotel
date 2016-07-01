#include "dialoggoods_amount.h"
#include "ui_dialoggoods_amount.h"

extern int amount;
extern int flagforbuygoods;

Dialoggoods_amount::Dialoggoods_amount(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialoggoods_amount)
{
    ui->setupUi(this);
}

Dialoggoods_amount::~Dialoggoods_amount()
{
    delete ui;
}

void Dialoggoods_amount::on_pushButton_yes_clicked()
{
    amount = ui->spinBox_amount->value();
    flagforbuygoods = 0;
    close();
}
